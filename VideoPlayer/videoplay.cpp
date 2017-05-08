#include"videoplay.h"

int thread_exit=0;
int thread_pause=0;
#define SFM_REFRESH_EVENT  (SDL_USEREVENT + 1)
#define SFM_BREAK_EVENT  (SDL_USEREVENT + 2)

int sfp_refresh_thread(void *opaque){

    int * fps=(int*)opaque;
    int delayTime=1000/(*fps);

    thread_exit=0;
    thread_pause=0;

    while (thread_exit==0) {
        if(!thread_pause){
            SDL_Event event;
            event.type = SFM_REFRESH_EVENT;
            SDL_PushEvent(&event);
        }
        SDL_Delay(delayTime);
    }
    //Quit
    SDL_Event event;
    event.type = SFM_BREAK_EVENT;
    SDL_PushEvent(&event);
    thread_exit=0;
    thread_pause=0;
    return 0;
}



VideoPlay::VideoPlay(QWidget * wParent,QObject* oParent)
    :QWidget(wParent)
    ,QThread(oParent)
{
    QThread::moveToThread(this);
}

VideoPlay::VideoPlay(QWidget * wParent,QString filePath,QObject* oParent)
    :QWidget(wParent)
    ,QThread(oParent)
    ,_filePath(filePath)
{
     QThread::moveToThread(this);

}

VideoPlay::~VideoPlay()
{
    UnInit();
}

bool VideoPlay::Init()
{
    //QByteArray ba=_filePath.toLatin1();
    if(_filePath.isEmpty())
    {
        isInit=false;
        return false;
    }

    QByteArray ba=_filePath.toLocal8Bit();
    const char * filePath=ba.data();
    av_register_all();
    avformat_network_init();
    pFormatCtx = avformat_alloc_context();

    if (avformat_open_input(&pFormatCtx, filePath, NULL, NULL) != 0)	//打开文件
    {
        printf("open input stream failed \n");
        isInit=false;
        return false;
    }
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0)  //寻找流信息
    {
        printf("Couldn't find stream infomation\n");
        isInit=false;
        return false;
    }

    //寻找视频流在数组中
    videoIndex = -1;
    for (int i = 0; i < pFormatCtx->nb_streams; ++i)
    {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoIndex = i;
            break;
        }
    }
    qDebug()<<"index:"<<videoIndex<<endl;
    if (videoIndex==-1)
    {
        printf("Didn't find a vedio stream \n");
        isInit=false;
        return false;
    }

    pCodecCtx = avcodec_alloc_context3(NULL);
    if (pCodecCtx == NULL)
    {
        printf("Could not allocate AVCodecContext\n");
        isInit=false;
        return false;
    }

    avcodec_parameters_to_context(pCodecCtx,pFormatCtx->streams[videoIndex]->codecpar);

    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);  //寻找解码器
    if (pCodec == NULL)
    {
        printf("decoder not found\n");
        isInit=false;
        return false;
    }
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
    {
        printf("Could not open codec \n");
        isInit=false;
        return false;
    }

    pFrame = av_frame_alloc();
    pFrameYUV = av_frame_alloc();
    outBuffer = (uint8_t *)av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P,pCodecCtx->width, pCodecCtx->height));
    avpicture_fill((AVPicture *)pFrameYUV, outBuffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);

    //获取sws上下文
    pimgConvertCtx = sws_getContext(pCodecCtx->width,
    pCodecCtx->height,
    pCodecCtx->pix_fmt,
    this->QWidget::width(),
    this->QWidget::height(),
    AV_PIX_FMT_YUV420P,
    SWS_BICUBIC,
    NULL,
    NULL,
    NULL);



    pPacket = (AVPacket*)av_malloc(sizeof(AVPacket));

    width=pCodecCtx->width;
    height=pCodecCtx->height;

    setGeometry(QRect(0, 0, width,height));

    //获取fps
    AVStream *stream=pFormatCtx->streams[videoIndex];
    fps=stream->avg_frame_rate.num/stream->avg_frame_rate.den;

    //SDL init
    char variable[64];
