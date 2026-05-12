#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QMediaPlayer>
#include <QAudioOutput>

class AudioManager
{
public:
    static AudioManager* instance();

    void play(const QString &source);   // 播放新音乐，自动停止旧的
    void stop();
    void pause();
    void resume();
    void toggle();
    bool isPlaying();

private:
    AudioManager();
    ~AudioManager();

    QMediaPlayer *player;
    QAudioOutput *output;
};

#endif
