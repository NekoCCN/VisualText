/* */
#include <float.h>
#include <inttypes.h>
#include <limits.h>
#include <malloc.h>
#include <math.h>
#include <memory.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <time.h>
#include <wchar.h>

int main(int argc, char** argv)
{
  (void)argv;
#ifndef ceilf
  return ((int*)(&ceilf))[argc];
#else
  (void)argc;
  return 0;
#endif
}
