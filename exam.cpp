#include "exam.h"
#include "ui_exam.h"
#include "keyhook.h"

Exam::Exam(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Exam)
{
    ui->setupUi(this);

    QTimer::singleShot(50000,this,SLOT(close()));

    if (1==1){
        this->showFullScreen();  // 全屏
        KeyHook *keyhook = new KeyHook;  // 启用键盘钩子 禁用组合键
        keyhook->EnableDebugPriv();
        keyhook->Freeze();
        keyhook->setHook();
    } else if (0==2){

    }

    QVBoxLayout * Layout = new QVBoxLayout;

    Layout.set

    QSqlQuery query;
    query.prepare("select * from question where subjectid in (select questionid from paper_question where paperid = :id)");
    query.bindValue(":id",1);
    query.exec();


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
    this->setLayout(Layout);
}

Exam::~Exam()
{
    delete ui;
}
