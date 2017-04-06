#include "audiolibrary.h"
#include <QDebug>

AudioLibrary *AUDIO_LIBRARY_INSTANCE = NULL;

AudioLibrary::AudioLibrary()
{
    _sounds[SOUND_PUSH_BOX] = new QSoundEffect();
    _sounds[SOUND_PUSH_BOX]->setSource(QUrl("qrc:/sounds/push_box_new.wav"));
    qDebug() << _sounds[SOUND_PUSH_BOX]->status();
}

AudioLibrary *AudioLibrary::Instance()
{
    if(AUDIO_LIBRARY_INSTANCE == NULL)
    {
        AUDIO_LIBRARY_INSTANCE = new AudioLibrary();
    }
    return AUDIO_LIBRARY_INSTANCE;
}

void AudioLibrary::playSound(Sound sound)
{
    _sounds[sound]->play();
}

AudioLibrary::~AudioLibrary()
{
    for(int i = 0; i < N_SOUNDS; i++)
    {
        delete _sounds[i];
    }
}
