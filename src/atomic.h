#ifndef ATOMIC_H
#define ATOMIC_H

#include <string>
#include <sys/types.h>

#ifndef GCC
#include <pthread.h>
#endif

using namespace std;

class Atomic {
    private:
        int      _fd;
        bool     _init;
        string   _path;

        volatile int64_t  _val;
#ifndef GCC
        pthread_mutex_t   _mutex;
#endif

    private:
        void save();

    public:
        Atomic(string &path);
        ~Atomic();
        bool Init();
        int64_t Val();
        int64_t Incr();
};

#endif
