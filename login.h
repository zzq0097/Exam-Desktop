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

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QWidget
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();

public slots:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void goExam(int id, int pattern);
    void showInfo(QString info);
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void getServerTime();

private:
    Ui::Login *ui;
    QPoint dragPosition;
    int *now;
};
#endif // LOGIN_H
