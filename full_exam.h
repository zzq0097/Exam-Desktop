#ifndef FULL_EXAM_H
#define FULL_EXAM_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>

namespace Ui {
class Full_Exam;
}

class Full_Exam : public QWidget
{
    Q_OBJECT

public:
    explicit Full_Exam(int id,QWidget *parent = nullptr);
    ~Full_Exam();

private:
    Ui::Full_Exam *ui;
};

#endif // FULL_EXAM_H
