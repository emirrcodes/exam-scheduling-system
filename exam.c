#include "../include/exam.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to create a new exam and link it properly in the day
struct Exam* CreateExam(int startTime, int endTime, const char* courseCode) {
    struct Exam* newexam = (struct Exam*) malloc(sizeof(struct Exam));
    if(newexam == NULL){
        printf("Memory allocation is failed \n");
        return NULL;
    }

    newexam ->startTime = startTime;
    newexam ->endTime = endTime;
    strncpy(newexam->courseCode, courseCode, sizeof(newexam->courseCode) - 1); 
    newexam ->courseCode[sizeof(newexam ->courseCode) -1] = '\0';
    newexam ->next = NULL;

    return newexam;
}