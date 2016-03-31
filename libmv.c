#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>

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


//Changes the path to /app
char* fixpath(char* str)
{
	if(!strncmp(str, "/usr", strlen("/usr"))) {

        char *new = malloc(1000);

        strcpy(new, str);

        assert(4 != 0 && new != 0);
        size_t len = strlen(new);
        if (4 > len)
            return;  // Or: n = len;
        memmove(new, new+4, len - 4 + 1); //Remove /usr

        strcat("/app",new);
        printf("[libmv] New path for %s is %s\n", str, new);

        return new;
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
