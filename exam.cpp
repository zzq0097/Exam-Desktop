#include "exam.h"
#include "ui_exam.h"
#include "keyhook.h"
#include "usbmgr.h"

Exam::Exam(Paper paper,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Exam)
{
    ui->setupUi(this);

    QTimer::singleShot(paper.countdown,this,SLOT(close()));


    if (0==1){  // 霸屏模式
        this->showFullScreen();  // 全屏
        KeyHook *keyhook = new KeyHook;  // 启用键盘钩子 禁用组合键
        keyhook->setHook();
    } else if (2==2){  // 限通信模式
        QProcess *p = new QProcess(this);
        QString pname = "TIM.exe";
        QString cmd = "taskkill /im "+pname+" /f";
        p->execute(cmd);
        p->close();

        UsbMgr *usbMgr = new UsbMgr;
        usbMgr->disableUSB();
        usbMgr->enableUSB();
    }

    QVBoxLayout * Layout = new QVBoxLayout;

    QSqlQuery query;
    query.prepare("select * from question where subjectid in (select questionid from paper_question where paperid = :id)");
    query.bindValue(":id",paper.id);
    query.exec();

    ui->info->setText("试卷ID:" + QString::number(paper.id)
                      + "    考试人: 123"
                      + "    考试时长：120分钟"
                      + "    考试科目：JAVA");
    int index = 0;
    while (query.next()) {
        index++;
        Layout->addWidget(new QLabel(QString::number(index)+". "+query.value(2).toString()));
        if (1==1){  // 选择题
            Layout->addWidget(new QLineEdit());
        }
        if (0==2){  // 判断
            Layout->addWidget(new QLineEdit());
        }
        if (0==3){  // 填空
            Layout->addWidget(new QLineEdit());
        }
        if (0==4){  // 简答
            Layout->addWidget(new QTextEdit());
        }
        if (0==5){  // 编程
            Layout->addWidget(new QLineEdit());
        }

    }
    ui->verticalLayout->addLayout(Layout);;
}

Exam::~Exam()
{
    delete ui;
}

