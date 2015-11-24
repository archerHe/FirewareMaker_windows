#ifndef COMMONPAGE_H
#define COMMONPAGE_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QScrollArea>
#include <QComboBox>
#include <QPushButton>
#include <QSpacerItem>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QProcess>
#include "texthelper.h"
#include "global.h"
#include "wizard.h"

namespace Ui {
class CommonPage;
}

class CommonPage : public QWidget
{
    Q_OBJECT

public:
    explicit CommonPage(QWidget *parent = 0);
    ~CommonPage();

    void initWidget();
    void disableWidget();
    void enableWidget();


public slots:
    void loadCfg();
    void saveCfg();
    void openLanguageList();
    void openTimeZonesList();

private:
    Ui::CommonPage *ui;
    TextHelper textHelper;

    QScrollArea  *common_page_scrollArea;
    QWidget     *common_scrollWidget;

    QLabel           *lbl_model;
    QLineEdit        *le_model;
    QLabel           *lbl_bt_name;
    QLineEdit        *le_bt_name;
    QLabel           *lbl_sleep_time;
    QLineEdit        *le_sleep_time;
    QLabel           *lbl_homepage;
    QLineEdit        *le_homepage;
    QLabel           *lbl_bright_level;
    QLineEdit        *le_bright_level;
    QLabel           *lbl_displayId;
    QLineEdit         *le_displayId;
    QLabel           *lbl_brand;
    QLineEdit         *le_brand;
    QLabel           *lbl_language;
    QLineEdit        *le_language;
    QLineEdit        *le_country;
    QPushButton         *btn_languageList;
    QComboBox       *cb_language;
    QLabel          *lbl_timezone;
//    QComboBox *cb_timezone;
    QLineEdit       *le_timezone;
    QPushButton     *btn_timezoneList;
    QLabel           *lbl_date_format;
    QComboBox       *cb_date_format;
    QLabel           *lbl_volume;
    QComboBox       *cb_volume;
    QLabel          *lbl_wifi_state;
    QComboBox           *cb_wifi_state;
    QLabel          *lbl_bt_state;
    QComboBox       *cb_bt_state;
    QLabel          *lbl_install_non_market;
    QComboBox        *cb_install_non_market;
    QLabel          *lbl_adb_state;
    QComboBox       *cb_adb_state;
    QLabel          *lbl_screenshot_btn;
    QComboBox       *cb_screenshot_btn;
};

#endif // COMMONPAGE_H
