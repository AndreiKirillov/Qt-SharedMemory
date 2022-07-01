#include "EventInterprocess.h"

std::string EventInterprocess::convertEventToString(EventType event)
{
    switch(event)
    {
    case EventType::start:
    {
        return "start";
    }
        break;
    case EventType::stop:
    {
        return "stop";
    }
        break;
    case EventType::receive_message:
    {
        return "message";
    }
        break;
    }
}

bool EventInterprocess::sendEvent(std::unique_ptr<QProcess>& process, EventType event)
{
    if(process->state() == QProcess::Running)
    {
        process->write(convertEventToString(event).c_str());

        if(process->waitForBytesWritten(5000))
            return true;
        else
            return false;
    }
    else
        return false;
}
