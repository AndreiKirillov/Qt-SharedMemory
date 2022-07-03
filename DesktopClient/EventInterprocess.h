#ifndef EVENTINTERPROCESS_H
#define EVENTINTERPROCESS_H

#include <QProcess>
#include <string>
#include <mutex>
#include <boost/interprocess/sync/named_condition.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/creation_tags.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

enum class EventType
{
    start,stop, message
};


class EventInterprocess
{
private:
    boost::interprocess::named_mutex _interprocess_mtx;

    boost::interprocess::named_condition _start_event;
    boost::interprocess::named_condition _stop_event;
    boost::interprocess::named_condition _message_event;

    boost::interprocess::named_condition _confirm_event;
public:
    EventInterprocess();
    ~EventInterprocess();

    void sendEvent(EventType event);

    bool waitForConfirm();

};

#endif // EVENTINTERPROCESS_H
