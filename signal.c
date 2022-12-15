#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MILLION 1000000



// Mutex and condition variable for synchronization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
// Function for the task to run
void *task_function(void *arg) {
    pthread_mutex_lock(&mutex);

    // Wait for the condition variable to be signaled
    pthread_cond_wait(&cond, &mutex);

    // Unlock the mutex
    pthread_mutex_unlock(&mutex);
    
    // Return from the thread
    return NULL;
}
void signal_func(){
     // Create the thread
    pthread_t thread;
    int result = pthread_create(&thread, NULL, task_function, NULL);

    // Check for errors
    if (result != 0) {
        // Handle error
    }

    // Wait for the task to go to sleep
    sleep(1);

    // Get the current time before signaling the condition variable
    struct timespec start;
    clock_gettime(CLOCK_REALTIME, &start);

    for(int i = 0;i<MILLION; i++){
    // Wake up the task using the condition variable
    pthread_cond_signal(&cond);
    }
    // Get the current time after the task has been awakened
    struct timespec end;
    clock_gettime(CLOCK_REALTIME, &end);

    // Calculate the elapsed time in nanoseconds
    long elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);

    // Print the elapsed time
    printf("Elapsed time: %ldns\n", elapsed_ns);

}



void fcntl_b(){//https://www.informit.com/articles/article.aspx?p=23618&seqNum=4
    char* file = "file.txt";
    int fd;
    struct flock lock;

    printf ("opening %s\n", file);
    /* Open a file descriptor to the file. */
    fd = open (file, O_WRONLY);
    struct timespec start;
    clock_gettime(CLOCK_REALTIME, &start);

    for (int i = 0; i < MILLION; i++)
    {
        // if(i%10000 ==0 )
        //     printf ("locking\n");

        /* Initialize the flock structure. */
        memset (&lock, 0, sizeof(lock));
        lock.l_type = F_WRLCK;
        /* Place a write lock on the file. */
        fcntl (fd, F_SETLKW, &lock);

        // printf ("locked; \n");

        // printf ("unlocking\n");
        /* Release the lock. */
        lock.l_type = F_UNLCK;
        fcntl (fd, F_SETLKW, &lock);
    }
    // Get the current time after the task has been awakened
    struct timespec end;
    clock_gettime(CLOCK_REALTIME, &end);

    // Calculate the elapsed time in nanoseconds
    long elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);

    // Print the elapsed time
    printf("Elapsed time: %ldns\n", elapsed_ns);

    close (fd);
}




// signal-handling function
void signal_handler(int signo) {
}

void packet(){
  // create signal-handling function
  struct sigaction act;
  act.sa_handler = signal_handler;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  sigaction(SIGUSR1, &act, NULL);

  // create sockets for communication
  int sockets[2];
  socketpair(AF_UNIX, SOCK_STREAM, 0, sockets);

  // record start time
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);

  for (int i = 0; i < MILLION; i++) {
    // send signal using socket
    char c = 1;
    write(sockets[0], &c, 1);
    read(sockets[1], &c, 1);
  }

  // record end time
  clock_gettime(CLOCK_MONOTONIC, &end);

  // calculate elapsed time
  long elapsed_nsec = (end.tv_sec - start.tv_sec) * 1000000000 +
                      (end.tv_nsec - start.tv_nsec);
  double elapsed_sec = elapsed_nsec / 1000000000.0;

  // print average time per iteration
  printf("Average time per iteration: %f seconds\n",
         elapsed_sec );

}



int main (int argc, char* argv[])
{
    // signal_func();
    // fcntl_b();
    packet();
    return 0;

    //in packet we calculated avg time and in seconds. in fcntl and signal we calculated sum time and in nanoseconds.
}