#include <QCoreApplication>
#include <iostream>
#include <mutex>
#include <boost/interprocess/sync/named_condition_any.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/creation_tags.hpp>

using namespace boost::interprocess;

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);
    std::cout << "Child started" << std::endl;

    named_mutex interprocess_mtx(open_only_t(), "interprocess_mtx");
    std::unique_lock<named_mutex> lck(interprocess_mtx);

    named_condition_any event(open_only_t(), "check_event");

    event.wait(lck);
    std::cout << "Event was received" << std::endl;

    int b;
    std::cin>>b;
    std::cout << "Child finished" << std::endl;
    return 0;
}
