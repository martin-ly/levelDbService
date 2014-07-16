#ifndef TAG_SERVICE_HANDLER_H
#define TAG_SERVICE_HANDLER_H

#include "tagService.h"
#include <string>

using namespace std;

class tagServiceHandler : virtual public tagServiceIf {
 public:
  tagServiceHandler() {}
  void getTag(string& _return, const int16_t tid);
  int16_t getTid(const string& tag);
};

#endif
