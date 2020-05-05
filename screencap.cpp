#include "screencap.h"
#include <QScreen>
#include <QGuiApplication>
#include <QApplication>
#include <QDesktopWidget>
#include <avilib.h>

Screencap::Screencap(){}

void Screencap::start()
{
    QScreen *scr = QGuiApplication::primaryScreen();
    QPixmap pix= scr->grabWindow(QApplication::desktop()->winId());
    pix.save("D:/imgs/xx.jpg");


//    avi_t* AVI_open_output_file(char * filename); //创建一个操作输出文件的对象

//    void AVI_set_video(avi_t *AVI, int width, int height, double fps, char *compressor);  //设置视频文件的分辨率， 多少帧一秒， 编码方式

//    int  AVI_write_frame(avi_t *AVI, char *data, long bytes, int keyframe); //保存一帧图像, keyframe表示是否为关键帧(即是否完整的一帧图像数据， jpg每张都是完整的)

//    int  AVI_close(avi_t *AVI); //结束保存视频，并会写入文件索引的。



}
