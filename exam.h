#ifndef EXAM_H
#define EXAM_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QTimer>

namespace Ui {
class Exam;
}

class Exam : public QWidget
{
    Q_OBJECT

public:
    explicit Exam(QWidget *parent = nullptr);
    ~Exam();

private:
    Ui::Exam *ui;
    struct Paper{
        int id;
        int partten;
        int countdown;
    };
};

#endif // EXAM_H
