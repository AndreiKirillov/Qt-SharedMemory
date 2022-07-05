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

MyEvent::MyEvent(const char *event_name, WorkingMode working_mode):
    _working_mode(working_mode)
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
        if(_working_mode == WorkingMode::Sender)
        {
            //_event->acquire(); нельзя подавать сигнал, ожидающий процесс сработает
        }
        // если тип - приёмник, то
        // семафор-отправитель уже был создан, ничего не делаем
    }
    else
    {
        _memory_guard->create(1);     // создаем защитную память и сам семафор
        _event = std::make_unique<QSystemSemaphore>(event_name, 1, QSystemSemaphore::AccessMode::Create);
        if(_working_mode == WorkingMode::Sender)
        {
            _event->acquire();  // Захватываем семафор
        }
    }
}

MyEvent::~MyEvent()
{
    //_event->release();   ??проблема, что при уничтожении ресурс освободится и будет ложное срабатывание приемника
}

StatusError MyEvent::set()       // отправка события
{
    if(_working_mode == WorkingMode::Sender)
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
        return StatusError::PermissionDenied;
}

StatusError MyEvent::wait()    // Ожидает наступление события
{
    if(_working_mode == WorkingMode::Receiver)   // только для типа получатель
    {
        if(_event->acquire())    // пытаемся получить ресурс
        {
            if(_event->release())  // получение ресурса - это наступление события, затем освобождаем
                return StatusError::NoError;
            else
                return translateError(_event->error());
        }
        else
            return translateError(_event->error());
    }
    else
        return StatusError::PermissionDenied;
}
