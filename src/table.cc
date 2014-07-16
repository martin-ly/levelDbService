#include "table.h"

#include <iostream>

using namespace std;

Table::Table(string &name, string &path)
    :_name(name), _path(path),
    _opened(false), _db(NULL)
{
    _ropt = leveldb::ReadOptions();
    _wopt = leveldb::WriteOptions();
}

Table::~Table()
{
    if (_opened && _db) {
        delete _db;
    }
}

bool Table::Open()
{
    if (_opened) {
        return true;
    }

    leveldb::Options opt;
    opt.create_if_missing = true;
    leveldb::Status s = leveldb::DB::Open(opt, _path, &_db);
    if (s.ok()) {
        _opened = true;
        return true;
    } else {
        cout << "open db <" << _name << "> error" << s.ToString() << endl;
        return false;
    }
}

bool Table::Opened()
{
    return _opened;
}

void Table::Close()
{
    if (_opened) {
        _opened = false;
        if (_db) {
            delete _db;
            _db = NULL;
        }
    }
}

bool Table::Get(const string &key, string *val)
{
    if (!_opened && !Open()) {
        cout << "Cannot Open table " << _name << " Get " << key << endl;
        return false;
    }
    leveldb::Status s = _db->Get(_ropt, key, val);
    if (!s.ok()) {
        cout << "Get " << key << " error: " << s.ToString() << endl;
    }
    return s.ok();
}

bool Table::Put(const string &key, const string &value)
{
    if (!_opened && !Open()) {
        cout << "Cannot Open table " << _name << " Put " << key << endl;
        return false;
    }
    leveldb::Status s = _db->Put(_wopt, key, value);
    if (!s.ok()) {
        cout << "Put " << key << " error: " << s.ToString() << endl;
    }
    return s.ok();
}

bool Table::Delete(const string &key)
{
    if (!_opened && !Open()) {
        cout << "Cannot Open table " << _name << " Delete " << key << endl;
        return false;
    }
    leveldb::Status s = _db->Delete(_wopt, key);
    if (!s.ok()) {
        cout << "Del " << key << " error: " << s.ToString() << endl;
    }
    return s.ok();
}

