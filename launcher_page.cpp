#include "launcher_page.h"
#include "ui_launcher_page.h"
#include "global.h"
#include <QWidget>
#include <QScrollArea>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include <QImage>
#include <QMessageBox>

Launcher_page::Launcher_page(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Launcher_page)
{
   // ui->setupUi(this);

    initWidget();
    disableWidget();
}

Launcher_page::~Launcher_page()
{
    delete ui;
}

void Launcher_page::initWidget()
{
    p = new QProcess(this);
    connect(p, SIGNAL(readyRead()), this, SLOT(showResult()));
    connect(p, SIGNAL(readyReadStandardError()), this, SLOT(showError()));
    connect(p, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(showState(QProcess::ProcessState)));
    lbl_wallpaper = new QLabel("添加壁纸路径:");
    lbl_wallpaper->setToolTip("需要提前准备小分辨率图片");
    le_wallpaper = new QLineEdit();
    le_wallpaper->setFixedWidth(500);
    btn_select_wallper = new QPushButton("选择添加壁纸文件夹");
    connect(btn_select_wallper, SIGNAL(clicked(bool)), this, SLOT(btn_wallpaper_choose()));
    btn_open_wallpaper_dir = new QPushButton(tr("打开源码壁纸文件夹"));
    connect(btn_open_wallpaper_dir, SIGNAL(clicked(bool)), this, SLOT(btnOpenWallpaperDir()));
    lbl_icon_site = new QLabel("桌面图标摆放:");
    btn_icon_site = new QPushButton("打开配置文件");
    btn_icon_site->setFixedWidth(100);
    btn_icon_site->setToolTip("打开桌面图标设置xml文件");

    lbl_def_wallpaper   = new QLabel("默认壁纸:");
    lbl_img         = new QLabel();

    btn_def_wallper     = new QPushButton("。。。");
    btn_def_wallper->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    lbl_extFiles    = new QLabel("额外打包数据:");
    le_extFiles     = new QLineEdit();
    btn_extFiles    = new QPushButton("选择壁纸");

    hSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);
    vSpacer = new QSpacerItem(20, 800, QSizePolicy::Expanding, QSizePolicy::Expanding);

    scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scrollWidget = new QWidget(scroll);
    scroll->setWidget(scrollWidget);
    gridLayout = new QGridLayout(scrollWidget);
    gridLayout->addWidget(lbl_wallpaper, 0, 0);
    gridLayout->addWidget(le_wallpaper, 0, 1);
    gridLayout->addWidget(btn_select_wallper, 0, 2);
    gridLayout->addWidget(btn_open_wallpaper_dir, 0, 3);
    gridLayout->addItem(hSpacer, 0, 4);
    gridLayout->addWidget(lbl_icon_site, 1, 0);
    gridLayout->addWidget(btn_icon_site, 1, 1);
    gridLayout->addItem(vSpacer, 2, 0);

    vLayout = new QVBoxLayout(this);
    vLayout->addWidget(scroll);


}

void Launcher_page::disableWidget()
{
    QList<QObject *> list = scrollWidget->children();
    QWidget *w;
    for(int i = 1; i < list.count(); i++)
    {
        w = qobject_cast<QWidget *>(list.at(i));
        w->setDisabled(true);
    }
}

void Launcher_page::enableWidget()
{
    QList<QObject *> list = scrollWidget->children();
    QWidget *w;
    for(int i = 1; i < list.count(); i++)
    {
        w = qobject_cast<QWidget *>(list.at(i));
        w->setEnabled(true);
    }
}

void Launcher_page::cpWallpaper(QString extWallpaperPath)
{
    /* 用bash脚本拷贝壁纸
    QStringList strList;
    QString command = Global::mingw64 + "\\bash.exe";
    strList << "test.sh" << Global::mingw64 << Global::srcPath << extWallpaperPath << "2";
    qDebug() << command;
   // strList << Global::mingw64 << Global::prj_home_path + "\\bin\\bash_helper.sh"<< Global::srcPath << extWallpaperPath << "2";
   qDebug() << strList;
    p->start("bash", strList);
*/

    QDir *extDir = new QDir(extWallpaperPath);
    QStringList fileList = extDir->entryList();
    QString pathWallpaperOverlay = Global::srcPath + "/" + Global::overlayPath + "/packages/apps/Launcher3/res/drawable-nodpi";
    QString wallpaperXml =  Global::srcPath + "/packages/apps/Launcher3/res/values/wallpapers.xml";
    QDir *overlayDir = new QDir(pathWallpaperOverlay);
    if(fileList.length() > 2)
    {
        if(!overlayDir->mkpath(pathWallpaperOverlay))
        {
            qDebug() << "mkpath fail.. " << pathWallpaperOverlay;
            QMessageBox::critical(this, tr("错误警告 : "), tr("创建目录") + pathWallpaperOverlay + tr("失败,请检查目标路径是否有可写权限，或目标路径被文件浏览器打开"));
            return;
        }
        for(int i = 2; i < fileList.length(); i++)
        {
            QStringList nameList = fileList[i].split(".");
            QImage srcImg(extWallpaperPath + "/" + fileList[i]);
            qDebug() << extWallpaperPath + "/" + fileList[i];
            QImage smlImg = srcImg.scaled(200,200);
            if(!smlImg.save(pathWallpaperOverlay + "/" + nameList[0] + "_small." + nameList[1]))
            {
                qDebug() << "small image save fail";
            }
            QFile::copy(extWallpaperPath + "/" + fileList[i], pathWallpaperOverlay +"/" + fileList[i]);
            qDebug() << pathWallpaperOverlay + "/" + nameList[0] + "_small." + nameList[1];
            textHelper.addWallpaperXml(wallpaperXml, nameList[0]);
        }
    }else
    {
        QMessageBox::information(this, tr("提示～～～"), tr("壁纸文件夹内空。。。。。"));
    }
}

void Launcher_page::loadCfg()
{
    enableWidget();
}

void Launcher_page::saveCfg()
{
    if(le_wallpaper->text() == "")
    {
        return;
    }
    cpWallpaper(le_wallpaper->text());
}

void Launcher_page::btn_wallpaper_choose()
{
    if(Global::srcPath == "")
    {
        QMessageBox::warning(this, "提示框", "必须先新建工厂或导入现有工程才可以修改！！", QMessageBox::Abort);
        return;
    }
    QFileDialog *fileDlg = new QFileDialog();
    QString wallpaperPath = fileDlg->getExistingDirectory(this);
    le_wallpaper->setText(wallpaperPath);
}

void Launcher_page::btnOpenWallpaperDir()
{
    if(Global::srcPath == "")
    {
        QMessageBox::warning(this, "提示框", "必须先新建工厂或导入现有工程才可以修改！！", QMessageBox::Abort);
        return;
    }
    QDesktopServices::openUrl(QUrl(Global::srcPath + "/packages/apps/Launcher3/res/drawable-sw600dp-nodpi", QUrl::TolerantMode));
}

void Launcher_page::showResult()
{
    qDebug() << "showResult: " << QString(p->readAll());
}

void Launcher_page::showState(QProcess::ProcessState state)
{
    if(state == QProcess::NotRunning)
        qDebug() << "process notRunning";
    else if(state == QProcess::Starting)
        qDebug() << "process starting";
    else{
        qDebug() << "Running";
    }
}

void Launcher_page::showError()
{
    qDebug() << "showError: " << p->readAllStandardError();
}


