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
    for (int studentID = 0; studentID < NUM_STUDENT; ++studentID) {
        for (int priority = 0; priority < NUM_HELP; ++priority) { // 0: highest, NUM_HELP - 1: lowest
            if (info.studentPriorityQueue[priority][studentID]) {
                // printf("Picked student %d to tutor. \n", studentID);
                int seatIndex = searchSeatArr(chairs_info.seatingList, NUM_CHAIR, studentID);
                struct student_info* student = chairs_info.seatingList[seatIndex];
                info.studentPriorityQueue[priority][studentID] = 0;
                waitingNum--;
                student->tutorID = tutorID;
                chairs_info.seatingList[seatIndex] = NULL;
                chairs_info.emptyChairs++;
                // pthread_mutex_lock(&seat_lock);
                // pthread_mutex_unlock(&seat_lock);
                // pthread_mutex_unlock(&seat_lock);
                return studentID;
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
        if (finished) {
            // printf("T Exitting. \n");
            pthread_exit(NULL);
        }
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
        tutoringNum++;
        // pthread_mutex_unlock(&seat_lock);
        // pthread_mutex_lock(&queue_lock);
        usleep(TUTOR_SLEEP_TIME);
        // usleep(1000000);
        // pthread_mutex_lock(&seat_lock);
        helpServed++;
        printf("T: Student %d tutored by Tutor %d. \
        Students tutored now = %d. \
        Total sessions tutored = %d.\n", STUDENT_ID, TUTOR_ID, tutoringNum, helpServed);
        tutoringNum--;
        if (helpServed >= NUM_STUDENT * NUM_HELP) finished = 1;
        // pthread_mutex_lock(&seat_lock);
        // int seatIndex = searchSeatArr(chairs_info.seatingList, NUM_CHAIR, STUDENT_ID);

        pthread_mutex_unlock(&seat_lock);
        // pthread_mutex_unlock(&queue_lock);
    }
    // printf("Tutor %d exitting. \n", TUTOR_ID);
    // pthread_exit(NULL);
    return NULL;
}