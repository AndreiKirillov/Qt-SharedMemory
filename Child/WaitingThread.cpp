#include "WaitingThread.h"

MyEvent WaitingThread::_confirm_event = MyEvent("confirm_event", WorkingMode::Sender);

// В конструкторе получаем именованное событие
WaitingThread::WaitingThread(const char* condition_name, std::function<void()>&& func):
    _event_to_wait(condition_name, WorkingMode::Receiver),
    _func_after_event(std::move(func))
{
    // Запускаем поток, котором будем ожидать событие и затем выполнять func
    _thread = std::thread([&](){
        while(true)
        {
            _event_to_wait.wait();
            _func_after_event();                           // действие в ответ на событие
            sendConfirm();
        }
    });
}

WaitingThread::~WaitingThread()
{
    if(_thread.joinable())
        _thread.detach();
}

void WaitingThread::sendConfirm()
{
    _confirm_event.set();
}
