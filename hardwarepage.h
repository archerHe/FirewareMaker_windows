#ifndef HARDWAREPAGE_H
#define HARDWAREPAGE_H

#include <QWidget>
#include <QtGui>
#include <QtCore>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QSpacerItem>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "global.h"
#include "texthelper.h"
#include "wizard.h"

namespace Ui {
class HardwarePage;
}

class HardwarePage : public QWidget
{
    Q_OBJECT

public:
    explicit HardwarePage(QWidget *parent = 0);
    ~HardwarePage();

    void initWidget();
    void disableWidget();
    void enableWidget();

public slots:
    void loadCfg();
    void saveCfg();
private:
    Ui::HardwarePage *ui;

    TextHelper textHelper;

    QScrollArea *scrollArea;
    QWidget *common_scrollWidget;

    QSpacerItem *vSpacer;
    QSpacerItem *hSpacer;

    QLabel *lbl_screen;
    QLabel *lbl_flash;
    QLabel *lbl_back_cam;
    QLabel *lbl_front_cam;
    QLabel *lbl_sim_num;
    QLabel *lbl_ddr_fre;

    QComboBox *cb_screen;
    QComboBox *cb_flash;
    QComboBox *cb_back_cam;
    QComboBox *cb_front_cam;
    QComboBox *cb_sim_num;
    QComboBox *cb_ddr_fre;

    QHBoxLayout *hLayout;
    QVBoxLayout *vLayout;
    QGridLayout *gridLayout;

    int preBackCamId;
    int CurBackCamId;
    int preFrontCamId;
    int CurFrontCamId;
};

#endif // HARDWAREPAGE_H
