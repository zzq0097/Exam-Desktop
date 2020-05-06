#include "QRecordingModule.h"
#include <QByteArray>
#include <QBuffer>
#include <QDebug>
#include <QDateTime>
#include <windows.h>

QRecordingModule::QRecordingModule(QObject *parent)
	:isRun(false)
	,	out_fd(NULL)
	
{
}

QRecordingModule::~QRecordingModule()
{
}

void QRecordingModule::startRecord()
{
	if (!isRun)
	{
		initAvi();
		isRun = true;
		start();
	}
}

void QRecordingModule::stopRecord()
{
	if (isRun)
	{
		isRun = false;
		Sleep(100);
		endAvi();
	}
}

void QRecordingModule::initAvi()
{
	QDateTime	dt = QDateTime::currentDateTime();
	QString	strFileName = "avi_" + dt.toString("yyyyMMddhhmmsszzz")+".avi";
	strFileName.toStdString();
	QByteArray	ar(strFileName.toLocal8Bit());
	char *filename = ar.data();//文件名以日期进行命名
	out_fd = AVI_open_output_file(filename);
	if (out_fd != NULL)
	{
		AVI_set_video(out_fd, 1280, 720, 20, "MJPG");
	}
	else
	{
		qDebug().noquote() << "Fail to open AVI\n";
		endAvi();
	}
}

void QRecordingModule::endAvi()
{
	AVI_close(out_fd);
	out_fd = NULL;
	isRun = false;
}

void QRecordingModule::run()
{
    QScreen *scr = QGuiApplication::primaryScreen();
	while (isRun)
	{
        QPixmap map = scr->grabWindow(QApplication::desktop()->winId()); //调用主窗口对象的捕捉窗口图像，并传递桌面窗口的id号
		map.save("/catch.jpg"); //保存图像

		//图片数据存入缓存
		QByteArray ba;
		QBuffer    bf(&ba);
		map.save(&bf, "jpg", 30); // 30表示压缩率，值从0–100, 值越小表示编码出来的图像文件就越小，当然也就越不清晰

		int res = 0;
		res = AVI_write_frame(out_fd, ba.data(), ba.size(), 0);
		if (res < 0)
		{
			qDebug().noquote() << "Fail to write frame\n";
			endAvi();
		}
		Sleep(34);

	}
}
