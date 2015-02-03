#include <sys/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <dlfcn.h>

#include <string.h>
#include <string>

#define max(a,b) ((a) < (b) ? (b) : (a))
#define min(a,b) ((a) < (b) ? (a) : (b))

#define ENABLE_DAEMON
