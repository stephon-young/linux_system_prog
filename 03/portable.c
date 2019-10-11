// for portable:
// _POSIX_SOURCE/_POSIX_C_SOURCE
// _XOPEN_SOURCE
// _BSD_SOURCE
// _SVID_SOURCE
// _GNU_SOURCE

#include <stdio.h>

#if defined(_POSIX_SOURCE)
long posix = _POSIX_SOURCE;
#else
long posix = -1;
#endif

#if defined(_POSIX_C_SOURCE)
long posix_c = _POSIX_C_SOURCE;
#else
long posix_c = -1;
#endif

#if defined(_XOPEN_SOURCE)
long xopen = _XOPEN_SOURCE;
#else
long xopen = -1;
#endif

#if defined(_BSD_SOURCE)
long bsd = _BSD_SOURCE;
#else
long bsd = -1;
#endif

#if defined(_SVID_SOURCE)
long svid = _SVID_SOURCE;
#else
long svid = -1;
#endif

#if defined(_GNU_SOURCE)
long gnu = _GNU_SOURCE;
#else
long gnu = -1;
#endif

long main() {
  if (posix != -1)
    prlongf("_POSIX_SOURCE defined = %ld\n", posix);

  if (posix_c != -1)
    prlongf("_POSIX_C_SOURCE defined = %ld\n", posix_c);

  if (xopen != -1)
    prlongf("_XOPEN_SOURCE defined = %ld\n", xopen);

  if (bsd != -1)
    prlongf("_BSD_SOURCE defined = %ld\n", bsd);

  if (svid != -1)
    prlongf("_SVID_SOURCE defined = %ld\n", svid);

  if (gnu != -1)
    prlongf("_GNU_SOURCE defined = %ld\n", gnu);

  return 0;
}