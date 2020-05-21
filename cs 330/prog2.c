//CS330 Prog 2
//Written by John DeRosa
//language = C
//Written 10/19/2018
//Worked with James Johnson
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/random.h>
#include <stdbool.h>

// array with the value 35 for each letter of the alphabet
int letterCount[26]={35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35};

// array to store if a letter is done changing its value
bool done[26]={false,false,false,false,false,false,false,false,false,false,false,false,false,
false,false,false,false,false,false,false,false,false,false,false,false,false,};

//prototype for the letter toggle function
void * letterToggle(void*arg);

//prototype for the presenter function
void * PRESENTER(void*arg);

//global variable to check how many threads are completed
int doneCounter=0;
//the main function
int main (void)
{
pthread_t letterHandler[26];
pthread_t presenterThread;
    char * i;
    //creating 26 threads for each letter of the alphabet
    for (char i='A';i<='Z';i++)
    {
        pthread_create(&(letterHandler[(int)i]),NULL, letterToggle, (void*)(intptr_t)i );
    }
//creating the presenter thread which prints the values
pthread_create(&presenterThread,NULL,PRESENTER,(void*)(intptr_t)letterCount);
// looping endlessly
for (;;)
{    //joining each letters thread
    for (char i='A';i<='Z';i++)
    {
        pthread_join(letterHandler[i],NULL);
    } 
    //checking to see if each thread has ended
    for(char i='A';i<='Z';i++)
    {
        //if the thread has ened
        if(done[(int)(i-'A')]!=false)
        {   //count that it has eneded
            doneCounter++; 
        }
        //if 26 threads have ended 
        if (doneCounter==26)
        {   // wait then stop presentingeach letter 
            usleep(1000000);
            pthread_cancel(presenterThread);
            break;
        } 
    }
    break;
}
//join the presenter thread again to prevent a zombie thread
pthread_join(presenterThread,NULL);
return EXIT_SUCCESS;
}
//the letter toggle function which randomly add and subrtacts to the threads counter 
//each letter has their own counter
 void * letterToggle (void * arg)
 {  int ltr = (int)(intptr_t)arg-'A';
     //looping endlessly
     for(;;)
     {      //generating a random number between 1 and 500
            int randomNum = rand() % 500;
            switch(randomNum%2)
                {
                //if the number is odd and less than 70 add 1
                case 1: 
                    if(letterCount[ltr]<70)
                    letterCount[ltr]++;
                    break;
                //if the number is even and greater than 0 subract 1
                case 0:
                    if (letterCount[ltr]>0)
                    letterCount[ltr]--;
                    break;
                default:
                    break;
                }
                //if the random number is 15 exit this thread and log that it was exited
                if (randomNum == 15)
                {
                    
                    pthread_exit((void*)(done[ltr]=true));
                }
                //stagger the output
                usleep(10000);
     }
 }
 //the presenter function
 // it prints each letter x amount of times where x= the letters value in letterCounter
void * PRESENTER (void* arg)
{   //looping endlessly
    for(;;)
    {   //spacing out the runs to create an animation effect
        printf("\n\n\n\n\n\n\n\n\n\n");
        //incrementing through letter count
        for(char letter='A';letter<='Z';letter++)
        {   // printing the specific letter x amounts of time
            for(int j = 0 ; j<=letterCount[(int)(letter-'A')] ; j++)
            {   // if the letters thread isnt done    
                if(done[(int)letter-'A']!=true)
                {   //print that letter the designated number of times
                    printf("%c", letter);
                }
                //if it is done
                else
                {   //print dots in place of the letter
                    printf(".");
                }
            }
            printf("\n");
        }
    usleep(100000);
    }
return NULL;
}