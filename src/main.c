/*
* File: main.c
* Author: UOC & cterrasid
* Course: 20241
* Description: PR4
*/

/* System header files */
#include "grades.h"

/*Constant for file reading */
#define MAX_FILE_NAME 100+1


/* main */
int main(int argc, char **argv)
{
	tStudentsTable studentsTable;
	int i, studentId;
	
	char filename[MAX_FILE_NAME];
	float approvedPercent = 0.0;
	
	bool isRead = false;
    bool found = false;
	
	/* load data from file*/ 
    printf("LOAD DATA FROM FILE. ENTER FILE NAME >>\n");
    scanf("%s", filename);

    /* Exercise 1 */	
    studentsLoadDataFromFile(filename, &studentsTable, &isRead);
	
	if (isRead) { /* Data successfully loaded */

           printf ("\nRESULTS:\n");
		   
           for(i = 0; i < studentsTable.nStudents; i++) {
               /* Exercise 2 */          
               calculateFinalMarkByActivityType(&studentsTable.students[i]);
               /* Exercise 3 */
               calculateFinalMark(&studentsTable.students[i]);
           }

           /* Exercise 4 */
           /* ... */
           printf("\nSTUDENTS MARKS\n");
           printf("==============\n");
           saveAndDisplayStudentsData(studentsTable);
           
           /* Exercise 5 */
           /* ... */
           printf("\nSTUDENTS SORTED BY FINAL MARK\n");
           printf("=============================\n");
           sortDescendingByFinalMark(&studentsTable);

           /* Exercise 6 */
           /* ... */
           printf("\nPercent students approved: %.2f\n", approvedPercent);
        
           /* Exercise 7 */
           /* ... */
           printf("\nSTUDENTS IN HONOR LIST\n");
           printf("======================\n");
           /* ... */

           /* Exercise 8 */
           printf("\nSELECT STUDENT BY ID >> \n");
           scanf("%d", &studentId);
           /* ... */
           if (found) {
               printf("STUDENT IN HONOR LIST\n");
           } else {
               printf("STUDENT NOT IN HONOR LIST\n");
           }
		
	}
	else {
		/* No data recovered */
		printf("NO STUDENTS RECOVERED\n");	
	}
		
	return 0;
}
