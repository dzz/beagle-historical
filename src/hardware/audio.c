#if defined(__STDC__)
# define C89
# if defined(__STDC_VERSION__)
#  define C90
#  if (__STDC_VERSION__ >= 199409L)
#   define C94
#  endif
#  if (__STDC_VERSION__ >= 199901L)
#   define C99
#  endif
# endif
#endif

#include <windows.h>
#include "audio.h"
#include "hf_timer.h"

#include <math.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_thread.h>

//tracks
int cur_channel = 0;
int cur_track   = 0;
double chunk_time = 0;
audio_track* tracks[ AUDIO_MAX_TRACKS ];

//threading
int audio_shutdown_flag = 0;

#define THREAD_OP_READ_WAV    0
#define THREAD_OP_FREE_CHUNK  1
#define THREAD_OP_SET_VOLUME  2
#define THREAD_OP_SET_PANNING 3
#define THREAD_OP_PLAY_CLIP   4

typedef struct {
    unsigned int code;
    void*        data;
} audio_thread_operation;

typedef struct {
    unsigned int channel;
    Mix_Chunk* chunk;
    int loops;
} op_play_data; 

typedef struct {
    audio_track* track;
    int vol;
} op_set_vol_data;

typedef struct {
    audio_track* track;
    unsigned char left;
    unsigned char right;
} op_set_pan_data;

audio_thread_operation* cur_thread_op = 0;
void* audio_operation_result = 0;

SDL_Thread* thread;
SDL_mutex * op_mutex;
SDL_cond  * op_result_ready;


void* athr_wait_result(audio_thread_operation* op) {
    SDL_LockMutex(op_mutex);
	cur_thread_op = op;
    SDL_CondWait( op_result_ready, op_mutex );
	SDL_UnlockMutex(op_mutex);
    return audio_operation_result;
}

Mix_Chunk* athr_ReadWav(char* filename) {
    audio_thread_operation op;

    op.code = THREAD_OP_READ_WAV;
    op.data = filename;

    return (Mix_Chunk*)athr_wait_result(&op);
}

void athr_FreeChunk(Mix_Chunk* chunk) {
    audio_thread_operation op;
    op.code = THREAD_OP_FREE_CHUNK;
    op.data = chunk;

    athr_wait_result(&op);
}

void athr_Volume(audio_track* track, int volume) {
    audio_thread_operation  op;
    op_set_vol_data         data;

    data.track = track;
    data.vol = volume;

    op.code = THREAD_OP_SET_VOLUME;
    op.data = &data;

    athr_wait_result(&op);
}

void athr_SetPanning(audio_track* track, unsigned char left, unsigned char right) {
    audio_thread_operation  op;
    op_set_pan_data         data;

    data.track  = track;
    data.left   = left;
    data.right  = right;

    op.code = THREAD_OP_SET_PANNING;
    op.data = &data;

    athr_wait_result(&op);
}

void athr_PlayChannel(unsigned int channel, Mix_Chunk* ChunkData, int loops) {

    audio_thread_operation  op;
    op_play_data            data;

    data.channel  = channel;
    data.chunk = ChunkData;
    data.loops = loops;

    op.code = THREAD_OP_PLAY_CLIP;
    op.data = &data;

    athr_wait_result(&op);
}

void audio_tracks_update_beat( int chan, void* stream, int len, void *udata) {
    int i;
    for(i=0; i<cur_track;++i) {
        tracks[i]->beat += tracks[i]->bpsmp*(double)len;
    }
}


void athr_HandleOp() {    
    switch(cur_thread_op->code) {
        case THREAD_OP_READ_WAV:
            audio_operation_result = (void*)Mix_LoadWAV((char*)cur_thread_op->data);
            break;
        case THREAD_OP_FREE_CHUNK:
            Mix_FreeChunk((Mix_Chunk*)cur_thread_op->data);
			audio_operation_result = 0;
			break;
        case THREAD_OP_SET_VOLUME:
            {
                op_set_vol_data* volop = (op_set_vol_data*)cur_thread_op->data;
                Mix_Volume(volop->track->master_track_channel, volop->vol );
                Mix_Volume(volop->track->slave_track_channel,  volop->vol );
                audio_operation_result = 0;
            }
            break;
        case THREAD_OP_SET_PANNING:
            {
                op_set_pan_data* panop = (op_set_pan_data*)cur_thread_op->data;
                Mix_SetPanning(panop->track->master_track_channel, panop->left, panop->right );
                Mix_SetPanning(panop->track->slave_track_channel,  panop->left, panop->right );
                audio_operation_result = 0;
            }
            break;
        case THREAD_OP_PLAY_CLIP:
            {
                op_play_data* playop = (op_play_data*)cur_thread_op->data;
                audio_operation_result = (void*)Mix_PlayChannel( playop->channel, playop->chunk, playop->loops );
            }
            break;
            
    }
}

