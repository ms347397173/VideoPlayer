#ifndef VIDEOPLAY_H
#define VIDEOPLAY_H

#include"videodecoder.h"
#include"sdldispaly.h"

#include<QObject>
#include<QString>
#include<QtTest/QTest>
#include<QThread>

enum play_state_type{STOP=0,PLAY,PAUSE};

/***********************************
 * Summary:视频播放类
 *
 *
***********************************/
class VideoPlay: public QThread ,public QWidget
{
    Q_OBJECT
public:
    //Construct Function
    VideoPlay(QWidget * parent);
    VideoPlay(QWidget * parent,QString filePath);

    ~VideoPlay();

    QString SetFilePath(QString newFilePath);  //return old filePath
    QString GetFilePath();

    enum play_state_type SetPlayState(enum play_state_type newPlayState);  //return old play state
    enum play_state_type GetPlayState();

    bool Init();
    bool UnInit();

    int PlayVideo();
//private functions
private:



protected:
    void run();

//private vars
private:
    QString _filePath;
    enum play_state_type _playState;

    //ffmpeg vars
    AVFormatContext *pFormatCtx=NULL;
    int videoIndex=0;
    AVCodecContext * pCodecCtx=NULL;
    AVCodec *pCodec=NULL;
    AVFrame * pFrame=NULL, *pFrameYUV=NULL;
    uint8_t * outBuffer=NULL;
    AVPacket * pPacket=NULL;
    int y_size=0;
    int ret=-1, gotPicture=0;
    SwsContext *pimgConvertCtx=NULL;
    int fps=0;

    //SDL vars
    int width=0, height=0;
    SDL_Window *window=NULL;
    SDL_Renderer* sdlRenderer=NULL;
    SDL_Texture* sdlTexture=NULL;
    SDL_Rect sdlRect;
    SDL_Thread *video_tid=NULL;
    SDL_Event event;

    //thread vars
    volatile bool _stopped=false;
};


#endif // VIDEOPLAY_H
