#include "exam.h"
#include "ui_exam.h"

Exam::Exam(Paper thispaper,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Exam)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    // 数据初始化
    this->paper = thispaper;
    int index = 0;
    QList<Question> questions1;
    QList<Question> questions2;
    QList<Question> questions3;
    QList<Question> questions4;
    QList<Question> questions5;
    QVBoxLayout * Layout = new QVBoxLayout;
    // 定时退出
    QTimer::singleShot(paper.countdown*1000,this,SLOT(close()));
    // 获取学生ID
    QSqlQuery query;
    query.prepare("select id from user where username = :username");
    query.bindValue(":username",paper.username);
    query.exec();
    while(query.next()){
        paper.studentid = query.value(0).toInt();
    }

    // 获取或插入考试记录
    query.prepare("select * from record where studentid = :studentid and paperid = :paperid");
    query.bindValue(":studentid",paper.studentid);
    query.bindValue(":paperid",paper.id);
    query.exec();
    if (query.size()>0) { // 如果已存在记录
        while (query.next()){
            recordid = query.value(0).toInt();
            isExist = true;
            break;
        }
    } else { // 不存在记录
        query.prepare("insert into record(studentid,paperid,status) values(:studentid,:paperid,'未批改')");
        query.bindValue(":studentid",paper.studentid);
        query.bindValue(":paperid",paper.id);
        query.exec();
        recordid = query.lastInsertId().toInt();
        isExist = false;
    }
    // 模式与功能初始化
    QString info = "试卷ID:" + QString::number(paper.id)
                    + "    考试人: " + paper.username;
    if (paper.pattern==1){  // 限通信模式
        qDebug()<<"限通信模式";
        this->setWindowTitle(info);
        // 杀死进程
        KillProgress *killer = new KillProgress;
        query.exec("select process from black_list");
        while(query.next()){
            killer->kill(query.value(0).toString());
        }
        // 禁用USB
        UsbMgr *usbMgr = new UsbMgr;
        usbMgr->disableUSB();
//        usbMgr->enableUSB();
        if (paper.monitor==1){  // 是否开启录屏
            QRecordingModule *qrm = new QRecordingModule;
            qrm->startRecord(QString::number(recordid));
        }
    } else if (paper.pattern==2){  // 霸屏模式
        qDebug()<<"霸屏模式";
        // 全屏
        this->showFullScreen();
        // 启用键盘钩子 禁用组合键
        ui->scrollArea->move((this->geometry().width()-ui->scrollArea->width())/2,60);
        ui->pushButton->move((this->geometry().width()-ui->pushButton->width())/2,1080-40);
        KeyHook *keyhook = new KeyHook;
        keyhook->setHook();
        keyhook->setLock();
        Layout->addWidget(new QLabel(info));
    }
    // 获取试题并分组
    query.prepare("select distinct subjectid,type,content,option1,option2,option3,option4,answer,difficulty,chapterid,a.paperid,value from "
                  "(select question.*,a.paperid from (select questionid,paper.paperid from paper_question left join paper on "
                  "paper.paperid=paper_question.paperid where paper.paperid=:id)a  left join question on question.subjectid=a.questionid)a, "
                  "(select strategy.value,paper.paperid from paper left join strategy on strategy.paperid=paper.paperid where "
                  "paper.paperid=:id)b where a.paperid=b.paperid");
    query.bindValue(":id",paper.id);
    query.exec();
    while (query.next()) {
        Question question;
        question.questionid = query.value("subjectid").toInt();
        question.type = query.value("type").toString();
        question.content = query.value("content").toString();
        question.value = query.value("value").toInt();
        question.option1 = query.value("option1").toString();
        question.option2 = query.value("option2").toString();
        question.option3 = query.value("option3").toString();
        question.option4 = query.value("option4").toString();

        QSqlQuery query2;
        if (isExist){  // 如果考试已存在，查询已插入的答案id
            query2.prepare("select * from answer where recordid = :recordid and questionid = :questionid");
            query2.bindValue(":recordid",recordid);
            query2.bindValue(":questionid",question.questionid);
            query2.exec();
            while(query2.next()){
                question.answerid = query2.value("answerid").toInt();
                question.answer = query2.value("answer").toString();
                break;
            }
        } else {  // 插入答案记录并返回id
            query2.prepare("insert into answer (recordid,questionid) values(:recordid,:questionid)");
            query2.bindValue(":recordid",recordid);
            query2.bindValue(":questionid",question.questionid);
            query2.exec();
            question.answerid = query2.lastInsertId().toInt();
        }

        if (question.type=="选择"){
            questions1.append(question);
        } else if (question.type=="判断"){
            questions2.append(question);
        } else if (question.type=="填空"){
            questions3.append(question);
        } else if (question.type=="简答"){
            questions4.append(question);
        } else if (question.type=="编程"){
            questions5.append(question);
        }
    }
    // 生成试题控件
    if (questions1.count() > 0){
        Layout->addWidget(new QLabel("一、选择题。 共"+QString::number(questions1.count())+"题"));
        for (int i=0;i<questions1.count();++i) {
            index++;
            QLabel *label = new QLabel(QString::number(index)+". " + questions1[i].content + "(分值:"+QString::number(questions1[i].value)+"分)");
            label->setObjectName(QString::number(questions1[i].answerid));
            Layout->addWidget(label);
            QButtonGroup *group = new QButtonGroup;
            QRadioButton *radioA = new QRadioButton("A:"+questions1[i].option1);
            QRadioButton *radioB = new QRadioButton("B:"+questions1[i].option2);
            QRadioButton *radioC = new QRadioButton("C:"+questions1[i].option3);
            QRadioButton *radioD = new QRadioButton("D:"+questions1[i].option4);
            if(questions1[i].answer == "A"){
                radioA->setChecked(true);
            } else if (questions1[i].answer == "B"){
                radioB->setChecked(true);
            } else if (questions1[i].answer == "C"){
                radioC->setChecked(true);
            } else if (questions1[i].answer == "D"){
                radioD->setChecked(true);
            }
            group->addButton(radioA);
            group->addButton(radioB);
            group->addButton(radioC);
            group->addButton(radioD);
            Layout->addWidget(radioA);
            Layout->addWidget(radioB);
            Layout->addWidget(radioC);
            Layout->addWidget(radioD);
        }
    }
    if (questions2.count() > 0){
        Layout->addWidget(new QLabel("二、判断题。 共"+QString::number(questions2.count())+"题"));
        for (int i=0;i<questions2.count();++i) {
            index++;
            QLabel *label = new QLabel(QString::number(index)+". " + questions2[i].content + "(分值:"+QString::number(questions2[i].value)+"分)");
            label->setObjectName(QString::number(questions2[i].answerid));
            Layout->addWidget(label);
            QButtonGroup *group = new QButtonGroup;
            QRadioButton *radioA = new QRadioButton("对");
            QRadioButton *radioB = new QRadioButton("错");
            if(questions2[i].answer == "A"){
                radioA->setChecked(true);
            } else if (questions2[i].answer == "B"){
                radioB->setChecked(true);
            }
            group->addButton(radioA);
            group->addButton(radioB);
            Layout->addWidget(radioA);
            Layout->addWidget(radioB);
        }
    }
    if (questions3.count() > 0){
        Layout->addWidget(new QLabel("三、填空题。 共"+QString::number(questions3.count())+"题"));
        for (int i=0;i<questions3.count();++i) {
            index++;
            QLabel *label = new QLabel(QString::number(index)+". " + questions3[i].content + "(分值:"+QString::number(questions3[i].value)+"分)");
            label->setObjectName(QString::number(questions3[i].answerid));
            Layout->addWidget(label);
            QLineEdit *lineEdit = new QLineEdit;
            lineEdit->setText(questions3[i].answer);
            Layout->addWidget(lineEdit);
        }
    }
    if (questions4.count() > 0){
        Layout->addWidget(new QLabel("四、简答题。 共"+QString::number(questions4.count())+"题"));
        for (int i=0;i<questions4.count();++i) {
            index++;
            QLabel *label = new QLabel(QString::number(index)+". " + questions4[i].content + "(分值:"+QString::number(questions4[i].value)+"分)");
            label->setObjectName(QString::number(questions4[i].answerid));
            Layout->addWidget(label);
            QTextEdit *textEdit = new QTextEdit;
            textEdit->setHtml(questions4[i].answer);
            Layout->addWidget(textEdit);
        }
    }
    if (questions5.count() > 0){
        Layout->addWidget(new QLabel("五、编程题。 共"+QString::number(questions5.count())+"题"));
        for (int i=0;i<questions5.count();++i) {
            index++;
            QLabel *label = new QLabel(QString::number(index)+". " + questions5[i].content + "(分值:"+QString::number(questions5[i].value)+"分)");
            label->setObjectName(QString::number(questions5[i].answerid));
            Layout->addWidget(label);
            QPushButton *qb = new QPushButton;
            connect(qb,&QPushButton::clicked,this,[ = ] {
                chooseFile(questions5[i].questionid);
            });
            qb->setText("选择文件");
            Layout->addWidget(qb);
        }
    }
    ui->scrollAreaWidget->setLayout(Layout);
    QTimer *m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(updateAnswer()));
    m_pTimer->start(1000*10);
}

