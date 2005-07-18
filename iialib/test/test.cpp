#include <stdio.h>
#include <stdint.h>

class A {
      public:
          uint32_t   type;
          uint8_t    data[14];
};

class B : public A {
      public:
          uint8_t data[16];
};

int main() {
    return 0;
}
