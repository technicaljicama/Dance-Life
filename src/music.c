#ifdef __3DS__
#include <SDL/SDL_mixer.h>
#else
#include <SDL2/SDL_mixer.h>
#endif

#include "music.h"

static Mix_Music* song;

int init_music() {
    Mix_Init(0);
    
    int result = Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512);
    if(result < 0)
    {
        printf("Unable to open audio: %s\n", SDL_GetError());
    }
    
    song = Mix_LoadMUS("assets/ddd.mp3");
    if(song == NULL)
        perror("Failed to load song");
    
    Mix_PlayMusic(song, 0);
    
    return 0;
}

void exit_music() {
    Mix_FreeMusic(song);
    Mix_CloseAudio();
}
