#include "handle.h"
#include "error.h"

#if defined (flagPOSIX)
#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

const int HANDLE_INVALID = -1;

int handle_cloexec(int handle, bool enable)
{
  int r = -1;

  r = fcntl(handle, F_GETFD, 0);
  if (r < 0) {
    return -errno;
  }

  r = enable ? r | FD_CLOEXEC : r & ~FD_CLOEXEC;

  r = fcntl(handle, F_SETFD, r);
  if (r < 0) {
    return -errno;
  }

  return 0;
}

int handle_destroy(int handle)
{
  if (handle == HANDLE_INVALID) {
    return HANDLE_INVALID;
  }

  int r = close(handle);
  ASSERT_UNUSED(r == 0);

  return HANDLE_INVALID;
}
#endif

#if defined(flagWIN32)
#include <windows.h>

const HANDLE HANDLE_INVALID = INVALID_HANDLE_VALUE; // NOLINT

// `handle_cloexec` is POSIX-only.

HANDLE handle_destroy(HANDLE handle)
{
  if (handle == NULL || handle == HANDLE_INVALID) {
    return HANDLE_INVALID;
  }

  int r = CloseHandle(handle);
  ASSERT_UNUSED(r != 0);

  return HANDLE_INVALID;
}
#endif