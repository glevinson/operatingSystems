/******************************************************************
 * The Main program with the two functions. A simple
 * example of creating and using a thread is provided.
 ******************************************************************/

#include "helper.h"

void *producer (void *id);
void *consumer (void *id);

int main (int argc, char **argv)
{

  // Gus' Work:

  int size_q = check_arg(argv[1]);
  int producer_jobs = check_arg(argv[2]);
  int number_producers = check_arg(argv[3]);
  int number_consumers = check_arg(argv[4]);

  // creating semaphores
  auto sem_id = sem_create(SEM_KEY, 3);
  // <=> 'item', to ensure queue is not empty
  sem_init(sem_id, 0, 0);
  // <=> mutex, mutual exclusion
  sem_init(sem_id, 1, 1);
  // <=> space, to ensure queue is not full
  sem_init(sem_id, 2, size_q);

  Circular_queue circular_queue = Circular_queue(size_q);

  // need to have as many of these for as is neccessary, with an array of pointers pointing to these structs
  // the need for this is so we have unique names
  // also need this as we need to pass pointers into the create thread functions
  Producer_parameters* producer_parameters[number_producers];
  Consumer_parameters* consumer_parameters[number_consumers];

  for (int n = 0; n < number_producers; n++){
    // The producer parameters are
    producer_parameters[n] = new Producer_parameters(producer_jobs, sem_id);
    producer_parameters[n]->circular_q_Pptr = &circular_queue;
  }

 for (int n = 0; n < number_consumers; n++){
   // The producer parameters are
   consumer_parameters[n] = new Consumer_parameters(sem_id);
   consumer_parameters[n]->circular_q_Cptr = &circular_queue;
 }

 // creating producer & consumer thread IDs
 pthread_t producerid[number_producers];
 pthread_t consumerid[number_consumers];

 // creating producer threads
 for (int n = 0; n < number_producers; n++){
   pthread_create(&producerid[n], NULL, producer, producer_parameters[n]);
 }

 // Creating consumer threads
 for (int n=0; n < number_consumers; n++){
   pthread_create(&consumerid[n], NULL, consumer, consumer_parameters[n]);
 }

 // Wait for all producer threads to be executed
 for (int n = 0; n < number_producers; n++){
   pthread_join(producerid[n], NULL);
 }

 // Wait for all consumer threads to be executed
 for (int n=0; n < number_consumers; n++){
   pthread_join(consumerid[n], NULL);
 }

 // Destroying sempahores
 sem_close(sem_id);

 return 0;
}


  // TESTS:

  // for (int i = 0; i < number_producers; i++){
  //   cout << endl << "pthread_t producerid: " << producerid[i] << endl;
  // }
  //
  // for (int i = 0; i < number_consumers; i++){
  //   cout << endl << "pthread_t consumerid: " << consumerid[i] << endl;
  // }

  // cout << endl << endl << "Producer Parameters: " << consumer_parameters[0]->sem_id << endl;

  // cout << size_q;
  // cout << producer_jobs;
  // cout << number_producers;
  // cout << number_consumers;

  // producer_parameters[1]->circular_q_Pptr->add(1);
  // producer_parameters[2]->circular_q_Pptr->add(2);
  // producer_parameters[3]->circular_q_Pptr->add(3);
  // consumer_parameters[4]->circular_q_Cptr->remove();
  // producer_parameters[4]->circular_q_Pptr->add(4);
  // producer_parameters[5]->circular_q_Pptr->add(5);
  // consumer_parameters[3]->circular_q_Cptr->remove();
  // producer_parameters[4]->circular_q_Pptr->add(6);
  // producer_parameters[2]->circular_q_Pptr->add(7);
  // consumer_parameters[2]->circular_q_Cptr->remove();
  // producer_parameters[1]->circular_q_Pptr->add(8);


  // Naming producer
  //
  // for (int n = 1; n <= number_producers; n++){
  //   pthread_create (n, NULL, producer, / )
  // }
  //
  // Naming Consumer
  //
  // for (int n = 1, n<= number_consumers; n++){
  //   pthread_create (n, NULL, consumer, / )
  // }


  // int parameter = 5;
  //
  // pthread_create (&producerid, NULL, producer, producer_parameters[0]); // why producer not have argument, what final argument
  //
  // pthread_join (producerid, NULL);

  // cout << "Doing some work after the join" << endl;



void *producer (void *parameter) // what is parameter here
{

  Producer_parameters* param = (Producer_parameters *) parameter;

  // struct timespec* time_spec = new time_spec;
  // time_spec-
  int count = 0;

  while (count < param->producer_jobs){

    // Produce itme
    int duration = 1 + rand()%10;
    int add_wait = 1 + rand()%5;

    // cout << endl << "Job iteration: " << count << endl;
    // cout << endl << "Duration is: " << duration << endl;
    // cout << endl << "Add wait is: " << add_wait << endl;

    // Checking for space (equivalent to down(space))
    sem_wait(param->sem_id, 2);

    // Check mutually exclusive access to circular queue (equivalent to down(mutex))
    sem_wait(param->sem_id, 1);

    // Takes 1 - 5 seconds to add
    sleep(add_wait);

    // Add job to circular queue
    param->circular_q_Pptr->add(duration);

    // Release mutual exclusivity
    sem_signal(param->sem_id, 1);

    // Letting consumer know of new item (job)
    sem_signal(param->sem_id, 0);

    count ++;

  }

  pthread_exit(0);
  }


void *consumer (void *id) // why do you need an id, for consumer
{

  Consumer_parameters* param = (Consumer_parameters *) id;

  // Checking for a new item (equivalent to down(item))
  sem_wait(param->sem_id, 0);

  // Check mutually exclusive access to circular queue (equivalent to down(mutex))
  sem_wait(param->sem_id, 1);

  // Remove queue from circular queue
  param->circular_q_Cptr->remove();

  // Release mutual exclusivity
  sem_signal(param->sem_id, 1);

  // Letting producer know of space
  sem_signal(param->sem_id, 2);

  pthread_exit (0);

}
