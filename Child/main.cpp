#include <QCoreApplication>
#include <QSharedMemory>
#include <QSystemSemaphore>
#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <memory>

#include "WaitingThread.h"
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
//QSharedMemory shared_memory("MyMemoryKey");
//QSystemSemaphore shared_mem_blocker("MySemaphoreKey",1);
void message()
{
//    shared_mem_blocker.acquire();

//    if(!shared_memory.attach(QSharedMemory::AccessMode::ReadOnly)) // присоединяемся к памяти
//    {
//        shared_mem_blocker.release();
//        std::cout << "Error attaching to shared memory" << std::endl;
//        return;
//    }

//    int message_size;
//    memcpy(&message_size, shared_memory.data(), sizeof(int));  // читаем размер сообщения

//    std::vector <char> v(message_size);
//    memcpy(&v[0], shared_memory.data() + sizeof(int), message_size);
//    std::string s(&v[0], message_size);


//    const char* message_buff = new char[message_size];
//    memcpy(&message_buff, shared_memory.data() + sizeof(int), message_size);      // затем само сообщение
//    std::string str_message(message_buff);
//    delete[] message_buff;

//    shared_memory.detach();                // отключаем процесс от памяти
//    shared_mem_blocker.release();

//    if(str_message != "")
//        std::cout << "Message was received: " << str_message << std::endl;
//    else
//        std::cout << "FAIL in receiving message" << std::endl;
}

int main(int argc, char *argv[])
{
    std::cout << "Child started" << std::endl;

    // Потоки, ожидающие разные события и выполняющие нужные функции после их наступления
    WaitingThread wait_for_start("start_event", start);
    WaitingThread wait_for_stop("stop_event", stop);
    WaitingThread wait_for_message("message_event", message);

    while(true)
    {

    }

    std::cout << "Event was received" << std::endl;

    int b;
    std::cin>>b;
    std::cout << "Child finished" << std::endl;
    return 0;
}
