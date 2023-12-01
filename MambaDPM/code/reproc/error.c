#include "error.h"

#if defined (flagPOSIX)
#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <reproc/reproc.h>

#include "macro.h"

const int REPROC_EINVAL = -EINVAL;
const int REPROC_EPIPE = -EPIPE;
const int REPROC_ETIMEDOUT = -ETIMEDOUT;
const int REPROC_ENOMEM = -ENOMEM;
const int REPROC_EWOULDBLOCK = -EWOULDBLOCK;

enum { ERROR_STRING_MAX_SIZE = 512 };

const char *error_string(int error)
{
  static THREAD_LOCAL char string[ERROR_STRING_MAX_SIZE];

  int r = strerror_r(abs(error), string, ARRAY_SIZE(string));
  if (r != 0) {
    return "Не удалось получить строку ошибки";
  }

  return string;
}
#endif

#if defined(flagWIN32)
#include <limits.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>

#include <reproc/reproc.h>

#include "macro.h"

const int REPROC_EINVAL = -ERROR_INVALID_PARAMETER;
const int REPROC_EPIPE = -ERROR_BROKEN_PIPE;
const int REPROC_ETIMEDOUT = -WAIT_TIMEOUT;
const int REPROC_ENOMEM = -ERROR_NOT_ENOUGH_MEMORY;
const int REPROC_EWOULDBLOCK = -WSAEWOULDBLOCK;

enum { ERROR_STRING_MAX_SIZE = 512 };

const char *error_string(int error)
{
  wchar_t *wstring = NULL;
  int r = -1;

  wstring = malloc(sizeof(wchar_t) * ERROR_STRING_MAX_SIZE);
  if (wstring == NULL) {
    return "Не удалось разместить память под строку ошибки";
  }

  // We don't expect message sizes larger than the maximum possible int.
  r = (int) FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM |
                               FORMAT_MESSAGE_IGNORE_INSERTS,
                           NULL, (DWORD) abs(error),
                           MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), wstring,
                           ERROR_STRING_MAX_SIZE, NULL);
  if (r == 0) {
    free(wstring);
    return "Не удалось получить строку ошибки";
  }

  static THREAD_LOCAL char string[ERROR_STRING_MAX_SIZE];

  r = WideCharToMultiByte(CP_UTF8, 0, wstring, -1, string, ARRAY_SIZE(string),
                          NULL, NULL);
  free(wstring);
  if (r == 0) {
    return "Не удалось преобразовать строку ошибки в UTF-8";
  }

  // Remove trailing whitespace and period.
  if (r >= 4) {
    string[r - 4] = '\0';
  }

  return string;
}

#endif
