#ifndef EVENTINTERPROCESS_H
#define EVENTINTERPROCESS_H

#include <QProcess>
#include <string>
#include <mutex>
#include "MyEvent.h"

enum class EventType
{
    start,stop, message
};


class EventInterprocess
{
private:
    MyEvent _start_event;
    MyEvent _stop_event;
    MyEvent _message_event;

    MyEvent _confirm_event;
public:
    EventInterprocess();
    ~EventInterprocess();

    void sendEvent(EventType event);

    bool waitForConfirm();

};

#endif // EVENTINTERPROCESS_H
