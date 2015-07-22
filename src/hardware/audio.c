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

#include "audio.h"

#include <math.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_mixer.h>

int cur_channel = 0;
audio_track* tracks[ AUDIO_MAX_TRACKS ];

void initAudio() {
    int max_tracks = AUDIO_MAX_TRACKS;
    Mix_Init(MIX_INIT_OGG);
    Mix_OpenAudio( AUDIO_SAMPLERATE, MIX_DEFAULT_FORMAT, AUDIO_CHANNELS, AUDIO_CHUNKSIZE );
    Mix_AllocateChannels( max_tracks );
    audio_reset_tracks();
}

void audio_create_clip(audio_clip* clip, char* clip_name) {
    clip->ChunkData = 0;
    clip->ChunkData=Mix_LoadWAV(clip_name);
    if(clip->ChunkData == 0) {

       printf(clip_name);
       printf(Mix_GetError());
       exit(1);
    }
}

void audio_drop_clip(audio_clip* clip) {
    Mix_FreeChunk(clip->ChunkData);
    clip->ChunkData = 0;
}

void dropAudio() {
    Mix_CloseAudio();
    Mix_Quit();
}

void audio_create_track(audio_track* track, double bpm, unsigned int beat_locked) {
    if(cur_channel != AUDIO_MAX_TRACKS ) {

        track->track_num        = cur_channel;
        track->volume_filtered  = 0.0;
        track->volume_set       = 0.0;
        track->bpm              = bpm;
        track->bps              = bpm / 60.0;
        track->beat             = 0.0;
        track->next_clip        = 0;
        track->beat_locked      = beat_locked;
        tracks[cur_channel]     = track;

        cur_channel++;
    } else {
        printf("Tried to allocate > %d tracks",AUDIO_MAX_TRACKS);
        exit(1);
    }
}

void audio_play_clip_on_track(audio_clip* clip, audio_track* track, unsigned int loop) {

    if(clip->ChunkData==0)
        return;

    if(track->beat_locked == BEAT_LOCKED ) {
        track->next_clip = clip;
        track->next_clip_loops = loop;
		return;
    }

    if(loop==0)  {
        Mix_PlayChannel(track->track_num, clip->ChunkData,0);
    }
    else {
        Mix_PlayChannel(track->track_num, clip->ChunkData,-1);
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
}

void audio_tick_tracks(double delta) {
    int i;
    for(i=0; i<cur_channel;++i) {

        tracks[i]->volume_filtered = (  TICK_FILTER_A * tracks[i]->volume_filtered) +
            (1-TICK_FILTER_A * tracks[i]->volume_set     );

        Mix_Volume(tracks[i]->track_num, (int)(tracks[i]->volume_filtered*127.0));

        tracks[i]->beat += tracks[i]->bps*delta;
        if(tracks[i]->beat_locked == BEAT_LOCKED && (tracks[i]->beat>1.0)) {
            while(tracks[i]->beat>1.0)
                tracks[i]->beat-=1.0;
            if(tracks[i]->next_clip != 0) {
                if(tracks[i]->next_clip_loops == 0) {
                    Mix_PlayChannel(tracks[i]->track_num, tracks[i]->next_clip->ChunkData,0);
                } else {
                    Mix_PlayChannel(tracks[i]->track_num, tracks[i]->next_clip->ChunkData,-1);
                }
                tracks[i]->next_clip = 0;
            }
        }
    }
}


int audio_realtime_processing = 0;
void audio_enable_realtime_processing() {
    audio_realtime_processing = 1;
}

void audio_set_volume_on_track(audio_track* track, double volume) {
    if(audio_realtime_processing==1) {
        track->volume_set = volume;
    } else {
	    Mix_Volume(track->track_num, (int)(volume*127.0));
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
        Mix_SetPanning(track->track_num, 127+pan_t, 127-pan_t);
    }
}
