#include "buildpage.h"
#include "ui_buildpage.h"
#include "global.h"
#include <QFile>
#include <QIODevice>
#include <QMessageBox>
#include <QTextStream>
#include <QScrollBar>

BuildPage::BuildPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BuildPage)
{
    ui->setupUi(this);
    initWidget();
}

BuildPage::~BuildPage()
{
    delete ui;
}

void BuildPage::initWidget()
{
    p = new QProcess();
 //   connect(p, SIGNAL(readyReadStandardOutput()), this, SLOT(showReadStdOut()));
//    connect(p, SIGNAL(readyReadStandardError()), this, SLOT(showStdError()));
    connect(p, SIGNAL(readyRead()), this, SLOT(showReadAll()));
    connect(p, SIGNAL(finished(int)), this, SLOT(qProcessFinish()));
    connect(p, SIGNAL(started()), this, SLOT(qProcessStart()));

    QStringList listModel;
    listModel << "" << "Public_PX7S105RR300-R34A_R300" << "Public_PX7S105RR420-X734A_R400" << "Public_PX7S706RR220-X722AS_F719SR"
              << "Public_PX7S706RR220-X722C_F719SR" << "Public_PX7S80RR210-X722EL_H1008-R-wifionly" << "Public_PX7S80RR210-X722EL_S101A"
              << "Public_PX7S706RR220-X722C_F719SR-jianrong-huike-DDR" << "Public_PX7S706RR220-X722C_F719SR-Portrait" << "Public_PX7S706RR220-X722W_F719SR-wifionly"
              << "Public_PX7S706RR220-X722W_F719SR-wifionly-gps" << "Public_PX7S706RR220-X723C_F719SR-emmc" << "Public_PX7S706RR300-X722D_F719SR"
              << "Public_PX7S706RR900_R900" << "Public_PX7S706RR900_R900-wifionly" << "Public_PX7S80RR210-X722EL_H1008-R"
              << "Public_PX7S80RR210-X722EL_H1008-R-wifionly" << "Public_PX7S80RR210-X722EL_S101A" << "Public_PX7S86RR210-X722B_86RW"
              << "Public_PX7S86RR210-X723A_86RW-emmc" << "Public_S707RR110_707" << " Public_S707RR210-R22B_707"
              << "Public_S707RR210-R22B_707-LC" << "Public_S707RR210-R22B_707-LC-emmc" << "Public_S71RR200-S71"
              << "Public_S80RR210-R22B_H1007-R" << "Public_S80RR210-R22B_H1007-R-wifionly" << "Public_S80RR210-R22B_H805R"
              << "Public_S80RR210-R22B_H805R-GC0310-GC2355-wifionly" << "Public_S80RR210-R22B_S101B" << "Public_S80RR210-R22B_S112"
              << "Public_S80RR900-R22A_R900-PRG" << "Public_S80RR900-R22B_R900-PRG" << "Public-808";
    ui->cb_model->addItems(listModel);
    ui->cb_lunch_mode->addItem("userdebug");
    ui->cb_lunch_mode->addItem("user");
    ui->cb_lunch_mode->addItem("eng");

    ui->te_log->setTextColor(Qt::white);
    ui->te_log->setTextBackgroundColor(Qt::black);
    ui->te_log->document()->setMaximumBlockCount(1000);
}

void BuildPage::showReadAll()
{
    ui->te_log->append(QString(p->readAll()));
}

void BuildPage::qProcessStart()
{
    ui->btn_build->setText("Stop");
}

void BuildPage::qProcessFinish()
{
    ui->btn_build->setText("Start");
    QMessageBox::information(this, tr("提示信息～～"), tr("执行完毕～～"));
}

void BuildPage::on_btn_build_clicked()
{
    if(ui->btn_build->text() == "Stop")
    {
        p->close();
        return;
    }
    Global::model = ui->cb_model->currentText();
    /*
    if(ui->cb_model->currentText() == "")
    {
        QMessageBox::warning(this, tr("错误提示"), tr("先选择对应的公版工程型号"));
        return;
    }
    */
    QFile *f = new QFile(Global::prj_home_path + "/Project/" + Global::prj_name + "/cmd.txt");
    QTextStream ts(f);
    if(!f->open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, tr("错误提示～～"), tr("无法生成命令执行文本，请检查此路径是否有可写权限"));
        return;
    }

    if(ui->ckb_clean->isChecked())
    {
        ts << "cd " + Global::srcAbsolutePath + "\n";
        ts << "wb_repo.sh clean -df\n";
    }

    if(ui->ckb_checkout->isChecked())
    {
         ts << "cd " + Global::srcAbsolutePath + "\n";
         ts << "wb_repo.sh checkout -f\n";
    }

    if(ui->ckb_pull->isChecked())
    {
        ts << "cd " + Global::srcAbsolutePath + "\n";
        ts << "wb_repo.sh pull\n";
    }

    if(ui->ckb_installclean->isChecked())
    {
         ts << "cd " + Global::srcAbsolutePath + "\n";
         ts << "make installclean\n";
    }

   if(ui->ckb_load_prj->isChecked())
   {
       ts << "cd " + Global::srcAbsolutePath + "/wb_project/" + Global::model + "\n";
       ts << "./loading_customer_info.sh\n";
   }

    if(ui->ckb_kernel->isChecked())
    {
        ts << "cd " + Global::srcAbsolutePath + "/kernel\n";
        ts << "make i386_sofia_defconfig\n";
        ts << "make SF_3GR-tablet.dtb\n";
        ts << "make bzImage";
        if(ui->le_thread_num->text() == "")
        {
            ts << " -j4\n";
        }else
        {
            ts << " -j" + ui->le_thread_num->text() + "\n";
        }
    }
    if(ui->ckb_android->isChecked())
    {
        ts << "cd " + Global::srcAbsolutePath + "\n";
        ts << "source build/envsetup.sh\n";
        if(ui->cb_lunch_mode->currentText() == "userdebug")
        {
            ts << "lunch sofia3gr-userdebug\n";
        }else if(ui->cb_lunch_mode->currentText() == "user")
        {
            ts << "lunch sofia3gr-user\n";
        }else
        {
            ts << "lunch sofia3gr-eng\n";
        }
        ts << "setpaths\n";
        if(ui->le_thread_num->text() == "")
        {
            ts << "make -j4\n";
        }else
        {
            ts << "make -j" << ui->le_thread_num->text();
        }

        f->close();
    }

    QString cmd = Global::mingw64 + "/plink.exe";
     QStringList paraList;
     paraList << "-l" << Global::serverUsername << "-pw" << Global::serverPwd
              << Global::serverIp << "-m" << Global::prj_home_path + "/Project/" + Global::prj_name + "/cmd.txt";
     qDebug() << "cmd: " << cmd;
     qDebug() << "paralist: " << paraList;
     p->start(cmd, paraList);

}
