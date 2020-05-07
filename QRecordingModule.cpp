#include "QRecordingModule.h"

QRecordingModule::QRecordingModule(QObject *parent){}
QRecordingModule::~QRecordingModule(){}

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
    char *filename = ar.data();
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
        QPixmap map = scr->grabWindow(QApplication::desktop()->winId());
        map.save("/catch.jpg");

		QByteArray ba;
		QBuffer    bf(&ba);
        map.save(&bf, "jpg", 30);

		int res = 0;
		res = AVI_write_frame(out_fd, ba.data(), ba.size(), 0);
		if (res < 0)
		{
            qDebug().noquote();
			endAvi();
		}
		Sleep(34);
	}
}
