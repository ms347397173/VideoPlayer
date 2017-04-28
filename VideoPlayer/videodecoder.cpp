#include "videodecoder.h"

VideoDecoder::VideoDecoder()
{}

bool VideoDecoder::Init(const char * filePath)
{
    av_register_all();
    avformat_network_init();
    pFormatCtx = avformat_alloc_context();

    if (avformat_open_input(&pFormatCtx, filePath, NULL, NULL) != 0)	//打开文件
    {
        printf("open input stream failed \n");
        return false;
    }
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0)  //寻找流信息
    {
        printf("Couldn't find stream infomation\n");
        return false;
    }

    //寻找视频流在数组中
    videoIndex = -1;
    for (i = 0; i < pFormatCtx->nb_streams; ++i)
    {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoIndex = i;
            break;
        }
    }
    if (videoIndex==-1)
    {
        printf("Didn't find a vedio stream \n");
        return false;
    }

    pCodecCtx = avcodec_alloc_context3(NULL);
    if (pCodecCtx == NULL)
    {
        printf("Could not allocate AVCodecContext\n");
        return false;
    }

    avcodec_parameters_to_context(pCodecCtx,pFormatCtx->streams[videoIndex]->codecpar);

    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);  //寻找解码器
    if (pCodec == NULL)
    {
        printf("decoder not found\n");
        return false;
    }
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
    {
        printf("Could not open codec \n");
        return false;
    }

    printf("时长：%d\n", pFormatCtx->duration);
    printf("封装格式：%s\n", pFormatCtx->iformat->long_name);

    pFrame = av_frame_alloc();
    pFrameYUV = av_frame_alloc();
    outBuffer = (uint8_t *)av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P,pCodecCtx->width, pCodecCtx->height));
    avpicture_fill((AVPicture *)pFrameYUV, outBuffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);


    printf("--------------- File Information ----------------\n");
    av_dump_format(pFormatCtx, 0, filePath, 0);
    printf("-------------------------------------------------\n");

    //获取sws上下文
    pimgConvertCtx = sws_getContext(pCodecCtx->width,
    pCodecCtx->height,
    pCodecCtx->pix_fmt,
    pCodecCtx->width,
    pCodecCtx->height,
    AV_PIX_FMT_YUV420P,
    SWS_BICUBIC,
    NULL,
    NULL,
    NULL);



    return true;
}

//有输出型参数
int VideoDecoder::GetYUV420pFrame(const char * * data)
{
    int linesize=0;
    if (av_read_frame(pFormatCtx, pPacket) >= 0)
    {
        if (pPacket->stream_index == videoIndex)
        {
            ret = avcodec_decode_video2(pCodecCtx, pFrame, &gotPicture, pPacket);
            if (ret < 0)
            {
                printf("Decode Error.\n");
                return -1;
            }
            if (gotPicture)
            {
                sws_scale(pimgConvertCtx, (const uint8_t * const *)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);

                //输出YUV数据 来自pFrameYUV
                *data=(const char *)pFrameYUV->data[0];
                linesize=pFrameYUV->linesize[0];
            }

        }
        av_free_packet(pPacket);  //每次调用read前调用该函数
    }
    return linesize;

}

bool VideoDecoder::UnInit()
{
    sws_freeContext(pimgConvertCtx);
    av_frame_free(&pFrame);
    av_frame_free(&pFrameYUV);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
    return true;
}
