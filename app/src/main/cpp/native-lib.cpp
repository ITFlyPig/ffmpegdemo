#include <jni.h>
#include <string>
#include <android/log.h>

extern "C" {
//编码
#include "libavcodec/avcodec.h"
//封装格式处理
#include "libavformat/avformat.h"
//像素处理
#include "libswscale/swscale.h"
}

#define LOGI(FORMAT, ...) __android_log_print(ANDROID_LOG_INFO,"wyl",FORMAT,##__VA_ARGS__);
#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,"wyl",FORMAT,##__VA_ARGS__);
extern "C"
JNIEXPORT jstring JNICALL
Java_com_dongnao_ffmpegdemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    av_register_all();
    return env->NewStringUTF(hello.c_str());
}


//流程：打开文件->获取流信息->找到视频流->得到解码器->循环解码获取AVPacket、AVFrame->转换->写入文件
JNIEXPORT void JNICALL
Java_com_dongnao_ffmpegdemo_MainActivity_convert(JNIEnv *env, jobject instance, jstring inputPath_,
                                                 jstring outPath_) {
    const char *inputPath = env->GetStringUTFChars(inputPath_, 0);
    const char *outPath = env->GetStringUTFChars(outPath_, 0);

    //注册各大组件
    av_register_all();

    AVFormatContext *pContext = avformat_alloc_context();

    //打开文件
    if(avformat_open_input(&pContext, inputPath, NULL, NULL) < 0){

        LOGE("文件打开失败，文件的路径:%s", inputPath);
        return;
    }

    if(avformat_find_stream_info(pContext, NULL) < 0){
        LOGE("获取流信息失败");
        return;
    }

    //找到视频流的位置
    int video_stream_idx = -1;
    for (int i = 0; i < pContext->nb_streams, i++){
        if(pContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
            video_stream_idx = i;
            break;
        }
    }
    if(video_stream_idx < 0){
        LOGE("寻找视频流的位置出错");
    }

    AVCodecContext *pCodecCtx = pContext->streams[video_stream_idx]->codec;

    AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec->id);
    //开始解码
    if(avcodec_open2(pCodecCtx, pCodec, NULL) < 0){
        LOGE("解码失败");
        return;
    }

    AVPacket *pPkt = (AVPacket *) av_malloc(sizeof(AVPacket));

    AVFrame *pFrame = (AVFrame *) av_malloc(sizeof(AVFrame));

    int got_frame;

    //获取转换的上下文
//    SwsContext *pSwsCtx = (SwsContext *) av_malloc(sizeof(SwsContext));
    SwsContext *pSwsCtx = sws_getContext(pCodecCtx->width, pCodecCtx->height,pCodecCtx->pix_fmt,
                                         pCodecCtx->width, pCodecCtx->height,AV_PIX_FMT_YUV420P, SWS_BILINEAR, NULL, NULL, NULL);




    while (av_read_frame(pContext, pPkt) >= 0){
        //由AVPacket解码得到Frame
        avcodec_decode_video2(pCodecCtx, pFrame, &got_frame, pPkt);
        if(got_frame == 0){
            LOGI("循环完成，没有剩余的frame可解析");
            break;
        }

        //转化
        if(got_frame > 0){
            sws_scale(pSwsCtx, )
        }





    }












    env->ReleaseStringUTFChars(inputPath_, inputPath);
    env->ReleaseStringUTFChars(outPath_, outPath);
}
