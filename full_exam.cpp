#include "full_exam.h"
#include "ui_full_exam.h"

Full_Exam::Full_Exam(int id,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Full_Exam)
{
    ui->setupUi(this);
    this->showFullScreen(); // 全屏
    this->setWindowFlags(Qt::FramelessWindowHint); // 去掉标题栏
    QVBoxLayout * Layout = new QVBoxLayout;


    for (int i=0;i<10;i++){
        Layout->addWidget(new QLabel("试卷id："+QString::number(id)));
        if (0==1){  // 选择题
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

Full_Exam::~Full_Exam()
{
    delete ui;
}
