#include "hardwarepage.h"
#include "ui_hardwarepage.h"
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

HardwarePage::HardwarePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HardwarePage)
{
    ui->setupUi(this);

    initWidget();
    disableWidget();
}

HardwarePage::~HardwarePage()
{
    delete ui;
}

void HardwarePage::initWidget()
{
    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    common_scrollWidget = new QWidget(scrollArea);
    scrollArea->setWidget(common_scrollWidget);
    gridLayout = new QGridLayout(common_scrollWidget);

    vSpacer = new QSpacerItem(20, 80, QSizePolicy::Expanding, QSizePolicy::Expanding);

    hSpacer = new QSpacerItem(1000, 20, QSizePolicy::Fixed, QSizePolicy::Fixed);

    lbl_screen = new QLabel("LCD类型:");

    lbl_flash = new QLabel("Flash 类型:");
    lbl_back_cam = new QLabel("后置摄像头:");
    lbl_front_cam = new QLabel("前置摄像头:");
    lbl_sim_num  = new QLabel("Sim卡");
    lbl_ddr_fre = new QLabel("DDR频率:");
    lbl_band    = new QLabel("3G频段");
    lbl_band->setToolTip("band1  2100\nband2  1900\nband5  850\nband8  900");



    cb_screen = new QComboBox();
    cb_screen->addItem("TN");
    cb_screen->addItem("IPS");

    cb_flash = new QComboBox();
    cb_flash->addItem("Nand");
    cb_flash->addItem("Emmc");
    cb_back_cam = new QComboBox();
    cb_back_cam->addItem("ov13850");
    cb_back_cam->addItem("OV8858");
    cb_back_cam->addItem("OV5648");
    cb_back_cam->addItem("gc2155");
    cb_back_cam->addItem("sp2508");
    cb_back_cam->addItem("ov2680");
    cb_front_cam = new QComboBox();
    cb_front_cam->addItem("gc2355");
    cb_front_cam->addItem("gc0310");
    cb_front_cam->addItem("sp0a20");
    cb_front_cam->addItem("hm2051");
    cb_sim_num = new QComboBox();
    cb_sim_num->addItem("双卡");
    cb_sim_num->addItem("单卡");
    cb_ddr_fre = new QComboBox();
    cb_ddr_fre->addItem("312");
    cb_ddr_fre->addItem("400");
    cb_ddr_fre->addItem("455");
    cb_band = new QComboBox();
    QStringList bandList;
    bandList << "weibu_band18_1530" << "weibu_band1_1530" << "weibu_band15_1530" << "weibu_band25_1530" << "weibu_band125_1530"
             << "weibu_band128_1530" << "weibu_band158_1530" << "weibu_band258_1530" << "weibu_band1258_1530" << "weibu_band1_GSM900-DCS1800_1530";
    cb_band->addItems(bandList);



    gridLayout->addWidget(lbl_screen, 0, 0);
    gridLayout->addWidget(cb_screen, 0, 1);
    gridLayout->addItem(hSpacer, 0, 2);
    gridLayout->addWidget(lbl_flash, 1, 0);
    gridLayout->addWidget(cb_flash, 1, 1);
    gridLayout->addWidget(lbl_back_cam, 2, 0);
    gridLayout->addWidget(cb_back_cam, 2, 1);
    gridLayout->addWidget(lbl_front_cam, 3, 0);
    gridLayout->addWidget(cb_front_cam, 3, 1);
    gridLayout->addWidget(lbl_sim_num, 4, 0);
    gridLayout->addWidget(cb_sim_num, 4, 1);
    gridLayout->addWidget(lbl_band, 5, 0);
    gridLayout->addWidget(cb_band, 5, 1);
    gridLayout->addItem(vSpacer,  6, 0);
    gridLayout->setSpacing(15);

    vLayout = new QVBoxLayout(this);
    vLayout->addWidget(scrollArea);

/*
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(lbl_battery);
    hLayout->addWidget(le_battery);
    vLayout->addLayout(hLayout);
    */
}

void HardwarePage::disableWidget()
{
    QList<QObject *> list = common_scrollWidget->children();

    for(int i = 1; i < list.count(); i++)
    {
        QWidget *w = qobject_cast<QWidget *>(list.at(i));
        w->setDisabled(true);
    }
}

