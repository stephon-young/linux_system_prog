
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include "parser.h"

static void
fail(const char *fname, const char *msg, const char *arg, const char *name)
{
  fprintf(stderr, "%s error", fname);
  if (fname != NULL)
    fprintf(stderr, " (in %s)", name);
  fprintf(stderr, ": %s\n", msg);
  if (arg != NULL && *arg != '\0')
    fprintf(stderr, "        offending text: %s\n", arg);

  exit(EXIT_FAILURE);
}

static long
parse(const char *fname, const char *arg, int flags, const char *name)
{
  long res;
  char *endptr;
  int base;

  if (arg == NULL || *arg == '\0')
    fail(fname, "null or empty string", arg, name);

  base = (flags & PSR_ANY_BASE) ? 0 : (flags & PSR_BASE_8) ? 8 : (flags & PSR_BASE_16) ? 16 : 10;

  errno = 0;
  res = strtol(arg, &endptr, base);
  if (errno != 0)
    fail(fname, "strtol() failed", arg, name);

  if (*endptr != '\0')
    fail(fname, "nonnumeric characters", arg, name);

  if ((flags & PSR_NONNEG) && res < 0)
    fail(fname, "negative value not allowed", arg, name);

  if ((flags & PSR_GT_0) && res <= 0)
    fail(fname, "value must be > 0", arg, name);

  return res;
}

long getLong(const char *arg, int flags, const char *name)
{
  return parse("getLong", arg, flags, name);
}

int getInt(const char *arg, int flags, const char *name)
{
  long res;

  res = parse("getInt", arg, flags, name);
  if (res > INT_MAX || res < INT_MIN)
    fail("getInt", "integer out of range", arg, name);

  return (int)res;
}