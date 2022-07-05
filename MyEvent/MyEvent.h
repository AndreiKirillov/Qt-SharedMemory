#ifndef MYEVENT_H
#define MYEVENT_H

#include <QSystemSemaphore>
#include <QSharedMemory>
#include <string>
#include <thread>
#include <chrono>
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
    std::string _event_name;

    std::unique_ptr<QSharedMemory> _memory_open_create_guard;
    std::unique_ptr<QSharedMemory> _memory_false_signal_guard;

    WorkingMode _working_mode;

    StatusError translateError(QSystemSemaphore::SystemSemaphoreError error_code);
    bool checkIfSenderExists();
public:
    MyEvent(const char* event_name, WorkingMode working_mode);
    ~MyEvent();

    StatusError set();

    StatusError wait();
};

#endif // MYEVENT_H
