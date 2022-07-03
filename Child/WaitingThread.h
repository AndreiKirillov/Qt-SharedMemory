#ifndef WAITINGTHREAD_H
#define WAITINGTHREAD_H
#include <thread>
#include <functional>
#include <boost/interprocess/sync/named_condition.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

using namespace boost::interprocess;

class WaitingThread
{
private:
    std::thread _thread;  // поток для ожидания события

    static named_mutex mutex_for_1_operation;

    static named_condition _confirm;

    named_condition _condition_to_wait;

    std::function<void()> _func_after_condition;

    void sendConfirm();
public:
    //WaitingThread();

    // В конструкторе получаем именованное событие
    WaitingThread(const char* condition_name, std::function<void()>&& func);

    ~WaitingThread();

};

#endif // WAITINGTHREAD_H
