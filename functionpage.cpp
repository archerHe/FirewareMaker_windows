#include "functionpage.h"
#include "ui_functionpage.h"
#include <QDebug>

FunctionPage::FunctionPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FunctionPage)
{
    ui->setupUi(this);

    disableWidget();
}

FunctionPage::~FunctionPage()
{
    delete ui;
}


void FunctionPage::disableWidget()
{
    QList<QObject *> list = ui->gridLayoutWidget->children();
    QWidget *w;
    for(int i = 1; i < list.count(); i++)
    {
        w = qobject_cast<QWidget *>(list.at(i));
        w->setDisabled(true);
    }
}

void FunctionPage::enableWidget()
{
    QList<QObject *> list = ui->gridLayoutWidget->children();
    QWidget *w;
    for(int i = 1; i < list.count(); i++)
    {
        w = qobject_cast<QWidget *>(list.at(i));
        w->setEnabled(true);
    }
}

void FunctionPage::loadCfg()
{
    enableWidget();
}
