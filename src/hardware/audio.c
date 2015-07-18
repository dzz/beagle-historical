#include "audio.h"

#include <SDL.h>
#include <SDL_mixer.h>

void initAudio() {
    Mix_Init(MIX_INIT_OGG);
    Mix_OpenAudio( AUDIO_SAMPLERATE, MIX_DEFAULT_FORMAT, AUDIO_CHANNELS, AUDIO_CHUNKSIZE );
}

void dropAudio() {
    Mix_CloseAudio();
    Mix_Quit();
}
