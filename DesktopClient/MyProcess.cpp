#include "MyProcess.h"

MyProcess::MyProcess():_events_handler(), _shared_memory("MyMemoryKey"),
    _memory_blocker("MemoryBlockerKey", 1, QSystemSemaphore::Create)
{
    _process = std::make_unique<QProcess>();
}

MyProcess::~MyProcess()
{
}

bool MyProcess::start(const QString &name)
{
    if(_process->state() == QProcess::NotRunning)
    {
        _process->start(name);

        if(_process->waitForStarted(5000))
            return true;
        else
            return false;
    }
    else
        throw std::runtime_error("Process is already started!");
}

void MyProcess::close()
{
    if(_process->state() == QProcess::Running || _process->state() == QProcess::Starting)
        _events_handler.sendEvent(EventType::exit);
}

QProcess::ProcessState MyProcess::state()
{
    return _process->state();
}

void MyProcess::read()
{
    auto data = _process->readAllStandardOutput();
    qDebug() << data;
}

bool MyProcess::sendEvent(EventType event)
{
    if(_events_handler.sendEvent(event))       // посылаем событие
        return _events_handler.waitForConfirm();   // ждём подтверждение
    else
        return false;
}

bool MyProcess::sendMessage(const QString& message)
{
    _memory_blocker.acquire(); // синхронизируем доступ к памяти

    if(_shared_memory.create(message.size() + sizeof(int)))
    {
        int message_size = message.size();
        memcpy(_shared_memory.data(),&message_size, sizeof(int));
        QByteArray message_buff = message.toLocal8Bit();
        memcpy(_shared_memory.data()+sizeof(int), message_buff.data(),message_size);

        _memory_blocker.release();
        bool result = sendEvent(EventType::message);
        _shared_memory.detach();
        return result;
    }
    _memory_blocker.release();
    return false;
}
