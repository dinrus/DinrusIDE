#include "clock.h"

#if defined (flagPOSIX)
#define _POSIX_C_SOURCE 200809L

#include <time.h>

#include "error.h"

int64_t now(void)
{
  struct timespec timespec = { 0 };

  int r = clock_gettime(CLOCK_REALTIME, &timespec);
  ASSERT_UNUSED(r == 0);

  return timespec.tv_sec * 1000 + timespec.tv_nsec / 1000000;
}
#endif

#if defined(flagWIN32)
#include <windows.h>

int64_t now(void)
{
  return (int64_t) GetTickCount64();
}
#endif