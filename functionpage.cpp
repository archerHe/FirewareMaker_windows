#include "functionpage.h"
#include "ui_functionpage.h"
#include <QSpacerItem>

FunctionPage::FunctionPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FunctionPage)
{
  //  ui->setupUi(this);
    initWidget();
  //  disableWidget();
}

FunctionPage::~FunctionPage()
{
    delete ui;
}


void FunctionPage::disableWidget()
{
    QList<QObject *> list = functionpage_scrollWidget->children();
    QWidget *w;
    for(int i = 1; i < list.count(); i++)
    {
        w = qobject_cast<QWidget *>(list.at(i));
        w->setDisabled(true);
    }
}

void FunctionPage::enableWidget()
{
    QList<QObject *> list = functionpage_scrollWidget->children();
    QWidget *w;
    for(int i = 1; i < list.count(); i++)
    {
        w = qobject_cast<QWidget *>(list.at(i));
        w->setEnabled(true);
    }
}

void FunctionPage::initWidget()
{
    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    functionpage_scrollWidget = new QWidget(scrollArea);
    scrollArea->setWidget(functionpage_scrollWidget);
    gridLayout = new QGridLayout(functionpage_scrollWidget);

    cb_app2sd =             new QCheckBox("app to sdcard");
    cb_app2sd->setCheckable(false);
    btn_apply_app2sd = new QPushButton("apply");
    btn_undo_app2sd  = new QPushButton("undo");

    gridLayout->addWidget(cb_app2sd, 0, 0);
    gridLayout->addWidget(btn_apply_app2sd, 0, 1);
    gridLayout->addWidget(btn_undo_app2sd, 0, 2);
    gridLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding,QSizePolicy::Expanding), 0, 3);
    gridLayout->addItem(new QSpacerItem(20,500, QSizePolicy::Expanding, QSizePolicy::Expanding), 1, 0);

    vLayout = new QVBoxLayout(this);
    vLayout->addWidget(scrollArea);

}

void FunctionPage::loadCfg()
{
    enableWidget();
}

void FunctionPage::app2sd_apply()
{

}

void FunctionPage::app2sd_undo()
{

}
