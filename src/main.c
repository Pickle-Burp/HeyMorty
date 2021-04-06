#include <stdio.h>
#include "sprec/sprec.h"

int main() {
  printf("Hello, World!\n");
  sprec_recognize_sync("fr", "fr", 100);
  return 0;
}
