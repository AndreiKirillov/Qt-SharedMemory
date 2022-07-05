#include "EventInterprocess.h"

EventInterprocess::EventInterprocess():
    _start_event("start_event", WorkingMode::Sender),
    _stop_event("stop_event", WorkingMode::Sender),
    _message_event("message_event", WorkingMode::Sender),
    _confirm_event("confirm_event", WorkingMode::Receiver)
{
}

EventInterprocess::~EventInterprocess()
{
}

void EventInterprocess::sendEvent(EventType event)
{
    switch(event)
    {
    case EventType::start:
        _start_event.set();
        break;
    case EventType::stop:
        _stop_event.set();
        break;
    case EventType::message:
        _message_event.set();
        break;
    }
}

bool EventInterprocess::waitForConfirm()
{
    if(_confirm_event.wait() == StatusError::NoError)
        return true;
    else
        return false;
}
