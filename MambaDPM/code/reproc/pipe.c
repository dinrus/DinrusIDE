
#ifdef flagWIN32
#include "pipe.h"

#include <limits.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>

#include "error.h"
#include "handle.h"
#include "macro.h"

const SOCKET PIPE_INVALID = INVALID_SOCKET;

const short PIPE_EVENT_IN = POLLIN;
const short PIPE_EVENT_OUT = POLLOUT;

// Inspired by https://gist.github.com/geertj/4325783.
static int socketpair(int domain, int type, int protocol, SOCKET *out)
{
  ASSERT(out);

  SOCKET server = PIPE_INVALID;
  SOCKET pair[] = { PIPE_INVALID, PIPE_INVALID };
  int r = -1;

  server = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
  if (server == INVALID_SOCKET) {
    r = -WSAGetLastError();
    goto finish;
  }

  SOCKADDR_IN localhost = { 0 };
  localhost.sin_family = AF_INET;
  localhost.sin_addr.S_un.S_addr = htonl(INADDR_LOOPBACK);
  localhost.sin_port = 0;

  r = bind(server, (SOCKADDR *) &localhost, sizeof(localhost));
  if (r < 0) {
    r = -WSAGetLastError();
    goto finish;
  }

  r = listen(server, 1);
  if (r < 0) {
    r = -WSAGetLastError();
    goto finish;
  }

  SOCKADDR_STORAGE name = { 0 };
  int size = sizeof(name);
  r = getsockname(server, (SOCKADDR *) &name, &size);
  if (r < 0) {
    r = -WSAGetLastError();
    goto finish;
  }

  pair[0] = WSASocketW(domain, type, protocol, NULL, 0, 0);
  if (pair[0] == INVALID_SOCKET) {
    r = -WSAGetLastError();
    goto finish;
  }

  struct {
    WSAPROTOCOL_INFOW data;
    int size;
  } info = { { 0 }, sizeof(WSAPROTOCOL_INFOW) };

  r = getsockopt(pair[0], SOL_SOCKET, SO_PROTOCOL_INFOW, (char *) &info.data,
                 &info.size);
  if (r < 0) {
    goto finish;
  }

  // We require the returned sockets to be usable as Windows file handles. This
  // might not be the case if extra LSP providers are installed.

  if (!(info.data.dwServiceFlags1 & XP1_IFS_HANDLES)) {
    r = -ERROR_NOT_SUPPORTED;
    goto finish;
  }

  r = pipe_nonblocking(pair[0], true);
  if (r < 0) {
    goto finish;
  }

  r = connect(pair[0], (SOCKADDR *) &name, size);
  if (r < 0 && WSAGetLastError() != WSAEWOULDBLOCK) {
    r = -WSAGetLastError();
    goto finish;
  }

  r = pipe_nonblocking(pair[0], false);
  if (r < 0) {
    goto finish;
  }

  pair[1] = accept(server, NULL, NULL);
  if (pair[1] == INVALID_SOCKET) {
    r = -WSAGetLastError();
    goto finish;
  }

  out[0] = pair[0];
  out[1] = pair[1];

  pair[0] = PIPE_INVALID;
  pair[1] = PIPE_INVALID;

finish:
  pipe_destroy(server);
  pipe_destroy(pair[0]);
  pipe_destroy(pair[1]);

  return r;
}

int pipe_init(SOCKET *read, SOCKET *write)
{
  ASSERT(read);
  ASSERT(write);

  SOCKET pair[] = { PIPE_INVALID, PIPE_INVALID };
  int r = -1;

  // Use sockets instead of pipes so we can use `WSAPoll` which only works with
  // sockets.
  r = socketpair(AF_INET, SOCK_STREAM, 0, pair);
  if (r < 0) {
    goto finish;
  }

  r = SetHandleInformation((HANDLE) pair[0], HANDLE_FLAG_INHERIT, 0);
  if (r == 0) {
    r = -(int) GetLastError();
    goto finish;
  }

  r = SetHandleInformation((HANDLE) pair[1], HANDLE_FLAG_INHERIT, 0);
  if (r == 0) {
    r = -(int) GetLastError();
    goto finish;
  }

  // Make the connection unidirectional to better emulate a pipe.

  r = shutdown(pair[0], SD_SEND);
  if (r < 0) {
    r = -WSAGetLastError();
    goto finish;
  }

  r = shutdown(pair[1], SD_RECEIVE);
  if (r < 0) {
    r = -WSAGetLastError();
    goto finish;
  }

  *read = pair[0];
  *write = pair[1];

  pair[0] = PIPE_INVALID;
  pair[1] = PIPE_INVALID;

finish:
  pipe_destroy(pair[0]);
  pipe_destroy(pair[1]);

  return r;
}

int pipe_nonblocking(SOCKET pipe, bool enable)
{
  u_long mode = enable;
  int r = ioctlsocket(pipe, (long) FIONBIO, &mode);
  return r < 0 ? -WSAGetLastError() : 0;
}

