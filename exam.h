#ifndef EXAM_H
#define EXAM_H

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
    explicit Exam(Paper paper,QWidget *parent = nullptr);
    ~Exam();

private slots:
    void on_pushButton_clicked();
    void closeEvent(QCloseEvent *event);
    void updateAnswer();
private:
    Ui::Exam *ui;
    Paper paper;
    int recordid;
};

#endif // EXAM_H
