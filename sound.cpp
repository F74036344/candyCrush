#include <QTimer>
#include "sound.h"


Sound::Sound()
{
    //sound:alert:initialize
    warningAlert = new QMediaPlayer(this);
    messageAlert = new QMediaPlayer(this);
    pauseAlert = new QMediaPlayer(this);
    alertCoin = new QMediaPlayer(this);
    //sound:music:initialize
    welcomeMusic = new QMediaPlayer(this);
    gameStartMusic = new QMediaPlayer(this);

    gameviewBackgroundMusic = new QMediaPlayer(this);
    gameviewBackgroundMusicHurry = new QMediaPlayer(this);

    gameOverMusic = new QMediaPlayer(this);
    gameWinMusic = new QMediaPlayer(this);

    //sound:alert:setMedia
    warningAlert->setMedia(QUrl(QString("qrc:/sounds/resource/warning.wav")));
    messageAlert->setMedia(QUrl(QString("qrc:/sounds/resource/warning.wav")));
    pauseAlert->setMedia(QUrl(QString("qrc:/sounds/resource/pause.wav")));
    alertCoin->setMedia(QUrl(QString("qrc:/sounds/resource/coin.wav")));
    //sound:music:setMedia
    welcomeMusic->setMedia(QUrl(QString("qrc:/sounds/resource/game_start.wav")));
    gameStartMusic->setMedia(QUrl(QString("qrc:/sounds/resource/game_start.wav")));

    gameviewBackgroundMusic->setMedia(QUrl(QString("qrc:/sounds/resource/gameview_music.mp3")));
    gameviewBackgroundMusicHurry->setMedia(QUrl(QString("qrc:/sounds/resource/gameview_musicHurry.mp3")));

    gameOverMusic->setMedia(QUrl(QString("qrc:/sounds/resource/gameOver.mp3")));
    gameWinMusic->setMedia(QUrl(QString("qrc:/sounds/resource/gameWin.mp3")));
}

Sound::~Sound()
{

}

void Sound::soundPlay(QMediaPlayer *sdplayer)
{
    soundPlayer = sdplayer;
    if(soundPlayer->state() == QMediaPlayer::PlayingState)
        soundPlayer->setPosition(0);
    else if(soundPlayer->state() == QMediaPlayer::StoppedState
            ||soundPlayer->state() == QMediaPlayer::PausedState)
        soundPlayer->play();
}

void Sound::soundPlay_stop()
{
    soundPlayer->stop();
}

void Sound::soundBgMusicPlay(QMediaPlayer *sdplayer, int soundTimeLength)
{
    soundBgMusicPlayer = sdplayer;
    if(soundBgMusicPlayer->state() == QMediaPlayer::PlayingState)
        soundBgMusicPlayer->setPosition(0);
    else if(soundBgMusicPlayer->state() == QMediaPlayer::StoppedState
            ||soundBgMusicPlayer->state() == QMediaPlayer::PausedState)
    {
        connect(&timerForBackgroundMusic,SIGNAL(timeout()),this,SLOT(soundBgMusicPlay_replay()));
        timerForBackgroundMusic.start(soundTimeLength);
        soundBgMusicPlayer->play();
    }
}

void Sound::soundBgMusicPlay_stop()
{
    timerForBackgroundMusic.stop();
    soundBgMusicPlayer->stop();
}


void Sound::soundBgMusicPlay_replay()
{
    soundBgMusicPlayer->stop();
    soundBgMusicPlayer->play();
}


