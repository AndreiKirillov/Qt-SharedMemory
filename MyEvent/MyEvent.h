#ifndef MYEVENT_H
#define MYEVENT_H

#include <QSystemSemaphore>
#include <QSharedMemory>
#include <string>
#include <memory>

enum class WorkingMode  // пока либо отправитель, либо приёмник, в двустороннем режиме чёт сложно сделать
{                       // вдобавок отправитель может быть в единственном экземпляре, да и приёмник тоже
    Sender, Receiver
};

enum class StatusError     // Типы возможных ошибок семафора
{
    NoError, PermissionDenied, KeyError, AlreadyExists,
    NotFound, OutOfResources, UnknownError
};

class MyEvent
{
private:
    std::unique_ptr<QSystemSemaphore> _event;

    std::unique_ptr<QSharedMemory> _memory_guard;

    WorkingMode _working_mode;

    bool _is_acquired;

    StatusError translateError(QSystemSemaphore::SystemSemaphoreError error_code);
public:
    MyEvent(const char* event_name, WorkingMode working_mode);
    ~MyEvent();

    StatusError set();

    StatusError wait();
};

#endif // MYEVENT_H
