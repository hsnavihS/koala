#pragma once

#include <ctime>

#include <types/Callable.h>

class Time : public Callable {
public:
  int arity() { return 0; }

  int call() { return time(nullptr); }

  string toString() { return "<stdlib func: time>"; }
};
