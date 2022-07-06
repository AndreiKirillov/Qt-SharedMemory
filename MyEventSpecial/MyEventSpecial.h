#ifndef MYEVENTSPECIAL_H
#define MYEVENTSPECIAL_H
#include <QSystemSemaphore>
#include <QSharedMemory>

enum class WorkingMode
{
    Sender, Receiver
};

class MyEventSpecial
{
private:
    QSystemSemaphore _event;

    WorkingMode _working_mode;

public:
    MyEventSpecial(const QString& event_name, WorkingMode working_mode = WorkingMode::Receiver);

    QSystemSemaphore::SystemSemaphoreError set();

    QSystemSemaphore::SystemSemaphoreError wait();
};


#endif // MYEVENTSPECIAL_H
