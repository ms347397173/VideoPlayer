#ifndef VIDEOPLAY_H
#define VIDEOPLAY_H

#define SDL_MAIN_HANDLED
#define __STDC_CONSTANT_MACROS
extern "C"
{

#include<libavcodec/avcodec.h>
#include<libavformat/avformat.h>
#include<libswscale/swscale.h>
#include<SDL.h>
}


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
    VideoPlay(QWidget * wParent,QObject* oParent=0);
    VideoPlay(QWidget * wParent,QString filePath,QObject* oParent=0);

    ~VideoPlay();

    QString SetFilePath(QString newFilePath);  //return old filePath
    QString GetFilePath();

    enum play_state_type SetPlayState(enum play_state_type newPlayState);  //return old play state
    enum play_state_type GetPlayState();

    bool Init();
    bool UnInit();\

    bool IsInit();

    int PlayVideo();

public:
    void Play();
    void Pause();
    void Stop();

    int GetWidth();
    int GetHeight();


//private functions
private:



protected:
    void run();

//private vars
private:
    QString _filePath;
    enum play_state_type _playState=STOP;

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
    bool isInit=false;
    bool stopped=false;

};


#endif // VIDEOPLAY_H
