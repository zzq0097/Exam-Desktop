#include "login.h"
#include "ui_login.h"

Exam::Paper paper;
Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
}

Login::~Login()
{
    delete ui;
}
void Login::on_pushButton_login_clicked()
{
    QSqlQuery query;
    query.prepare("select *from user where username = :username and password = :password");
    query.bindValue(":username",ui->username->text());
    query.bindValue(":password",ui->password->text());
    query.exec();
    if(query.size()>0){
        query.prepare("select *,unix_timestamp() from "
                      "(select * from paper where courseid in "
                      "(select courseid from teach where teachid in "
                      "(select teachid from getclass where classid = "
                      "(select classid from user where username = :username))))a "
                      "where finishtime > now() and Hour(starttime)-Hour(now()) < 8 "
                      "ORDER BY starttime limit 1");
        query.bindValue(":username",ui->username->text());
        query.exec();
        if (query.size() < 1){
            showInfo("今天没有考试");
        }
        while (query.next()) {
            id = query.value("paperid").toInt();
            start = query.value("starttime").toDateTime().toTime_t();
            end = query.value("finishtime").toDateTime().toTime_t();
            pattern = query.value("pattern").toInt();
            monitor = query.value("ismonitor").toInt();
            *now = query.value("unix_timestamp()").toInt();

            paper.id = id;
            paper.pattern = pattern;
            paper.username = ui->username->text();
            paper.monitor = monitor;
            paper.countdown = *now - start;

            if (start<=*now) {
                goExam();
            } else {
                QString startTime = QDateTime::fromTime_t(start).toString("yyyy-MM-dd hh:mm:ss");
                showInfo(QString("最近一场考试: %1").arg(startTime));
//                QTimer *timer = new QTimer(this);
//                connect(timer,SIGNAL(timeout()),this,SLOT(getServerTime()));
//                timer->start(1000);
                QTimer::singleShot((start-*now)*1000,this,SLOT(goExam()));
                qDebug()<<start-*now;
            }
        }
    } else {
        QMessageBox::about(NULL, "Error", "账号或密码错误");
    }
}

void Login::getServerTime() //循环获取服务器时间
{
    QSqlQuery query;
    query.exec("select unix_timestamp()");
    while (query.next()) {
        *now = query.value(0).toInt();
    }
    qDebug()<<QDateTime::fromTime_t(*now).toString("yyyy-MM-dd hh:mm:ss");
    int hour = ((start - *now)/3600)%12;
    int min = ((start - *now)/60)%60;
    int sec = (start - *now)%60;
    qDebug()<<QString("start-now:%1秒-----%2:%3：%4").arg(start-*now).arg(hour).arg(min).arg(sec);
}

void Login::showInfo(QString info)
{
    ui->title->setText(info);
    ui->title->move(0,50);
    ui->pushButton_quit->move((this->geometry().width()-ui->pushButton_quit->width())/2,220);
    delete ui->l_username;
    delete ui->l_password;
    delete ui->password;
    delete ui->username;
    delete ui->pushButton_login;
}

void Login::goExam()
{
    this->close();
    Exam *exam = new Exam(paper);
    exam->show();
}

void Login::mousePressEvent(QMouseEvent *event)//令窗口可以被拖动
{
    if(event->buttons() == Qt::LeftButton)
    {
        dragPosition = event->globalPos()-frameGeometry().topLeft();
        event->accept();
    }
}
void Login::mouseMoveEvent(QMouseEvent *event)//令窗口可以被拖动
{
    if(event->buttons() & Qt::LeftButton)
    {
        this ->move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void Login::on_pushButton_quit_clicked()
{
    this->close();
}
