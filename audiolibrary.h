#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H
#include <QSoundEffect>
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
    QSoundEffect *_sounds[N_SOUNDS];
};


#endif // AUDIOPLAYER_H
