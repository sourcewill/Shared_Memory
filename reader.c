#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define SIZE 256
#define  SHARED_MEMORY__MESSAGE "/message"
#define  SHARED_MEMORY__COUNTER "/counter"

int main() {
    int fd_message = shm_open(SHARED_MEMORY__MESSAGE, O_RDONLY, S_IRUSR);
    int fd_counter = shm_open(SHARED_MEMORY__COUNTER, O_RDWR, S_IRUSR | S_IWUSR);

    if (fd_message == -1 || fd_counter == -1) {
        printf("Error opening the shared memory file\n");
        return 1;
    }

    ftruncate(fd_message, SIZE);
    char *msg = mmap(NULL, SIZE, PROT_READ, MAP_SHARED, fd_message, 0);
    ftruncate(fd_counter, sizeof(sem_t));
    sem_t *s = mmap(NULL, SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd_counter, 0);

    do {
        sem_wait(s);
        getchar();
        printf("Last message: %s\n", msg);
    } while (strcmp(msg, "") != 0);

    munmap(msg, SIZE);
    shm_unlink(SHARED_MEMORY__MESSAGE);
    munmap(s, sizeof(sem_t));
    shm_unlink(SHARED_MEMORY__MESSAGE);

    return 0;
}