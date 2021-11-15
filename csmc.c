#define _BSD_SOURCE
#include "stdio.h"
#include "stdlib.h"
#include "globals.h"
#include "pthread.h"
#include "t_student.h"
#include "t_tutor.h"
#include "t_coordinator.h"

int NUM_STUDENT, NUM_TUTOR, NUM_CHAIR, NUM_HELP;

void infoPrint() {
    printf("Number of students: %d \n", NUM_STUDENT);
    printf("Number of tutors: %d \n", NUM_TUTOR);
    printf("Number of chairs: %d \n", NUM_CHAIR);
    printf("Number of help: %d \n", NUM_HELP);
}

void students_tutors_init(pthread_t* student_ptid, pthread_t* tutor_ptid) {
    int studentIDs[NUM_STUDENT];
    int tutorIDs[NUM_TUTOR];

    for (int i = 0; i < NUM_TUTOR; ++i) {
        tutorIDs[i] = i;
        pthread_create(&tutor_ptid[i], NULL, &t_tutor, (void*) &tutorIDs[i]);
    }

    for (int i = 0; i < NUM_STUDENT; ++i) {
        studentIDs[i] = i;
        pthread_create(&student_ptid[i], NULL, &t_student, (void*) &studentIDs[i]);
    }

    for (int i = 0; i < NUM_STUDENT; ++i) {
        pthread_join(student_ptid[i], NULL);
    }

    for (int i = 0; i < NUM_TUTOR; ++i) {
        pthread_join(tutor_ptid[i], NULL);
    }
  
}

void tutors_init(pthread_t* tutor_ptid) {
    int tutorIDs[NUM_TUTOR];
    for (int i = 0; i < NUM_TUTOR; ++i) {
        tutorIDs[i] = i;
        pthread_create(&tutor_ptid[i], NULL, &t_tutor, (void*) &tutorIDs[i]);
    }

    for (int i = 0; i < NUM_TUTOR; ++i) {
        pthread_join(tutor_ptid[i], NULL);
    }

}

int main(int argc, const char* argv[]) {

    if (argc != 5) {
        dup2(STDERR_FILENO, STDOUT_FILENO);
        printf("Invalid arguments. Use csmc #students #tutors #chairs #help \n");
        return -1;
    }

    NUM_STUDENT = atoi(argv[1]);
    NUM_TUTOR = atoi(argv[2]);
    NUM_CHAIR = atoi(argv[3]);
    NUM_HELP = atoi(argv[4]);
 
    pthread_t student_ptid[NUM_STUDENT];
    pthread_t coordinator;
    pthread_t tutor_ptid[NUM_TUTOR];


    pthread_create(&coordinator, NULL, &t_coordinator, NULL);


    students_tutors_init(student_ptid, tutor_ptid);
    pthread_join(coordinator, NULL);



    return 0;
}
