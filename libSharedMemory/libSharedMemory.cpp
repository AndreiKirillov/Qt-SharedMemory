#include "libSharedMemory.h"

LibSharedMemory::LibSharedMemory():_shared_memory("MyMemoryKey"), _semaphore("MySemaphoreKey",1)
{
}


bool LibSharedMemory::writeToSharedMem(const char *message)
{
    _semaphore.acquire();      // синхронизируем доступ

    if(!_shared_memory.create(strlen(message))) // Создаём сегмент памяти нужного размера
    {
        _semaphore.release();
        return false;
    }

    if(!_shared_memory.attach()) // присоединяемся к памяти
    {
        _semaphore.release();
        return false;
    }

    int message_size = strlen(message);

    memcpy(_shared_memory.data(), &message_size, sizeof(int));
    memcpy(_shared_memory.data(), message, strlen(message));

    _semaphore.release();
    return true;
}

const char* LibSharedMemory::readFromSharedMem()
{
    _semaphore.acquire();

    if(!_shared_memory.attach()) // присоединяемся к памяти
    {
        _semaphore.release();
        return "";
    }

    int message_size;
    std::string message;

    memcpy(&message_size, _shared_memory.data(), sizeof(int));  // читаем размер сообщения
    memcpy(&message, _shared_memory.data(), message_size);      // затем само сообщение

    _shared_memory.detach();                // отключаем процесс от памяти
    _semaphore.release();

    return message.c_str();
}



