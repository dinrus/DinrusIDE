#ifdef flagWIN32
#include "redirect.h"

#include <io.h>
#include <stdlib.h>
#include <windows.h>

#include "error.h"
#include "pipe.h"
#include "utf.h"

static DWORD stream_to_id(REPROC_STREAM stream)
{
  switch (stream) {
    case REPROC_STREAM_IN:
      return STD_INPUT_HANDLE;
    case REPROC_STREAM_OUT:
      return STD_OUTPUT_HANDLE;
    case REPROC_STREAM_ERR:
      return STD_ERROR_HANDLE;
  }

  return 0;
}

int redirect_parent(HANDLE *child, REPROC_STREAM stream)
{
  ASSERT(child);

  DWORD id = stream_to_id(stream);
  if (id == 0) {
    return -ERROR_INVALID_PARAMETER;
  }

  HANDLE *handle = GetStdHandle(id);
  if (handle == INVALID_HANDLE_VALUE) {
    return -(int) GetLastError();
  }

  if (handle == NULL) {
    return -ERROR_BROKEN_PIPE;
  }

  *child = handle;

  return 0;
}

enum { FILE_NO_TEMPLATE = 0 };

int redirect_discard(HANDLE *child, REPROC_STREAM stream)
{
  return redirect_path(child, stream, "NUL");
}

int redirect_file(HANDLE *child, FILE *file)
{
  ASSERT(child);
  ASSERT(file);

  int r = _fileno(file);
  if (r < 0) {
    return -ERROR_INVALID_HANDLE;
  }

  intptr_t result = _get_osfhandle(r);
  if (result == -1) {
    return -ERROR_INVALID_HANDLE;
  }

  *child = (HANDLE) result;

  return 0;
}

int redirect_path(handle_type *child, REPROC_STREAM stream, const char *path)
{
  ASSERT(child);
  ASSERT(path);

  DWORD mode = stream == REPROC_STREAM_IN ? GENERIC_READ : GENERIC_WRITE;
  HANDLE handle = HANDLE_INVALID;
  int r = -1;

  wchar_t *wpath = utf16_from_utf8(path, -1);
  if (wpath == NULL) {
    r = -(int) GetLastError();
    goto finish;
  }

  SECURITY_ATTRIBUTES do_not_inherit = { .nLength = sizeof(SECURITY_ATTRIBUTES),
                                         .bInheritHandle = false,
                                         .lpSecurityDescriptor = NULL };

  handle = CreateFileW(wpath, mode, FILE_SHARE_READ | FILE_SHARE_WRITE,
                       &do_not_inherit, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL,
                       (HANDLE) FILE_NO_TEMPLATE);
  if (handle == INVALID_HANDLE_VALUE) {
    r = -(int) GetLastError();
    goto finish;
  }

  *child = handle;
  handle = HANDLE_INVALID;
  r = 0;

finish:
  free(wpath);
  handle_destroy(handle);

  return r;
}

#else
#define _POSIX_C_SOURCE 200809L

#include "redirect.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>

#include "error.h"
#include "pipe.h"

static FILE *stream_to_file(REPROC_STREAM stream)
{
  switch (stream) {
    case REPROC_STREAM_IN:
      return stdin;
    case REPROC_STREAM_OUT:
      return stdout;
    case REPROC_STREAM_ERR:
      return stderr;
  }

  return NULL;
}

int redirect_parent(int *child, REPROC_STREAM stream)
{
  ASSERT(child);

  FILE *file = stream_to_file(stream);
  if (file == NULL) {
    return -EINVAL;
  }

  int r = fileno(file);
  if (r < 0) {
    return errno == EBADF ? -EPIPE : -errno;
  }

  *child = r; // `r` contains the duplicated file descriptor.

  return 0;
}

int redirect_discard(int *child, REPROC_STREAM stream)
{
  return redirect_path(child, stream, "/dev/null");
}

int redirect_file(int *child, FILE *file)
{
  ASSERT(child);

  int r = fileno(file);
  if (r < 0) {
    return -errno;
  }

  *child = r;

  return 0;
}

int redirect_path(int *child, REPROC_STREAM stream, const char *path)
{
  ASSERT(child);
  ASSERT(path);

  int mode = stream == REPROC_STREAM_IN ? O_RDONLY : O_WRONLY;

  int r = open(path, mode | O_CREAT | O_CLOEXEC, 0640);
  if (r < 0) {
    return -errno;
  }

  *child = r;

  return 0;
}
#endif