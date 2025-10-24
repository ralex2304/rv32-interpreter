#pragma once

#ifdef __cplusplus
extern "C" {
#endif

extern long read( int fd, char* data, long maxlen);
extern long write( int fd, const char* data, long len);

#ifdef __cplusplus
}
#endif

#define stdin 0
#define stdout 1
#define stderr 2

