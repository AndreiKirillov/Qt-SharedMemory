#include "MyProcess.h"

MyProcess::MyProcess():_events_handler(), _shared_memory()
{
    _process = std::make_unique<QProcess>();
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
        _process->close();
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
    _events_handler.sendEvent(event);

    return _events_handler.waitForConfirm();
}

bool MyProcess::sendMessage(const char* message)
{
    return _shared_memory.writeToSharedMem(message);
}
