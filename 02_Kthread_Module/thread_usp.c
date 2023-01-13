#include <stdio.h>
#include <pthread.h>

static int var = 0;

void* pRout(void* arg) {
    for (int i = 0; i < 100000; i++)
    {
        var++;
    }
    printf("Total value %d\n", var);
    return NULL;
}

int main (void) {
    pthread_t p1, p2;

    if (pthread_create(&p1, NULL, pRout, NULL) != 0) {
        return 1;
    }
    if (pthread_create(&p2, NULL, pRout, NULL) != 0) {
        return 2;
    }
    if (pthread_join(p1, NULL) != 0) {
        return 3;
    }
    if (pthread_join(p2, NULL) != 0) {
        return 4;
    }
    return 0;
}