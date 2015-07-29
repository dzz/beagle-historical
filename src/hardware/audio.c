
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

#ifndef C99

float fmin(double a, double b) {
    if(a<b) return a; return a;
}

float fmax(double a, double b) {
    if(a>b) return a; return b;
}

#endif

#include <windows.h>
#include "audio.h"
#include "hf_timer.h"

#include <math.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_thread.h>
#include <portaudio.h>

#define MAX_BACKDOORS 256

float backdoors[MAX_BACKDOORS];

void ae_set_backdoor(unsigned int idx,float value) {
    backdoors[idx]  = value;
}

SDL_Thread *ae_thread;

typedef struct {
    float time;
} ae_renderer_context;


typedef struct {
    int shutdown;
    int initialized;
    int finalized;
    PaStream* stream;
    ae_renderer_context context;
} ae_data;

ae_data AE_Data;

static int ae_renderer( const void* inputBuffer, void* outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void* userData ) {

            unsigned int i;
            ae_renderer_context* context = (ae_renderer_context*)userData;
			float* out = (float*)outputBuffer;
            for(i=0; i<framesPerBuffer; ++i ) {

                float wave1 = sin( context->time*(backdoors[0]*22.0) );
                float wave2 = cos( context->time*(backdoors[1]*(44.0+(wave1*22)) ) );
                float wave3 = cos( context->time*(backdoors[2]*(88.0+(wave2*77) ) ));
                context->time+=1.0f/44100.0f;

                *out++ = (wave3+wave1)/2.0f;
                *out++ = (wave2+wave1)/2.0f;
            }
            return 0;
}

void ae_init(ae_data* self) {
	int err;
	err = Pa_Initialize();
	if(err!=paNoError) {
       return;
	} 

    err = Pa_OpenDefaultStream( &self->stream,
                                0,
                                2,
                                paFloat32,
                                AE_SAMPLERATE,
                                512,
                                ae_renderer,
                                &self->context );
    if(err!=paNoError) {
		printf("Error:%s\n", Pa_GetErrorText( err ) );
      return;
    }

	Pa_StartStream(self->stream);
    self->initialized = 1;
}

void ae_drop(ae_data* self) {
    self->initialized = 0;
	if(self->stream!=0)
		Pa_StopStream(self->stream);
	Pa_Terminate();
    self->finalized = 1;
}

int ae_thread_run(void* data) {
    ae_data* self=(ae_data*) data;
    ae_init(self);
    while(!self->shutdown) {
        Sleep(0);
    }
    ae_drop(self);
    return self->finalized;
}



void initAudio() {


    AE_Data.shutdown = 0;
    AE_Data.initialized = 0;
	AE_Data.stream = 0;
    SDL_CreateThread( ae_thread_run, "audio", &AE_Data);
}

void dropAudio() {
    int result;
    AE_Data.shutdown = 1;
    SDL_WaitThread(ae_thread, &result);
}

/***************************************/
void audio_tracks_update_beat( int chan, void *stream, int len, void *udata) {
}

void audio_garbage_collect_channels() {
}

void audio_create_clip(audio_clip* clip, char* clip_name, double clip_beats, double clip_trigger_offset) {
}

void audio_drop_clip(audio_clip* clip) {
}

void audio_create_track(audio_track* track, double bpm, unsigned int beat_locked) {
}

void audio_play_clip_on_track(audio_clip* clip, audio_track* track, unsigned int loop) {
}

void audio_reset_tracks() {
}

void audio_track_swap_channels(audio_track* track) {
}

void audio_tick_tracks(double delta) {
}

void audio_enable_realtime_processing() {
}

void audio_disable_realtime_processing() {
}

void audio_set_volume_on_track(audio_track* track, double volume) {
}

void audio_set_track_panning(audio_track* track, double pan) {
}
