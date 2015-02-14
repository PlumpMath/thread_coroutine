#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "tc_module.h"

void *
_runner(void *arg) {
    int *id = arg;

    tc_module_init(tc_module_create(), *id);

    while (1) {
        tc_module_recv(tc_module_create(), *id, "hello", 5, "normal");
        //sleep(1);
    }
}

int
main() {
    int max = 10, i, *temp;
    pthread_t *threadid = (pthread_t *)malloc(max*sizeof(pthread_t));
    tc_module *module = tc_module_create();

    //tc_module_load(module, "./lualib/tc.lua");
    //tc_module_call(module);
    //tc_module_load(module, "./lualib/normal.lua");
    //tc_module_call(module);

    for (i=1; i<max; i++) {
        temp = (int *)malloc(sizeof(int));
        *temp = i;
        pthread_create((pthread_t *)&(threadid[i]), NULL, _runner, temp);
    }

    for (i=1; i<max; i++) {
        pthread_join(threadid[i], NULL);
    }

    free(threadid);
    tc_module_release(module);

    return 0;
}
