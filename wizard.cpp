#include "wizard.h"
#include "ui_wizard.h"
#include "mainwindow.h"
#include "global.h"
#include <QtCore>
#include <QtGui>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QObject>


Wizard::Wizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::Wizard)
{
    //ui->setupUi(this);
    prj_home_path = QDir::currentPath();
    initFirstPage();
    addPage(firstPage);


}

Wizard::~Wizard()
{
    delete ui;
}

void Wizard::initFirstPage()
{
    firstPage = new QWizardPage(this);
    lblPrjName = new QLabel();
    lblPrjPath = new QLabel();
    lePrjName = new QLineEdit();
    lePrjPath = new QLineEdit();
    choosePrjBtn = new QPushButton("...");
    connect(choosePrjBtn,SIGNAL(clicked(bool)),this,SLOT(on_choosePrjBtn_clicked()));
    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    hBoxLayout->addWidget(lePrjPath);
    hBoxLayout->addWidget(choosePrjBtn);


    lblPrjName->setText(tr("工程名"));
    lblPrjPath->setText(tr("Android工程路径"));

    QGridLayout *gridlayout = new QGridLayout();
    gridlayout->addWidget(lblPrjName,0,0);
    gridlayout->addWidget(lePrjName,0,1);
    gridlayout->addWidget(lblPrjPath,1,0);
    gridlayout->addLayout(hBoxLayout,1,1);
    firstPage->setLayout(gridlayout);
}

/*
 *暂没用第二页导航
 *
*/
void Wizard::initSecondPage()
{
    secondPage = new QWizardPage(this);

    QLabel *label = new QLabel("adfafsfddf");
    QGridLayout *layout = new QGridLayout();
    layout->addWidget(label,0,0);
    secondPage->setLayout(layout);
}

void Wizard::createPrj()
{


    QDir *qDir = new QDir();
    qDebug() << "currentPath::: " <<  QDir::currentPath();
    if(qDir->exists("Project"))
    {
        qDebug() << "Project dir is exist";
    }
    else
    {
        qDir->mkdir("Project");

        qDebug() << "create dir Project";
    }
    if(!qDir->exists("tmp"))
    {
        qDebug() << "tmp dir is not exist, then create";
        qDir->mkdir("tmp");
    }
    if(!qDir->exists("log"))
    {
        qDebug() << "log dir is not exist, then create";
        qDir->mkdir("log");
    }

    qDir->mkdir("Project/" + lePrjName->text());
    QDir::setCurrent("Project/" + lePrjName->text());

    Global::prj_name = lePrjName->text();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", Global::prj_name);
    db.setDatabaseName(lePrjName->text() + ".db");
    if(!db.open())
    {
        qDebug()<<" Wizard: create fail database";
    }
    QSqlQuery query = QSqlQuery(db);
    if(db.tables().contains("common_page"))
    {
        qDebug()<< "table common_page is exist,delete";
        if(query.exec("drop table common_page"))
        {
            qDebug()<<"drop table common_page";
        }
    }
    QString strCreatePage1 = "create table commonpage("
                       "prj_name varchar,"
                       "model varchar,"
                       "bt_name varchar,"
                        "homepage varchar,"
                        "sleep_time varchar,"
                        "def_language varchar,"
                        "def_country varchar,"
                        "display_id varchar,"
                        "wifi_state integer,"
                        "bt_state integer,"
                        "timezone varchar,"
                        "adb_state integer,"
                        "screenshot_btn integer"
                        ")";

    QString strCreatePage2 = "create table hardware("
                        "lcd_type integer,"
                        "flash_type integer,"
                        "back_cam varchar,"
                        "front_cam varchar,"
                        "simCard integer,"
                        "ddrFreq varchar"
                        ")";

    QString strInsertPage1 = "insert into commonpage values("
                             "null,"
                             "null,"
                             "null,"
                             "null,"
                             "null,"
                             "null,"
                             "null,"
                             "null,"
                             "0,"
                             "0,"
                             "null,"
                             "0,"
                             "0"
                             ")";

    QString strInsertPage2 = "insert into hardware values("
                             "0,"
                             "0,"
                             "null,"
                             "null,"
                             "0,"
                             "null"
                             ")";

    if(query.exec(strCreatePage1))
    {
        qDebug() << "fail create commonpage table";
    }
    if(query.exec(strCreatePage2))
    {
        qDebug() << "fail create hardware table";
    }
    if(query.exec(strInsertPage1))
    {
        qDebug() << "insert table commonpage fail";
        qDebug() << strInsertPage1;
    }
    if(query.exec(strInsertPage2))
    {
         qDebug() << "insert table hardware fail";
         qDebug() << strInsertPage2;
    }
    QFile cfg(lePrjName->text() + "." + "prj");
    QTextStream out(&cfg);
    if(!cfg.open(QFile::WriteOnly))
    {
        qDebug() << "cfg open fail";
    }
    out << "ProjectName=" << lePrjName->text() << endl;
    out << "ProjectPath=" << lePrjPath->text() << endl;
    cfg.close();
    //reset current path
    QDir::setCurrent(Global::prj_home_path);
#ifdef Q_OS_LINUX
    Global::textEditorPath = Global::geditTE;
#endif
    Global::textEditorPath = Global::notepad2TE;
}

void Wizard::on_choosePrjBtn_clicked()
{
    QDir *qDir = new QDir();
    QFileDialog *fileDlg = new QFileDialog();
    QString dirName =  fileDlg->getExistingDirectory(this);
    if(qDir->exists(dirName + "/wb_project"))
    {
        Global::srcPath = dirName;
        qDebug() << "this is android root dir : " << Global::srcPath;

    }
    else
    {
        QMessageBox::warning(this, tr("错误提示信息"), dirName + "不是一个有效的android源码根目录,\n请重新选择", QMessageBox::Abort);
        qDebug() << "not exist android root dir";
        return;
    }
    lePrjPath->setText(dirName);
}

void Wizard::accept()
{

    if(lePrjPath->text() == "" || lePrjName->text() == "")
    {
        QMessageBox::warning(this, tr("错误提示信息"), tr("工程名，路径名不能为空"), QMessageBox::Abort);
        qDebug() << "prjName srcPath  null";
        return;
    }

      createPrj();
      close();

}
