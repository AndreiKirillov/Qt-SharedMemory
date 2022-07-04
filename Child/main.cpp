#include <QCoreApplication>
#include <iostream>
#include <string>
#include <mutex>
#include <memory>
#include <boost/interprocess/sync/named_condition.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/creation_tags.hpp>

#include "WaitingThread.h"
#include "libSharedMemory.h"
using namespace boost::interprocess;

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

LibSharedMemory shared_mem;

void message()
{
    std::string received_message = shared_mem.readFromSharedMem();

    if(received_message != "")
        std::cout << "Message was received: " << received_message << std::endl;
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

    while(true)
    {

    }

    std::cout << "Event was received" << std::endl;

    int b;
    std::cin>>b;
    std::cout << "Child finished" << std::endl;
    return 0;
}
