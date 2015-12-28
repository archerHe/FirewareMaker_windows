#include "global.h"
#include <QString>


QString Global::prj_home_path = QDir::currentPath();
QString Global::srcPath = "";
QString Global::devicePath = "device/rockchip/sofia3gr";
QString Global::settingproviderPath = "frameworks/base/packages/SettingsProvider";
QString Global::kernelCfgPath = "kernel/arch/x86/configs/i386_sofia_defconfig";
QString Global::dtsPath  = "kernel/arch/x86/boot/dts/SF_3GR-tablet.dts";
QString Global::overlayPath = "device/rockchip/sofia3gr/overlay_aosp";
QString Global::prj_name = "";
QString Global::geditTE = "/usr/bin/gedit";
QString Global::textEditorPath = "";
QString Global::evincePdf = "/usr/bin/evince";
QString Global::notepad2TE = QDir::currentPath() + "/tools/Notepad2.exe";
QString Global::mingw64 = "D:/WorkTools/Git/usr/bin";
QString Global::saveErr = "";
