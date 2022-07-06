#include "WaitingThread.h"

MyEventSpecial WaitingThread::_confirm_event = MyEventSpecial("confirm_event", WorkingMode::Sender);

// В конструкторе получаем именованное событие
WaitingThread::WaitingThread(const char* condition_name, std::function<void()>&& func):
    _event_to_wait(condition_name),
    _func_after_event(std::move(func))
{
    // Запускаем поток, котором будем ожидать событие и затем выполнять func
    _thread = std::thread([this](){
        while(true)
        {
            _event_to_wait.wait();
            _func_after_event();   // действие в ответ на событие
            sendConfirm();
        }
    });
}

WaitingThread::~WaitingThread()
{
    if(_thread.joinable())
        _thread.detach();
}

void WaitingThread::stop()
{
    _thread.~thread();
}

void WaitingThread::sendConfirm()
{
    _confirm_event.set();
}
