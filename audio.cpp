#include "audio.h"

// Constructeur
Audio::Audio(){
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    loadAudio();
}

int findIndex(const std::vector<std::string>& soundList, const std::string& targetName) {
    for (int i = 0; i < soundList.size(); i++) {
        if (soundList[i] == targetName) return i;
    }
    return -1;
}

void Audio::loadAudio(){

    string audioFilePath;
    Mix_Chunk *audio = NULL;
    for(int i=0; i<(int)m_soundName.size(); i++){
        audioFilePath = "resources/" + m_soundName[i] + ".wav";
        audio = Mix_LoadWAV(audioFilePath.c_str());
        m_sound.push_back(audio);
    }
}

void Audio::checkChannels(){
    for(int i=0; i<m_nbChannel; i++) if(!Mix_Playing(i)) Mix_HaltChannel(i);
}

void Audio::playSound(string soundToPlay){
	int soundID = findIndex(m_soundName,soundToPlay);
    Mix_PlayChannel(searchChannel(), m_sound[soundID], 0);
}

int Audio::searchChannel(){
    bool channelFound(false);
    int currentChannel(0);
    while(currentChannel<m_nbChannel){
        if(!Mix_Playing(currentChannel)){
            channelFound = true;
            break;
        }
        currentChannel++;
    }
    if(!channelFound){
        Mix_AllocateChannels(m_nbChannel+1);
        currentChannel=m_nbChannel;
        m_nbChannel++;
    }
    return currentChannel;
}
