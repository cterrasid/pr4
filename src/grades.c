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
// ACTUALIZA LA NOTA FINAL EL REGISTRO DE NO PRESENTADO
void calculateFinalMark(tStudent *student) {
	bool isNotApproved;
	
	isNotApproved = false;
	
	// SI LA NOTA FINAL DE CAA ES MENOR A 4
	if (student->caaMark < 4) {
		// LA NOTA FINAL ES LA NOTA DE CAA
		student->finalMark = student->caaMark;
		/*printf("CACA IF CAA < 4: %f\n", student->finalMark);*/
		isNotApproved = true;
	} else {
		// SI LA NOTA FINAL DE PR ES MENOR A 5
		if (student->prMark < 5) {
			// LA NOTA FINAL ES LA NOTA DE PR
			student->finalMark = student->prMark;
			/*printf("CACA IF PR < 5: %f\n", student->finalMark);*/
			isNotApproved = true;
		} else {		
			// SI NO, LA NOTA FINAL ES LA MEDIA PONDERADA ENTRE CAA Y PR
			student->finalMark = (student->caaMark * FINAL_CAA_WEIGHT + student->prMark * FINAL_PR_WEIGHT) / TOTAL_MARKS_WEIGHT;
			// SI LA NOTA FINAL ES OK PERO HAY NO PRESENTADO
			// FALTA CONDICION DE APROBADO
			if (!isNotApproved && !allSubmittedPr(student->nPr)) {
				// LA NOTA FINAL SERA 4
				student->finalMark = 4;
			}
			/*printf("CACA IF OK: %f\n", student->finalMark);*/
		}
	}
	student->absent = student->nCaa < 3 && student->nPr < 2;
}


/* Exercise 4 */
/* Action that writes a student's ID, grade and NP in a file */
void saveAndDisplayStudentsData(tStudentsTable studentsTable) {
	FILE* fileToWrite;
	tStudent student;
	int i;
		
	/* Open the file */
	fileToWrite = fopen("grades.txt","w");
	
	if (fileToWrite != NULL) {
		if (studentsTable.nStudents == 0) {
			printf("STUDENT LIST EMPTY");
		} else {
			for (i = 0; i < studentsTable.nStudents; i++) {
				student = studentsTable.students[i];
				
				/* Save student data to the file */
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
	}
	/* Display student data */
	writeStudentsData(studentsTable);
	
	/* Close the file */
	fclose(fileToWrite);
}


/* Exercise 5 */ 
/* Action that order a student's table by mark and student id */	
void sortDescendingByFinalMark(tStudentsTable *studentsTable) {
	int i, j, posMin;
	tStudent aux;
	
	/* Initialize variables */
	i = 0;
	
	while (i < studentsTable->nStudents) {
		posMin = i;
		j = i + 1;
		while (j <= studentsTable->nStudents) {
			if (studentsTable->students[j].finalMark > studentsTable->students[posMin].finalMark) {
				posMin = j;
			} else {
				if (studentsTable->students[j].finalMark == studentsTable->students[posMin].finalMark) {
					if (studentsTable->students[j].studentId < studentsTable->students[posMin].studentId) {
						posMin = j;
					}
				}
			}
			j++;
		}
		
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
