#include "../include/schedule.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// Create a new schedule with 7 days
struct Schedule* CreateSchedule() {
    struct Schedule* schedule = (struct Schedule*) malloc(sizeof(struct Schedule));
    const char* days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    struct Day* prevDay = NULL;
    struct Day* firstDay = NULL;

    for (int i = 0; i < 7; i++) {
        struct Day* currentDay = (struct Day*) malloc(sizeof(struct Day));
        strncpy(currentDay->dayName, days[i], sizeof(currentDay->dayName) - 1);
        currentDay->dayName[sizeof(currentDay->dayName) - 1] = '\0';
        currentDay->examList = NULL;

        if (prevDay) {
            prevDay->nextDay = currentDay;
        } else {
            firstDay = currentDay;
        }
        prevDay = currentDay;
    }

    prevDay->nextDay = firstDay;
    schedule->head = firstDay;
    printf("Schedule creation complete.\n");
    return schedule;
}
// Add an exam to a day in the schedule
int AddExamToSchedule(struct Schedule* schedule, const char* day, int startTime, int endTime, const char* courseCode) {
    if ((endTime - startTime) > 3 || (endTime - startTime) < 1 || startTime < 8 || startTime > 17 || endTime < 9 || endTime > 20) {
        printf("Invalid exam.\n");
        return 3;
    }
    struct Day *tmp = schedule->head;
    while(strcmp(tmp->dayName, day) != 0){
        tmp = tmp->nextDay;
    }
    if(!tmp->examList){
        struct Exam* newexam = CreateExam(startTime, endTime, courseCode);
        tmp->examList = newexam;
        printf("[ %s ] exam added to [ %s ] at time [ %d ] to [ %d ] without conflict.\n", courseCode, tmp->dayName, startTime, endTime);
        return 0;
    }
    int duration = endTime-startTime;
    do{
        struct Exam* previous = NULL;
        struct Exam* current = tmp->examList;
        if(current->startTime >= endTime){
            struct Exam* newexam = CreateExam(startTime, endTime, courseCode);
            newexam->next = current;
            tmp->examList = newexam;
            printf("[ %s ] exam added to [ %s ] at time [ %d ] to [ %d ] without conflict.\n", courseCode, tmp->dayName, startTime, endTime);
            return 0;
        }else if (current->next){
            previous = current;
            current = current->next;
            while(current->next && startTime <= previous->startTime){
                if(previous->endTime <= startTime && endTime <= current->startTime){
                    struct Exam* newexam = CreateExam(startTime, endTime, courseCode);
                    previous->next = newexam;
                    newexam -> next = current;
                    printf("[ %s ] exam added to [ %s ] at time [ %d ] to [ %d ] without conflict.\n", courseCode, tmp->dayName, startTime, endTime);
                    return 0;
                }
                previous = current;
                current = current->next;
            }
            while(current->next){
                if(current->startTime - previous->endTime >= duration){
                    int newStartTime = previous->endTime;
                    struct Exam* newexam = CreateExam(newStartTime, newStartTime + duration, courseCode);
                    previous->next = newexam;
                    newexam->next = current;
                    printf("[ %s ] exam added to [ %s ] at time [ %d ] to [ %d ] due to conflict.\n", courseCode, tmp->dayName, newStartTime, newStartTime+duration);
                    return 1;
                }
                previous = current;
                current = current->next;
            }
            if((current->startTime - previous->endTime) >= duration){
                int newStartTime = previous->endTime;
                struct Exam* newexam = CreateExam(newStartTime, newStartTime + duration, courseCode);
                previous->next = newexam;
                newexam->next = current;
                printf("[ %s ] exam added to [ %s ] at time [ %d ] to [ %d ] due to conflict.\n", courseCode, tmp->dayName, newStartTime, newStartTime+duration);
                return 1;
            }
        }
        if(!current->next){
            if(current->endTime <= startTime){
                struct Exam* newexam = CreateExam(startTime, endTime, courseCode);
                current ->next = newexam;
                printf("[ %s ] exam added to [ %s ] at time [ %d ] to [ %d ] without conflict.\n", courseCode, tmp->dayName, startTime, endTime);
                return 0;
            }
            else if(current->endTime <= 17){
                    int newStartTime = current->endTime;
                    struct Exam* newexam = CreateExam(newStartTime, newStartTime + duration, courseCode);
                    current->next = newexam;
                    printf("[ %s ] exam added to [ %s ] at time [ %d ] to [ %d ] due to conflict.\n", courseCode, tmp->dayName, newStartTime, newStartTime+duration);
                    return 1;
                }
        }
        tmp = tmp->nextDay;
    }while(strcmp(tmp->dayName, day) != 0);

    printf("Schedule full. Exam cannot be added.\n");
    return 2;
}
// Another AddExamToSchedule but that one is to use in ClearDay function. Because if I use the normal one it may relocate the exam in the day that we want to clear.
// Althought in this one I changed parameter of while so it will check until oldDay.
int AddExamToScheduleForClearDayFunction(struct Schedule* schedule, const char* day, const char* oldDay, int startTime, int endTime, const char* courseCode) {
    if ((endTime - startTime) > 3 || (endTime - startTime) < 1 || startTime < 8 || startTime > 17 || endTime < 9 || endTime > 20) {
        printf("Invalid exam.\n");
        return 3;
    }
    struct Day *tmp = schedule->head;
    while(strcmp(tmp->dayName, day) != 0){
        tmp = tmp->nextDay;
    }
    if(!tmp->examList){
        struct Exam* newexam = CreateExam(startTime, endTime, courseCode);
        tmp->examList = newexam;
        printf("[ %s ] exam added to [ %s ] at time [ %d ] to [ %d ] without conflict.\n", courseCode, tmp->dayName, startTime, endTime);
        return 0;
    }
    int duration = endTime-startTime;
    do{
        struct Exam* previous = NULL;
        struct Exam* current = tmp->examList;
        if(current->startTime >= endTime){
            struct Exam* newexam = CreateExam(startTime, endTime, courseCode);
            newexam->next = current;
            tmp->examList = newexam;
            printf("[ %s ] exam added to [ %s ] at time [ %d ] to [ %d ] without conflict.\n", courseCode, tmp->dayName, startTime, endTime);
            return 0;
        }else if (current->next){
            previous = current;
            current = current->next;
            while(current->next && startTime <= previous->startTime){
                if(previous->endTime <= startTime && endTime <= current->startTime){
                    struct Exam* newexam = CreateExam(startTime, endTime, courseCode);
                    previous->next = newexam;
                    newexam -> next = current;
                    printf("[ %s ] exam added to [ %s ] at time [ %d ] to [ %d ] without conflict.\n", courseCode, tmp->dayName, startTime, endTime);
                    return 0;
                }
                previous = current;
                current = current->next;
            }
            while(current->next){
                if(current->startTime - previous->endTime >= duration){
                    int newStartTime = previous->endTime;
                    struct Exam* newexam = CreateExam(newStartTime, newStartTime + duration, courseCode);
                    previous->next = newexam;
                    newexam->next = current;
                    printf("[ %s ] exam added to [ %s ] at time [ %d ] to [ %d ] due to conflict.\n", courseCode, tmp->dayName, newStartTime, newStartTime+duration);
                    return 1;
                }
                previous = current;
                current = current->next;
            }
            if((current->startTime - previous->endTime) >= duration){
                int newStartTime = previous->endTime;
                struct Exam* newexam = CreateExam(newStartTime, newStartTime + duration, courseCode);
                previous->next = newexam;
                newexam->next = current;
                printf("[ %s ] exam added to [ %s ] at time [ %d ] to [ %d ] due to conflict.\n", courseCode, tmp->dayName, newStartTime, newStartTime+duration);
                return 1;
            }
        }
        if(!current->next){
            if(current->endTime <= startTime){
                struct Exam* newexam = CreateExam(startTime, endTime, courseCode);
                current ->next = newexam;
                printf("[ %s ] exam added to [ %s ] at time [ %d ] to [ %d ] without conflict.\n", courseCode, tmp->dayName, startTime, endTime);
                return 0;
            }
            else if(current->endTime <= 17){
                    int newStartTime = current->endTime;
                    struct Exam* newexam = CreateExam(newStartTime, newStartTime + duration, courseCode);
                    current->next = newexam;
                    printf("[ %s ] exam added to [ %s ] at time [ %d ] to [ %d ] due to conflict.\n", courseCode, tmp->dayName, newStartTime, newStartTime+duration);
                    return 1;
                }
        }
        tmp = tmp->nextDay;
    }while(strcmp(tmp->dayName, oldDay) != 0);

    printf("Schedule full. Exam cannot be added.\n");
    return 2;
}

