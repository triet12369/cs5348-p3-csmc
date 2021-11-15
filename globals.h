#ifndef GLOBALS_H
#define GLOBALS_H
#include "semaphore.h"
#include "pthread.h"

#define TUTOR_SLEEP_TIME 0.2 * 1000 // 0.2 ms
#define T_SLEEP_PROGRAMMING 2 * 1000 // 2ms

struct student_info {
    int ID;
    int num_help;
    int tutorID;
};

struct shared_info {
    int** studentPriorityQueue;
};

extern int NUM_STUDENT;
extern int NUM_TUTOR;
extern int NUM_CHAIR;
extern int NUM_HELP;

extern int helpServed;
extern int tutoringNum;
extern int finished;
extern int totalRequest;
extern int waitingNum;
extern struct shared_info info;


struct chairs_info {
    int emptyChairs;
    struct student_info** seatingList;
};

extern struct chairs_info chairs_info;


sem_t student_arrived;

sem_t student_ready;


pthread_mutex_t seat_lock;
pthread_mutex_t queue_lock;


#endif
