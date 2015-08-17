#include "../system/log.h"
#include "audio/load_snd.h"
#include <stdio.h>

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

float fmin(float a, float b) {
    if(a<b) return a; return a;
}

float fmax(float a, float b) {
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

#include "../system/rt_module_codes.h"

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

static float delta = 1.0f / 44100.0f;
static float time = 0.0f;
unsigned long smpl;


seq_sequencer* Sequencer;

void sequencer_track_create( seq_track* track) {
    track->cur_sample = 0;
    track->active_event = 0;
    track->control_message = 0;
}

void sequencer_create(seq_sequencer* seq) {
    seq->tracks = (seq_track*)malloc(sizeof(seq_track) * SEQUENCER_TRACKS);
	seq->n_tracks = SEQUENCER_TRACKS;
    {
        unsigned int i; 
        for(i=0; i< SEQUENCER_TRACKS;++i) {
            sequencer_track_create( &seq->tracks[i] );
        }
    }
}

static void sequencer_destroy_active_message(unsigned int tr_id) {
	seq_track* track = &(Sequencer->tracks[tr_id]);
    if( track->control_message!=0) {
        seq_track_msg* control_message = track->control_message; 
        track->control_message = 0;
        if(control_message->data!=0) {
            free(control_message->data);
        }
        free(control_message);
    }
}
static void sequencer_destroy_active_event(seq_track* track) {
    if(track->active_event != 0 ) {
        void* hndl = (void*)track->active_event;
        track->active_event = 0;
        free(hndl);
    }
}

void sequencer_drop(seq_sequencer* seq) {
    unsigned int i;
    for(i=0; i< SEQUENCER_TRACKS; ++i) {
       sequencer_destroy_active_message(i);
       sequencer_destroy_active_event(&seq->tracks[i]);
    }
    free(seq->tracks);
}


void sequencer_issue_message( unsigned int track, seq_track_msg* message) {
    sequencer_destroy_active_message(track);
    Sequencer->tracks[track].control_message = message;
}

void sequencer_set_track_event( seq_track* track, seq_track_event* event) {
    sequencer_destroy_active_event(track);
    track->active_event = event;
}

void sequencer_handle_messages() {
    unsigned int trk_id;
    log_message( CTT2_RT_MODULE_AUDIO, LOG_LEVEL_AUDMSG, "Sequencer control thread checking messages");
    for(trk_id=0; trk_id<Sequencer->n_tracks;++trk_id) {
        seq_track* track = &Sequencer->tracks[trk_id];
        if(track->control_message!=0) {
                seq_track_msg* msg = track->control_message;
                log_message( CTT2_RT_MODULE_AUDIO, LOG_LEVEL_INFO, "Sequencer Received Message %x", track->control_message );
                switch(msg->code) {
                    case msg_set_event:
						sequencer_set_track_event( track, (seq_track_event*)msg->data );
                        msg->data = 0; 
						break;
                    default:
						break;
                }
                sequencer_destroy_active_message(trk_id);
        }
    }
}

void sequencer_queue_wav( unsigned int track, hw_audio_wav_data* wav) {
    seq_track_event* event = (seq_track_event*)malloc(sizeof(seq_track_event));
    seq_track_msg* control_message = (seq_track_msg*)malloc(sizeof(seq_track_msg));
    event->evt_type = evt_loop;
    event->wav = wav;
    event->loop_smpls = 128;
    control_message->code = msg_set_event;
    control_message->data = event;
    sequencer_issue_message( track, control_message );
}


static void InitSequencer() {
    log_message(CTT2_RT_MODULE_AUDIO, LOG_LEVEL_INFO, "Sequencer bootup...");
    Sequencer = (seq_sequencer*)malloc(sizeof(seq_sequencer));
    sequencer_create( Sequencer );
}

static void DropSequencer() {
    log_message(CTT2_RT_MODULE_AUDIO, LOG_LEVEL_INFO, "Sequencer shutdown...");
	sequencer_drop(Sequencer);
    free(Sequencer);
}

static int ae_renderer( const void* inputBuffer, void* outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void* userData ) {


  
    float* out = (float*)outputBuffer;
    unsigned int tr_idx;
	unsigned int i;
    for(i=0; i<framesPerBuffer; ++i ) {

        float left = 0.0f;
        float right = 0.0f;
        for(tr_idx=0; tr_idx<Sequencer->n_tracks;++tr_idx) {
                seq_track* track = &Sequencer->tracks[tr_idx];
                track->cur_sample++;
                
                //log_message( CTT2_RT_MODULE_AUDIO, LOG_LEVEL_AUDMSG, "trackptr%x, track:%x sample:%x", track, tr_idx, track->cur_sample );
                if(track->active_event != 0 ) {
                    seq_track_event* evt = track->active_event;
					unsigned int idx_head = track->cur_sample % evt->wav->smpl_cnt;
                    left  += (float)(evt->wav->left[idx_head]) / 32767.0f;
                    right += (float)(evt->wav->right[idx_head]) / 32767.0f;
                }
                track->cur_sample++;
            }
        *out++ = left;
        *out++ = right;
    }

    /*
            unsigned int i;
            ae_renderer_context* context = (ae_renderer_context*)userData;
			float* out = (float*)outputBuffer;
            for(i=0; i<framesPerBuffer; ++i ) {
			
				
				float wave1 = sin(time*440.0f);
				float wave2 = sin(time*440.0f);
				smpl++;
				time=(float)smpl/(float)AE_SAMPLERATE;
				*out++ = wave1;
                *out++ = wave2;
            }

            if(smpl>AE_SAMPLERATE)
                smpl=smpl-AE_SAMPLERATE;
*/
            return 0;
}

void ae_init(ae_data* self) {
	int err;
	err = Pa_Initialize();
	if(err!=paNoError) {
       return;
	} 

	self->context.time = 0.0f;
    err = Pa_OpenDefaultStream( &self->stream,
                                0,
                                2,
                                paFloat32,
                                AE_SAMPLERATE,
								paFramesPerBufferUnspecified,
                                ae_renderer,
                                &self->context );
    if(err!=paNoError) {
		log_message(CTT2_RT_MODULE_AUDIO, LOG_LEVEL_ERROR, "Error:%s\n", Pa_GetErrorText( err ) );
        return;
    }

	Pa_StartStream(self->stream);
	log_message(CTT2_RT_MODULE_AUDIO, LOG_LEVEL_INFO, "Initialized audio control thread.");
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
        Sleep(AUDIO_CONTROL_MS);
        sequencer_handle_messages();
    }
    DropSequencer();
    ae_drop(self);
    return self->finalized;
}

unsigned int initAudio() {
    AE_Data.shutdown = 0;
    AE_Data.initialized = 0;
	AE_Data.stream = 0;
	InitSequencer();
    if(!SDL_CreateThread( ae_thread_run, "audio_control", &AE_Data))
		return MODULE_FAILURE;
    return MODULE_LOADED;
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
