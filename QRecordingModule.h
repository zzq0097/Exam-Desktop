#pragma once

#include <QObject>
#include <QThread>
#include <QScreen>
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>

extern "C"
{
#include"avilib.h" 
}

class QRecordingModule : public QThread
{
	Q_OBJECT

public:
    QRecordingModule(QObject *parent = NULL);
	~QRecordingModule();

    void startRecord();
	void stopRecord();

private:
	void initAvi();
	void endAvi();

protected:
	virtual	void	run();

private:
	avi_t *out_fd;
    bool isRun;
};