void HardwarePage::enableWidget()
{
    QList<QObject *> list = common_scrollWidget->children();

    for(int i = 1; i < list.count(); i++)
    {
        QWidget *w = qobject_cast<QWidget *>(list.at(i));
        w->setEnabled(true);
    }
}

void HardwarePage::loadCfg()
{
    if(Global::srcPath == "")
    {
        return;
    }
    enableWidget();
    QString boardCfg = Global::srcPath + "/" + Global::devicePath + "/BoardConfig.mk";
    QString dtsCfg = Global::srcPath + "/" + Global::dtsPath;
    QString kernelCfg = Global::srcPath + "/" + Global::kernelCfgPath;

    //dts文件里gc0310项少了status一项
    textHelper.addState2Gc0310Dts(dtsCfg);

    preBackCamId    = textHelper.readCam("back", dtsCfg);
    preFrontCamId   = textHelper.readCam("front", dtsCfg);
    cb_back_cam->setCurrentIndex(preBackCamId);
    cb_front_cam->setCurrentIndex(preFrontCamId);

    QString strDDR = textHelper.readTextStr(boardCfg, "FEAT_POW_EMIF_MAX_DDR2_FREQ", "boardCfg");
    if(strDDR == "312")
    {
        cb_ddr_fre->setCurrentIndex(0);
    }else if(strDDR == "400")
    {
        cb_ddr_fre->setCurrentIndex(1);
    }else if(strDDR == "455")
    {
        cb_ddr_fre->setCurrentIndex(2);
    }

    QString strSim = textHelper.readTextStr(boardCfg, "BUILD_DSDS", "boardCfg");
    if(strSim == "true")
    {
        cb_sim_num->setCurrentIndex(0);
    }else
    {
        cb_sim_num->setCurrentIndex(1);
    }

    QString strScreenType = textHelper.readTextStr(kernelCfg, "CONFIG_USE_IPS_LCD=y", "kernelCfg");
    if(strScreenType == "IPS")
    {
        cb_screen->setCurrentIndex(1);
    }else
    {
        cb_screen->setCurrentIndex(0);
    }

    strBand = textHelper.readTextStr(boardCfg, "BAND_FEID", "boardCfg");
    if(strBand == "weibu_band18_1530")
    {
        cb_band->setCurrentIndex(0);
    }else if(strBand == "weibu_band1_1530")
    {
        cb_band->setCurrentIndex(1);
    }else if(strBand == "weibu_band15_1530")
    {
        cb_band->setCurrentIndex(2);
    }else if(strBand == "weibu_band25_1530")
    {
        cb_band->setCurrentIndex(3);
    }else if(strBand == "weibu_band125_1530")
    {
        cb_band->setCurrentIndex(4);
    }else if(strBand == "weibu_band128_1530")
    {
        cb_band->setCurrentIndex(5);
    }else if(strBand == "weibu_band158_1530")
    {
        cb_band->setCurrentIndex(6);
    }else if(strBand == "weibu_band258_1530")
    {
        cb_band->setCurrentIndex(7);
    }else if(strBand == "weibu_band1258_1530")
    {
        cb_band->setCurrentIndex(8);
    }else if(strBand == "weibu_band1_GSM900-DCS1800_1530")
    {
        cb_band->setCurrentIndex(9);
    }

    QString flash = textHelper.readTextStr(dtsCfg, "&emmc ", "flash");
    if(flash == "\"disabled\";")
    {
        cb_flash->setCurrentIndex(0);
    }else
    {
        cb_flash->setCurrentIndex(1);
    }
}

