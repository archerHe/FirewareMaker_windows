#ifndef FUNCTIONPAGE_H
#define FUNCTIONPAGE_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QDebug>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QCheckBox>
#include <QDir>
#include <QFile>
#include "global.h"

namespace Ui {
class FunctionPage;
}

class FunctionPage : public QWidget
{
    Q_OBJECT

public:
    explicit FunctionPage(QWidget *parent = 0);
    ~FunctionPage();

    void disableWidget();
    void enableWidget();
    void initWidget();

public slots:
    void loadCfg();
    void saveCfg();
    void readErr();
    void readStdOut();
private:
    Ui::FunctionPage *ui;
    QScrollArea     *scrollArea;
    QWidget         *functionpage_scrollWidget;
    QGridLayout     *gridLayout;
    QVBoxLayout     *vLayout;
    QProcess            *p;
    QFile                  *prjFile;

    QCheckBox       *cb_app2sd;
    QCheckBox       *cb_ptest;
    QCheckBox       *cb_speak;



};

#endif // FUNCTIONPAGE_H
