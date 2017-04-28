#ifndef VIDEODECODER_H
#define VIDEODECODER_H

#define __STDC_CONSTANT_MACROS
extern "C"
{

#include<libavcodec/avcodec.h>
#include<libavformat/avformat.h>
#include<libswscale/swscale.h>
}
#include<QString>
#include<QDebug>
class VideoDecoder
{

public:
    VideoDecoder();
    bool Init(const char * filePath);
    bool UnInit();
    //bool IsInit();

    int GetYUV420pFrame(const char * * data);  //return linesize

private:
    //ffmpeg vars
    AVFormatContext *pFormatCtx=NULL;
    int i = 0, videoIndex=0;
    AVCodecContext * pCodecCtx=NULL;
    AVCodec *pCodec=NULL;
    AVFrame * pFrame=NULL, *pFrameYUV=NULL;
    uint8_t * outBuffer=NULL;
    AVPacket * pPacket=NULL;
    int y_size=0;
    int ret=-1, gotPicture=0;
    SwsContext *pimgConvertCtx=NULL;
};

#endif // VIDEODECODER_H
