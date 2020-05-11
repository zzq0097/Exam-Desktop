#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QMessageBox>
#include <QSqlQuery>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPoint>
#include <QSqlDatabase>
#include <QVBoxLayout>
#include <QTimer>
#include <QDebug>
#include <QDateTime>
#include "exam.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QWidget
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void showInfo(QString info);
    void getServerTime();
    void on_pushButton_quit_clicked();
    void on_pushButton_login_clicked();
    void goExam();
private:
    Ui::Login *ui;
    QPoint dragPosition;
    int *now;
    int id;
    int start;
    int end;
    int monitor;
    int pattern;
};
#endif // LOGIN_H
