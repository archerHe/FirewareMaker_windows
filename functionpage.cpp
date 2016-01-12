#include "functionpage.h"
#include "ui_functionpage.h"
#include <QSpacerItem>
#include <QProcess>
#include <QMessageBox>

FunctionPage::FunctionPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FunctionPage)
{
  //  ui->setupUi(this);
    initWidget();
  //  disableWidget();
}

FunctionPage::~FunctionPage()
{
    delete ui;
}


void FunctionPage::disableWidget()
{
    QList<QObject *> list = functionpage_scrollWidget->children();
    QWidget *w;
    for(int i = 1; i < list.count(); i++)
    {
        w = qobject_cast<QWidget *>(list.at(i));
        w->setDisabled(true);
    }
}

void FunctionPage::enableWidget()
{
    QList<QObject *> list = functionpage_scrollWidget->children();
    QWidget *w;
    for(int i = 1; i < list.count(); i++)
    {
        w = qobject_cast<QWidget *>(list.at(i));
        w->setEnabled(true);
    }
}

void FunctionPage::initWidget()
{
    p = new QProcess(this);
    connect(p, SIGNAL(readyReadStandardError()), this, SLOT(readErr()));
    connect(p, SIGNAL(readyReadStandardOutput()), this, SLOT(readStdOut()));
    prjFile = new QFile(Global::prj_home_path + "/" + Global::prj_name + "/" + Global::prj_name + ".prj");
    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    functionpage_scrollWidget = new QWidget(scrollArea);
    scrollArea->setWidget(functionpage_scrollWidget);
    gridLayout = new QGridLayout(functionpage_scrollWidget);



    cb_app2sd =  new QCheckBox(tr("apk安装到sd卡"));


    cb_ptest = new QCheckBox(tr("关机进入ptest模式"));


    cb_speak = new QCheckBox(tr("增大喇叭mic功率"));


    gridLayout->setSpacing(20);
    gridLayout->addWidget(cb_app2sd, 0, 0);
    gridLayout->addWidget(cb_ptest, 1, 0);
    gridLayout->addWidget(cb_speak, 2, 0);
    gridLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Expanding), 3, 0);
    vLayout = new QVBoxLayout(this);
    vLayout->addWidget(scrollArea);

}

void FunctionPage::loadCfg()
{
    enableWidget();
}

void FunctionPage::saveCfg()
{
    if(cb_app2sd->checkState())
    {
        if(!prjFile->open(QIODevice::ReadWrite))
        {
            QMessageBox::critical(this, tr("错误提示"), Global::prj_name + tr("prj文件打开失败"));
            return;
        }

        QStringList strList1;
        QString     cmd1 = Global::mingw64 + "/patch";
         strList1 << "-i"  << Global::prj_home_path + "/patch/LanChen/app2sd/device_rockchip_sofia3gr-app2sd.patch";
        QDir::setCurrent(Global::srcPath + "/device/rockchip/sofia3gr");
        p->execute(cmd1, strList1);


        QStringList strList2;
        QString cmd2 = Global::mingw64 + "/patch";
        strList2 << "--strip=1" << "-i" << Global::prj_home_path + "/patch/LanChen/app2sd/frameworks.patch";
        QDir::setCurrent(Global::srcPath + "/frameworks");
        qDebug() << QDir::currentPath();
        qDebug() << strList2;
        p->execute(cmd2, strList2);

        QStringList strList3;
        QString cmd3 = Global::mingw64 + "/patch";
        strList3 << " -i" << Global::prj_home_path + "/patch/LanChen/app2sd/packages- Setting.patch";
        QDir::setCurrent(Global::srcPath + "/packages/apps");
    //    p->execute(cmd3, strList3);

        QStringList strList4;
        QString cmd4 = Global::mingw64 + "/patch";
        strList3 << " -i" << Global::prj_home_path + "/patch/LanChen/app2sd/system_vold-App2Sd-support.patch";
        QDir::setCurrent(Global::srcPath + "system/vold");
     //   p->execute(cmd4, strList4);

        qDebug() << "Functionpage:: saveCfg()";
    }
}

void FunctionPage::readErr()
{
    qDebug() << "QProcess err:  " << p->readAllStandardError();
}

void FunctionPage::readStdOut()
{

}


