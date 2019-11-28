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
    int fd_message = shm_open(SHARED_MEMORY__MESSAGE, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd_message, SIZE);
    char *msg = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_message, 0);

    int fd_counter = shm_open(SHARED_MEMORY__COUNTER, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd_counter, sizeof(sem_t));
    sem_t *s = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_counter, 0);

    sem_init(s,1, 0);
    
    do {
	scanf("%s", msg);
        sem_post(s);
    } while (strcmp(msg, "") != 0);

    munmap(msg, SIZE);
    shm_unlink(SHARED_MEMORY__MESSAGE);
    munmap(s, sizeof(sem_t));
    shm_unlink(SHARED_MEMORY__MESSAGE);

    return 0;
}
