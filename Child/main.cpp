#include <QCoreApplication>
#include <iostream>
#include <mutex>
#include <memory>
#include <boost/interprocess/sync/named_condition.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/creation_tags.hpp>

#include "WaitingThread.h"
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

void message()
{
    std::cout<<"message function"<<std::endl;
    for(int i=0;i<10;i++)
        std::cout<<"work imitation"<<std::endl;
    std::cout<<"message function FINISHED"<<std::endl;
}

int main(int argc, char *argv[])
{
    std::cout << "Child started" << std::endl;

    // Разные события
    //named_condition start_condition(boost::interprocess::open_only_t(), "start_event");
    //named_condition stop_condition(boost::interprocess::open_only_t(), "stop_event");
    //named_condition message_condition(boost::interprocess::open_only_t(), "message_event");

    // Потоки, ожидающие разные события и выполняющие нужные функции после их наступления
    WaitingThread wait_for_start("start_event", start);
    WaitingThread wait_for_stop("stop_event", stop);
    WaitingThread wait_for_message("message_event", message);

    while(true)
    {

    }

    //named_mutex interprocess_mtx(open_only_t(), "interprocess_mtx");
    //named_condition event(open_only_t(), "start_event");

    //boost::interprocess::scoped_lock lck(interprocess_mtx);
    //event.wait(lck);

    std::cout << "Event was received" << std::endl;

    int b;
    std::cin>>b;
    std::cout << "Child finished" << std::endl;
    return 0;
}
