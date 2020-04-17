#include "killprogress.h"

KillProgress::KillProgress(){}

void KillProgress::kill(QString name)
{
    QProcess *p = new QProcess;
    QString cmd = "taskkill /im "+name+" /f";
    p->execute(cmd);
    p->close();
}
