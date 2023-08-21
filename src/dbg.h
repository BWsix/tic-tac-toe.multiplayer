// Zed’s Awesome Debug Macros, Slightly modified by BWsix
// Source: "Learn C the Hard Way" chapter 19 (page 107-108)

#ifndef DBG_H
#define DBG_H

#include <errno.h>
#include <stdio.h>
#include <string.h>

#ifdef NO_DEBUG
#define dbg(M, ...)
#else
#define dbg(M, ...)                                                            \
  fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...)                                                        \
  fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__,    \
          clean_errno(), ##__VA_ARGS__)

#define log_warn(M, ...)                                                       \
  fprintf(stderr, "[WARN] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__,     \
          clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...)                                                       \
  fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define check(A, M, ...)                                                       \
  if (!(A)) {                                                                  \
    log_err(M, ##__VA_ARGS__);                                                 \
    errno = 0;                                                                 \
    goto error;                                                                \
  }

#define sentinel(M, ...)                                                       \
  {                                                                            \
    log_err(M, ##__VA_ARGS__);                                                 \
    errno = 0;                                                                 \
    goto error;                                                                \
  }

#define check_mem(A) check((A), "Out of memory.")
#define check_dbg(A, M, ...)                                                   \
  if (!(A)) {                                                                  \
    dbg(M, ##__VA_ARGS__);                                                     \
    errno = 0;                                                                 \
    goto error;                                                                \
  }

#endif // !DBG_H
