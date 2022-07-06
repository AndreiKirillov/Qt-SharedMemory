#ifndef WAITINGTHREAD_H
#define WAITINGTHREAD_H
#include <thread>
#include <functional>

//#include "MyEvent.h"
#include "MyEventSpecial.h"

// Ожидающий поток ждет какое-то событие и совершает действие, дождавшись
class WaitingThread
{
private:
    std::thread _thread;  // поток для ожидания события

    MyEventSpecial _event_to_wait;  // ожидаемое событие

    static MyEventSpecial _confirm_event;  // будет посылать подтверждение родительскому процессу

    std::function<void()> _func_after_event; // в ответ на наступление события

    void sendConfirm();
public:
    //WaitingThread();

    // В конструкторе получаем именованное событие, raii style
    WaitingThread(const char* condition_name, std::function<void()>&& func);
    ~WaitingThread();

    void stop();
};

#endif // WAITINGTHREAD_H
