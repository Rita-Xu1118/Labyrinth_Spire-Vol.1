#include "audiomanager.h"

AudioManager* AudioManager::instance()
{
    static AudioManager mgr;
    return &mgr;
}

AudioManager::AudioManager()
{
    output = new QAudioOutput();
    output->setVolume(0.5);

    player = new QMediaPlayer();
    player->setAudioOutput(output);
    player->setLoops(QMediaPlayer::Infinite);
}

AudioManager::~AudioManager()
{
    delete player;
    delete output;
}

void AudioManager::play(const QString &source)
{
    player->stop();
    player->setSource(QUrl(source));
    player->play();
}

void AudioManager::stop()
{
    player->stop();
}

void AudioManager::pause()
{
    player->pause();
}

void AudioManager::resume()
{
    player->play();
}

void AudioManager::toggle()
{
    if (player->playbackState() == QMediaPlayer::PlayingState)
        player->pause();
    else
        player->play();
}

bool AudioManager::isPlaying()
{
    return player->playbackState() == QMediaPlayer::PlayingState;
}