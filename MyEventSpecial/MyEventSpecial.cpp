#include "MyEventSpecial.h"

// переводит код ошибки семафора
StatusError MyEventSpecial::translateError(QSystemSemaphore::SystemSemaphoreError error_code)
{
    switch(error_code)
    {
    case QSystemSemaphore::NoError:
        return StatusError::NoError;

    case QSystemSemaphore::PermissionDenied:
        return StatusError::PermissionDenied;

    case QSystemSemaphore::KeyError:
        return StatusError::KeyError;

    case QSystemSemaphore::AlreadyExists:
        return StatusError::AlreadyExists;

    case QSystemSemaphore::NotFound:
        return StatusError::NotFound;

    case QSystemSemaphore::OutOfResources:
        return StatusError::OutOfResources;

    case QSystemSemaphore::UnknownError:
        return StatusError::UnknownError;
    }
}


MyEventSpecial::MyEventSpecial(const char *event_name, WorkingMode working_mode):
    _event_name(event_name),
    _working_mode(working_mode)
{
    if (_working_mode == WorkingMode::Sender)   // Если присоединимся к памяти, значит семафор уже создан
    {
        _event = std::make_unique<QSystemSemaphore>(event_name, 1, QSystemSemaphore::AccessMode::Create);
        _event->acquire();
        std::cout << "Sender was created"<<std::endl;
    }
    else
    {
        _event = std::make_unique<QSystemSemaphore>(event_name, 1, QSystemSemaphore::AccessMode::Open);
        std::cout << "Receiver was created"<<std::endl;
    }
}

MyEventSpecial::~MyEventSpecial()
{
}

StatusError MyEventSpecial::set()       // отправка события
{
    if(_working_mode == WorkingMode::Sender)
    {
        std::cout << "Trying to SEND: before releasing"<<std::endl;
        if(_event->release())   // освобождаем семафор, давая ждущему процессу его захватить
        {
            std::cout << "Trying to SEND: before acquiring"<<std::endl;
            //std::this_thread::sleep_for(std::chrono::microseconds(1));
            if(_event->acquire())   // снова захватываем
            {
                std::cout << "SEND was DONE"<<std::endl;
                return StatusError::NoError;
            }
            else
                return translateError(_event->error());  // в случае неудачи возвращаем ошибку
        }
        else
        {
            return translateError(_event->error());
        }
    }
    else
        return StatusError::PermissionDenied;
}

StatusError MyEventSpecial::wait()    // Ожидает наступление события
{
    if(_working_mode == WorkingMode::Receiver)   // только для типа получатель
    {
        std::cout << "Trying to WAIT: before acquiring"<<std::endl;
        if(_event->acquire())    // пытаемся получить ресурс
        {
            std::cout << "Trying to WAIT: before releasing"<<std::endl;
            if(_event->release())  // получение ресурса - это наступление события, затем освобождаем
            {
                std::cout << "WAIT DONE. Event was received"<<std::endl;
                return StatusError::NoError;
            }
            else
                return translateError(_event->error());
        }
        else
            return translateError(_event->error());
    }
    else
        return StatusError::PermissionDenied;
}
