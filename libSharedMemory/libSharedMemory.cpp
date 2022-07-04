#include "libSharedMemory.h"

LibSharedMemory::LibSharedMemory():_shared_memory("MyMemoryKey"), _semaphore("MySemaphoreKey",1)
{
}


bool LibSharedMemory::writeToSharedMem(const char *message)
{
    _semaphore.acquire();      // синхронизируем доступ

    if(!_shared_memory.create(strlen(message) + sizeof(int) + 1)) // Создаём сегмент памяти нужного размера
    {
        _semaphore.release();
        return false;
    }

    if(!_shared_memory.isAttached()) // присоединяемся к памяти
    {
        _semaphore.release();
        return false;
    }

    int message_size = strlen(message) + 1;

    memcpy(_shared_memory.data(), &message_size, sizeof(int));
    memcpy(_shared_memory.data() + sizeof(int), message, message_size);

    _semaphore.release();
    return true;
}

std::string LibSharedMemory::readFromSharedMem()
{
    _semaphore.acquire();

    if(!_shared_memory.attach()) // присоединяемся к памяти
    {
        _semaphore.release();
        return "";
    }

    int message_size;
    memcpy(&message_size, _shared_memory.data(), sizeof(int));  // читаем размер сообщения

    const char* message = new char[message_size];
    memcpy(&message, _shared_memory.data() + sizeof(int), message_size);      // затем само сообщение
    std::string str_message(message);
    delete[] message;

    _shared_memory.detach();                // отключаем процесс от памяти
    _semaphore.release();

    return message;
}

bool LibSharedMemory::detachMemory()
{
    return _shared_memory.detach();
}