int pipe_read(SOCKET pipe, uint8_t *buffer, size_t size)
{
  ASSERT(pipe != PIPE_INVALID);
  ASSERT(buffer);
  ASSERT(size <= INT_MAX);

  int r = recv(pipe, (char *) buffer, (int) size, 0);

  if (r == 0) {
    return -ERROR_BROKEN_PIPE;
  }

  return r < 0 ? -WSAGetLastError() : r;
}

int pipe_write(SOCKET pipe, const uint8_t *buffer, size_t size)
{
  ASSERT(pipe != PIPE_INVALID);
  ASSERT(buffer);
  ASSERT(size <= INT_MAX);

  int r = send(pipe, (const char *) buffer, (int) size, 0);

  return r < 0 ? -WSAGetLastError() : r;
}

int pipe_poll(pipe_event_source *sources, size_t num_sources, int timeout)
{
  ASSERT(num_sources <= INT_MAX);

  WSAPOLLFD *pollfds = NULL;
  int r = -1;

  pollfds = calloc(num_sources, sizeof(WSAPOLLFD));
  if (pollfds == NULL) {
    r = -ERROR_NOT_ENOUGH_MEMORY;
    goto finish;
  }

  for (size_t i = 0; i < num_sources; i++) {
    pollfds[i].fd = sources[i].pipe;
    pollfds[i].events = sources[i].interests;
  }

  r = WSAPoll(pollfds, (ULONG) num_sources, timeout);
  if (r < 0) {
    r = -WSAGetLastError();
    goto finish;
  }

  for (size_t i = 0; i < num_sources; i++) {
    sources[i].events = pollfds[i].revents;
  }

finish:
  free(pollfds);

  return r;
}

int pipe_shutdown(SOCKET pipe)
{
  if (pipe == PIPE_INVALID) {
    return 0;
  }

  int r = shutdown(pipe, SD_SEND);
  return r < 0 ? -WSAGetLastError() : 0;
}

SOCKET pipe_destroy(SOCKET pipe)
{
  if (pipe == PIPE_INVALID) {
    return PIPE_INVALID;
  }

  int r = closesocket(pipe);
  ASSERT_UNUSED(r == 0);

  return PIPE_INVALID;
}

#else
#define _POSIX_C_SOURCE 200809L

#include "pipe.h"

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <poll.h>
#include <stdlib.h>
#include <unistd.h>

#include "error.h"
#include "handle.h"

const int PIPE_INVALID = -1;

const short PIPE_EVENT_IN = POLLIN;
const short PIPE_EVENT_OUT = POLLOUT;

int pipe_init(int *read, int *write)
{
  ASSERT(read);
  ASSERT(write);

  int pair[] = { PIPE_INVALID, PIPE_INVALID };
  int r = -1;

  r = pipe(pair);
  if (r < 0) {
    r = -errno;
    goto finish;
  }

  r = handle_cloexec(pair[0], true);
  if (r < 0) {
    goto finish;
  }

  r = handle_cloexec(pair[1], true);
  if (r < 0) {
    goto finish;
  }

  *read = pair[0];
  *write = pair[1];

  pair[0] = PIPE_INVALID;
  pair[1] = PIPE_INVALID;

finish:
  pipe_destroy(pair[0]);
  pipe_destroy(pair[1]);

  return r;
}

int pipe_nonblocking(int pipe, bool enable)
{
  int r = -1;

  r = fcntl(pipe, F_GETFL, 0);
  if (r < 0) {
    return -errno;
  }

  r = enable ? r | O_NONBLOCK : r & ~O_NONBLOCK;

  r = fcntl(pipe, F_SETFL, r);

  return r < 0 ? -errno : 0;
}

int pipe_read(int pipe, uint8_t *buffer, size_t size)
{
  ASSERT(pipe != PIPE_INVALID);
  ASSERT(buffer);

  int r = (int) read(pipe, buffer, size);

  if (r == 0) {
    // `read` returns 0 to indicate the other end of the pipe was closed.
    return -EPIPE;
  }

  return r < 0 ? -errno : r;
}

int pipe_write(int pipe, const uint8_t *buffer, size_t size)
{
  ASSERT(pipe != PIPE_INVALID);
  ASSERT(buffer);

  int r = (int) write(pipe, buffer, size);

  return r < 0 ? -errno : r;
}

int pipe_poll(pipe_event_source *sources, size_t num_sources, int timeout)
{
  ASSERT(num_sources <= INT_MAX);

  struct pollfd *pollfds = NULL;
  int r = -1;

  pollfds = calloc(num_sources, sizeof(struct pollfd));
  if (pollfds == NULL) {
    r = -errno;
    goto finish;
  }

  for (size_t i = 0; i < num_sources; i++) {
    pollfds[i].fd = sources[i].pipe;
    pollfds[i].events = sources[i].interests;
  }

  r = poll(pollfds, (nfds_t) num_sources, timeout);
  if (r < 0) {
    r = -errno;
    goto finish;
  }

  for (size_t i = 0; i < num_sources; i++) {
    sources[i].events = pollfds[i].revents;
  }

finish:
  free(pollfds);

  return r;
}

int pipe_shutdown(int pipe)
{
  (void) pipe;
  return 0;
}

int pipe_destroy(int pipe)
{
  return handle_destroy(pipe);
}
#endif