void Exam::updateAnswer()//更新答案
{
    QVector<QObject *> list = ui->scrollAreaWidget->children().toVector();
    QSqlQuery query;
    QLabel *label;
    QLineEdit *lineEdit;
    QTextEdit *textEdit;
    QRadioButton *radioButton;
    bool isQuesion = false;
    QString answer;
    QList<QString> abcd = {"","A","B","C","D"};
    int x;
    for (int var = 0; var < list.size(); ++var) {
        if (list[var]->metaObject()->className() == QStringLiteral("QLabel")){
            x = 0;
            if (isQuesion){
                query.prepare("update answer set answer = :answer where answerid = :answerid");
                query.bindValue(":answerid",label->objectName());
                query.bindValue(":answer",answer);
                if(query.exec()){
                    qDebug()<<"success update for answerid:" + label->objectName() + "||answer:" + answer;
                }
            }
            label = qobject_cast<QLabel*>(list[var]);
            if (label->objectName()==nullptr){  // 跳过大题提示
                continue;
            }
            isQuesion = true;
        } else if (list[var]->metaObject()->className() == QStringLiteral("QRadioButton")){
            radioButton = qobject_cast<QRadioButton*>(list[var]);
            ++x;
            if (radioButton->isChecked()){
                answer = abcd[x];
            }
        } else if (list[var]->metaObject()->className() == QStringLiteral("QLineEdit")){
            lineEdit = qobject_cast<QLineEdit*>(list[var]);
            answer = lineEdit->text();
        } else if (list[var]->metaObject()->className() == QStringLiteral("QTextEdit")){
            textEdit = qobject_cast<QTextEdit*>(list[var]);
            answer = textEdit->toHtml();
        }
    }
}

