#include "otherspage.h"
#include "ui_otherspage.h"

OthersPage::OthersPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OthersPage)
{
    ui->setupUi(this);

    initWidget();

}

OthersPage::~OthersPage()
{
    delete ui;
}

void OthersPage::initWidget()
{
    scroll          = new QScrollArea();
    scroll->setWidgetResizable(true);
    w               = new QWidget(scroll);
    scroll->setWidget(w);
    gridLayout      = new QGridLayout(w);

    QSpacerItem *hSpace = new QSpacerItem(40,20, QSizePolicy::Fixed, QSizePolicy::Fixed);
    lbl_wallpaper   = new QLabel("默认壁纸:");
    lbl_img         = new QLabel();
    le_wallpaper    = new QLineEdit();
    le_wallpaper->setFocusPolicy(Qt::NoFocus);
    btn_wallper     = new QPushButton("...");
    btn_wallper->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(btn_wallper, SIGNAL(clicked(bool)), this, SLOT(selectWallpaper()));
    lbl_extFiles    = new QLabel("额外打包数据:");
    le_extFiles     = new QLineEdit();
    le_extFiles->setFocusPolicy(Qt::NoFocus);
    btn_extFiles    = new QPushButton("选择额外打包数据");
    connect(btn_extFiles, SIGNAL(clicked(bool)), this, SLOT(selectExtFiles()));

    QImage img;
    img.load(":/new/img/img/black.png");
    QImage result = img.scaled(200,200);
    lbl_img->setPixmap(QPixmap::fromImage(result));
    gridLayout->setColumnStretch(1,3);
    gridLayout->addWidget(lbl_wallpaper, 0, 0, 1, 1);
    gridLayout->addWidget(lbl_img, 0, 1, 1, 2);
    gridLayout->addItem(new QSpacerItem(40,20), 0, 2);
    gridLayout->addWidget(le_wallpaper, 1, 0, 1, 3);
    gridLayout->addWidget(btn_wallper, 1, 3, 1, 1);
    gridLayout->addWidget(lbl_extFiles,3, 0);
    gridLayout->addWidget(le_extFiles, 3, 1, 1, 3);
    gridLayout->addWidget(btn_extFiles, 3,3);
    gridLayout->addItem(new QSpacerItem(20,20, QSizePolicy::Expanding, QSizePolicy::Expanding), 3, 0);
    gridLayout->setSpacing(20);
    vLayout         = new QVBoxLayout(this);
    vLayout->addWidget(scroll);
}

void OthersPage::selectWallpaper()
{
    QString def_wallpaper_path = QFileDialog::getOpenFileName(this, "选择默认壁纸", "/home/heyuan", "图片文件(*jpg)");
    if(def_wallpaper_path == "")
        return;
    le_wallpaper->setText(def_wallpaper_path);

    QImage img(def_wallpaper_path);
    QImage res = img.scaled(200,200);
    lbl_img->setPixmap(QPixmap::fromImage(res));
}

void OthersPage::loadCfg()
{
    if(Global::srcPath == "")
    {
        return;
    }
    wallpaper_600dp = Global::srcPath + "/frameworks/base/core/res/res/drawable-sw600dp-nodpi/default_wallpaper.jpg";
    wallpaper_720dp = Global::srcPath + "/frameworks/base/core/res/res/drawable-sw720dp-nodpi/default_wallpaper.jpg";
    wallpaper_nodp  = Global::srcPath + "/frameworks/base/core/res/res/drawable-nodpi/default_wallpaper.jpg";
    if(QFile::exists(wallpaper_600dp))
    {
        QImage img;
        img.load(wallpaper_600dp);
        QImage result = img.scaled(200,200);
        lbl_img->setPixmap(QPixmap::fromImage(result));
    }
}

void OthersPage::saveCfg()
{
    copyExtFiles();
    copyDefWallpaper();
}

void OthersPage::selectExtFiles()
{
    QString extFilesPath = QFileDialog::getExistingDirectory(this);
    if(extFilesPath == "")
        return;
    le_extFiles->setText(extFilesPath);
}

void OthersPage::copyDefWallpaper()
{
    if(le_wallpaper->text() == "")
    {
        return;
    }
    QFile::remove(wallpaper_600dp);
    QFile::remove(wallpaper_720dp);
    QFile::remove(wallpaper_nodp);

    QFile::copy(le_wallpaper->text(), Global::srcPath + "/frameworks/base/core/res/res/drawable-sw600dp-nodpi/default_wallpaper.jpg");
    QFile::copy(le_wallpaper->text(), Global::srcPath + "/frameworks/base/core/res/res/drawable-sw720dp-nodpi/default_wallpaper.jpg");
    QFile::copy(le_wallpaper->text(), Global::srcPath + "/frameworks/base/core/res/res/drawable-nodpi/default_wallpaper.jpg");
}

void OthersPage::copyExtFiles()
{
    if(le_extFiles->text() == "")
    {
        qDebug() << "le_extFils =";
        return;
    }

    QDir dir(Global::srcPath + "/" + Global::devicePath);
    qDebug() << Global::srcPath + "/" + Global::devicePath;
    if(!dir.exists(Global::srcPath + "/" + Global::devicePath + "/presentation_file"))
    {
        dir.mkdir("presentation_file");
    }

    copyDir(le_extFiles->text(), Global::srcPath + "/" + Global::devicePath + "/presentation_file", false);
}
/*
 bool OthersPage::copyRecursively(const QString &srcFilePath,
                            const QString &tgtFilePath)
{
    QFileInfo srcFileInfo(srcFilePath);
    if (srcFileInfo.isDir()) {
        QDir targetDir(tgtFilePath);
        targetDir.cdUp();
        if (!targetDir.mkdir(QFileInfo(tgtFilePath).fileName()))
            return false;
        QDir sourceDir(srcFilePath);
        QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
        foreach (const QString &fileName, fileNames) {
            const QString newSrcFilePath
                    = srcFilePath + QLatin1Char('/') + fileName;
            const QString newTgtFilePath
                    = tgtFilePath + QLatin1Char('/') + fileName;
            if (!copyRecursively(newSrcFilePath, newTgtFilePath))
                return false;
        }
    } else {
        if (!QFile::copy(srcFilePath, tgtFilePath))
            return false;
    }
    return true;
}
*/
 bool OthersPage::copyDir(const QString &source, const QString &destination, bool override)
 {
     QDir directory(source);
     if (!directory.exists())
     {
         return false;
     }

     QString srcPath = QDir::toNativeSeparators(source);
     if (!srcPath.endsWith(QDir::separator()))
         srcPath += QDir::separator();
     QString dstPath = QDir::toNativeSeparators(destination);
     if (!dstPath.endsWith(QDir::separator()))
         dstPath += QDir::separator();

     bool error = false;
     QStringList fileNames = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
     for (QStringList::size_type i=0; i != fileNames.size(); ++i)
     {
         QString fileName = fileNames.at(i);
         QString srcFilePath = srcPath + fileName;
         QString dstFilePath = dstPath + fileName;
         QFileInfo fileInfo(srcFilePath);
         if (fileInfo.isFile() || fileInfo.isSymLink())
         {
             if (override)
             {
                 QFile::setPermissions(dstFilePath, QFile::WriteOwner);
             }
             QFile::copy(srcFilePath, dstFilePath);
         }
         else if (fileInfo.isDir())
         {
             QDir dstDir(dstFilePath);
             dstDir.mkpath(dstFilePath);
             if (!copyDir(srcFilePath, dstFilePath, override))
             {
                 error = true;
             }
         }
     }

     return !error;
 }
