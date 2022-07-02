#ifndef MYPROCESS_H
#define MYPROCESS_H

#include <QProcess>
#include <QDebug>
#include <string>
#include <memory>
#include <thread>
#include <chrono>
#include <boost/interprocess/sync/named_condition_any.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/creation_tags.hpp>

#include "EventInterprocess.h"

class MyProcess// : private QObject
{
    //Q_OBJECT

private:
    // объект процесса
    std::unique_ptr<QProcess> _process;

    //средства для отправки событий процессу
    EventInterprocess _events_handler;

public:
    MyProcess();

    bool start(const QString& name);
    void close();
    QProcess::ProcessState state();
    void read();

    void sendEvent(EventType event);
};

#endif // MYPROCESS_H