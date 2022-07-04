#ifndef MYEVENT_H
#define MYEVENT_H

#include <QSystemSemaphore>
#include <QSharedMemory>
#include <string>
#include <memory>

enum class ResetMode
{
    Manual, Auto
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

    ResetMode _mode;

    StatusError translateError(QSystemSemaphore::SystemSemaphoreError error_code);
public:
    MyEvent(const char* event_name, ResetMode mode = ResetMode::Auto);
    ~MyEvent();

    StatusError set();

    StatusError reset();

    StatusError wait();
};

#endif // MYEVENT_H
