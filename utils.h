#ifndef UTILS_H
#define UTILS_H

void printArr(int* arr, int size, char* header) {
    printf("%s: ", header);
    for (int i = 0; i < size; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// void insertEnd(int* arr, int arrSize, int value) {
//     for (int i = 0; i < arrSize; ++i) {
//         if (arr[i] == -1) arr[i] = value;
//     }
// }

int insertIfNull(struct student_info** array, int arrSize, struct student_info* student) {
    int success = 0;
    for (int i = 0; i < arrSize; ++i) {
        if (array[i] == NULL) {
            // printf("Insert success \n");
            array[i] = student;
            success = 1;
            break;
        }
    }
    return success;
}

// int removeFirst(int* arr, int arrSize, int value) {
//     int ret = arr[0];
//     for (int i = 1; i < arrSize - 1; ++i) {
//         arr[i-1] = arr[i];
//     }
//     arr[arrSize - 1] = -1;
//     return ret;
// }

int searchSeatArr(struct student_info** arr, int arrSize, int studentID) {
    // printf("searchSeatArr args %d %d \n", arrSize, studentID);
    int ret = -1;
    for (int i = 0; i < arrSize; ++i) {
        if (arr[i] != NULL) {
            if (arr[i]->ID == studentID) {
                // printf("success arr[i] %p %d\n", arr[i], arr[i]->ID);
                ret = i;
            }
        } else {
            // printf("failed arr[i] %p\n", arr[i]);
        }
    }
    // printf("Search arr result: %d \n", ret);
    return ret;
}

#endif // UTILS_H