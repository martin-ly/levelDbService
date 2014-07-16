#ifndef LEVEL_SERVICE_TABLE_H
#define LEVEL_SERVICE_TABLE_H

#include "leveldb/db.h"

using namespace std;

class Table {
    private:
        string        _name;
        string        _path;
        bool          _opened;
        leveldb::DB  *_db;

        leveldb::ReadOptions  _ropt;
        leveldb::WriteOptions _wopt;

    public:
        Table(string &name, string &path);
        virtual ~Table();
        bool Open();
        bool Opened();
        void Close();
        bool Get(const string &key, string *val);
        bool Put(const string &key, const string &value);
        bool Delete(const string &key);
};

#endif
