#include <QCoreApplication>
#include <iostream>
#include <mutex>
#include <memory>
#include <boost/interprocess/sync/named_condition.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/creation_tags.hpp>

using namespace boost::interprocess;

int main(int argc, char *argv[])
{
    std::cout << "Child started" << std::endl;

    named_mutex interprocess_mtx(open_only_t(), "interprocess_mtx");
    named_condition event(open_only_t(), "start_event");

    boost::interprocess::scoped_lock lck(interprocess_mtx);
    event.wait(lck);

    std::cout << "Event was received" << std::endl;

    int b;
    std::cin>>b;
    std::cout << "Child finished" << std::endl;
    return 0;
}
