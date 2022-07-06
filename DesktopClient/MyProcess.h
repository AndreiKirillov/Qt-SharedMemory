#ifndef MYPROCESS_H
#define MYPROCESS_H

#include <QProcess>
#include <QDebug>
#include <string>
#include <memory>
#include <thread>
#include <chrono>

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
    LibSharedMemory _shared_memory;
public:
    MyProcess();
    ~MyProcess();

    bool start(const QString& name);
    void close();
    QProcess::ProcessState state();
    void read();

    bool sendEvent(EventType event);
    bool sendMessage(std::string message);
};

#endif // MYPROCESS_H
