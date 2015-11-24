#ifndef WIZARD_H
#define WIZARD_H

#include <QWizard>
#include <QWizardPage>
#include <QLabel>
#include <QLineEdit>
#include <QTextBrowser>
#include <QPushButton>


namespace Ui {
class Wizard;
}

class Wizard : public QWizard
{
    Q_OBJECT

public:
    explicit Wizard(QWidget *parent = 0);
    ~Wizard();



private:
    Ui::Wizard *ui;

    QTextBrowser *text;
    QLabel *lblPrjName;
    QLineEdit *lePrjName;
    QLabel *lblPrjPath;
    QLineEdit *lePrjPath;
    QPushButton *choosePrjBtn;
    QWizardPage *firstPage;
    QWizardPage *secondPage;

    QString prj_home_path;




    void initFirstPage();
    void initSecondPage();
    void createPrj();

protected slots:
    void accept();

    void on_choosePrjBtn_clicked();

};

#endif // WIZARD_H
