#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *thread(void *args);

int main() {
    pthread_t t;
    pthread_create(&t, NULL, thread, NULL);

    return 0;
}

// Thread function
void *thread(void *args) {
    sleep(1);
    printf("Hello world!\n");
    return NULL;
}
