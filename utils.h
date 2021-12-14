#ifndef UTILS_H
#define UTILS_H

void printArr(int* arr, int size, char* header) {
    printf("%s: ", header);
    for (int i = 0; i < size; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}



int insertIfNull(struct student_info** array, int arrSize, struct student_info* student) {
    int success = 0;
    for (int i = 0; i < arrSize; ++i) {
        if (array[i] == NULL) {
           
            array[i] = student;
            success = 1;
            break;
        }
    }
    return success;
}



int searchSeatArr(struct student_info** arr, int arrSize, int studentID) {
   
    int ret = -1;
    for (int i = 0; i < arrSize; ++i) {
        if (arr[i] != NULL) {
            if (arr[i]->ID == studentID) {
     
                ret = i;
            }
        } else {
 
        }
    }
  
    return ret;
}

#endif // UTILS_H
