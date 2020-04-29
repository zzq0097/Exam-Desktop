#include "exam.h"
#include "ui_exam.h"

Exam::Exam(Paper thispaper,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Exam)
{
    ui->setupUi(this);
    // 初始化
    this->paper = thispaper;
    int index = 0;
    QList<Question> questions1;
    QList<Question> questions2;
    QList<Question> questions3;
    QList<Question> questions4;
    QList<Question> questions5;

    QTimer::singleShot(paper.countdown,this,SLOT(close()));

    if (paper.pattern==1){  // 限通信模式
        // 杀死进程
        KillProgress *killer = new KillProgress;
        killer->kill("Typora.exe");
        // 禁用USB
        UsbMgr *usbMgr = new UsbMgr;
        usbMgr->disableUSB();
        usbMgr->enableUSB();
        // 开启录屏
        Screencap *screencap = new Screencap;
        screencap->start();
    } else if (paper.pattern==2){  // 霸屏模式
        // 全屏
        this->showFullScreen();
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

    query.prepare("select * from (select question.*,a.paperid from"
                  "(select questionid,paper.paperid from paper_question left join paper on paper.paperid=paper_question.paperid where paper.paperid=:id)a "
                  "left join question on question.subjectid=a.questionid)a, "
                  "(select strategy.value,paper.paperid from paper left join strategy on strategy.paperid=paper.paperid where paper.paperid=:id)b "
                  "where a.paperid=b.paperid");
    query.bindValue(":id",paper.id);
    query.exec();
    while (query.next()) {
        Question question;
        question.questionid = query.value("subjectid").toInt();
        question.type = query.value("type").toString();
        question.content = query.value("content").toString();
        question.value = query.value("value").toInt();
        question.option1 = query.value("option1").toString();
        question.option2 = query.value("option2").toString();
        question.option3 = query.value("option3").toString();
        question.option4 = query.value("option4").toString();

        // 插入答案记录并返回id
        QSqlQuery query2;
        query2.prepare("insert into answer (recordid,questionid) values(:recordid,:questionid)");
        query2.bindValue(":recordid",recordid);
        query2.bindValue(":questionid",question.questionid);
        query2.exec();
        question.answerid = query2.lastInsertId().toInt();

        if (question.type=="选择"){
            questions1.append(question);
        } else if (question.type=="判断"){
            questions2.append(question);
        } else if (question.type=="填空"){
            questions3.append(question);
        } else if (question.type=="简答"){
            questions4.append(question);
        } else if (question.type=="编程"){
            questions5.append(question);
        }
    }

    if (questions1.count() > 0){
        Layout->addWidget(new QLabel("一、选择题。 共"+QString::number(questions1.count())+"题"));
        for (int i=0;i<questions1.count();++i) {
            index++;
            QLabel *label = new QLabel(QString::number(index)+". " + questions1[i].content + "(分值:"+QString::number(questions1[i].value)+"分)");
            label->setObjectName(QString::number(questions1[i].answerid));
            Layout->addWidget(label);
            QButtonGroup *group = new QButtonGroup;
            QRadioButton *radioA = new QRadioButton("A:"+questions1[i].option1);
            QRadioButton *radioB = new QRadioButton("B:"+questions1[i].option2);
            QRadioButton *radioC = new QRadioButton("C:"+questions1[i].option3);
            QRadioButton *radioD = new QRadioButton("D:"+questions1[i].option4);
            group->addButton(radioA);
            group->addButton(radioB);
            group->addButton(radioC);
            group->addButton(radioD);
            Layout->addWidget(radioA);
            Layout->addWidget(radioB);
            Layout->addWidget(radioC);
            Layout->addWidget(radioD);
        }
    }

    if (questions2.count() > 0){
        Layout->addWidget(new QLabel("二、判断题。 共"+QString::number(questions2.count())+"题"));
        for (int i=0;i<questions2.count();++i) {
            index++;
            QLabel *label = new QLabel(QString::number(index)+". " + questions2[i].content + "(分值:"+QString::number(questions2[i].value)+"分)");
            label->setObjectName(QString::number(questions2[i].answerid));
            Layout->addWidget(label);
            QButtonGroup *group = new QButtonGroup;
            QRadioButton *radioA = new QRadioButton("对");
            QRadioButton *radioB = new QRadioButton("错");
            group->addButton(radioA);
            group->addButton(radioB);
            Layout->addWidget(radioA);
            Layout->addWidget(radioB);
        }
    }

    if (questions3.count() > 0){
        Layout->addWidget(new QLabel("三、判断题。 共"+QString::number(questions3.count())+"题"));
        for (int i=0;i<questions3.count();++i) {
            index++;
            QLabel *label = new QLabel(QString::number(index)+". " + questions3[i].content + "(分值:"+QString::number(questions3[i].value)+"分)");
            label->setObjectName(QString::number(questions3[i].answerid));
            Layout->addWidget(label);
            Layout->addWidget(new QLineEdit);
        }
    }

    if (questions4.count() > 0){
        Layout->addWidget(new QLabel("四、简答题。 共"+QString::number(questions4.count())+"题"));
        for (int i=0;i<questions4.count();++i) {
            index++;
            QLabel *label = new QLabel(QString::number(index)+". " + questions4[i].content + "(分值:"+QString::number(questions4[i].value)+"分)");
            label->setObjectName(QString::number(questions4[i].answerid));
            Layout->addWidget(label);
            Layout->addWidget(new QTextEdit);
        }
    }

    if (questions5.count() > 0){
        Layout->addWidget(new QLabel("五、编程题。 共"+QString::number(questions5.count())+"题"));
        for (int i=0;i<questions5.count();++i) {
            index++;
            QLabel *label = new QLabel(QString::number(index)+". " + questions5[i].content + "(分值:"+QString::number(questions5[i].value)+"分)");
            label->setObjectName(QString::number(questions5[i].answerid));
            Layout->addWidget(label);
            Layout->addWidget(new QTextEdit);
        }
    }

    this->setWindowTitle("试卷ID:" + QString::number(paper.id)
                         + "    考试人: " + paper.username
                         + "    考试时长：120分钟"
                         + "    考试科目：JAVA"
                         + "    试卷总分：100");
    ui->scrollAreaWidget->setLayout(Layout);
}

Exam::~Exam()
{
    delete ui;
}

void Exam::closeEvent(QCloseEvent *event) //退出事件检测
{
    updateAnswer();
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
    this->close();
}
void Exam::updateAnswer()
{
//    QVector<QObject *> list = ui->scrollAreaWidget->children().toVector();
//    QSqlQuery query;
//    QLabel *label;
//    QLineEdit *lineEdit;
//    QTextEdit *textEdit;
//    QRadioButton *radioButton;
//    bool isQuesion = false;
//    QString answer;
//    for (int var = 0; var < list.size(); ++var) {
//        if (isQuesion){
//            query.prepare("update answer set answer = :answer where answerid = :answerid");
//            query.bindValue(":answerid",label->objectName());
//            query.bindValue(":answer",answer);
//            if(query.exec()){
//                qDebug()<<"success update for answer:" + label->objectName();
//            }
//        }
//        if (list[var]->metaObject()->className() == QStringLiteral("QLabel")){
//            label = qobject_cast<QLabel*>(list[var]);
//        } else if (list[var]->metaObject()->className() == QStringLiteral("QRadioButton")){
//            radioButton = qobject_cast<QRadioButton*>(list[var]);
//        } else if (list[var]->metaObject()->className() == QStringLiteral("QLineEdit")){
//            lineEdit = qobject_cast<QLineEdit*>(list[var]);
//            answer = lineEdit->text();
//            isQuesion = true;
//        } else if (list[var]->metaObject()->className() == QStringLiteral("QTextEdit")){
//            textEdit = qobject_cast<QTextEdit*>(list[var]);
//            answer = textEdit->toHtml();
//            qDebug()<<answer;
//            isQuesion = true;
//        }
//    }
}
