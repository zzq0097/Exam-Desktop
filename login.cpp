#include "login.h"
#include "ui_login.h"
#include "exam.h"
#include "full_exam.h"
#include <QDebug>
#include <QDateTime>
#include <QTimer>
#include <windows.h>

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("122.51.73.146");
    db.setDatabaseName("exam");
    db.setUserName("root");
    db.setPassword("zzq123456");
    if(!db.open()){
        QMessageBox::about(NULL,"Error","数据库连接失败");
    }
    getServerTime();
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_clicked()
{
    QSqlQuery query;
    query.prepare("select *from user where username = :username and password = :password");
    query.bindValue(":username",ui->username->text());
    query.bindValue(":password",ui->password->text());
    query.exec();
    if(query.size()>0){
        query.prepare("select * from "
                      "(select * from paper where courseid in "
                      "(select courseid from teach where teachid in "
                      "(select teachid from getclass where classid = "
                      "(select classid from stu_class where userid = "
                      "(select id from user where username = :username)))))a "
                      "where finishtime > now() and Hour(starttime)-Hour(now()) < 8 "
                      "ORDER BY starttime limit 1");
        query.bindValue(":username",ui->username->text());
        query.exec();
        if (query.size() < 1){
            Login::showInfo("今天没有考试");
        }
        while (query.next()) {
            int id = query.value(0).toInt();
            int start = query.value(1).toDateTime().toTime_t();
            int end = query.value(2).toDateTime().toTime_t();
            int pattern = query.value(3).toInt();
            if (start > *now){
                QString startTime = QDateTime::fromTime_t(start).toString("yyyy-MM-dd hh:mm:ss");
                Login::showInfo(QString("最近一场考试: %1").arg(startTime));
                QTimer *timer = new QTimer(this);
                connect(timer,SIGNAL(timeout()),this,SLOT(getServerTime()));
                timer->start(1000);
            }
            if (start<=*now && *now<end) {
                this->close();
                Login::goExam(id,pattern);
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
    qDebug()<<*now;
}

void Login::goExam(int id,int pattern)
{
    if (pattern == 1){
        Exam *exam = new Exam(id);
        exam->show();
    } else if (pattern == 2){
        Full_Exam *full_exam = new Full_Exam(id);
        full_exam->show();
    }
}

void Login::showInfo(QString info)
{
    ui->title->setText(info);
    ui->title->move(0,50);
    ui->pushButton_2->move(200,220);
    delete ui->l_username;
    delete ui->l_password;
    delete ui->password;
    delete ui->username;
    delete ui->pushButton;
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

void Login::on_pushButton_2_clicked()
{
    this->close();
}
