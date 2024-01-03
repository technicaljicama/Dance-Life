#include <stdint.h>
#include <stdio.h>
#ifdef __3DS__
#include <SDL/SDL_mixer.h>
#else
#include <SDL2/SDL_mixer.h>
#endif

#include "structs.h"
#include "music.h"

static Mix_Music* song;

MusicPack mpack;
uint32_t* music;

int init_music() {
    Mix_Init(0);
    
    int result = Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512);
    if(result < 0)
    {
        printf("Unable to open audio: %s\n", SDL_GetError());
    }
    
    FILE* pack = fopen("assets/pack.ddp", "rb");
    
    fread(&mpack.mp3_filename_size, sizeof(uint32_t), 1, pack);
    mpack.mp3_file = (char *)malloc(mpack.mp3_filename_size+1);
    
    
    fread(mpack.mp3_file, mpack.mp3_filename_size, 1, pack);
    mpack.mp3_file[mpack.mp3_filename_size] = '\0';
    fread(&mpack.num_arrows, sizeof(uint32_t), 1, pack);
    music = malloc(mpack.num_arrows*sizeof(uint32_t));
    printf("Loading... %s\n", mpack.mp3_file);
    
    for(int i = 0; i < mpack.num_arrows; i++)
        fread(&music[i], sizeof(uint32_t), 1, pack);
    
    fclose(pack);
    
    printf("Loaded music pack!\n");
          
    song = Mix_LoadMUS(mpack.mp3_file);
    if(song == NULL)
        perror("Failed to load song");
    
    Mix_PlayMusic(song, 0);
    
    return 0;
}

void exit_music() {
    free(mpack.mp3_file);
    free(music);
    Mix_FreeMusic(song);
    Mix_CloseAudio();
}
