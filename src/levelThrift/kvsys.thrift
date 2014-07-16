service usrService {
    string getUsr(1: i32 uid)
    i32 getUid(1: string usr)
}

service tagService {
    string getTag(1: i16 tid)
    i16 getTid(1: string tag)
}
