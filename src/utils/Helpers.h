#ifndef SPACESHOOTER_HELPERS_H
#define SPACESHOOTER_HELPERS_H

template <typename T> int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}

#endif
