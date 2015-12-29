#ifndef CHECKTHREAD_H
#define CHECKTHREAD_H

#include <QThread>

class CheckThread : public QThread
{
public:
    CheckThread();
    ~CheckThread();

protected:
    void run();
};

#endif // CHECKTHREAD_H
