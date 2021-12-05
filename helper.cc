/******************************************************************
 * The helper file that contains the following helper functions:
 * check_arg - Checks if command line input is a number and returns it
 * sem_create - Create number of sempahores required in a semaphore array
 * sem_init - Initialise particular semaphore in semaphore array
 * sem_wait - Waits on a semaphore (akin to down ()) in the semaphore array
 * sem_signal - Signals a semaphore (akin to up ()) in the semaphore array
 * sem_close - Destroy the semaphore array
 ******************************************************************/

# include "helper.h"

/******************************************************************

Gus' functions

******************************************************************/


bool Circular_queue::is_full(){
  if (front == rear +1){
    return true;
  }
  if (rear == size_q-1 && front == 0){
      return true;
    }
  return false;
}

void Circular_queue::add (int job){
// input into next space in queue

  if (is_full()){
    cout << endl << "Cannot add, the queue is full!" << endl;
    return;
  }

  // adding first element
  else if ( front == -1 && rear == -1){
    rear++;
    front++;
    *q_components[rear] = job;
    cout << endl << "Added " << job <<  " to component 0. " << endl;
  }

  else{

    if (rear == size_q -1 ){
      rear = 0;
      //cout << endl << " adfas" << endl;
    }

    else {
      rear ++;
    }

    *q_components[rear] = job;
    cout << endl << "Added " << job <<  " to component: " << rear << endl;
  }

}

bool Circular_queue::is_empty(){
  if (front == -1 && rear == -1){
    return true;
  }
  return false;
}

int Circular_queue::remove(){

  if (is_empty()){
    cout << endl << "Cannot remove, the queue is empty!" << endl;
    return -1;
  }

  // take the front pointers job
  int job = *q_components[front];

  cout << endl << "removed " << job <<  " from component: " << front << endl;

  // update fronts position

  // if only 1 job left
  if (front == rear){
    front = -1;
    rear = -1;
    //cout << endl << "Updated front: " << front << " updated rear: " << rear << endl;
    return job;
  }

  // increment front by 1
  else {

   if (front == size_q - 1){
     front = 0;
     //cout << endl << "Updated front: " << front << " updated rear: " << rear << endl;
   }

   else{
     front++;
     //cout << endl << "Updated front: " << front << " updated rear: " << rear << endl;
   }
 }
}


/******************************************************************

Given Functions

******************************************************************/

int check_arg (char *buffer)
{
 int i, num = 0, temp = 0;
 if (strlen (buffer) == 0)
   return -1;
 for (i=0; i < (int) strlen (buffer); i++)
 {
   temp = 0 + buffer[i];
   if (temp > 57 || temp < 48)
     return -1;
     num += pow (10, strlen (buffer)-i-1) * (buffer[i] - 48);
   }
   return num;
}

 int sem_create (key_t key, int num)
 {
   int id;
   if ((id = semget (key, num,  0666 | IPC_CREAT | IPC_EXCL)) < 0)
     return -1;
   return id;
 }

 int sem_init (int id, int num, int value)
 {
   union semun semctl_arg;
   semctl_arg.val = value;
   if (semctl (id, num, SETVAL, semctl_arg) < 0)
     return -1;
   return 0;
 }

 void sem_wait (int id, short unsigned int num)
 {
   struct sembuf op[] = {
     {num, -1, SEM_UNDO}
   };
   semop (id, op, 1);
 }

 int sem_wait_timed (int id, short unsigned int num, timespec* ts)
 {
   struct sembuf op[] = {
     {num, -1, SEM_UNDO}
   };
  return semtimedop (id, op, 1, ts); // returns an integer,
 }

 void sem_signal (int id, short unsigned int num)
 {
   struct sembuf op[] = {
     {num, 1, SEM_UNDO}
   };
   semop (id, op, 1);
 }

 int sem_close (int id)
 {
   if (semctl (id, 0, IPC_RMID, 0) < 0)
     return -1;
     return 0;
   }
