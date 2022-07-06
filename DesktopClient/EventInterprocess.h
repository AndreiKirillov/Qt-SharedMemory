#ifndef EVENTINTERPROCESS_H
#define EVENTINTERPROCESS_H

#include <QProcess>
#include <string>

#include "MyEventSpecial.h"

enum class EventType
{
    start,stop, message, exit
};

class EventInterprocess
{
private:
    MyEventSpecial _start_event;
    MyEventSpecial _stop_event;
    MyEventSpecial _message_event;
    MyEventSpecial _exit_event;

    MyEventSpecial _confirm_event;
public:
    EventInterprocess();

    bool sendEvent(EventType event);

    bool waitForConfirm();

};

#endif // EVENTINTERPROCESS_H
