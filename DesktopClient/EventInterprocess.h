#ifndef EVENTINTERPROCESS_H
#define EVENTINTERPROCESS_H

#include <QProcess>
#include <string>
#include <mutex>
#include "MyEventSpecial.h"

enum class EventType
{
    start,stop, message
};


class EventInterprocess
{
private:
    MyEventSpecial _start_event;
    MyEventSpecial _stop_event;
    MyEventSpecial _message_event;

    //MyEvent _confirm_event;
public:
    EventInterprocess();
    ~EventInterprocess();

    void sendEvent(EventType event);

    bool waitForConfirm();

};

#endif // EVENTINTERPROCESS_H