int audioThread( void* data) {

#define ATREAD_YIELD_MS 3

    int max_tracks = AUDIO_MAX_TRACKS;
	
    audio_reset_tracks();
    chunk_time = (1.0 / (double)AUDIO_SAMPLERATE) * (AUDIO_CHUNKSIZE/2);
	SDL_InitSubSystem( SDL_INIT_AUDIO );
    Mix_Init(MIX_INIT_OGG);
    Mix_OpenAudio( AUDIO_SAMPLERATE, MIX_DEFAULT_FORMAT, AUDIO_CHANNELS, AUDIO_CHUNKSIZE );
    Mix_AllocateChannels( max_tracks );
    Mix_RegisterEffect( MIX_CHANNEL_POST, audio_tracks_update_beat, NULL, tracks);

	{
		double secs = getTimeMs()/1000.0;



		while(audio_shutdown_flag ==0) {

	
			double secs_t = getTimeMs()/1000.0;
								if(cur_thread_op!=0) {
			SDL_LockMutex(op_mutex);
            athr_HandleOp();
            cur_thread_op = 0;
            SDL_CondSignal(op_result_ready);
			SDL_UnlockMutex(op_mutex);
			}

			audio_tick_tracks(secs_t-secs);
			secs = secs_t;

            Sleep(ATREAD_YIELD_MS);

		}
	}
	Mix_UnregisterEffect( MIX_CHANNEL_POST, audio_tracks_update_beat);
    Mix_CloseAudio();
    Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

    return 0;
}

void initAudio() {
    /*
    int max_tracks = AUDIO_MAX_TRACKS;
    Mix_Init(MIX_INIT_OGG);
    Mix_OpenAudio( AUDIO_SAMPLERATE, MIX_DEFAULT_FORMAT, AUDIO_CHANNELS, AUDIO_CHUNKSIZE );
    Mix_AllocateChannels( max_tracks );
    Mix_RegisterEffect( MIX_CHANNEL_POST, audio_tracks_update_beat, NULL, tracks);
    */
    
    thread = SDL_CreateThread( audioThread, "audio thread", 0 );
    op_mutex = SDL_CreateMutex();
	op_result_ready = SDL_CreateCond();

}

void audio_create_clip(audio_clip* clip, char* clip_name, double clip_beats, double clip_trigger_offset) {
    clip->ChunkData = 0;
    clip->ChunkData=athr_ReadWav(clip_name);
    if(clip->ChunkData == 0) {

       printf(clip_name);
       printf(Mix_GetError());
       exit(1);
    }
    clip->clip_beats = clip_beats;
    clip->clip_trigger_offset = clip_trigger_offset;
}

void audio_drop_clip(audio_clip* clip) {
    athr_FreeChunk(clip->ChunkData);
    clip->ChunkData = 0;
}

void dropAudio() {
	int thread_result = 0;
    audio_shutdown_flag = 1;
	SDL_WaitThread(thread,&thread_result);
    SDL_DestroyMutex(op_mutex);
    SDL_DestroyCond(op_result_ready);
}


void audio_create_track(audio_track* track, double bpm, unsigned int beat_locked) {
    if(cur_track != AUDIO_MAX_TRACKS ) {

        track->master_track_channel        = cur_channel;
        track->slave_track_channel         = cur_channel+1;
        track->active_track_channel        = track->master_track_channel;
        track->volume_filtered  = 0.0;
        track->volume_set       = 0.0;
        track->bpm              = bpm;
        track->bpsec            = bpm / 60.0;
        track->bpsmp            = track->bpsec/(double)AUDIO_SAMPLERATE;
        track->beat             = 0.0;
        track->next_clip        = 0;
        track->beat_locked      = beat_locked;
        track->active_clip      = 0;
        track->next_beat_trigger = 1.0;
        tracks[cur_track]     = track;


        cur_track+=1;
        cur_channel+=2;
    } else {
        printf("Tried to allocate > %d tracks\n",AUDIO_MAX_TRACKS);
        exit(1);
    }
}

