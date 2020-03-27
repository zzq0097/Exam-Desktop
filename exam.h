#ifndef EXAM_H
#define EXAM_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QSqlQuery>

namespace Ui {
class Exam;
}

class Exam : public QWidget
{
    Q_OBJECT

public:
    explicit Exam(int id,QWidget *parent = nullptr);
    ~Exam();

private:
    Ui::Exam *ui;
};

#endif // EXAM_H
