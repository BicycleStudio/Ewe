#ifndef DATA_H_
#define DATA_H_

#include "Space.h"

namespace data {

class Object {
public:
  space::Point position;
  space::Point scale;

  Object ( );
  ~Object ( );
};

}

#endif // DATA_H_