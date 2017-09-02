#ifndef getcwd_H
#define getcwd_H

#ifdef getcwd_IMPORT
#define EXTERN
#else
#define EXTERN extern
#endif

char mainFilePath[100];

#undef getcwd_IMPORT
#undef EXTERN
#endif
