#ifndef FUNCTIONPAGE_H
#define FUNCTIONPAGE_H

#include <QWidget>
#include "global.h"

class QScrollArea;
class QGridLayout;
class QVBoxLayout;
class QLabel;
class QCheckBox;
class QProcess;
class QFile;
class QTextBrowser;

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
    void processFinished();
    void readAll();
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
    QTextBrowser    *tb_readall;
};

#endif // FUNCTIONPAGE_H
