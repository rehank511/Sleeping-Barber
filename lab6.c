#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>

void *barber(void *param);
void *client(void *param);
const int HAIRCUT_TIME=4;

int numc,maxc,customer;

pthread_mutex_t cmutex;
pthread_cond_t ccond;

void *barber(void *param)
{
  while(1)
  {
    pthread_mutex_lock(&cmutex);

    if(numc == maxc)
    {
      printf("No Customers waiting. Barber taking nap.\n\n");
      pthread_cond_wait(&ccond,&cmutex);
      pthread_mutex_unlock(&cmutex);
    }

    else
    {
      printf("Barber attended a customer\n");

      numc+=1;
      printf("Available Chairs:%i\n", numc);
      pthread_mutex_unlock(&cmutex);

    printf("Barber is cutting hair. Time for haircut is %i seconds\n\n",HAIRCUT_TIME);
      sleep(HAIRCUT_TIME);
    }
  }
}

void *client(void *param)
{
  sleep((rand()%20)+5);
  pthread_mutex_lock(&cmutex);

  if(numc == maxc)
  {
    pthread_cond_signal(&ccond);
  }

  printf("Customer arrived!!\n");

  if (numc <= 0)
  {
    printf("No seat Available. Customer Left :(\n\n");
    pthread_mutex_unlock(&cmutex);
    pthread_exit(NULL);
  }

  numc-=1;
  printf("Available Chairs:%i\n\n", numc);

  pthread_mutex_unlock(&cmutex);
  pthread_exit(0);
}

int main()
{
  pthread_t barberid;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  int runtime;
  int clients,i;

  pthread_mutex_init(&cmutex,NULL);       //Initialize mutex and cond variables.
  pthread_cond_init(&ccond,NULL);

  maxc = 6;    // Number of chair in the shop. Value can be changed for different test cases.
  customer = 8;    // Number of customer arriving. Value can be changed for different test cases.
  runtime = 30;    // Rum time of the shop. Value can be changed for different test cases.
  numc = maxc;
  printf("\nBarber shop is open for %i seconds.\n\n",runtime);

  pthread_t clientids[customer];     //Initialize client threads.

  pthread_create(&barberid, &attr, barber, NULL);    // Create barber thread.

  for (i = 0; i < customer; )   //Create client threads.
  {
    pthread_create(&clientids[i], &attr, client, NULL);
    i++;
  }
  sleep(runtime);   //Sleep.

  printf("Shop Closed!!! \n");   //Exit.
  exit(0);
}
