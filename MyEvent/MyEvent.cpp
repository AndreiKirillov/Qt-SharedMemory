#include "MyEvent.h"

// переводит код ошибки семафора
StatusError MyEvent::translateError(QSystemSemaphore::SystemSemaphoreError error_code)
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

MyEvent::MyEvent(const char* event_name, ResetMode mode):_mode(mode)
{
    std::string memory_name = event_name;   // имя-ключ для защитной памяти
    memory_name += "_memory";               // составляется из имени события
    {
        QSharedMemory reset_links_counter(memory_name.c_str());   // сброс счетчика ссылок памяти после возможного сбоя
        reset_links_counter.attach();                           //https://habr.com/ru/post/173281/
    }

    _memory_guard = std::make_unique<QSharedMemory>(memory_name.c_str());
    if (_memory_guard->attach())   // Если присоединимся к памяти, значит семафор уже создан
    {
        _event = std::make_unique<QSystemSemaphore>(event_name, 1, QSystemSemaphore::AccessMode::Open);
    }
    else
    {
        _memory_guard->create(1);     // создаем защитную память и сам семафор
        _event = std::make_unique<QSystemSemaphore>(event_name, 1, QSystemSemaphore::AccessMode::Create);
        _event->acquire();  // Захватываем семафор
    }
}

MyEvent::~MyEvent()
{
    _memory_guard->detach();
    //_event->release();
}

StatusError MyEvent::set()       // отправка события
{
    if(_mode == ResetMode::Auto)
    {
        if(_event->release())   // освобождаем семафор, давая ждущему процессу его захватить
        {
            if(_event->acquire())   // снова захватываем
                return StatusError::NoError;
            else
                return translateError(_event->error());  // в случае неудачи возвращаем ошибку
        }
        else
        {
            return translateError(_event->error());
        }
    }
    else
    {
        if(_event->release())  // при ручном сбросе просто освобождаем семафор
            return StatusError::NoError;
        else
            return translateError(_event->error());
    }
}

StatusError MyEvent::reset()   // нужна только при ручном типе сброса
{
    if(_mode == ResetMode::Manual)
    {
        if(_event->acquire())
            return StatusError::NoError;
        else
            return translateError(_event->error());
    }
}

StatusError MyEvent::wait()    // Ожидает наступление события
{
    if(_event->acquire())    // пытаемся получить ресурс
    {
        if(_event->release())  // получение ресурса - наступление события, затем освобождаем
            return StatusError::NoError;
        else
            return translateError(_event->error());
    }
    else
        return translateError(_event->error());
}
