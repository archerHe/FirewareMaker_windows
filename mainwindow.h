#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "commonpage.h"
#include "texthelper.h"
#include "launcher_page.h"
#include "hardwarepage.h"
#include "functionpage.h"
#include "otherspage.h"
#include "buildpage.h"
#include <QMainWindow>
#include <QListWidget>
#include <QStackedWidget>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QScrollArea>
#include <QComboBox>
#include <QListWidgetItem>
#include <QIcon>
#include <QUiLoader>
#include <QFile>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QLabel  *lbl_statusbar;

    void initDir();
    void initMainWindow();
    void closeEvent(QCloseEvent *event);
public slots:
    void settingTEOK();
    void settingTECancel();


private slots:
    void on_actNew_triggered();

    void on_actOpen_triggered();

    void on_actSetingTE_triggered();

    void on_actSave_triggered();

    void on_actClose_triggered();

    void on_actExit_triggered();

    void on_actAbout_triggered();

    void on_actDoc_triggered();

    void on_actReload_triggered();

    void on_actApplyPage_triggered();

private:
    Ui::MainWindow *ui;

    QWidget *w;
    QWidget *wSettingTE;

    CommonPage      commonPage;
    Launcher_page   launcher_page;
    HardwarePage    hardwarePage;
    FunctionPage    functionPage;
    OthersPage      othersPage;
    BuildPage           buildPage;

    TextHelper textHelper;

    QListWidget     *listWidget;
    QStackedWidget  *stackedWidget;
    QScrollArea     *common_page_scrollArea;
    QListWidgetItem *listItemCommon;
    QListWidgetItem *listItemHardware;
    QListWidgetItem *listItemLauncher;
    QListWidgetItem *listItemFunction;
    QListWidgetItem *listitemOthers;
    QListWidgetItem *listItemBuild;

    QString prj_home_path;
    QString textEditorPath;
};

#endif // MAINWINDOW_H
