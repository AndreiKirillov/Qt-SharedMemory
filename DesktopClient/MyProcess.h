#ifndef MYPROCESS_H
#define MYPROCESS_H

#include <QProcess>
#include <QSharedMemory>
#include <QSystemSemaphore>
#include <QDebug>
#include <string>
#include <memory>

#include "EventInterprocess.h"
#include "libSharedMemory.h"     // созданная библиотека для разделяемой памяти

class MyProcess// : private QObject
{
    //Q_OBJECT

private:
    // объект процесса
    std::unique_ptr<QProcess> _process;

    //средства для отправки событий процессу
    EventInterprocess _events_handler;

    // для работы с разделяемой памятью
    QSharedMemory _shared_memory;
    QSystemSemaphore _memory_blocker;
public:
    MyProcess();
    ~MyProcess();

    bool start(const QString& name);
    void close();
    QProcess::ProcessState state();
    void read();

    bool sendEvent(EventType event);
    bool sendMessage(const QString& message);
};

#endif // MYPROCESS_H
