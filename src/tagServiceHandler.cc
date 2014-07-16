#include "tagServiceHandler.h"
#include "table.h"
#include "atomic.h"
#include <stdio.h>

using namespace std;

extern Table *gTagTid;
extern Table *gTidTag;
extern Atomic *gAtomic;

void tagServiceHandler::getTag(string &_return, const int16_t tid)
{
    int   n;
    char  numbuf[32];

    n = snprintf(numbuf, sizeof(numbuf), "%u", tid);
    n = n <= sizeof(numbuf) ? n : sizeof(numbuf);

    string key(numbuf, n);
    string val;

    if (gTidTag->Get(key, &val)) {
        _return = val;
    } else {
        _return = "";
    }
}

int16_t tagServiceHandler::getTid(const string &tag)
{
    string   tid;

    if (gTagTid->Get(tag, &tid)) {
        return (int16_t)atoi(tid.c_str());
    }

    int      n;
    char     numbuf[32];
    int16_t  id;

    id = (int16_t)gAtomic->Incr();
    n = snprintf(numbuf, sizeof(numbuf), "%u", id);
    n = n <= sizeof(numbuf) ? n : sizeof(numbuf);
    tid.assign(numbuf, n);
    gTagTid->Put(tag, tid);
    gTidTag->Put(tid, tag);
    return id;
}



