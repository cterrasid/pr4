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
				
				/* Add student to students vector */
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

/* Function that check all PR activities are submitted */
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
void calculateFinalMark(tStudent *student) {
	bool isNotApproved;
	
	/* Initialize variables */
	isNotApproved = false;
	
	/* If student's CAA mark is less than 4, final mark will be CAA mark and student is not approved */
	if (student->caaMark < 4.0) {
		student->finalMark = student->caaMark;
		isNotApproved = true;
	} else {
		/* If student's PR mark is less than 5, final mark will be PR mark and student is not approved */
		if (student->prMark < 5.0) {
			student->finalMark = student->prMark;
			isNotApproved = true;
		} else {		
			/* In any other case, final mark will be the weighted average between CAA and PR */
			student->finalMark = (student->caaMark * FINAL_CAA_WEIGHT + student->prMark * FINAL_PR_WEIGHT) / TOTAL_MARKS_WEIGHT;
			/* If student is not approved and hasn't submitted all the PR, final mark is 4 */
			if (!isNotApproved && !allSubmittedPr(student->nPr)) {
				student->finalMark = 4.0;
			}
		}
	}
	/* Student is absent when has submitted less than 3 CAA and less than 2 PR */
	student->absent = student->nCaa < 3 && student->nPr < 2;
}


/* Exercise 4 */
/* Action that writes a student's ID, grade and NP in a file */
void saveAndDisplayStudentsData(tStudentsTable studentsTable) {
	FILE* fileToWrite;
	tStudent student;
	int i;
		
	/* Open the file */
	fileToWrite = fopen(GRADES_FILE,"w");
	
	if (studentsTable.nStudents == 0) {
		printf("STUDENT LIST EMPTY");
	} else {
		for (i = 0; i < studentsTable.nStudents; i++) {
			student = studentsTable.students[i];
				
			/* Write student data into the file */
			fprintf(fileToWrite, "%d %s %0.2f %0.2f %d %d %0.2f %d\n",
					student.studentId,
					student.name,
					student.caaMark,
					student.prMark,
					student.nCaa,
					student.nPr,
					student.finalMark,
					student.absent);
		}
	}
	/* Display student data */
	writeStudentsData(studentsTable);
	
	/* Close the file */
	fclose(fileToWrite);
}


/* Exercise 5 */ 
/* Action that order a student's table by mark and student id */	
void sortDescendingByFinalMark(tStudentsTable *studentsTable) {
	int i, j, posMin;	/* Iteration (i), second position (j) and minimum position (posMin) variables */
	tStudent aux;		/* Auxiliary variable to swap students */
	
	/* Initialize variables */
	i = 0;
	
	/* Iterate over the students table */
	while (i < studentsTable->nStudents) {
		posMin = i;		/* posMin is the current position */
		j = i + 1;		/* j is the next position to i */
		
		/* Evaluate j position */
		while (j <= studentsTable->nStudents) {
			/* If the student in j position has a higher mark than the student in the minimum position */
			if (studentsTable->students[j].finalMark > studentsTable->students[posMin].finalMark) {
				/* Update the minimum position */
				posMin = j;
			} else {
				/* If the student in j position has the same mark as the student in the minimum position */
				if (studentsTable->students[j].finalMark == studentsTable->students[posMin].finalMark) {
					/* If the student in j position has a lower student id than the student in the minimum position */
					if (studentsTable->students[j].studentId < studentsTable->students[posMin].studentId) {
						/* Update the minimum position */
						posMin = j;
					}
				}
			}
			/* Increment j position */
			j++;
		}
		
		/* If the minimum position is different from i, swap the students */
		if (posMin != i) {
			aux = studentsTable->students[posMin];
			studentsTable->students[posMin] = studentsTable->students[i];
			studentsTable->students[i] = aux;
		}
		i++;
	}
	
	/* Display sorted students list */
	writeStudentsData(*studentsTable);
}


/* Exercise 6 */ 
/* Function that calculates percent of approved students */
float displayApprovedStats(tStudentsTable studentsTable) {
	int approvedStudentsCount, i;
	bool isApproved;
	tStudent student;
	
	/* Initialize variables */
	approvedStudentsCount = 0;
	
	for (i = 0; i < studentsTable.nStudents; i++) {
		student = studentsTable.students[i];
		isApproved = student.finalMark >= MIN_C_PLUS;
		
		if (isApproved && !student.absent) {
			approvedStudentsCount++;
		}
	}
	
	if (studentsTable.nStudents == 0) {
		return 0.0;
	}
	
	return ((float)approvedStudentsCount * 100.0) / (float)studentsTable.nStudents;
}


/* Exercise 7 */
/* Action that obtains students candidates to honors */
void listStudentsWithHonors(tStudentsTable studentsTable, tStudentsTable *studentsWithHonorsTable) {
	int i, j;
	
	/* Initialize variables */
	i = 0;
	j = 0;
	studentsWithHonorsTable->nStudents = 0;
	
	for (i = 0; i < studentsTable.nStudents; i++) {
		if (studentsTable.students[i].finalMark >= MIN_A) {
			studentsWithHonorsTable->students[j] = studentsTable.students[i];
			j++;
		}
	}
	
	/* Update the number of students in studentsWithHonorsTable */
	studentsWithHonorsTable->nStudents = j;
	
	writeStudentsData(*studentsWithHonorsTable);
}


/* Exercise 8 */
/* Function that test if a student is in honors table */
bool isEligibleForHonors(int studentId, tStudentsTable *studentsWithHonorsTable) {
	int i;
	bool found;
	
	/* Initialize variables */
	i = 0;
	found = false;

	while(i < studentsWithHonorsTable->nStudents && !found) {
		found = studentsWithHonorsTable->students[i].studentId == studentId;
		i++;
	}
	return found;
}


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
