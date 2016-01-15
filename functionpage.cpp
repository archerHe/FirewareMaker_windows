#include "functionpage.h"
#include "ui_functionpage.h"
#include <QSpacerItem>
#include <QProcess>
#include <QMessageBox>
#include <QtCore>
#include <QtGui>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QDebug>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QCheckBox>
#include <QDir>
#include <QFile>
#include <QTextBrowser>
#include <QTextStream>

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

    connect(p, SIGNAL(readyRead()), this, SLOT(readAll()));
    prjFile = new QFile(Global::prj_home_path + "/" + Global::prj_name + "/" + Global::prj_name + ".prj");
    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    functionpage_scrollWidget = new QWidget(scrollArea);
    scrollArea->setWidget(functionpage_scrollWidget);
    gridLayout = new QGridLayout(functionpage_scrollWidget);

    cb_app2sd =  new QCheckBox(tr("apk安装到sd卡"));
    cb_ptest = new QCheckBox(tr("关机进入ptest模式"));
    cb_speak = new QCheckBox(tr("增大喇叭mic功率"));
    tb_readall = new QTextBrowser();
    gridLayout->setSpacing(20);
    gridLayout->addWidget(cb_app2sd, 0, 0);
    gridLayout->addWidget(cb_ptest, 1, 0);
    gridLayout->addWidget(cb_speak, 2, 0);
    gridLayout->addWidget(tb_readall, 3, 0, 3, 3);
    gridLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Expanding), 4, 0);
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
        tb_readall->append("-------------apk安装到sd卡补丁--------------");
        if(!prjFile->open(QIODevice::ReadWrite))
        {
            QMessageBox::critical(this, tr("错误提示"), Global::prj_name + tr("prj文件打开失败"));
            return;
        }
        QFile *processFile = new QFile(Global::prj_home_path + "/tmp/pCmd.bat");
        QTextStream ts(processFile);
        if(!processFile->open(QIODevice::WriteOnly))
        {
            QMessageBox::critical(this,  "ProcessFile" ,"procesFile open fail");
            return;
        }
        //QStringList strList1;
        QString str1;
        QString     cmd1 = Global::mingw64 + "/Patch";
        // strList1 << "-i"  << Global::prj_home_path + "/Patch/LanChen/app2sd/device_rockchip_sofia3gr-app2sd.patch";
        str1 = " --strip=1 -i " + Global::prj_home_path + "/Patch/LanChen/app2sd/device_rockchip_sofia3gr-app2sd.patch\r\n";
       // QDir::setCurrent(Global::srcPath + "/device/rockchip/sofia3gr");

        ts << Global::srcPath.at(0) << ":\r\n";

        ts << "cd " << Global::srcPath + "/device/rockchip/sofia3gr\r\n";
        ts << cmd1 << str1;

       // QStringList strList2;
        QString str2;
        QString cmd2 = Global::mingw64 + "/Patch";
       // strList2 << "--strip=1" << "-i" << Global::prj_home_path + "/Patch/LanChen/app2sd/frameworks.patch";
        str2 = " --strip=1 -i " + Global::prj_home_path + "/Patch/LanChen/app2sd/frameworks.patch\r\n";
     //   QDir::setCurrent(Global::srcPath + "/frameworks");
        ts << "cd " << Global::srcPath + "/frameworks\r\n";
       ts << cmd2 << str2;
        //QStringList strList3;
        QString str3;
        QString cmd3 = Global::mingw64 + "/Patch";
    //    strList3 << " -i" << Global::prj_home_path + "/Patch/LanChen/app2sd/packages-Setting.patch";
        str3 = " --strip=1 -i " + Global::prj_home_path + "/Patch/LanChen/app2sd/packages-Setting.patch\r\n";
//        QDir::setCurrent(Global::srcPath + "/packages/apps");
        ts << "cd " << Global::srcPath + "/packages\r\n";
        ts << cmd3 << str3;
