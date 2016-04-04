#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <limits.h>
#include <stdlib.h>

 #ifndef _ASSERT_H
 #define _ASSERT_H
 
 #ifdef NDEBUG
 # define assert(EX)
 #else
 # define assert(EX) (void)((EX) || (__assert (#EX, __FILE__, __LINE__),0))
 #endif
 
 #ifdef __cplusplus
 extern "C" {
 #endif
 
 extern void __assert (const char *msg, const char *file, int line);
 
 #ifdef __cplusplus
 };
 #endif
 #endif



char buf[PATH_MAX + 1]; /* not sure about the "+ 1" */

//Changes the path to /app
char* fixpath(char* str)
{
	if(!strncmp(str, "/usr", strlen("/usr"))) {

        strcpy(str, realpath(str, buf)); //Run realpath on string to improve compatibility

        memcpy(str, "/app", strlen("/app"));

        printf("[libmv] New path is %s\n",str);

        //We cannot use replace because if we use replace /usr/dir/usr/file will become /app/dir/app/file
        //The currently used method removes the first 4 characters and inserts /app to the start.	
    }
	return str;
}

int open(const char *fn, int flags) {
    static int (*real_open)(const char *fn, int flags);

    if (!real_open) {
    	real_open = dlsym(RTLD_NEXT, "open");
    }
    return real_open(fixpath(fn), flags);
}
