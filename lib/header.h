// for all usage header.
//

#if !defined(HEADER_H__)
#define HEADER_H__

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "parser.h"

#include "error.h"

typedef enum
{
  FALSE,
  TRUE
} Boolean;

#define min(m, n) ((m) < (n) ? (m) : (n))
#define max(m, n) ((m) > (n) ? (m) : (n))

#endif // HEADER_H__
