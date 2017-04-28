#ifndef VIDEOPLAY_H
#define VIDEOPLAY_H

#include"videodecoder.h"
#include"sdldispaly.h"

#include<QObject>
#include<QString>

enum play_state_type{STOP=0,PLAY,PAUSE};

/***********************************
 * Summary:视频播放类
 *
 *
***********************************/
class VideoPlay: public QObject
{
    Q_OBJECT
public slots:
    void Paly();
    void Pause();
    void Stop();

public:
    //Construct Function
    VideoPlay(QWidget * widget);
    VideoPlay(QWidget * widget,QString filePath);

    ~VideoPlay();

    QString SetFilePath(QString newFilePath);  //return old filePath
    QString GetFilePath();

    enum play_state_type SetPlayState(enum play_state_type newPlayState);  //return old play state
    enum play_state_type GetPlayState();

//private functions
private:
    bool Init();
    bool UnInit();

//private vars
private:
    QString _filePath;
    enum play_state_type _playState;
    QWidget * _widget;

    SDLDispaly *_sdlHandle;

    VideoDecoder *_decoder;
};


#endif // VIDEOPLAY_H
