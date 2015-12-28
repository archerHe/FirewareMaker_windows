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
    void app2sd_apply();
    void app2sd_undo();
private:
    Ui::FunctionPage *ui;
    QScrollArea     *scrollArea;
    QWidget         *functionpage_scrollWidget;
    QGridLayout     *gridLayout;
    QVBoxLayout     *vLayout;
    QCheckBox       *cb_app2sd;
    QPushButton     *btn_apply_app2sd;
    QPushButton     *btn_undo_app2sd;


};

#endif // FUNCTIONPAGE_H
