#include "EventInterprocess.h"

EventInterprocess::EventInterprocess():
    _start_event("start_event", WorkingMode::Sender),
    _stop_event("stop_event", WorkingMode::Sender),
    _message_event("message_event", WorkingMode::Sender),
    _exit_event("exit_event", WorkingMode::Sender),
    _confirm_event("confirm_event")
{
}

bool EventInterprocess::sendEvent(EventType event)
{
    switch(event)
    {
    case EventType::start:
    {
        auto status = _start_event.set();
        if(status == QSystemSemaphore::NoError)
            return true;
        else
            return false;
    }
        break;
    case EventType::stop:
    {
        auto status = _stop_event.set();
        if(status == QSystemSemaphore::NoError)
            return true;
        else
            return false;
    }
        break;
    case EventType::message:
    {
        auto status = _message_event.set();
        if(status == QSystemSemaphore::NoError)
            return true;
        else
            return false;
    }
        break;
    case EventType::exit:
    {
        auto status = _exit_event.set();
        if(status == QSystemSemaphore::NoError)
            return true;
        else
            return false;
    }
        break;
    }
}

bool EventInterprocess::waitForConfirm()
{
    if(_confirm_event.wait() == QSystemSemaphore::NoError)
        return true;
    else
        return false;
}
