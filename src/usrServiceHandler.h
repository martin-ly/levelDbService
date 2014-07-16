#ifndef USR_SERVICE_HANDLER_H
#define USR_SERVICE_HANDLER_H

#include "usrService.h"
#include <string>

using namespace std;

class usrServiceHandler : virtual public usrServiceIf {
 public:
  usrServiceHandler() {}
  void getUsr(std::string& _return, const int32_t uid);
  int32_t getUid(const std::string& usr);
};

#endif