void Exam::on_pushButton_clicked()// 提交按钮
{
    updateAnswer();
    if (paper.monitor==1){
        uploadFile("video",0);
    }
    this->close();
}

void Exam::chooseFile(int questionid)
{
    QString filepath = QFileDialog::getOpenFileName(NULL, "请选择数据文件", "", "ZIP(*ZIP)");
    if (filepath.isEmpty())
    {
        QMessageBox::warning(NULL, "提示", "未选择数据文件", "确定");
        return;
    } else {
        uploadFile(filepath,questionid);
    }
}

void Exam::uploadFile(QString name,int questionid)// 文件上传
{
    QNetworkAccessManager manager;
    QUrl url;
    url.setScheme("ftp");
    url.setUserName("zzq");
    url.setPassword("zzq123");
    url.setHost("122.51.73.146");
    url.setPort(21);
    QString fileName;
    if (name == "video"){
        // 本地文件路径
        fileName = QDir::currentPath()+"/avi_"+QString::number(recordid)+".avi";
        // ftp 服务器文件路径
        url.setPath("/video/avi_"+QString::number(recordid)+".avi");
    } else {
        fileName = name;
        qDebug()<<"/code/r"+QString::number(recordid)+"q"+QString::number(questionid)+".zip";
        url.setPath("/code/r"+QString::number(recordid)+"q"+QString::number(questionid)+".zip");
    }
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    file.close();
    QNetworkRequest request(url);
    QNetworkReply* reply = manager.put(request, data);
    QEventLoop eventLoop;
    QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Error: " << reply->errorString();
        QMessageBox::about(NULL,"提示","上传失败，请联系管理员");
    } else {
        qDebug() << "Success: " << "上传成功！->"<<fileName;
        QMessageBox::about(NULL,"提示","上传成功");
    }
}

void Exam::closeEvent(QCloseEvent *event)//退出事件检测
{
    switch(QMessageBox::information(this,tr("提示"),tr("是否提交答案并退出？"),tr("是的"),tr("取消"),0,1))
    {
    case 0:
        event->accept();
        break;
    case 1:
    default:
        event->ignore();
        break;
    }
}

Exam::~Exam()
{
    delete ui;
}
