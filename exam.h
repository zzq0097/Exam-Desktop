#ifndef EXAM_H
#define EXAM_H

#include "keyhook.h"
#include "usbmgr.h"
#include "killprogress.h"
#include "QRecordingModule.h"
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QTimer>
#include <QMessageBox>
#include <QCloseEvent>
#include <QList>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLayoutItem>
#include <QScrollArea>
#include <QFile>
#include <QNetworkRequest>
#include <QHttpMultiPart>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class Exam;
}

class Exam : public QWidget
{
    Q_OBJECT

public:
    struct Paper{
        QString name;
        QString username;
        int studentid;
        int id;
        int pattern;
        int countdown;

    };
    struct Question{
        int questionid;
        int value;
        QString content;
        QString type;
        QString option1;
        QString option2;
        QString option3;
        QString option4;
        int answerid;
        QString answer;
    };

    explicit Exam(Paper paper,QWidget *parent = nullptr);
    ~Exam();

private slots:
    void on_pushButton_clicked();
    void closeEvent(QCloseEvent *event);
    void updateAnswer();
    void uploadFile();
private:
    Ui::Exam *ui;
    Paper paper;
    int recordid;
    bool isExist;
};

#endif // EXAM_H
