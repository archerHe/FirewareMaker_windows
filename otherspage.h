#ifndef OTHERSPAGE_H
#define OTHERSPAGE_H

#include <QWidget>
#include "global.h"

class QLabel;
class QScrollArea;
class QPushButton;
class QGridLayout;
class QVBoxLayout;
class QLineEdit;
class QCheckBox;

namespace Ui {
class OthersPage;
}

class OthersPage : public QWidget
{
    Q_OBJECT

public:
    explicit OthersPage(QWidget *parent = 0);
    ~OthersPage();

    void initWidget();
    bool copyRecursively(const QString &srcFilePath,const QString &tgtFilePath);
    bool copyDir(const QString &source, const QString &destination, bool override);
    void disableWidget();
    void enableWidget();
    void copyLogo();
    void copyPreinstall();
    void copySystemApp();
public slots:
    void selectWallpaper();
    void loadCfg();
    void saveCfg();
    void selectExtFiles();
    void copyDefWallpaper();
    void copyExtFiles();
    void selectLogo();
    void selectPreinstall();
    void selectSystemApp();
    void enableBatteryLineedit();

private:
    Ui::OthersPage *ui;


    QScrollArea     *scroll;
    QWidget         *w;
    QGridLayout     *gridLayout;
    QVBoxLayout     *vLayout;

    QLabel          *lbl_img;
    QLabel          *lbl_wallpaper;
    QLineEdit       *le_wallpaper;
    QPushButton     *btn_wallper;
    QLabel          *lbl_extFiles;
    QLineEdit       *le_extFiles;
    QPushButton     *btn_extFiles;
    QLabel          *lbl_logo;
    QLabel          *lbl_img_logo;
    QLineEdit       *le_logo;
    QPushButton *btn_logo;
    QLabel           *lbl_preinstall;
    QLineEdit       *le_preinstall;
    QPushButton *btn_preinstall;
    QLabel           *lbl_system;
    QLineEdit       *le_system;
    QPushButton *btn_system;
    QLabel           *lbl_battery;
    QLineEdit       *le_battery;
    QCheckBox   *ckb_battery;

    QString         wallpaper_600dp;
    QString         wallpaper_720dp;
    QString         wallpaper_nodp;



};

#endif // OTHERSPAGE_H
