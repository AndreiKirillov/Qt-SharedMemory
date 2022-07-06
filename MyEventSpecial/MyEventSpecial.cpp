#include "MyEventSpecial.h"

MyEventSpecial::MyEventSpecial(const QString& event_name, WorkingMode working_mode):
   _event(event_name, 0, (working_mode == WorkingMode::Sender) ?
             QSystemSemaphore::AccessMode::Create:QSystemSemaphore::AccessMode::Open),
    _working_mode(working_mode)
{
}

QSystemSemaphore::SystemSemaphoreError MyEventSpecial::set()       // отправка события
{
    if(_working_mode == WorkingMode::Sender)
    {
        if(_event.release())   // освобождаем семафор, давая ждущему процессу его захватить
            return QSystemSemaphore::NoError;
        else
            return _event.error();
    }
    else
        return QSystemSemaphore::PermissionDenied;
}

QSystemSemaphore::SystemSemaphoreError MyEventSpecial::wait()    // Ожидает наступление события
{
    if(_working_mode == WorkingMode::Receiver)   // только для типа получатель
    {
        if(_event.acquire())    // пытаемся получить ресурс
            return QSystemSemaphore::NoError;
        else
            return _event.error();
    }
    else
        return QSystemSemaphore::PermissionDenied;
}
