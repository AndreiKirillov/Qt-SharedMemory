#ifndef LIBSHAREDMEMORY_H
#define LIBSHAREDMEMORY_H

#include <QSharedMemory>
#include <QSystemSemaphore>
#include <string>
#include "libSharedMemory_global.h"

class LIBSHAREDMEMORY_EXPORT LibSharedMemory
{
private:
    QSharedMemory _shared_memory;
    QSystemSemaphore _semaphore;
public:
    LibSharedMemory();

    bool writeToSharedMem(const char* message);

    std::string readFromSharedMem();

    bool detachMemory();
};

#endif // LIBSHAREDMEMORY_H
