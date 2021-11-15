#include "stdio.h"
#include "globals.h"
#include "utils.h"


int popStudentToTutor(int tutorID) {
   
    for (int studentID = 0; studentID < NUM_STUDENT; ++studentID) {
        for (int priority = 0; priority < NUM_HELP; ++priority) { // 0: highest, NUM_HELP - 1: lowest
            if (info.studentPriorityQueue[priority][studentID]) {
                
                int seatIndex = searchSeatArr(chairs_info.seatingList, NUM_CHAIR, studentID);
                struct student_info* student = chairs_info.seatingList[seatIndex];
                info.studentPriorityQueue[priority][studentID] = 0;
                waitingNum--;
                student->tutorID = tutorID;
                chairs_info.seatingList[seatIndex] = NULL;
                chairs_info.emptyChairs++;
               
                return studentID;
            }
        }
     
    }
    return -1;
}

void* t_tutor(void* tutorID) {
    int TUTOR_ID = *(int*)tutorID;

    while (1) {
     
        sem_wait(&student_ready);
        if (finished) {
           
            pthread_exit(NULL);
        }
     
        pthread_mutex_lock(&seat_lock);
      
        int STUDENT_ID = popStudentToTutor(TUTOR_ID);
     
        if (STUDENT_ID == -1) {
            pthread_mutex_unlock(&seat_lock);
            continue;
        }
        tutoringNum++;
        
        usleep(TUTOR_SLEEP_TIME);
     
        helpServed++;
        printf("T: Student %d tutored by Tutor %d. \
        Students tutored now = %d. \
        Total sessions tutored = %d.\n", STUDENT_ID, TUTOR_ID, tutoringNum, helpServed);
        tutoringNum--;
        if (helpServed >= NUM_STUDENT * NUM_HELP) finished = 1;
    

        pthread_mutex_unlock(&seat_lock);
   
    }

    return NULL;
}
