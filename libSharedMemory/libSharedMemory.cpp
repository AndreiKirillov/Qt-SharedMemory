#include "libSharedMemory.h"

LibSharedMemory::LibSharedMemory():_shared_memory("MyMemoryKey"), _semaphore("MemoryBlockerKey",1)
{
}


bool LibSharedMemory::writeToSharedMem(const QString& message)
{
    _semaphore.acquire();      // синхронизируем доступ

    if(!_shared_memory.create(message.size() + sizeof(int))) // Создаём сегмент памяти нужного размера
    {
        _semaphore.release();
        return false;
    }

    if(!_shared_memory.isAttached()) // присоединяемся к памяти
    {
        _semaphore.release();
        return false;
    }

    int message_size = message.size();

    memcpy(_shared_memory.data(), &message_size, sizeof(int));
    QByteArray message_buff = message.toLocal8Bit();
    memcpy(_shared_memory.data() + sizeof(int), message_buff.data(), message_size);

    _semaphore.release();
    return true;
}

QString LibSharedMemory::readFromSharedMem()
{
    _semaphore.acquire();

    if(!_shared_memory.attach()) // присоединяемся к памяти
    {
        _semaphore.release();
        return "";
    }

    int message_size;
    memcpy(&message_size, _shared_memory.data(), sizeof(int));  // читаем размер сообщения

    char* message_buffer = new char[message_size];
    memcpy(message_buffer, _shared_memory.data() + sizeof(int), message_size);
    QString str_message(message_buffer);
    delete[] message_buffer;

    _shared_memory.detach();                // отключаем процесс от памяти
    _semaphore.release();

    return str_message;
}

bool LibSharedMemory::detachMemory()
{
    return _shared_memory.detach();
}



