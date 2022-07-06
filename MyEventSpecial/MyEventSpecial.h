#ifndef MYEVENTSPECIAL_H
#define MYEVENTSPECIAL_H
#include <QSystemSemaphore>
#include <QSharedMemory>
#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <chrono>

enum class WorkingMode  // пока либо отправитель, либо приёмник, в двустороннем режиме чёт сложно сделать
{                       // вдобавок отправитель может быть в единственном экземпляре, да и приёмник тоже
    Sender, Receiver
};

enum class StatusError     // Типы возможных ошибок семафора
{
    NoError, PermissionDenied, KeyError, AlreadyExists,
    NotFound, OutOfResources, UnknownError
};

class MyEventSpecial
{
private:
    std::unique_ptr<QSystemSemaphore> _event;
    std::string _event_name;

    WorkingMode _working_mode;

    StatusError translateError(QSystemSemaphore::SystemSemaphoreError error_code);
public:
    MyEventSpecial(const char* event_name, WorkingMode working_mode);
    ~MyEventSpecial();

    StatusError set();

    StatusError wait();
};


#endif // MYEVENTSPECIAL_H
