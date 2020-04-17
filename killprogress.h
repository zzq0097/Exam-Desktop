#ifndef KILLPROGRESS_H
#define KILLPROGRESS_H

#include <QString>
#include <QList>
#include <QProcess>

class KillProgress
{
public:
    KillProgress();
    void kill(QString name);
};

#endif // KILLPROGRESS_H
