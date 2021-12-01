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

  int size_q = *argv[1] - '0';
  int producer_jobs = *argv[2] - '0';
  int number_producers = *argv[3] - '0';
  int number_consumers = *argv[4] - '0';

  cout << size_q;
  cout << producer_jobs;
  cout << number_producers;
  cout << number_consumers;

  // TODO

  pthread_t producerid;
  int parameter = 5;

  pthread_create (&producerid, NULL, producer, (void *) &parameter);

  pthread_join (producerid, NULL);

  cout << "Doing some work after the join" << endl;

  return 0;
}

void *producer (void *parameter)
{

  // TODO

  int *param = (int *) parameter;

  cout << "Parameter = " << *param << endl;

  sleep (5);

  cout << "\nThat was a good sleep - thank you \n" << endl;

  pthread_exit(0);
}

void *consumer (void *id)
{
    // TODO

  pthread_exit (0);

}
