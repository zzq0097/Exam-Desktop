#include "screencap.h"
#include <QScreen>
#include <QGuiApplication>
#include <QApplication>
#include <QDesktopWidget>

Screencap::Screencap(){}

void Screencap::start()
{
    QScreen *scr = QGuiApplication::primaryScreen();
    QPixmap pix= scr->grabWindow(QApplication::desktop()->winId());
    pix.save("D:/imgs/xx.jpg");
}
