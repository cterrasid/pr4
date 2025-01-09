#include "grades.h"

/* Function to load student data from file */
void studentsLoadDataFromFile(const char* filename, tStudentsTable *studentsTable, bool *isRead)
{
	FILE *fin=0;
	char line[MAX_LINE];
	char buffer[MAX_LINE];
	tStudent newStudent;
	int i;
	
	/* Open the input file */
	if ((fin = fopen(filename,"r")) != NULL) {
		/* Read all the students */
		studentsTable->nStudents = 0;
		while (!feof(fin) && studentsTable->nStudents < MAX_STUDENTS) {
			/* Remove any content from the line */
			line[0] = '\0';
			
			/* Read one line (maximum 511 chars) and store it in "line" variable */
			fgets (line,MAX_LINE - 1, fin);
			
			/* Ensure that the string is ended by 0*/
			line[MAX_LINE - 1] = '\0';
			
			if (strlen(line) > 0) {
				/* Obtain the data from file line*/
				sscanf(line, "%d %s %[^\n]s", &newStudent.studentId, newStudent.name, buffer);
	
				/* Read the grades and add them to the table */
				for (i = 0; i < NUM_ACTIVITIES; i++) {
                    sscanf(buffer, "%f %u %[^\n]s", &newStudent.activities[i].mark, &newStudent.activities[i].state, buffer);
                    if (i == 0) {
                        newStudent.activities[i].name = CAA1;
                    } else {
                        if (i == 1) {
                            newStudent.activities[i].name = CAA2;
                        } else {
                            if (i == 2) {
                                newStudent.activities[i].name = CAA3;
                            } else {
                                if (i == 3) {
                                    newStudent.activities[i].name = CAA4;
                                } else {
                                    if (i == 4) {
                                        newStudent.activities[i].name = PR1;
                                    } else {
                                        if (i == 5) {
                                            newStudent.activities[i].name = PR2;
                                        } else {
                                            newStudent.activities[i].name = PR3;                                                  
                                        }                                
                                    }
                                }
                            }
                        }
                    }
				}	
				
				/* Add student to students vector*/
				studentsTable->students[studentsTable->nStudents] = newStudent;
                /* Increment the counter. */
				studentsTable->nStudents++ ;
			}
		}
		/* Close the file */
		fclose(fin);
		*isRead = true;
	}
	else {
		*isRead = false;
	}
}

/* Action that returns the type of activity and its weight */
void activityTypeWeight (tActivityName activity, tActivityType *activityType, float *activityWeight) {
	*activityType = CAA;
    if (activity == CAA1) {
        *activityWeight = (float)CAA1_WEIGHT;
    } else {
        if (activity == CAA2) {
            *activityWeight = (float)CAA2_WEIGHT;
        } else {
            if (activity == CAA3) {
                *activityWeight = (float)CAA3_WEIGHT;
            } else {
                if (activity == CAA4) {
                    *activityWeight = (float)CAA4_WEIGHT;
                } else {
					*activityType = PR;
                    if (activity == PR1) {
                        *activityWeight = (float)PR1_WEIGHT;
                    } else {
                        if (activity == PR2) {
                            *activityWeight = (float)PR2_WEIGHT;
                        } else {
							*activityWeight = (float)PR3_WEIGHT;
                        }
                    }
                }
            }
        }
    }
}

/* Function that test all PR activies are submited */
bool allSubmittedPr(int nSubmittedPr) {
    return nSubmittedPr == NUM_PR_ACTIVITIES;
}

/* Exercise 2 */
/* Action to calculate a student's CAA and PR marks and number of activities */
void calculateFinalMarkByActivityType(tStudent *student) {
	tActivityType activityType;
	float activityWeight;
	float mark;
	int i;
	
	/* Initialize variables */
	student->caaMark = 0.0;
	student->prMark = 0.0;
	student->nCaa = 0;
	student->nPr = 0;
	
	for (i = 0; i < NUM_ACTIVITIES; i++) {
		activityTypeWeight(student->activities[i].name, &activityType, &activityWeight);
		mark = student->activities[i].mark * activityWeight;
		
		if (activityType == CAA) {
			student->caaMark += mark;
			if (student->activities[i].state == SUBMITTED) {
				student->nCaa++;
			}
		} else {
			student->prMark += mark;
			if (student->activities[i].state == SUBMITTED) {
				student->nPr++;
			}
		}
	}

	student->caaMark = student->caaMark / TOTAL_MARKS_WEIGHT;
	student->prMark = student->prMark / TOTAL_MARKS_WEIGHT;
}


/* Exercise 3 */
/* Action that calculates student's final mark and NP registry */
/* ... */


/* Exercise 4 */
/* Action that writes a student's ID, grade and NP in a file */
/* ... */


/* Exercise 5 */ 
/* Action that order a student's table by mark and student id */	
/* ... */


/* Exercise 6 */ 
/* Function that calculates percent of approved students */
/* ... */


/* Exercise 7 */
/* Action that obtains students candidates to honors */
/* ... */


/* Exercise 8 */
/* Function that test if a student is in honors table */
/* ... */


/* Action that writes a student's ID, grade and NP registry */
void writeStudentsData(tStudentsTable studentsTable) {
#ifdef TYPEDEF_COMPLETED
	int i;
    
	/* Data output */
    if (studentsTable.nStudents == 0) {
        printf("STUDENT LIST EMPTY\n");
    } else {
    printf("ID NAME CAA_MARK PR_MARK N_CAA N_PR FINAL_MARK ABSENT?(1=YES)\n");
    printf("-------------------------------------------------------------\n");

        for (i = 0 ; i < studentsTable.nStudents ; i++) {
            printf("%d %s %.2f %.2f %d %d %.2f %d\n",
					studentsTable.students[i].studentId,
					studentsTable.students[i].name, 
                    studentsTable.students[i].caaMark,
					studentsTable.students[i].prMark,
                    studentsTable.students[i].nCaa,
					studentsTable.students[i].nPr,
                    studentsTable.students[i].finalMark,
					studentsTable.students[i].absent);                                       
        }
    }	
#endif
}