// Remove an exam from a specific day in the schedule
int RemoveExamFromSchedule(struct Schedule* schedule, const char* day, int startTime) {
    struct Day* tmp = schedule ->head;
    while (strcmp(tmp->dayName, day) != 0){
        tmp = tmp->nextDay;
    }
    struct Exam* tmpExam = (struct Exam*) malloc(sizeof(struct Exam));
    tmpExam ->next = tmp->examList;
    while(tmpExam->next && tmpExam->next->startTime != startTime){
        tmpExam = tmpExam->next;
    }
    if(!tmpExam->next){
        printf("Exam could not be found.\n");
        return 1;
    }
    if(tmpExam->next->next){
        tmpExam->next = tmpExam->next->next;
    }else{
        tmpExam->next = NULL;
    }
    printf("Exam removed successfully.\n");
    return 0;

}

// Update an exam in the schedule
int UpdateExam(struct Schedule* schedule, const char* oldDay, int oldStartTime, const char* newDay, int newStartTime, int newEndTime) {
    if ((newEndTime - newStartTime) > 3 || newStartTime < 8 || newStartTime > 17 || newEndTime < 9 || newEndTime > 20) {
        printf("Invalid exam.\n");
        return 3;
    }

    struct Day* oldDayPtr = schedule->head;
    while (strcmp(oldDayPtr->dayName, oldDay) != 0) {
        oldDayPtr = oldDayPtr->nextDay;
        if (oldDayPtr == schedule->head) {
            printf("Exam could not be found.\n");
            return 2;
        }
    }

    struct Exam* tmpExam = oldDayPtr->examList;
    struct Exam* prevExam = NULL;
    while (tmpExam && tmpExam->startTime != oldStartTime) {
        prevExam = tmpExam;
        tmpExam = tmpExam->next;
    }
    if (!tmpExam) {
        printf("Exam could not be found.\n");
        return 2;
    }

    if (prevExam) {
        prevExam->next = tmpExam->next;
    } else {
        oldDayPtr->examList = tmpExam->next;
    }
    int examDuration = tmpExam->endTime - tmpExam->startTime;
    char courseCode[20];
    strncpy(courseCode, tmpExam->courseCode, sizeof(courseCode) - 1);
    courseCode[sizeof(courseCode) - 1] = '\0';
    free(tmpExam);

    struct Day* newDayPtr = schedule->head;
    while (strcmp(newDayPtr->dayName, newDay) != 0) {
        newDayPtr = newDayPtr->nextDay;
        if (newDayPtr == schedule->head) {
            printf("Schedule full. Exam cannot be added.\n");
            return 2;
        }
    }

    struct Exam* current = newDayPtr->examList;
    struct Exam* previous = NULL;

    while (current) {
        if (newEndTime <= current->startTime) {
            struct Exam* newExam = CreateExam(newStartTime, newEndTime, courseCode);
            newExam->next = current;
            if (previous) {
                previous->next = newExam;
            } else {
                newDayPtr->examList = newExam;
            }
            printf("Update successful.\n");
            return 0;
        } else if (current->endTime <= newStartTime) {
            previous = current;
            current = current->next;
        } else {
            newStartTime = current->endTime;
            newEndTime = newStartTime + examDuration;

            if (newStartTime < 8 || newStartTime > 17 || newEndTime < 9 || newEndTime > 20) {
                printf("Invalid exam.\n");
                return 3;
            }
            
            previous = current;
            current = current->next;
        }
    }

    struct Exam* newExam = CreateExam(newStartTime, newEndTime, courseCode);
    if (previous) {
        previous->next = newExam;
    } else {
        newDayPtr->examList = newExam;
    }
    printf("Update successful.\n");
    return 0;
}

