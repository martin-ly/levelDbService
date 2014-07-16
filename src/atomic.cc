#include "atomic.h"

#include <iostream>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

using namespace std;

Atomic::Atomic(string &path) :
    _init(false),
    _path(path),
    _val(-1)
{
    _fd = open(path.c_str(), O_RDWR | O_CREAT);
    if (_fd < 0) {
        cout << "open file " << path << " errno " << errno << endl;
    }
}

Atomic::~Atomic()
{
    struct flock  lock;

    if (_init) {
        lock.l_type = F_UNLCK;
        lock.l_start = 0;
        lock.l_whence = SEEK_SET;
        lock.l_len = 0;
        save();
        fcntl(_fd, F_SETLK, &lock);
        close(_fd);
#ifndef GCC
        pthread_mutex_destroy(&_mutex, NULL);
#endif
    }
}

bool Atomic::Init()
{
    if (_init) {
        return true;
    }
    if (_fd == -1) {
        cout << "open file error" << endl;
        return false;
    }

    struct flock  lock;
    lock.l_type = F_WRLCK;
    lock.l_start = 0;
    lock.l_whence = SEEK_SET;
    lock.l_len = 0;
    if (fcntl(_fd, F_SETLK, &lock) == -1) {
        cout << "fcntl F_SETLK errno: " << errno << endl;
        return false;
    }

    char     buf[32];
    ssize_t  n;

    n = read(_fd, buf, sizeof(buf));
    lseek(_fd, 0, SEEK_SET);
    switch (n) {
        /* some error happen */
        case -1:
            cout << "read atomic file " << _path << " errno: " << errno << endl;
            return false;

        /* empty file */
        case 0:
            write(_fd, "0", 1);
            lseek(_fd, 0, SEEK_SET);
            _val = 0;
            break;

        /* atomic val exist */
        default:
            buf[31] = 0;
            _val = (int64_t)atoll(buf);
            break;
    }
#ifndef GCC
    pthread_mutex_init(&_mutex, NULL);
#endif
    _init = true;
    return true;
}

int64_t Atomic::Val()
{
    return _init ? _val : (int64_t)-1;
}

int64_t Atomic::Incr()
{
    int64_t v;

    if (!_init) {
        return (int64_t)-1;
    }

#ifdef GCC
    v = __sync_add_and_fetch(&_val, 1);
#else
    pthread_mutex_lock(&_mutex);
    v = ++_val;
    pthread_mutex_unlock(&_mutex);
#endif

    /* 这里save的实现不能保证并发环境下
     * 文件中存储的值和真实的_val一致，
     * 但是由于在~Atomic()中也调用了save，
     * 能够保证最终一致性，
     * 对于目前的需求是足够了*/
    save();
    return v;
}

void Atomic::save()
{
    int   n;
    char  buf[32];

    n = snprintf(buf, sizeof(buf), "%llu", _val);
    n = n <= sizeof(buf) ? n : sizeof(buf);
    pwrite(_fd, buf, n, (off_t)0);
}









