#include "login.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login w;
    w.show();

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("122.51.73.146");
    db.setDatabaseName("exam");
    db.setUserName("root");
    db.setPassword("zzq123456");
    if(!db.open()){
        QMessageBox::about(NULL,"Error","数据库连接失败");
    }

    return a.exec();
}
