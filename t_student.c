#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "globals.h"
#include "t_coordinator.h"

// struct {
//     int tutorID;
//     int studentID;
// }

void action_programming(struct student_info* student) {
    float sleepTime = (float)rand()/(float)(RAND_MAX) * T_SLEEP_PROGRAMMING;
    usleep(sleepTime);
    // printf("Student %d slept for %f \n", student->ID, sleepTime);
}

int seekHelp(struct student_info* student) {
    // printf("Coordinator received help signal from student %d\n", studentID);
    // printf("Available chair: %d \n", availableChairs);
    pthread_mutex_lock(&seat_lock);
    if (!insertIfNull(chairs_info.seatingList, NUM_CHAIR, student)) {
        pthread_mutex_unlock(&seat_lock);
        return 0;
    }
    chairs_info.emptyChairs--;

    printf("S: Student %d takes a seat. Empty chairs = %d. \n", student->ID, chairs_info.emptyChairs);

    // info.chairList[student->ID] = 1;
    // insertEnd(info.chairList, NUM_STUDENT, studentID);
    pthread_mutex_unlock(&seat_lock);

    // wake up coordinator
    totalRequest++;
    sem_post(&student_arrived);

    while (1) {
        pthread_mutex_lock(&seat_lock);
        if (searchSeatArr(chairs_info.seatingList, NUM_CHAIR, student->ID) == -1) {
            pthread_mutex_unlock(&seat_lock);
            break;
        }
        pthread_mutex_unlock(&seat_lock);
        // sleep(0.5);
    };
    // while(searchArr(info.chairList, NUM_STUDENT, studentID) > -1);
    printf("S: Student %d received help from Tutor %d. \n", student->ID, student->tutorID);
    // info.tutorServeStudentList[student->ID] = -1;
    // sem_wait(&tutor_ready);

    return 1;
}

void* t_student(void* stdNumber) {
    int STUDENT_ID = *(int*) stdNumber;
    struct student_info* student = malloc(sizeof(struct student_info));
    student->ID = STUDENT_ID;
    student->num_help = NUM_HELP;

    // int numHelp = NUM_HELP;
    // printf("Hello i am student %d \n", student->ID);

    while (student->num_help > 0) {
        // Programming state
        action_programming(student);

        if (seekHelp(student)) student->num_help--;
        else {
            printf("S: Student %d found no empty chair. Will try again later.\n", STUDENT_ID);
        }
        // printf("Student %d help remaining: %d\n", STUDENT_ID, numHelp);
        // numHelp--; // assume receive help immediately
    }
    sem_post(&student_arrived); // signal coordinator to stop
    // printf("Student %d exitting. \n", STUDENT_ID);
    // printf("S Exitting. \n");
    pthread_exit(NULL);
    return NULL;
}