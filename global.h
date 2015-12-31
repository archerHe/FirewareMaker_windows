#ifndef GLOBAL_H
#define GLOBAL_H
#include <QString>
#include <QDir>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFile>
#include <QStringList>
#include <QProcess>
#include <QDebug>
#include <QAbstractAnimation>
#include <QAccessibleCalendarWidget>
#include <QBackingStore>
class Global
{
public:
    //this software path
    static QString prj_home_path;
    //android source path
    static QString srcPath;
    //device/rockchip/sofia3gr
    static QString devicePath;
    //frameworks/base/packages/SettingsProvider
    static QString settingproviderPath;
    static QString launcherPath;
    //i386_sofia_defconfig
    static QString kernelCfgPath;
    //SF_3GR-tablet.dts
    static QString dtsPath;
    //overlay_aosp
    static QString overlayPath;
    static QString prj_name;
    //current text editor
    static QString textEditorPath;
    //text editor on ubuntu
    static QString geditTE;
    //pdf reader on ubuntu
    static QString evincePdf;
    //text editor on windows
    static QString notepad2TE;
    //linux command on windows
    static QString mingw64;
    static QString saveErr;
    static QString serverIp;
    static QString srcAbsolutePath;
    static QString serverUsername;
    static QString serverPwd;
    static QString wb_repoPath;
    static QString model;

};
#endif