// Clear all exams from a specific day and relocate them to other days
int ClearDay(struct Schedule* schedule, const char* day) {
    struct Day* tmp = schedule ->head;
    while(strcmp(tmp->dayName, day) != 0){
        tmp = tmp->nextDay;
    }
    if(!tmp->examList){
        printf("%s is already clear.\n", day);
        return 1;
    }
    struct Exam* currentExam = tmp->examList;
    struct Day* destinationDay = tmp->nextDay;

    while (currentExam) {
        struct Exam* nextExam = currentExam->next;
        int examDuration = currentExam->endTime - currentExam->startTime;

        int relocated = 0;
        while (!relocated) {
            int startTime = 8;
            int endTime = startTime + examDuration;

            int res = AddExamToScheduleForClearDayFunction(schedule, destinationDay->dayName, day, startTime, endTime, currentExam->courseCode);
            if (res == 0 || res == 1) {
                relocated = 1;
            }

            if (!relocated) {
                destinationDay = destinationDay->nextDay;
                if (destinationDay == tmp) {
                    printf("Schedule full. Exams from %s could not be relocated.\n", day);
                    return 2;
                }
            }
        }
        free(currentExam);
        currentExam = nextExam;
    }

    tmp->examList = NULL;
    printf("%s is cleared, exams relocated.\n", day);
    
    return 0;

}

