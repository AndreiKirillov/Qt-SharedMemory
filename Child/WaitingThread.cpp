#include "WaitingThread.h"

using namespace boost::interprocess;
named_mutex WaitingThread::mutex_for_1_operation = named_mutex(open_only_t(), "interprocess_mtx");

// В конструкторе получаем именованное событие
WaitingThread::WaitingThread(const char* condition_name, std::function<void()>&& func):
    _condition_to_wait(open_only_t(), condition_name),
    _func_after_condition(std::move(func))
{
    // Запускаем поток, котором будем ожидать событие и затем выполнять func
    // при этом программа не будет одновременно выполнять func двух разных waitingThread
    _thread = std::thread([&](){
        while(true)
        {
            scoped_lock operation_lock(mutex_for_1_operation);
            _condition_to_wait.wait(operation_lock);           // ждём событие
            _func_after_condition();                           // действие в ответ на событие
        }
    });
}