#ifdef Q_OS_WIN
    sprintf(variable, "SDL_WINDOWID=0x%lx",winId());
#else
    sprintf(variable, "SDL_WINDOWID=0x%lx",winId());
#endif
    qDebug()<<variable<<endl;
    putenv(variable);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
    {
        printf("Could not initialize SDL - %s\n", SDL_GetError());
        isInit=false;
        return false;
    }

    window=SDL_CreateWindowFrom((void *)winId());

    sdlRenderer=SDL_CreateRenderer(window,-1,0);  //设置软件渲染
    sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, width, height);
    sdlRect.x = 0;
    sdlRect.y = 0;
    sdlRect.w = width;
    sdlRect.h = height;

    video_tid = SDL_CreateThread(sfp_refresh_thread,NULL,&fps);

    show();
    isInit=true;
    return true;
}

int VideoPlay::PlayVideo()
{
    while (!stopped) {
        //Wait
        SDL_WaitEvent(&event);
        if(event.type==SFM_REFRESH_EVENT){
            //------------------------------
            if(av_read_frame(pFormatCtx, pPacket)>=0){
                if(pPacket->stream_index==videoIndex){
                    ret = avcodec_decode_video2(pCodecCtx, pFrame, &gotPicture, pPacket);
                    if(ret < 0){
                        qDebug()<<"decode error"<<endl;
                        return -1;
                    }
                    if(gotPicture){
                        sws_scale(pimgConvertCtx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
                        //SDL---------------------------
                        SDL_UpdateTexture( sdlTexture, NULL, pFrameYUV->data[0], pFrameYUV->linesize[0] );
                        SDL_RenderClear( sdlRenderer );
                        //SDL_RenderCopy( sdlRenderer, sdlTexture, &sdlRect, &sdlRect );
                        SDL_RenderCopy( sdlRenderer, sdlTexture, NULL, NULL);
                        SDL_RenderPresent( sdlRenderer );
                    }
                }
                av_free_packet(pPacket);
            }else{
                //Exit Thread
                thread_exit=1;
            }
        }else if(event.type==SDL_QUIT){
            thread_exit=1;
        }else if(event.type==SFM_BREAK_EVENT){
            break;
        }

    }

    return 0;
}



bool VideoPlay::UnInit()
{
    if(_playState==PLAY || _playState==PAUSE)
    {
        thread_exit=1;
    }

    sws_freeContext(pimgConvertCtx);
    //SDL uninit
    SDL_Quit();

    SDL_DestroyWindow(window);
    //ffmpeg uninit

    av_frame_free(&pFrame);
    av_frame_free(&pFrameYUV);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);


    isInit=false;
    return true;
}

QString VideoPlay::SetFilePath(QString newFilePath)
{
    swap(this->_filePath,newFilePath);
    return newFilePath;
}

QString VideoPlay::GetFilePath()
{
    return this->_filePath;
}

enum play_state_type VideoPlay::SetPlayState(enum play_state_type newPlayState)
{
    enum play_state_type oldState;
    oldState=this->_playState;
    this->_playState=newPlayState;

    return oldState;
}
enum play_state_type VideoPlay::GetPlayState()
{
    return this->_playState;
}

void VideoPlay::run()
{
    /*
    if(!Init())
    {
        qDebug()<<"init failed"<<endl;
        return;
    }
    else
    {
        qDebug()<<"init successed"<<endl;
    }
    */
    PlayVideo();
}


bool VideoPlay::IsInit()
{
    return isInit;
}

void VideoPlay::Play()
{
    SetPlayState(PLAY);
    thread_pause=0;
}

void VideoPlay::Pause()
{
    SetPlayState(PAUSE);
    thread_pause=1;
}


void VideoPlay::Stop()
{
    SetPlayState(STOP);
    thread_exit=1;
}

int VideoPlay::GetWidth()
{
    return width;
}

int VideoPlay::GetHeight()
{
    return height;
}
