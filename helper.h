/******************************************************************
 * Header file for the helper functions. This file includes the
 * required header files, as well as the function signatures and
 * the semaphore values (which are to be changed as needed).
 ******************************************************************/


# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <sys/sem.h>
# include <sys/time.h>
# include <math.h>
# include <errno.h>
# include <string.h>
# include <pthread.h>
# include <ctype.h>
# include <iostream>
using namespace std;
 // - Create number of sempahores required in a semaphore array
 // * sem_init - Initialise particular semaphore in semaphore array
 // * sem_wait - Waits on a semaphore (akin to down ()) in the semaphore array
 // * sem_signal - Signals a semaphore (akin to up ()) in the semaphore array
# define SEM_KEY 0x129575 // Change this number as needed

/******************************************************************
Circular Queue
******************************************************************/

struct Circular_queue{

  // Data Members
  int size_q;
  int front;
  int rear;
  int** q_components;

  // Member Functions
  bool is_full();
  void add (int job);

  bool is_empty();
  int remove();

  // Constructor
  Circular_queue(int size_q)
    : front(-1),
        rear(-1),
          size_q(size_q){
      q_components = new int*[size_q];
      for (int i = 0; i < size_q; i++){
        q_components[i] = new int;
      }
    };

  // Destructor
  ~Circular_queue()
  {
    for (int i = 0; i < size_q; i++){
      delete q_components[i];
      // delete[] q_components;
    }
  };

};

/******************************************************************
Producer_parameters
******************************************************************/

struct Producer_parameters{

  // Data members
  int producer_jobs;
  int sem_id;
  Circular_queue* circular_q_Pptr;

  // Constructor
  Producer_parameters(int producer_jobs, int sem_id)
    : producer_jobs(producer_jobs),
        sem_id(sem_id){};

  // Destructor
  ~Producer_parameters()
  {
    delete circular_q_Pptr;
  };
};

/******************************************************************
Consumer_parameters
******************************************************************/

struct Consumer_parameters{

  // Data members
  int sem_id;
  Circular_queue* circular_q_Cptr;

  // Constructor
  Consumer_parameters(int sem_id)
    : sem_id(sem_id){};

  // Destructor
  ~Consumer_parameters()
  {
    delete circular_q_Cptr;
  };

  };

// #if (defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)) \
//     || defined(__FreeBSD__) || ...
// /* union semun is defined by including <sys/sem.h> */
// #else

// union semun {
//     int val;               /* used for SETVAL only */
//     struct semid_ds *buf;  /* used for IPC_STAT and IPC_SET */
//     ushort *array;         /* used for GETALL and SETALL */
// };

// #endif

int check_arg (char *);
int sem_create (key_t, int);
int sem_init (int, int, int);
void sem_wait (int, short unsigned int);
void sem_signal (int, short unsigned int);
int sem_close (int);
