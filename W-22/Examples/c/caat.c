#include <unistd.h>

#define MASK (1 | (1 << 4) | (1 << 8) | (1 << 14) | (1 << 20))
#define ORD(c) (1 << ((c | 040) - 'a'))
int main(void) {
  for (int c; read(0, &c, 1) > 0; (ORD(c) & MASK && write(1, &c, 1))) write(1, &c, 1);
  return 0;
}
