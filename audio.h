#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

#include <string>
#include <vector>

#include <SDL2/SDL_mixer.h>

using namespace std;

class Audio{

    public:

    Audio();

    void loadAudio();
    void checkChannels();

    void playSound(string soundToPlay);
    int searchChannel();

    private:

    vector<string> m_soundName = {"castle","check","gameover","move","stalemate","start","take"};
    vector<Mix_Chunk*> m_sound;

    int m_nbChannel = 0;
};

#endif
