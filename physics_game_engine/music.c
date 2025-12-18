#include "raylib.h"
#include "music.h"
#include <stdlib.h>
#include <time.h>
// no. of music tracks
#define MAX_TRACK 3
//music array
const char *track[MAX_TRACK] = { //const is used so that we can not change inside of string, 

    "resources/game_music.ogg",
    "resources/game_music2.ogg",
    "resources/inverted_music.ogg"
};

Music PlayRandomMusic(){
    srand(time(NULL)); //randomness
    
    int index = rand() % MAX_TRACK;
    Music music = LoadMusicStream(track[index]);
    PlayMusicStream(music);
    return music;
}