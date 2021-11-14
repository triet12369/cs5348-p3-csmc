#include "stdio.h"
#include "globals.h"
#include "utils.h"

// void served() {
//     // pthread_mutex_lock(&seat_lock);
//     helpServed++;
//     tutoringNum--;
//     if (helpServed >= NUM_STUDENT * NUM_HELP) finished = 1;
//     // pthread_mutex_unlock(&seat_lock);
// }

int popStudentToTutor(int tutorID) {
    // scan through priority list and pick a student
    for (int i = 0; i < NUM_HELP; ++i) {

        for (int j = 0; j < NUM_STUDENT; ++j) {
            if (info.studentPriorityQueue[i][j]) {
                // printf("Picked student %d to tutor. \n", j);
                int seatIndex = searchSeatArr(chairs_info.seatingList, NUM_CHAIR, j);
                struct student_info* student = chairs_info.seatingList[seatIndex];
                info.studentPriorityQueue[i][j] = 0;
                student->tutorID = tutorID;
                tutoringNum++;
                // pthread_mutex_lock(&seat_lock);
                chairs_info.seatingList[seatIndex] = NULL;
                chairs_info.emptyChairs++;
                // pthread_mutex_unlock(&seat_lock);
                // pthread_mutex_unlock(&seat_lock);
                return j;
            }
        }
        // pthread_mutex_lock(&seat_lock);
        // pthread_mutex_unlock(&seat_lock);
    }
    return -1;
}

void* t_tutor(void* tutorID) {
    int TUTOR_ID = *(int*)tutorID;
    // printf("T: Tutor Id %d. \n", TUTOR_ID);
    while (1) {
        // if (finished) break;
        sem_wait(&student_ready);
        if (finished) pthread_exit(NULL);
        // int sem_tutors_val;
        // sem_getvalue(&tutors, &sem_tutors_val);
        pthread_mutex_lock(&seat_lock);
        // pthread_mutex_lock(&queue_lock);
        int STUDENT_ID = popStudentToTutor(TUTOR_ID);
        // pthread_mutex_unlock(&queue_lock);
        if (STUDENT_ID == -1) {
            pthread_mutex_unlock(&seat_lock);
            continue;
        }
        sleep(TUTOR_SLEEP_TIME);
        helpServed++;
        if (helpServed >= NUM_STUDENT * NUM_HELP) finished = 1;
        printf("T: Student %d tutored by Tutor %d. \
        Students tutored now = %d. \
        Total sessions tutored = %d.\n", STUDENT_ID, TUTOR_ID, tutoringNum, helpServed);
        tutoringNum--;
        pthread_mutex_unlock(&seat_lock);
        // served();
        // printf("T: student queue init: %d \n", info.studentPriorityQueue[1][1]);
        // sem_post(&tutor_ready);
    }
    printf("Tutor %d exitting. \n", TUTOR_ID);
    pthread_exit(NULL);
    return NULL;
}