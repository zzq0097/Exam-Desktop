#include "exam.h"
#include "ui_exam.h"

Exam::Exam(Paper thispaper,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Exam)
{
    ui->setupUi(this);
    this->paper = thispaper;
    QTimer::singleShot(paper.countdown,this,SLOT(close()));

    if (paper.pattern==1){  // 限通信模式
        // 杀死进程
        KillProgress *killer = new KillProgress;
        killer->kill("Typora.exe");
        // 禁用USB
        UsbMgr *usbMgr = new UsbMgr;
        usbMgr->disableUSB();
        usbMgr->enableUSB();
    } else if (paper.pattern==2){  // 霸屏模式
        // 全屏
        this->showFullScreen();
        // 开启录屏
        Screencap *screencap = new Screencap;
        screencap->start();
        // 启用键盘钩子 禁用组合键
        KeyHook *keyhook = new KeyHook;
        keyhook->setHook();
        keyhook->setLock();
    }

    QVBoxLayout * Layout = new QVBoxLayout;

    QSqlQuery query;
    query.prepare("select id from user where username = :username");
    query.bindValue(":username",paper.username);
    query.exec();
    while(query.next()){
        paper.studentid = query.value(0).toInt();
    }
    query.prepare("insert into record(studentid,paperid,status) values(:studentid,:paperid,'未批改')");
    query.bindValue(":studentid",paper.studentid);
    query.bindValue(":paperid",paper.id);
    query.exec();
    recordid = query.lastInsertId().toInt();
    query.prepare("select * from question where subjectid in (select questionid from paper_question where paperid = :id)");
    query.bindValue(":id",paper.id);
    query.exec();

    ui->info->setText("试卷ID:" + QString::number(paper.id)
                      + "    考试人: " + paper.username
                      + "    考试时长：120分钟"
                      + "    考试科目：JAVA");
    int index = 0;
    while (query.next()) {
        int questionid = query.value(0).toInt();
        QString type = query.value(1).toString();
        QString content = query.value(2).toString();

        QSqlQuery query2;
        query2.prepare("insert into answer (recordid,questionid) values(:recordid,:questionid)");
        query2.bindValue(":recordid",recordid);
        query2.bindValue(":questionid",questionid);
        query2.exec();
        int answerid = query2.lastInsertId().toInt();

        index++;
        QLabel *label = new QLabel(QString::number(index)+". " + content);
        label->setObjectName(QString::number(answerid));
        Layout->addWidget(label);
        if (type=="选择"){  // 选择题
            QButtonGroup *group = new QButtonGroup;
            QRadioButton *radioA = new QRadioButton("A:"+query.value(3).toString());
            QRadioButton *radioB = new QRadioButton("B:"+query.value(4).toString());
            QRadioButton *radioC = new QRadioButton("C:"+query.value(5).toString());
            QRadioButton *radioD = new QRadioButton("D:"+query.value(6).toString());
            group->addButton(radioA);
            group->addButton(radioB);
            group->addButton(radioC);
            group->addButton(radioD);
            Layout->addWidget(radioA);
            Layout->addWidget(radioB);
            Layout->addWidget(radioC);
            Layout->addWidget(radioD);
        }
        else if (type=="判断"){  // 判断
            QButtonGroup *group = new QButtonGroup;
            QRadioButton *radioA = new QRadioButton("对");
            QRadioButton *radioB = new QRadioButton("错");
            group->addButton(radioA);
            group->addButton(radioB);
            Layout->addWidget(radioA);
            Layout->addWidget(radioB);
        }
        else if (type=="填空"){  // 填空
            Layout->addWidget(new QLineEdit());
        }
        else if (type=="简答"){  // 简答
            Layout->addWidget(new QTextEdit());
        }
        else if (type=="编程"){  // 编程
            QTextEdit *textEdit = new QTextEdit;
            Layout->addWidget(textEdit);
        }
    }
    ui->verticalLayout->addLayout(Layout);

    ui->verticalLayout->update();
}

Exam::~Exam()
{
    delete ui;
}

void Exam::closeEvent(QCloseEvent *event) //退出事件检测
{
    switch(QMessageBox::information(this,tr("提示"),tr("是否提交答案并退出？"),tr("是的"),tr("取消"),0,1))
    {
    case 0:
        event->accept();
        break;
    case 1:
    default:
        event->ignore();
        break;
    }
}

void Exam::on_pushButton_clicked()
{
    updateAnswer();
    this->close();
}
void Exam::updateAnswer()
{
    QVector<QObject *> list = ui->verticalLayoutWidget->children().toVector();
    QSqlQuery query;
    QLabel *label;
    QLineEdit *lineEdit;
    QTextEdit *textEdit;
    QRadioButton *radioButton;
    bool isQuesion = false;
    QString answer;
    for (int var = 0; var < list.size(); ++var) {
        if (isQuesion){
            query.prepare("update answer set answer = :answer where answerid = :answerid");
            query.bindValue(":answerid",label->objectName());
            query.bindValue(":answer",answer);
            if(query.exec()){
                qDebug()<<"success update for answer:" + label->objectName();
            }
        }
        if (list[var]->metaObject()->className() == QStringLiteral("QLabel")){
            label = qobject_cast<QLabel*>(list[var]);
        } else if (list[var]->metaObject()->className() == QStringLiteral("QRadioButton")){
            radioButton = qobject_cast<QRadioButton*>(list[var]);
        } else if (list[var]->metaObject()->className() == QStringLiteral("QLineEdit")){
            lineEdit = qobject_cast<QLineEdit*>(list[var]);
            answer = lineEdit->text();
            isQuesion = true;
        } else if (list[var]->metaObject()->className() == QStringLiteral("QTextEdit")){
            textEdit = qobject_cast<QTextEdit*>(list[var]);
            answer = textEdit->toHtml();
            qDebug()<<answer;
            isQuesion = true;
        }
    }
}
