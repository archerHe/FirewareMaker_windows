#ifndef BUILDPAGE_H
#define BUILDPAGE_H

#include <QWidget>
#include <QProcess>
#include <QDebug>

namespace Ui {
class BuildPage;
}

class BuildPage : public QWidget
{
    Q_OBJECT

public:
    explicit BuildPage(QWidget *parent = 0);
    ~BuildPage();

    void initWidget();

private:
    Ui::BuildPage *ui;
    QProcess    *p;

private slots:
    void showReadAll();
//    void showReadStdOut();
 //   void showStdError();
    void qProcessStart();
    void qProcessFinish();
    void on_btn_build_clicked();
};

#endif // BUILDPAGE_H
