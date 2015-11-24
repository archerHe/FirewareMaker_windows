
#include <QApplication>
#include <QDesktopWidget>
#include "mainwindow.h"
#include "wizard.h"
#include "launcher_page.h"
#include "hardwarepage.h"
#include "commonpage.h"
#include "otherspage.h"
#include <QMessageBox>
#include <qstyle.h>
#include <QStyleFactory>

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();

    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;

    case QtWarningMsg:
        text = QString("Warning:");
        break;

    case QtCriticalMsg:
        text = QString("Critical:");
        break;

    case QtFatalMsg:
        text = QString("Fatal:");
    }

    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(msg).arg(current_date);

    QFile file("log/log.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();

    mutex.unlock();
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //a.setStyle("Fusion");
    MainWindow w;
    w.setWindowFlags(w.windowFlags()&~Qt::WindowMaximizeButtonHint);
    QDesktopWidget *dw = QApplication::desktop();
    w.setFixedSize(dw->width()*0.8, dw->height()*0.7);

    qInstallMessageHandler(outputMessage);

    w.show();



    return a.exec();
}



