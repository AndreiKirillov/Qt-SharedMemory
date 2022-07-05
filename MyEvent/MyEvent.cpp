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
    _event_name(event_name),
    _working_mode(working_mode)
{
    std::string memory_name = _event_name + "_memory";   // имя-ключ для защитной памяти составляется из имени события
    std::string memory_name_false_signal = memory_name + "_signal";
    {
        QSharedMemory reset_links_counter(memory_name.c_str());   // сброс счетчика ссылок памяти после возможного сбоя
        reset_links_counter.attach();                           //https://habr.com/ru/post/173281/
        QSharedMemory reset_false_signal(memory_name_false_signal.c_str());
        reset_false_signal.attach();
    }

    _memory_open_create_guard = std::make_unique<QSharedMemory>(memory_name.c_str());
    _memory_open_create_guard->lock();
    if (_memory_open_create_guard->attach())   // Если присоединимся к памяти, значит семафор уже создан
    {
        _event = std::make_unique<QSystemSemaphore>(event_name, 1, QSystemSemaphore::AccessMode::Open);
        if(_working_mode == WorkingMode::Sender)
        {
            _event->acquire();
            _memory_false_signal_guard = std::make_unique<QSharedMemory>(memory_name_false_signal.c_str());
            _memory_false_signal_guard->lock();
            _memory_false_signal_guard->create(1);  // эту память будет проверять приёмник
            _memory_false_signal_guard->unlock();   // чтобы перед wait узнать о существовании отправителя
        }
        // если тип - приёмник, то
        // семафор-отправитель уже был создан, ничего не делаем
    }
    else
    {
        _memory_open_create_guard->create(1);     // создаем защитную память и сам семафор
        _event = std::make_unique<QSystemSemaphore>(event_name, 1, QSystemSemaphore::AccessMode::Create);
        if(_working_mode == WorkingMode::Sender)
        {
            _event->acquire();  // Захватываем семафор
            _memory_false_signal_guard = std::make_unique<QSharedMemory>(memory_name_false_signal.c_str());
            _memory_false_signal_guard->lock();
            _memory_false_signal_guard->create(1);  // эту память будет проверять приёмник
            _memory_false_signal_guard->unlock();   // чтобы перед wait узнать о существовании отправителя
        }
    }
    _memory_open_create_guard->unlock();
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

bool MyEvent::checkIfSenderExists() // только для приёмника
{                                   // проверяет существование отправителя
    std::string mem_name = _event_name + "_memory_signal";
    QSharedMemory check_mem(mem_name.c_str());
    return check_mem.attach();     //true если присоединились, значит отправитель есть и можно вызывать wait
}

StatusError MyEvent::wait()    // Ожидает наступление события
{
    if(_working_mode == WorkingMode::Receiver)   // только для типа получатель
    {
        while(true)       // постоянно проверяем появился ли отправитель
        {
            if(checkIfSenderExists())
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
        }
    }
    else
        return StatusError::PermissionDenied;
}