void HardwarePage::saveCfg()
{
    disableWidget();
    QString boardCfg = Global::srcPath + "/" + Global::devicePath + "/BoardConfig.mk";
    QString dtsCfg = Global::srcPath + "/" + Global::dtsPath;
    QString kernelCfg = Global::srcPath + "/" + Global::kernelCfgPath;

    textHelper.writeToText(boardCfg, "FEAT_POW_EMIF_MAX_DDR2_FREQ", cb_ddr_fre->currentText(), ":=");
    if(cb_sim_num->currentIndex() == 0)
    {
        textHelper.writeToText(boardCfg, "BUILD_DSDS", "true", ":=");
    }else
    {
        textHelper.writeToText(boardCfg, "BUILD_DSDS", "false", ":=");
    }
/*
 * 读取emmc的status值作为判断，写入时，隔5行写emmc的status值，再隔7行写nand的status值，如果以后这部分结构改变可能导致读取写入失败
 *
 */
    if(cb_flash->currentIndex() == 0)
    {
        textHelper.writeToText(dtsCfg, "&emmc ", "disabled", "&emmc ");
    }else
    {
        textHelper.writeToText(dtsCfg, "&emmc ", "okay", "&emmc ");
    }

    if(cb_screen->currentIndex() == 0)
    {
        textHelper.writeToText(kernelCfg, "CONFIG_USE_IPS_LCD", "n", "=" );
    }else
    {
        textHelper.writeToText(kernelCfg, "CONFIG_USE_IPS_LCD", "y", "=" );
    }

    textHelper.addState2Gc0310Dts(dtsCfg);

    textHelper.writeCam(preBackCamId, cb_back_cam->currentIndex(), dtsCfg);
    textHelper.writeCam(preFrontCamId + 6, cb_front_cam->currentIndex() + 6, dtsCfg);
    if(strBand != cb_band->currentText())
    {
        textHelper.writeBand(strBand, cb_band->currentText());
        strBand = cb_band->currentText();
    }
/*
    switch (cb_band->currentIndex()) {
         //weibu_band18_1530
    case 0:
    textHelper.writeBand(strBand, cb_band->currentText());
        break;
        //weibu_band1_1530
    case 1:

        break;
        //weibu_band15_1530
    case 2:

        break;
        //weibu_band25_1530
    case 3:

        break;
        //weibu_band125_1530
    case 4:

        break;
        //weibu_band128_1530
    case 5:

        break;
        //weibu_band158_1530
    case 6:

        break;
        //weibu_band258_1530
    case 7:

        break;
        //weibu_band1258_1530
    case 8:

        break;
        //weibu_band1_GSM900-DCS1800_1530
    case 9:

        break;
    default:
        break;

    }*/
    //change current path
    QDir::setCurrent("Project/" + Global::prj_name);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QSqlQuery query = QSqlQuery(db);
   // db.setDatabaseName(Global::prj_home_path + "/" + Global::prj_name +  "/" + Global::prj_name + ".db");
    db.setDatabaseName(Global::prj_name + ".db");


    QString colon = "\"";
    QString strExec =   "insert into hardware values("
                        + QString::number(cb_screen->currentIndex(), 10) + ","
                        + QString::number(cb_flash->currentIndex(), 10) + ","
                        + colon + cb_back_cam->currentText() + colon + ","
                        + colon + cb_front_cam->currentText() + colon  + ","
                        + QString::number(cb_sim_num->currentIndex(), 10) + ","
                        + colon + cb_ddr_fre->currentText() + colon
                        + ")";
    QString strSpace = " ";
   QString strUpdate = "update hardware set "
                      + strSpace + "lcd_type=" + QString::number(cb_screen->currentIndex(),10) + ","
                      + strSpace + "flash_type=" + QString::number(cb_flash->currentIndex(), 10) + ","
                      + strSpace + "back_cam=" + colon + cb_back_cam->currentText() + colon + ","
                      + strSpace + "front_cam=" + colon + cb_front_cam->currentText() + colon  + ","
                      + strSpace + "simCard=" + QString::number(cb_sim_num->currentIndex(), 10) + ","
                      + strSpace + "ddrFreq=" + colon + cb_ddr_fre->currentText() + colon;

 /*
    if(query.exec(strExec))
    {
        qDebug() << "insert hardware ok";
    }
    else{
        qDebug() << "insert hardware fail";
        qDebug() << strExec;
    }
*/
    if(db.isOpen())
    {
        if(!query.exec(strUpdate))
        {
            qDebug() << "update hardware fail  : db is already open";
        }
    }else
    {
        if(!db.open())
        {
            qDebug() << "db path : " + Global::prj_home_path + "/Project/" + Global::prj_name +  "/" + Global::prj_name + ".db";
            qDebug() << "db open fail  :   HardwarePage::saveCfg()";
            QMessageBox::warning(this, "Warning", tr("hardware::数据库打开失败～～"));
        }else
        {
            QSqlQuery query = QSqlQuery(db);
            if(!query.exec(strUpdate))
            {
                qDebug() << "update hardware fail... db is not open and to open";
                qDebug() << strUpdate;
            }
        }
    }
    QDir::setCurrent(Global::prj_home_path);
    enableWidget();
    QMessageBox::information(this, tr("Info"), tr("此页面选项修改完毕"));
}
