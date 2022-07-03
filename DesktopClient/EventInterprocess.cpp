#include "EventInterprocess.h"

EventInterprocess::EventInterprocess():
    _interprocess_mtx(boost::interprocess::create_only_t(), "interprocess_mtx"),
    _start_event(boost::interprocess::create_only_t(), "start_event"),
    _stop_event(boost::interprocess::create_only_t(), "stop_event"),
    _message_event(boost::interprocess::create_only_t(), "message_event"),
    _confirm_event(boost::interprocess::create_only_t(), "confirm_event")
{
}

EventInterprocess::~EventInterprocess()
{
    boost::interprocess::named_mutex::remove("interprocess_mtx");
    boost::interprocess::named_condition::remove("start_event");
    boost::interprocess::named_condition::remove("stop_event");
    boost::interprocess::named_condition::remove("message_event");
    boost::interprocess::named_condition::remove("confirm_event");
}

void EventInterprocess::sendEvent(EventType event)
{
    boost::interprocess::scoped_lock lck(_interprocess_mtx);
    switch(event)
    {
    case EventType::start:
        _start_event.notify_one();
        break;
    case EventType::stop:
        _stop_event.notify_one();
        break;
    case EventType::message:
        _message_event.notify_one();
        break;
    }
}

bool EventInterprocess::waitForConfirm()
{
    using namespace boost;
    posix_time::time_duration waiting_time = posix_time::seconds(5);

    interprocess::scoped_lock lck(_interprocess_mtx);
    return _confirm_event.wait_for(lck, waiting_time, [](){return true;}); // Возвращаем результат ожидания подтверждения
}
