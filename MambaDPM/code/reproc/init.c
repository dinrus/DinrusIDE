#include "init.h"

#if defined(flagWIN32)
#include <winsock2.h>
#include "error.h"

int init(void)
{
  WSADATA data;
  int r = WSAStartup(MAKEWORD(2, 2), &data);
  return -r;
}

void deinit(void)
{
  int saved = WSAGetLastError();

  int r = WSACleanup();
  ASSERT_UNUSED(r == 0);

  WSASetLastError(saved);
}
#else
#define _POSIX_C_SOURCE 200809L
int init(void)
{
  return 0;
}
void deinit(void) {}
#endif
