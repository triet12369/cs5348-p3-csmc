#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "globals.h"
#include "t_coordinator.h"



void action_programming(struct student_info* student) {
    float sleepTime = (float)rand()/(float)(RAND_MAX) * T_SLEEP_PROGRAMMING;
    usleep(sleepTime);
    
}

int seekHelp(struct student_info* student) {

    pthread_mutex_lock(&seat_lock);
    if (!insertIfNull(chairs_info.seatingList, NUM_CHAIR, student)) {
        pthread_mutex_unlock(&seat_lock);
        return 0;
    }
    chairs_info.emptyChairs--;

    printf("S: Student %d takes a seat. Empty chairs = %d. \n", student->ID, chairs_info.emptyChairs);

    pthread_mutex_unlock(&seat_lock);


    totalRequest++;
    sem_post(&student_arrived);

    while (1) {
        pthread_mutex_lock(&seat_lock);
        if (searchSeatArr(chairs_info.seatingList, NUM_CHAIR, student->ID) == -1) {
            pthread_mutex_unlock(&seat_lock);
            break;
        }
        pthread_mutex_unlock(&seat_lock);
    
    };

    printf("S: Student %d received help from Tutor %d. \n", student->ID, student->tutorID);
   

    return 1;
}

void* t_student(void* stdNumber) {
    int STUDENT_ID = *(int*) stdNumber;
    struct student_info* student = malloc(sizeof(struct student_info));
    student->ID = STUDENT_ID;
    student->num_help = NUM_HELP;


    while (student->num_help > 0) {
  
        action_programming(student);

        if (seekHelp(student)) student->num_help--;
        else {
            printf("S: Student %d found no empty chair. Will try again later.\n", STUDENT_ID);
        }

    }
    sem_post(&student_arrived); 
    pthread_exit(NULL);
    return NULL;
}
