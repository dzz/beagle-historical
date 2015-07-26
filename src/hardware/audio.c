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
int max_playing_channels = 0;
double chunk_time = 0;
audio_track* tracks[ AUDIO_MAX_TRACKS ];

unsigned int is_first_tick = 1;
unsigned int audio_realtime_processing = 0;
void audio_tracks_update_beat( int chan, void* stream, int len, void *udata) {
    int i;
    for(i=0; i<cur_track;++i) {
        if(is_first_tick!=1) {
            tracks[i]->beat += tracks[i]->bpsmp*(double)len;
        }
    }
    audio_tick_tracks( (double)len / (double)AUDIO_SAMPLERATE );
}


void audio_garbage_collect_channels() {
    int max_active_tracks = cur_track + (cur_track/2); //> more than this we start killing slaves
    int to_cull = Mix_Playing(-1) - max_active_tracks;
    if(to_cull>0) {
        int culled = 0;
		int i;
        for(i=0; i<cur_track;++i) {
            if( Mix_Playing(tracks[i]->slave_track_channel) == 1 ) {
				Mix_FadeOutChannel(tracks[i]->slave_track_channel,64);
                culled++;
                if(culled==to_cull)
                    break;
            }
        }
    }
}

void initAudio() {
    int max_tracks = AUDIO_MAX_TRACKS;
	SDL_InitSubSystem( SDL_INIT_AUDIO );
    Mix_Init(MIX_INIT_OGG);
    Mix_OpenAudio( AUDIO_SAMPLERATE, MIX_DEFAULT_FORMAT, AUDIO_CHANNELS, AUDIO_CHUNKSIZE );
    Mix_AllocateChannels( max_tracks*2 );
}

void audio_create_clip(audio_clip* clip, char* clip_name, double clip_beats, double clip_trigger_offset) {
    clip->ChunkData = 0;
	clip->ChunkData=Mix_LoadWAV(clip_name);
    if(clip->ChunkData == 0) {

       printf(clip_name);
       printf(Mix_GetError());
       exit(1);
    }
    clip->clip_beats = clip_beats;
    clip->clip_trigger_offset = clip_trigger_offset;
}

void audio_drop_clip(audio_clip* clip) {
	Mix_FreeChunk(clip->ChunkData);
    clip->ChunkData = 0;
}

void dropAudio() {
    if(audio_realtime_processing == 1) {
        audio_disable_realtime_processing();
    }
    Mix_CloseAudio();
    Mix_Quit();
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

    {
        track->active_clip = clip;

        //in non realtime mode infini looping tracks will go to slave
        if(loop==0)  {
            Mix_PlayChannel(track->master_track_channel, clip->ChunkData,0);
        }
        else {
            Mix_PlayChannel(track->slave_track_channel, clip->ChunkData,-1);
        }
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

            if(tracks[i]->beat_locked == BEAT_LOCKED && (( (tracks[i]->beat/*+
                                (chunk_time * tracks[i]->bpsec)*/
                                
                                ) > tracks[i]->next_beat_trigger) || (is_first_tick==1))) {
                if(tracks[i]->next_clip != 0) {
                    Mix_PlayChannel(tracks[i]->active_track_channel, tracks[i]->next_clip->ChunkData,0);
                    tracks[i]->active_clip = tracks[i]->next_clip;

                    if(tracks[i]->beat>tracks[i]->next_beat_trigger) {
                        tracks[i]->beat = 0;
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



void audio_enable_realtime_processing() {
    audio_realtime_processing = 1;
    Mix_RegisterEffect( MIX_CHANNEL_POST, audio_tracks_update_beat, NULL, tracks);
}

void audio_disable_realtime_processing() {
    audio_realtime_processing = 0;
	Mix_UnregisterEffect( MIX_CHANNEL_POST, audio_tracks_update_beat);
}

void audio_set_volume_on_track(audio_track* track, double volume) {
    if(audio_realtime_processing==1) {
        track->volume_set = volume;
    } else {
	    Mix_Volume(track->master_track_channel, (int)(volume*127.0));
	    Mix_Volume(track->slave_track_channel, (int)(volume*127.0));
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
        Mix_SetPanning(track->master_track_channel, 127+pan_t, 127-pan_t);
        Mix_SetPanning(track->slave_track_channel, 127+pan_t, 127-pan_t);
    }
}
