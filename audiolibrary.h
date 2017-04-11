#if 0

#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H
#include "global.h"

enum Sound
{
    SOUND_PUSH_BOX
};
#define N_SOUNDS 1

class AudioLibrary
{
public:
    static AudioLibrary *Instance();
    void playSound(Sound sound);
private:
    AudioLibrary();
    ~AudioLibrary();
};


#endif // AUDIOPLAYER_H


#endif
