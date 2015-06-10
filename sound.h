#ifndef SOUND_H
#define SOUND_H

#include <QObject>
#include <QMediaPlayer>
#include <QTimer>

class Sound:public QObject
{
    Q_OBJECT
public:
    Sound();
    ~Sound();
    //sound:Alert
    QMediaPlayer *alertCoin;
    QMediaPlayer *warningAlert;
    QMediaPlayer *messageAlert;
    QMediaPlayer *pauseAlert;

    //sound:music
    QMediaPlayer *welcomeMusic;
    QMediaPlayer *gameStartMusic;
    QMediaPlayer *gameOverMusic;
    QMediaPlayer *gameWinMusic;

    QMediaPlayer *gameviewBackgroundMusic;  //length: 76800
    QMediaPlayer *gameviewBackgroundMusicHurry; //length: 34500

    void soundPlay(QMediaPlayer*);
    void soundPlay_stop();
    void soundBgMusicPlay(QMediaPlayer*,int soundTimeLength);
    void soundBgMusicPlay_stop();

public slots:
    void soundBgMusicPlay_replay();

private:
    QMediaPlayer *soundBgMusicPlayer;
    QMediaPlayer *soundPlayer;
    QTimer timer,timerForBackgroundMusic;

};

#endif // SOUND_H
