//cs330 fall 2018
//prog1.c by John DeRosa
//Written with help from Matt Peek
//written in c
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int acb(void)// creating the abc function which will randomly print each letter of the alphabet from 26 child processes
{   int y=0;
    
    for (y=0;y<26;y++){//looping 26 times to generate 26 child processes
        
        if (fork()==0)// actually generating 26 child processes
        {
        printf("%c",(char)65+y);//printing the next letter in the alphabet
        exit(EXIT_SUCCESS);// killing the child process after it prints its letter of the alphabet
        
        }
       
    }
    for(y=0; y<26; y++)
    {
        wait(NULL);// waiting for all 26 child processes to finish
    }

    return(0);
}


int main (void)
{
int x = 0 , y = 0;
for (x=0;x<12;x++)// looping 12 times to print the alphabet 12 times
{ 
acb(); //calling the abc function which forks 26 times printing the entire alphabet in random order
printf("\n"); // printing a new line after the 26 child processes finish    
}

return(0);
}