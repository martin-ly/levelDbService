#include "usrServiceHandler.h"
#include "table.h"
#include "atomic.h"
#include <stdio.h>

using namespace std;

extern Table *gUsrUid;
extern Table *gUidUsr;
extern Atomic *gAtomic;

void usrServiceHandler::getUsr(string &_return, const int32_t uid)
{
    int   n;
    char  numbuf[32];

    n = snprintf(numbuf, sizeof(numbuf), "%u", uid);
    n = n <= sizeof(numbuf) ? n : sizeof(numbuf);

    string key(numbuf, n);
    string val;

    if (gUidUsr->Get(key, &val)) {
        _return = val;
    } else {
        _return = "";
    }
}

int32_t usrServiceHandler::getUid(const string &usr)
{
    string   uid;

    if (gUsrUid->Get(usr, &uid)) {
        return (int32_t)atoi(uid.c_str());
    }

    int      n;
    char     numbuf[32];
    int32_t  id;

    id = (int32_t)gAtomic->Incr();
    n = snprintf(numbuf, sizeof(numbuf), "%u", id);
    n = n <= sizeof(numbuf) ? n : sizeof(numbuf);
    uid.assign(numbuf, n);
    gUsrUid->Put(usr, uid);
    gUidUsr->Put(uid, usr);
    return id;
}

