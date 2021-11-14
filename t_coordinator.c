#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "pthread.h"
#include "semaphore.h"
#include "globals.h"
#include "utils.h"


#define TUTOR_TIME 0.2

// int availableChairs;
// int availableTutors;
int finished = 0;
int helpServed = 0;
int tutoringNum = 0;

struct shared_info info;
struct chairs_info chairs_info;

int** initializeStudentPriorityQueue() {
    int** queue = malloc(sizeof(int*) * NUM_HELP);
    for (int i = 0; i < NUM_HELP; ++i) {
        queue[i] = malloc(sizeof(int*) * NUM_STUDENT);
        for (int j = 0; j < NUM_STUDENT; ++j) {
            queue[i][j] = 0;
        }
    }
    return queue;
}

int* initializeStudentArray(int initialValue) {
    int* list = malloc(sizeof(int) * NUM_STUDENT);
    for (int i = 0; i < NUM_STUDENT; ++i) {
        list[i] = initialValue;
    }
    return list;
}

void* t_coordinator() {
    printf("Hello I am coordinator. \n");

    sem_init(&student_arrived, 0, 0);
    sem_init(&student_ready, 0, 0);

    pthread_mutex_init(&seat_lock, NULL);
    pthread_mutex_init(&queue_lock, NULL);

    info.studentPriorityQueue = initializeStudentPriorityQueue();
    chairs_info.emptyChairs = NUM_CHAIR;
    chairs_info.seatingList = malloc(sizeof(struct student_info*) * NUM_CHAIR);

    while (1) {
        if (finished) {
            for (int i = 0; i < NUM_TUTOR; ++i) {
                sem_post(&student_ready);
            }
            pthread_exit(NULL);
        }
        sem_wait(&student_arrived);
        pthread_mutex_lock(&seat_lock);
        // printArr(info.chairList, NUM_STUDENT, "Student in chairs");
        for (int i = 0; i < NUM_CHAIR; ++i) {
            if (chairs_info.seatingList[i] != NULL) {
                // put student in chair in queue
                struct student_info* student = chairs_info.seatingList[i];
                info.studentPriorityQueue[NUM_HELP - student->num_help][student->ID] = 1; // assign queue priority
                sem_post(&student_ready);
            }
        }
        pthread_mutex_unlock(&seat_lock);
    }
    printf("Exitting. \n");
    pthread_exit(NULL);
    return NULL;
}