// prog32.c by John DeRosa
// this program is code written by William "Amos" Confer which 
// has been edited by John DeRosa
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>

#define CNT 5

unsigned long eat_cnt[CNT] = {0};
sem_t chopsticks[CNT];

void *roger(void *arg);
void *present(void *arg);

int main(int argc, char *argv[]) {
     int i;
     pthread_t rthread[CNT];
     pthread_t view_thread;

     for(i = 0; i < CNT; i++){
          sem_init(&(chopsticks[i]), 0, 1);
     }

     for(i = 0; i < CNT; i++) {
          pthread_create(&(rthread[i]), NULL, roger, (void*)(intptr_t)i);
     }

     pthread_create(&view_thread, NULL, present, NULL);

     for(;;);

     for(i = 0; i < CNT; i++){
          sem_destroy(&(chopsticks[i]));
     }

     return EXIT_SUCCESS;
}



void *roger(void *arg) {
     int r = (int)(intptr_t)arg;
     int left = r;
     int right = (left + 1) % CNT;

     for(;;) {
          //try to get a left chopstick if not restart the loop
          if(sem_trywait(&(chopsticks[left])) == 0)
          {
            // try to get the right chopstick  
            if(sem_trywait(&(chopsticks[right]))==0)
            {   // if the right is aquired eat and put both sticks back
                eat_cnt[r]++;
                sem_post(&(chopsticks[left]));
                sem_post(&(chopsticks[right]));
            }
            //if the right chopstick cant be aquired put the left back
            else
            {
                sem_post(&(chopsticks[left]));
            }
            
          }
     }

     return NULL;
}



void *present(void *arg) {

     int i;

     for(;;) {
          for(i = 0; i < CNT; i++) {
               printf("%6lx ", eat_cnt[i]);
          }
          printf("\n");
          usleep(10000);
     }

     return NULL;
}