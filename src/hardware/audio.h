#ifndef __HW_AUDIO__
#define __HW_AUDIO__

#include <SDL.h>
#include <SDL_mixer.h>

#define AUDIO_SAMPLERATE 44100
#define AUDIO_CHANNELS 2
#define AUDIO_CHUNKSIZE 2048
#define AUDIO__MAX_CHANNELS 24

void initAudio();
void dropAudio();

typedef struct {
    Mix_Chunk* ChunkData;
} audio_clip;

typedef struct {
    int track_num;
} audio_track;

void audio_create_clip(audio_clip* clip, char* clip_name);
void audio_drop_clip(audio_clip* clip);
void audio_create_track(audio_track* track);
void audio_reset_tracks();
void audio_play_clip_on_track(audio_clip* clip, audio_track* track, unsigned int loop);
void audio_set_volume_on_track(audio_track* track, double volume);
void audio_set_track_panning(audio_track* track, double pan);
#endif

