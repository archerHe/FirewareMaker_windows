#ifndef FUNCTIONPAGE_H
#define FUNCTIONPAGE_H

#include <QWidget>

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

public slots:
    void loadCfg();
private:
    Ui::FunctionPage *ui;


};

#endif // FUNCTIONPAGE_H
