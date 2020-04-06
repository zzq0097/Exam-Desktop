#ifndef EXAM_H
#define EXAM_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QTimer>
#include <QProcess>

namespace Ui {
class Exam;
}

class Exam : public QWidget
{
    Q_OBJECT

public:
    struct Paper{
        int id;
        int pattern;
        int countdown;
    };
    explicit Exam(Paper paper,QWidget *parent = nullptr);
    ~Exam();

private slots:
    void banUSB();
private:
    Ui::Exam *ui;
};

#endif // EXAM_H
