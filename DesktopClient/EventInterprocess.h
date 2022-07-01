#ifndef EVENTINTERPROCESS_H
#define EVENTINTERPROCESS_H

#include <QProcess>
#include <string>
#include <memory>
#include <boost/interprocess/sync/named_condition_any.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/creation_tags.hpp>

enum class EventType
{
    start,stop, receive_message
};


class EventInterprocess
{
private:
    static std::string convertEventToString(EventType event);
public:

    static bool sendEvent(std::unique_ptr<QProcess>& process, EventType event);

};

#endif // EVENTINTERPROCESS_H
