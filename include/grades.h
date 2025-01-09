
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/* Uncomment the line below once you've completed the exercise 1*/
#define TYPEDEF_COMPLETED

#define MAX_LINE 514

#define MAX_NAME_LENGTH 20+1  
#define MAX_STUDENTS      20   /* Max. Number of students */
#define NUM_ACTIVITIES     7   /* Max. Number of activities of the subject */
#define NUM_CAA_ACTIVITIES 4   /* Number of CAA activities */
#define NUM_PR_ACTIVITIES  3   /* Number of PR activities */

#define MIN_C_MINUS 3.0f   /* Minimum mark for grade C-  */
#define MIN_C_PLUS  5.0f   /* Minimum mark for grade C+  */
#define MIN_B       7.0f   /* Minimum mark for grade B  */
#define MIN_A       9.0f   /* Minimum mark for grade A  */

#define CAA1_WEIGHT 10 /* Percent weight of CAA1 in AC grade */
#define CAA2_WEIGHT 20 /* Percent weight of CAA2 in AC grade */
#define CAA3_WEIGHT 30 /* Percent weight of CAA3 in AC grade */
#define CAA4_WEIGHT 40 /* Percent weight of CAA4 in AC grade */
#define PR1_WEIGHT  20 /* Percent weight of PR1 in PR grade */
#define PR2_WEIGHT  30 /* Percent weight of PR2 in PR grade */
#define PR3_WEIGHT  50 /* Percent weight of PR3 in PR grade */

/* Constants defined for PR4 */
#define TOTAL_MARKS_WEIGHT 100 /* Total weight for CAA or PR activities group */

/* User defined types */
typedef enum {A = 1, B, C_PLUS, C_MINUS, D} tGrade;
typedef enum {CAA = 1, PR} tActivityType;
typedef enum {CAA1 = 1, CAA2, CAA3, CAA4, PR1, PR2, PR3} tActivityName;
typedef enum {SUBMITTED = 1, NOT_SUBMITTED, EXCLUDED} tActivityState;

typedef struct {
    tActivityName name;     /* Activity Name */
    float mark;             /* Activity Mark */
    tActivityState state;   /* Activity State */
} tActivity;

typedef struct {   
    int studentId;                          /* Student ID */               
    char name[MAX_NAME_LENGTH];             /* Student Name */
    tActivity activities [NUM_ACTIVITIES];  /* Student Activities */
/* Exercise 1 */
	float caaMark;	/* Student final CAA mark */
	float prMark;	/* Student final PR mark */
	int nCaa;	/* Student number of submitted CAA activities */
	int nPr;	/* Student number of submitted PR activities */
	float finalMark;	/* Student final mark */
	bool absent;	/* Student is absent or not */
} tStudent;

typedef struct {
    tStudent students[MAX_STUDENTS];    /* Students info and grades */
    int nStudents;                      /* Number of students */ 
} tStudentsTable;


/* Functions prototypes declaration */
void studentsLoadDataFromFile(const char* filename, tStudentsTable *studentsTable, bool *isRead);
void activityTypeWeight(tActivityName activity, tActivityType *activityType, float *activityWeight);
bool allSubmittedPr(int nSubmittedPr);
void writeStudentsData(tStudentsTable studentsTable);
void calculateFinalMarkByActivityType(tStudent *student);
/* ... */