// Clear all exams and days from the schedule and deallocate memory
void DeleteSchedule(struct Schedule* schedule) {
    if(!schedule || !schedule->head){
        return ;
    }
    struct Day* tmp = schedule ->head;
    struct Day* nexttmp;
    do{
        struct Exam* currentExam = tmp->examList;
        while(currentExam){
            struct Exam* nextExam = currentExam->next;
            free(currentExam);
            currentExam = nextExam;
        }
        nexttmp = tmp->nextDay;
        free(tmp);
        tmp = nexttmp;
    }while(tmp != schedule->head);

    schedule->head = NULL;
    printf("Schedule is cleared.\n");
}

// Read schedule from file
int ReadScheduleFromFile(struct Schedule* schedule, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file.\n");
        return 1;
    }

    struct Day* currentDay = NULL;
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        if (strchr(line, '\n') != NULL) line[strlen(line) - 1] = '\0';

        if (strlen(line) == 0) continue;

        if (isalpha(line[0])) {
            currentDay = schedule->head;
            while (currentDay && strcmp(currentDay->dayName, line) != 0) {
                currentDay = currentDay->nextDay;
                if (currentDay == schedule->head) {
                    break;
                }
            }
        } else {
            int startTime, endTime;
            char courseCode[50];
            sscanf(line, "%d %d %s", &startTime, &endTime, courseCode);
            struct Exam* newExam = CreateExam(startTime, endTime, courseCode);

            if (!currentDay->examList) {
                currentDay->examList = newExam;
            } else {
                struct Exam* tempExam = currentDay->examList;
                while (tempExam->next) {
                    tempExam = tempExam->next;
                }
                tempExam->next = newExam;
            }
        }
    }
    fclose(file);
    printf("Schedule read successfully from file.\n");
    return 0;
}

// Write schedule to file
int WriteScheduleToFile(struct Schedule* schedule, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Failed to open file.\n");
        return 1;
    }

    struct Day* currentDay = schedule->head;
    do {
        fprintf(file, "%s\n", currentDay->dayName);

        struct Exam* currentExam = currentDay->examList;
        if (!currentExam) {
            fprintf(file, "(No exams scheduled)\n");
        } else {
            while (currentExam) {
                fprintf(file, "%d %d %s\n", currentExam->startTime, currentExam->endTime, currentExam->courseCode);
                currentExam = currentExam->next;
            }
        }
        fprintf(file, "\n");

        currentDay = currentDay->nextDay;
    } while (currentDay != schedule->head);

    fclose(file);
    printf("Schedule written successfully to file.\n");
    return 0;
}