void audio_play_clip_on_track(audio_clip* clip, audio_track* track, unsigned int loop) {

    if(clip->ChunkData==0)
        return;

    if(track->active_clip!=0) {
        track->next_beat_trigger = track->active_clip->clip_beats - clip->clip_trigger_offset;
    } else {
        track->next_beat_trigger = clip->clip_trigger_offset;
    }

    if(track->beat_locked == BEAT_LOCKED ) {
        track->next_clip = clip;
        track->next_clip_loops = loop;
		return;
    }

    track->active_clip = clip;

    if(loop==0)  {
        athr_PlayChannel(track->active_track_channel, clip->ChunkData,0);
    }
    else {
        athr_PlayChannel(track->master_track_channel, clip->ChunkData,-1);
    }
}

#ifndef C99

float fmin(double a, double b) {
    if(a<b) return a; return a;
}

float fmax(double a, double b) {
    if(a>b) return a; return b;
}

#endif

void audio_reset_tracks() {
    int i;
    for(i=0; i<AUDIO_MAX_TRACKS;++i) {
        tracks[i] = 0;
    }
    cur_channel=0;
    cur_track = 0;
}

void audio_track_swap_channels(audio_track* track) {
    if(track->active_track_channel == track->master_track_channel)
        track->active_track_channel = track->slave_track_channel;
    else
        track->active_track_channel = track->master_track_channel;
}


unsigned int is_first_tick = 1;
void audio_tick_tracks(double delta) {
    int i;
    for(i=0; i<cur_track;++i) {
        {
            //iir filter volume --> both channels

            tracks[i]->volume_filtered = (  TICK_FILTER_A * tracks[i]->volume_filtered) +
                (1-TICK_FILTER_A * tracks[i]->volume_set     );

            Mix_Volume(tracks[i]->master_track_channel, (int)(tracks[i]->volume_filtered*127.0));
            Mix_Volume(tracks[i]->slave_track_channel, (int)(tracks[i]->volume_filtered*127.0));
        }

        {

            if(tracks[i]->beat_locked == BEAT_LOCKED && (( (tracks[i]->beat+
                                (chunk_time * tracks[i]->bpsec)
                                
                                ) > tracks[i]->next_beat_trigger) || (is_first_tick==1))) {
                if(tracks[i]->next_clip != 0) {
                    Mix_PlayChannel(tracks[i]->active_track_channel, tracks[i]->next_clip->ChunkData,0);
                    tracks[i]->active_clip = tracks[i]->next_clip;

                    if(tracks[i]->beat>tracks[i]->next_beat_trigger) {
                        tracks[i]->beat-=tracks[i]->next_beat_trigger;
                    }


                    if(tracks[i]->next_clip_loops == 0) {
                        tracks[i]->next_clip = 0;
                        tracks[i]->next_beat_trigger = tracks[i]->active_clip->clip_beats;
                    }  else {
                        tracks[i]->next_beat_trigger = tracks[i]->active_clip->clip_beats+tracks[i]->next_clip->clip_trigger_offset;
                    }

                    audio_track_swap_channels(tracks[i]);
                }
            }
            //tracks[i]->beat += tracks[i]->bpsec*delta;
        }
    }
    is_first_tick = 0;
}


int audio_realtime_processing = 0;
void audio_enable_realtime_processing() {
    audio_realtime_processing = 1;
}

void audio_set_volume_on_track(audio_track* track, double volume) {
    if(audio_realtime_processing==1) {
        track->volume_set = volume;
    } else {
	    athr_Volume(track, (int)(volume*127.0));
    }
}

void audio_set_track_panning(audio_track* track, double pan) {
    if(pan<-1) {
           pan=-1;
    } 
    if(pan>1) {
        pan = 1;
    }

    {
        unsigned int pan_t = (unsigned int)(pan*127);
        unsigned char left = (unsigned char)pan_t+127;
        unsigned char right = 255-left;
        athr_SetPanning(track, 127+pan_t, 127-pan_t);
    }
}
