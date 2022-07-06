#include <QCoreApplication>
#include <QSharedMemory>
#include <QSystemSemaphore>
#include <iostream>
#include <string>

#include "WaitingThread.h"
#include "MyEventSpecial.h"
//#include "libSharedMemory.h"

void start()
{
    std::cout<<"start function"<<std::endl;
    for(int i=0;i<10;i++)
        std::cout<<"work imitation"<<std::endl;
    std::cout<<"start function FINISHED"<<std::endl;
}

void stop()
{
    std::cout<<"stop function"<<std::endl;
    for(int i=0;i<10;i++)
        std::cout<<"work imitation"<<std::endl;
    std::cout<<"stop function FINISHED"<<std::endl;
}

//LibSharedMemory shared_mem;
QSharedMemory shared_memory("MyMemoryKey");
QSystemSemaphore shared_mem_blocker("MemoryBlockerKey",1, QSystemSemaphore::Open);
void message()
{
    std::cout << "Try to receive message" << std::endl;
    shared_mem_blocker.acquire();

    if(!shared_memory.attach(QSharedMemory::AccessMode::ReadOnly)) // присоединяемся к памяти
    {
        shared_mem_blocker.release();
        std::cout << "Error attaching to shared memory" << std::endl;
        return;
    }

    int message_size;
    memcpy(&message_size, shared_memory.data(), sizeof(int));  // читаем размер сообщения

    char* message_buffer = new char[message_size];
    memcpy(message_buffer, shared_memory.data() + sizeof(int), message_size);
    std::string str_message(message_buffer, message_size);

    delete[] message_buffer;

    shared_memory.detach();                // отключаем процесс от памяти
    shared_mem_blocker.release();

    if(str_message != "")
        std::cout << "Message was received: " << str_message << std::endl;
    else
        std::cout << "FAIL in receiving message" << std::endl;
}

int main(int argc, char *argv[])
{
    std::cout << "Child started" << std::endl;

    // Потоки, ожидающие разные события и выполняющие нужные функции после их наступления
    WaitingThread wait_for_start("start_event", start);
    WaitingThread wait_for_stop("stop_event", stop);
    WaitingThread wait_for_message("message_event", message);

    MyEventSpecial exit_event("exit_event");
    exit_event.wait();

    wait_for_start.stop();
    wait_for_stop.stop();
    wait_for_message.stop();
    return 0;
}