//        QStringList strList4;
        QString str4;
        QString cmd4 = Global::mingw64 + "/Patch";
 //       strList4 << " -i" << Global::prj_home_path + "/Patch/LanChen/app2sd/system_vold-App2Sd-support.patch";
        str4 = " --strip=1 -i " + Global::prj_home_path + "/Patch/LanChen/app2sd/system_vold-App2Sd-support.patch\r\n";
        QDir::setCurrent(Global::srcPath + "/system/vold");

        ts << "cd " << Global::srcPath + "/system/vold\r\n";
        ts << cmd4 << str4;

        processFile->flush();
        processFile->close();
        prjFile->close();

        p->start(Global::prj_home_path + "/tmp/pCmd.bat");

    }

    if(cb_ptest->isChecked())
    {
        tb_readall->append("-------------ptest补丁--------------");
        if(QFile::exists(Global::srcPath + "/vendor/intel/images/debug/slb.elf"))
        {
            if(!QFile::remove(Global::srcPath + "/vendor/intel/images/debug/slb.elf"))
            {
                QMessageBox::critical(this, tr("ptest补丁"), tr("/vendor/intel/images/debug/slb.elf删除失败，检查是否有linux挂载到windows下目录写的权限"));
            }
        }
        if(!QFile::copy(Global::prj_home_path + "/Patch/LanChen/ptest-emmc-nand/slb.elf", Global::srcPath + "/vendor/intel/images/debug/slb.elf"))
        {
            QMessageBox::critical(this, tr("ptest补丁"), tr("ptest补丁文件拷贝失败,检查Patch/LanChen/ptest-emmc-nand下补丁的完整下，及是否有linux挂载到windows下目录写的权限"));
        }else
        {
            tb_readall->append("/vendor/intel/images/debug/slb.elf 拷贝成功");
        }


        if(QFile::exists(Global::srcPath + "/vendor/intel/images/hex/slb.hex"))
        {
            if(!QFile::remove(Global::srcPath + "/vendor/intel/images/hex/slb.hex"))
            {
                QMessageBox::critical(this, tr("ptest补丁"), tr("/vendor/intel/images/hex/slb.hex删除失败，检查是否有linux挂载到windows下目录写的权限"));
            }
        }
        if(!QFile::copy(Global::prj_home_path + "/Patch/LanChen/ptest-emmc-nand/slb.hex", Global::srcPath + "/vendor/intel/images/hex/slb.hex"))
        {
            QMessageBox::critical(this, tr("ptest补丁"), tr("ptest补丁文件slb.hex拷贝失败,检查Patch/LanChen/ptest-emmc-nand下补丁的完整下，及是否有linux挂载到windows下目录写的权限"));
        }else
        {
            tb_readall->append("/vendor/intel/images/hex/slb.hex 拷贝成功");
        }

        if(QFile::exists(Global::srcPath + "/vendor/intel/images/hex/slb.version.txt"))
        {
            if(!QFile::remove(Global::srcPath + "/vendor/intel/images/hex/slb.version.txt"))
            {
                QMessageBox::critical(this, tr("ptest补丁"), tr("/vendor/intel/images/hex/slb.version.txt"));
            }
        }
        if(!QFile::copy(Global::prj_home_path + "/Patch/LanChen/ptest-emmc-nand/slb.version.txt", Global::srcPath + "/vendor/intel/images/hex/slb.version.txt"))
        {
            QMessageBox::critical(this, tr("ptest补丁"), tr("ptest补丁文件slb.version.txt拷贝失败,检查Patch/LanChen/ptest-emmc-nand下补丁的完整下，及是否有linux挂载到windows下目录写的权限"));
        }else
        {
             tb_readall->append("/vendor/intel/images/hex/slb.version.txt 拷贝成功");
        }
        tb_readall->append("-------------ptest补丁--------------");
    }

    if(cb_speak->isChecked())
    {
        tb_readall->append("-------------增大喇叭功率mic灵敏度补丁--------------");
        if(QFile::exists(Global::srcPath + "/hardware/intel/libauddriver/aud/mhw_drv_src/audio_modem/aud/src/xmm6321/aud_nvm_sta_cal_driverif.c"))
        {
            if(!QFile::remove(Global::srcPath + "/hardware/intel/libauddriver/aud/mhw_drv_src/audio_modem/aud/src/xmm6321/aud_nvm_sta_cal_driverif.c"))
            {
                QMessageBox::critical(this, tr("增大喇叭功率mic灵敏度补丁"), tr("/hardware/intel/libauddriver/aud/mhw_drv_src/audio_modem/aud/src/xmm6321/aud_nvm_sta_cal_driverif.c"));
            }
        }
        if(!QFile::copy(Global::prj_home_path + "/Patch/LanChen/speakerMic/aud_nvm_sta_cal_driverif.c", Global::srcPath + "/hardware/intel/libauddriver/aud/mhw_drv_src/audio_modem/aud/src/xmm6321/aud_nvm_sta_cal_driverif.c"))
        {
            QMessageBox::critical(this, tr("增大喇叭功率mic灵敏度补丁"), tr("增大喇叭功率mic灵敏度补丁文件拷贝失败,检查Patch/LanChen/speakerMic下补丁的完整下，及是否有linux挂载到windows下目录写的权限"));
        }else
        {
            tb_readall->append("/hardware/intel/libauddriver/aud/mhw_drv_src/audio_modem/aud/src/xmm6321/aud_nvm_sta_cal_driverif.c拷贝成功");
        }
        tb_readall->append("-------------增大喇叭功率mic灵敏度补丁--------------");
    }

}

void FunctionPage::processFinished()
{
    QMessageBox::information(this, "补丁信息", "补丁执行完毕，检查打印信息，是否成功");
}


void FunctionPage::readAll()
{
    tb_readall->append(QString(p->readAll()));
}


