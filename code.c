/* Solution to comp10002 Assignment 2, 2019 semester 2.

   Authorship Declaration:

   (1) I certify that the program contained in this submission is completely
   my own individual work, except where explicitly noted by comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students,
   or by non-students as a result of request, solicitation, or payment,
   may not be submitted for assessment in this subject.  I understand that
   submitting for assessment work developed by or in collaboration with
   other students or non-students constitutes Academic Misconduct, and
   may be penalized by mark deductions, or by other penalties determined
   via the University of Melbourne Academic Honesty Policy, as described
   at https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will
   I do so until after the marks are released. I understand that providing
   my work to other students, regardless of my intention or any undertakings
   made to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring
   service, or drawing the attention of others to such services and code
   that may have been made available via such a service, may be regarded
   as Student General Misconduct (interfering with the teaching activities
   of the University and/or inciting others to commit Academic Misconduct).
   I understand that an allegation of Student General Misconduct may arise
   regardless of whether or not I personally make use of such solutions
   or sought benefit from such actions.

   Signed by: Jiayuan Zhu 1067734
   Dated:  2019.10.20

*/


#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>

#define STARTCONSTANT 3 /* Store three attributes (cell size, initial position 
						and goal position */
#define SIZE 0 /* Store size as first element in size */
#define START 1 /* Store start block as first element in size */
#define GOAL 2 /* Store goal block as second element in size */
#define INITIAL 2 /* Initial size of word array */
#define DOUBLE 2 /* Double the current size when malloc is full */
#define SCAN 2 /* Scan 2 attributes each time (coordinate of row and column) */
#define MAXPERLINE 5 /* At most print five route per line */
#define TEN 10 /* Print the map can only use 0 to 9 (only one digit) */
#define EMPTY -1 /* Give empty places value -1 in two dimention number array */
#define ROUTE -2 /* Give theroute after blocks value -2 
				in two dimention number array */
#define BLOCK -3 /* Give block value -3 in two dimention number array */
#define FAIL 0 /* Represent it reaches status 1 or status 2 or status 3 
				or can't be repaired */
#define CONTINUE 1 /* Represent it reaches status 4 and needs repairation */
#define VALID 2 /* Represent it's valid route (reach status5) */
#define SEPERATE1 "------------------------------------------------\n" 
				/* Small seperate line */
#define SEPERATE2 "================================================\n" 
				/* Large seperate line */
#define UNFOUND "The route cannot be repaired!\n" 
				/* Statement for route can't be repaired */
#define STATUS1 "Initial cell in the route is wrong!\n" 
				/* Statement for status1 error */
#define STATUS2 "Goal cell in the route is wrong!\n" 
				/* Statement for status2 error */
#define STATUS3 "There is an illegal move in this route!\n"
				/* Statement for status3 error */
#define STATUS4 "There is a block on this route!\n" 
				/* Statement for status4 error */
#define STATUS5 "The route is valid!\n" /* Statement for the route is valid */
#define TRUE 1 /* Statement is true */
#define FALSE 0 /* Statement is false */

typedef struct {
	int row;
	int column;
} position_t;

typedef struct {
	int row;
	int column;
	int num;
} addnum_t;

typedef struct route route_t;
struct route{
	position_t position;
	route_t *next;
};

position_t* storeconstantvoid();
position_t* storeblock(int *num_block);
route_t* storeroute(int *num_route);
void printroute(route_t *head, int num_route);
void checkstatus3(route_t *head, int *status3, position_t *size);
void checkstatus4(route_t *head, int *status4, int *row1, int *column1, 
	int num_block, position_t *block);
void checkstatus(route_t *head, position_t *size, position_t *block, 
	int *status1, int *status2, int *status3, int *status4, int *status5, 
	int num_block);
void printstatement(route_t *head, position_t *size, int num_block, 
	int num_route);
void printstatus(route_t *head, position_t *size, position_t *block, 
	int num_block, int *continues);
char** twodiarray(route_t *head, position_t *size, position_t *block, 
	int num_block);
void printarray(char **twoarray, position_t *size);
void printstage(int *stage);
route_t* findblock(position_t *block, route_t *head, int num_block);
int** numdiarray(route_t *onebefore, position_t *size, position_t *block, 
	int num_block);
void morespace(int total, size_t *current_size, addnum_t **stack);
void movedirection(int **numarray, int new_row, int new_column, int *found, 
	position_t *endposition, int num, addnum_t *stack, int *total);
int** editnumarray(route_t *onebefore, position_t *size, position_t *block, 
	int num_block, position_t *endposition, int *found);
int tmproute(int **numarray, position_t *size, position_t endposition, 
	route_t **newhead, route_t **newtail);
route_t *newroute(route_t *head, route_t *newhead, route_t *newtail, 
	int *finalnum_route, int newnum_route, route_t *onebefore);
void notfound(route_t *onebefore, int *finish, char **twoarray, 
	position_t *size);
void end(char **twoarray, position_t *size, route_t *head, int finalnum_route);
void endfail(char **twoarray, position_t *size, int continues, int found);
void stage0ans(position_t **size, position_t **block, int *num_block, 
	route_t **head, int *num_route, int *stage, int *continues, 
	char ***twoarray);
void continueornot(route_t *head, position_t *size, position_t *block, 
	int num_block, int *continues);
void iffound(int **numarray, position_t *size, position_t endposition,
	int *finalnum_route, route_t **head, route_t *onebefore, 
	char ***twoarray, position_t *block, int num_block);
void stage1ans(position_t *block, route_t **head, int num_block, 
	char ***twoarray, position_t *size, int ***numarray, int *continues);
void initial(int *loop, int *stage, int *finish, int *num_block, 
	position_t **block, route_t *head, position_t *size, int *continues, 
	char ***twoarray);
void valid(int finish, int ***numarray, route_t *onebefore, position_t *size, 
	position_t *block, int num_block, int *found, route_t **head, 
	char ***twoarray, int *continues);
void stage2ans(int *loop, int *stage, route_t **head, position_t *size,
	int *continues, char ***twoarray, int ***numarray);
void freetwoarray(position_t *size, char **twoarray);
void freelist(route_t *head);

int
/* Print stage0, stage1 and stage2 */
main(int argc, char *argv[]) {
	position_t *size;
	position_t *block;
	route_t *head;
	char **twoarray;
	int **numarray;
	int continues;
	int num_block = 0, num_route = 0, stage = 0, loop = 0;
	/* Print stage0 answer */
	stage0ans(&size, &block, &num_block, &head, &num_route, &stage, &continues, 
		&twoarray);
	/* Print stage1 answer*/
	if (continues == CONTINUE) {
		stage1ans(block, &head, num_block, &twoarray, size, &numarray, 
			&continues);
	}
	else if (continues == FAIL) {
		printf(SEPERATE2);
		free(size);
		freelist(head);
		return 0;
	}
	/* Print stage2 answer */
	stage2ans(&loop, &stage, &head, size, &continues, &twoarray, &numarray);
	if (loop == FALSE) {
		printf(SEPERATE2);
	}
	/* Free memory space */
	free(size);
	freelist(head);
	return 0;
}

/* Get size and positions of initial and goal cell */
position_t*
storeconstantvoid() {
	position_t *size;
	int i, rrow, ccolumn;
	/* Malloc space */
	size = (position_t*)malloc(STARTCONSTANT * sizeof(position_t));
	assert(size != NULL);
	scanf("%dx%d ", &rrow, &ccolumn);
	/* Store size */
	size[SIZE].row = rrow;
	size[SIZE].column = ccolumn;
	/* Store initial position and goal position */
	for (i = 1; i < STARTCONSTANT; i++) {	
		scanf("[%d,%d]\n", &rrow, &ccolumn);
		size[i].row = rrow;
		size[i].column = ccolumn;
	}
	return size;
}

/* Get position of blocks */
position_t*
storeblock(int *num_block){
	size_t current_size = INITIAL;
	position_t *block;
	int rrow, ccolumn;
	/* Malloc space */
	block = (position_t*)malloc(INITIAL * sizeof(position_t));
	assert(block != NULL);
	while (scanf("[%d,%d] ", &rrow, &ccolumn) == SCAN) {
		/* Realloc space when space is not enough */
		if (*num_block == current_size) {
			current_size *= DOUBLE;
			block = realloc(block, current_size * sizeof(position_t));
			assert(block != NULL);
		}
		/* Store coordinates of blocks */
		block[*num_block].row = rrow;
		block[*num_block].column = ccolumn;
		/* Calculate block number */
		*num_block += 1;
	}
	return block;
}

/* Store original given route */
route_t*
storeroute(int *num_route) {
	route_t *head;
	route_t *p1;
	route_t *p2;
	int rrow, ccolumn;
	head =NULL;
	p2 = NULL; 
	/* Scan coordinates until meets '$' */
	scanf(" $ ");
	while (((scanf("[%d,%d]", &rrow, &ccolumn)) == SCAN)) {
		scanf(" -> ");
		/* Calculate the number of routes */
		*num_route += 1;
		/* Malloc space */
		p1 = (route_t*)malloc(sizeof(route_t));
		assert(p1 != NULL);
		/* Store row and column coordinates */
		(p1 -> position).row = rrow;
		(p1 -> position).column = ccolumn;
		/* Head points to the start when it's the first element*/
		if (*num_route == 1) {
			head = p1;
			p2 = p1;
		}
		/* Link each single attribute */
		else {
			p2 -> next = p1;
		}
		p2 = p1;
	}
	p2 -> next = NULL;
	return head;
}

/* Print the route */
void
printroute(route_t *head, int num_route) {
	route_t *p;
	int perline = 0;
	int i;
	p = head;
	assert(head != NULL);
	for (i = 0; i < num_route; i++) {
		/* At most print five route perline */
		if (perline == MAXPERLINE) {
			printf("\n");
			perline = 0;
		}
		perline++;
		printf("[%d,%d]", (p -> position).row, (p->position).column);
		if (i + 1 < num_route) {
			printf("->");
		}
		/* Read next attribute */
		p = p -> next;
	}
	printf(".\n");
}

/* Check if it belongs to status3 */
void
checkstatus3(route_t *head, int *status3, position_t *size) {
	route_t *p1;
	route_t *p2;
	int row1, row2, column1, column2;
	int alltrue = 0;
	assert(head != NULL);
	p1 = head -> next;
	p2 = head;	
	while ((p1 != NULL) && (*status3 == FALSE)) {
		/* Get coordinates of current cell and next cell */
		row1 = (p1 -> position).row;
		column1 = (p1 -> position).column;
		row2 = (p2 -> position).row;
		column2 = (p2 -> position).column;
		/* If the movement is outside the cell then it belongs to status3 */
		if ((row2 >= size[SIZE].row) || (column2 >= size[SIZE].column)) {
			*status3 = TRUE;
		}
		/* If the movement isn't one step further then it belongs to status3 */
		if ((abs(row1 - row2) == 0 && abs(column1 - column2) == 1) 
			|| (abs(row1 - row2) == 1 && abs(column1 - column2) == 0)) {
			alltrue++;
		}
		else {
			*status3 = TRUE;
		}
		/* Read next attribute */
		p2 = p2 -> next;
		p1 = p1 -> next;
	}
	/* If the last movement is outside the cell then it belongs to status3 */
	if ((row1 >= size[SIZE].row) || (column1 >= size[SIZE].column)) {
		*status3 = TRUE;
	}
}

/* Check if it belongs to status4 */
void
checkstatus4(route_t *head, int *status4, int *row1, int *column1, 
	int num_block, position_t *block) {
	int row2, column2, i;
	route_t *p;
	assert(head != NULL);
	p = head;
	while (p != NULL) {
		*row1 = (p -> position).row;
		*column1 = (p -> position).column;
		/* Run all the blocks */
		for (i = 0; i < num_block; i++) {
			row2 = block[i].row;
			column2 = block[i].column;
			/* Compare current cell and block cells */
			if ((*row1 == row2) && (*column1 == column2)){
				*status4 = TRUE;
			}
		}
		/* Read next attribute */
		p = p -> next;
	}
}

/* Check which status it belongs to */
void
checkstatus(route_t *head, position_t *size, position_t *block, int *status1, 
	int *status2, int *status3, int *status4, int *status5, int num_block) {
	int row1, column1;
	/* If initial cell is different to start cell then it belongs to status1 */
	if ((size[START].row != (head -> position).row) || 
		(size[START].column != (head -> position).column)) {
		*status1 = TRUE;
	}
	checkstatus3(head, status3, size);
	checkstatus4(head, status4, &row1, &column1, num_block, block);
	/* If goal cell is different to final cell then it belongs to status2 */
	if ((size[GOAL].row != row1) || (size[GOAL].column != column1)){
		*status2 = TRUE;
	}
	/* Otherwise, it's valid which is belong to status5 */
	if (*status1 != TRUE && *status2 != TRUE && *status3 != TRUE 
		&& *status4 != TRUE) {
		*status5 = TRUE;
	}
}

/* Print part of stage0 statements */
void
printstatement(route_t *head, position_t *size, int num_block, int num_route) {
	printf("The grid has %d rows and %d columns.\n", size[SIZE].row, 
		size[SIZE].column);
	printf("The grid has %d block(s).\n", num_block);
	printf("The initial cell in the grid is [%d,%d].\n", size[START].row, 
		size[START].column);
	printf("The goal cell in the grid is [%d,%d].\n", size[GOAL].row, 
		size[GOAL].column);
	printf("The proposed route in the grid is:\n");
	printroute(head, num_route);
}

/* Print the status */
void
printstatus(route_t *head, position_t *size, position_t *block, int num_block, 
	int *continues) {
	int status1 = FALSE, status2 = FALSE, status3 = FALSE;
	int status4 = FALSE, status5 = FALSE;
	checkstatus(head, size, block, &status1, &status2, &status3, &status4, 
		&status5, num_block);
	if (status1 == TRUE) {
		printf(STATUS1);
	}
	else if (status2 == TRUE) {
		printf(STATUS2);
	}
	else if (status3 == TRUE) {
		printf(STATUS3);
	}
	else if (status4 == TRUE) {
		printf(STATUS4);
	}
	else {
		status5 = TRUE;
		printf(STATUS5);
	}
	/* Check whether the route can be further repaired */
	continueornot(head, size, block, num_block, continues);
}

/* Create the two dimention route map */
char**
twodiarray(route_t *head, position_t *size, position_t *block, int num_block) {
	route_t *p;
	char **twoarray;
	int i, j;
	assert(head != NULL);
	p = head;
	/* Malloc space for rows */
	twoarray = (char**)malloc(size[SIZE].row * sizeof(*twoarray));
	assert(twoarray != NULL);
	for (i = 0; i < size[SIZE].row; i++) {
		/* Malloc space for columns */
		twoarray[i] = (char*)malloc(size[SIZE].column * sizeof(char));
		/* Consume all are empty places */
		for (j = 0; j < size[SIZE].column; j++) {
			twoarray[i][j] = ' ';
		}
	}
	/* Find spots of routes */
	while (p != NULL) {
		twoarray[(p -> position).row][(p -> position).column] = '*';
		p = p -> next;
	}
	/* Find spots of blocks */
	for (i = 0; i< num_block; i++) {
		twoarray[block[i].row][block[i].column] = '#';
	}
	/* Fins spots of initial cell and goal cell */
	twoarray[size[START].row][size[START].column] = 'I';
	twoarray[size[GOAL].row][size[GOAL].column] = 'G';
	return twoarray;
}

/* Print the two dimention route map */
void
printarray(char **twoarray, position_t *size) {
	int i, j;
	printf(" ");
	/* Print ordinal number for columns (one digit each) */
	for (i = 0; i < size[SIZE].column; i++) {
		printf("%d", i % TEN);
	}
	printf("\n");
	/* Print ordinal number for rows (one digit each) */
	for (i = 0; i < size[SIZE].row; i++) {
		printf("%d", i % TEN);
		/* Print symbol character for each spot */
		for (j = 0; j < size[SIZE].column; j++) {
			printf("%c", twoarray[i][j]);
		}
		printf("\n");
	}
}

/* Print stage seperate line */
void
printstage(int *stage){
	printf("==STAGE %d=======================================\n", *stage);
	*stage += 1;	
}

/* Find the spot one before the block */
route_t*
findblock(position_t *block, route_t *head, int num_block) {
	route_t *p1;
	route_t *p2;
	int i;
	assert(head != NULL);
	p2 = head;
	p1 = head;
	while ((p1 != NULL)) {
		/* Compare each block position to current route */
		/* p2 always points one attribute before p1 */
		for (i = 0; i < num_block; i++) {
			if ((block[i].row == (p1 -> position).row) && 
				(block[i].column == (p1 -> position).column)) {
				return p2;
			}
		}
		/* Read next attribute */ 
		p2 = p1;
		p1 = p1 -> next;
	}
	return p2;
}

/* Initialize two dimention number array */
int**
numdiarray(route_t *onebefore, position_t *size, position_t *block, 
	int num_block){
	int **numarray;
	int i, j;
	route_t *p;
	assert(onebefore -> next != NULL);
	p = onebefore -> next;
	/* Allocate space for rows */
	numarray = (int**)malloc(size[SIZE].row * sizeof(*numarray));
	assert(numarray != NULL); 
	for (i = 0; i < size[SIZE].row; i++) {
		/* Allocate space for columns */
		numarray[i] = (int*)malloc(size[SIZE].column * sizeof(int));
		/* Assume each spot is empty */
		for (j = 0; j < size[SIZE].column; j++) {
			numarray[i][j] = EMPTY;
		}
	}
	/* Initialize current routes after the block */
	while(p !=NULL) {
		numarray[(p -> position).row][(p -> position).column] = ROUTE;
		p = p -> next;
	}
	/* Initialize spots of blocks */
	for (i = 0; i < num_block; i++) {
		numarray[block[i].row][block[i].column] = BLOCK;
	}	
	return numarray;
}

/* Allocate more space */
void
morespace(int total, size_t *current_size, addnum_t **stack) {
	/* When stack is full, allocate more space */
	if (total == *current_size) {
		*current_size *= DOUBLE;
		*stack = realloc(*stack, *current_size * sizeof(addnum_t));
		assert(*stack != NULL);
	}
}

/* Move one cell up or down or left or right */
void
movedirection(int **numarray, int new_row, int new_column, int *found, 
	position_t *endposition, int num, addnum_t *stack, int *total) {
	/* If meets the original route, get the final coordinates 
	and ends searching */
	if (numarray[new_row][new_column] == ROUTE) {
		*found = TRUE;
		endposition -> row = new_row;
		endposition -> column = new_column;
	}
	/* Update the number array and adds this information to stack */
	numarray[new_row][new_column] = num;
	stack[*total].row = new_row;
	stack[*total].column = new_column;
	stack[*total].num = num;
	*total += 1;		
}

/* Update the two dimention number array */
int**
editnumarray(route_t *onebefore, position_t *size, position_t *block, 
	int num_block, position_t *endposition, int *found) {
	addnum_t *stack;
	int **numarray;
	int findelement = 0, num = 0, total = 0;
	int current_row = (onebefore -> position).row;
	int current_column = (onebefore -> position).column;
	int new_row, new_column;
	size_t current_size = INITIAL;
	stack = (addnum_t*)malloc(INITIAL * sizeof(addnum_t));
	assert(stack != NULL);
	/* Initialize the two dimention number array */
	numarray = numdiarray(onebefore, size, block, num_block);
	numarray[current_row][current_column] = num;
	stack[total].row = current_row;
	stack[total].column = current_column;
	stack[total].num = num;
	total++;
	while ((*found == FALSE) && (findelement <= total)){
		/* Get coordinates of the current checking stack */
		current_row = stack[findelement].row;
		current_column = stack[findelement].column;
		num = stack[findelement].num + 1;
		/* Check whether can move one cell up */
		if ((current_row - 1 >= 0) && (*found == FALSE)
			&& ((numarray[current_row - 1][current_column] == EMPTY ) 
				|| (numarray[current_row - 1][current_column] == ROUTE))) {
			morespace(total, &current_size, &stack);
			new_row = current_row - 1;
			movedirection(numarray, new_row, current_column, found, 
				endposition, num, stack, &total);
		}
		/* Check whether can move one cell down */
		if ((current_row + 1 < size[SIZE].row) && (*found == FALSE)
			&& ((numarray[current_row + 1][current_column] == EMPTY ) 
				|| (numarray[current_row + 1][current_column] == ROUTE))) {
			morespace(total, &current_size, &stack);
			new_row = current_row + 1;
			movedirection(numarray, new_row, current_column, found, 
				endposition, num, stack, &total);
		}
		/* Check whether can move one cell left */
		if ((current_column - 1 >= 0) && (*found == FALSE)
			&& ((numarray[current_row][current_column - 1]== EMPTY) 
				|| (numarray[current_row][current_column - 1]== ROUTE))) {
			morespace(total, &current_size, &stack);
			new_column = current_column - 1;
			movedirection(numarray, current_row, new_column, found, 
				endposition, num, stack, &total);
		}
		/* Check whether can move one cell right */
		if ((current_column + 1 < size[SIZE].column) && (*found == FALSE)
			&& ((numarray[current_row][current_column + 1] == EMPTY) 
				|| (numarray[current_row][current_column + 1] == ROUTE))) {
			morespace(total, &current_size, &stack);
			new_column = current_column + 1;
			movedirection(numarray, current_row, new_column, found, 
				endposition, num, stack, &total);
		}
		findelement++;
	}	
	return numarray;
}

/* Find the temporary route which avoids the block */
int
tmproute(int **numarray, position_t *size, position_t endposition, 
	route_t **newhead, route_t **newtail) {
	route_t *p1;
	route_t *p2;
	int i;
	int newnum_route = 0;;
	int maxnum = numarray[endposition.row][endposition.column];
	int currentrow = endposition.row;
	int currentcolumn = endposition.column;
	/* Start from the final spot */
	for (i = maxnum; i >= 0; i--) {
		newnum_route++;
		/* Malloc space */
		p1 = (route_t*)malloc(sizeof(route_t));
		assert(p1 != NULL);
		(p1 -> position).row = currentrow;
		(p1 -> position).column = currentcolumn;
		/* The first attribute is the tail */
		if (i == maxnum) {
			p1 -> next = NULL;
			*newtail = p1;
		}
		else {
			p1 -> next = p2;
		}
		p2 = p1;
		/* Check whether can move one cell up */
		if ((currentrow - 1 >= 0) 
			&& (numarray[currentrow - 1][currentcolumn] == i -1)) {
			currentrow -= 1;
		}
		/* Check whether can move one cell down */
		else if ((currentrow + 1 <size[SIZE].row) 
			&& (numarray[currentrow + 1][currentcolumn] == i - 1)) {
			currentrow += 1;
		}
		/* Check whether can move one cell left */
		else if ((currentcolumn - 1 >= 0) 
			&& (numarray[currentrow][currentcolumn - 1] == i - 1)) {
			currentcolumn -= 1;
		}
		/* Check whether can move one cell right */
		else if ((currentcolumn + 1 < size[SIZE].column) 
			&& (numarray[currentrow][currentcolumn + 1] == i - 1)) {
			currentcolumn += 1;
		}
	}
	*newhead = p2;
	return newnum_route;
}

/* Update to new route */
route_t*
newroute(route_t *head, route_t *newhead, route_t *newtail, 
	int *finalnum_route, int newnum_route, route_t *onebefore) {
	route_t *p1;
	route_t *p2;
	route_t *p3;
	assert(head != NULL);
	p1 = head;
	p2 = head;
	p3 = onebefore -> next;
	/* Remain first part of original route until it meets the block */
	while (((p1 -> position.row == p3 -> position.row) 
		&& (p1 -> position.column == p3 -> position.column)) != TRUE) {
		*finalnum_route += 1;
		p2 = p1;
		p1 = p1 -> next;
	}
	*finalnum_route -= 1;
	p1 = p1 -> next;
	/* Link with the head of temporary route */
	p2 -> next = newhead -> next;
	/* Find the cell which can link after the temporary route's tail */
	while (((p1 -> position.row == newtail -> position.row) 
		&& (p1 -> position.column == newtail -> position.column)) != TRUE) {
		p1 = p1 -> next;
	}
	/* Link with remaining part of original cell */
	newtail -> next = p1 -> next;
	*finalnum_route += newnum_route;
	/* Free useless blocking part */
	p1 -> next = NULL;
	freelist(p3);
	/* Calculate total route number of the updated route */
	while (newtail -> next != NULL) {	
		newtail = newtail -> next;
		*finalnum_route += 1;
	}
	return head;
}

/* Print map route and unfound statement when impossible to repair route */
/* It's one no spot can be found one before the block */
void
notfound(route_t *onebefore, int *finish, char **twoarray, position_t *size) {
	if ((onebefore -> next) == NULL) {
		printarray(twoarray, size);
		printf(SEPERATE1);
		printf(UNFOUND);
		*finish = TRUE;
	}
}

/* Print map route and route when it meets the end */
void
end(char **twoarray, position_t *size, route_t *head, int finalnum_route) {
	printarray(twoarray, size); 
	printf(SEPERATE1);
	printroute(head, finalnum_route);
}

/* Print map route and unfound statement when impossible to repair route */
/* It's when can't find a replacement route */
void
endfail(char **twoarray, position_t *size, int continues, int found) {
	if ((continues == FAIL)|| (found == FALSE)) {		
		printarray(twoarray, size); 
		printf(SEPERATE1);
		printf(UNFOUND);
	}
}

/* Gather all statements for stage0 and part of stage1 */
void
stage0ans(position_t **size, position_t **block, int *num_block, route_t **head, 
	int *num_route, int *stage, int *continues, char ***twoarray) {
	*size = storeconstantvoid();
	*block = storeblock(num_block);
	*head = storeroute(num_route);
	printstage(stage);
	printstatement(*head, *size, *num_block, *num_route);
	printstatus(*head, *size, *block, *num_block, continues);
	*twoarray = twodiarray(*head, *size, *block, *num_block);
	printstage(stage);
	printarray(*twoarray, *size);
}

/* Judge whether further repairation is needed  or not */
void
continueornot(route_t *head, position_t *size, position_t *block, 
	int num_block, int *continues) {
	int status1 = FALSE, status2 = FALSE, status3 = FALSE;
	int status4 = FALSE, status5 = FALSE;
	checkstatus(head, size, block, &status1, &status2, &status3, &status4, 
		&status5, num_block);
	/* Need further repairation when meets only status4 */
	if ((status1 == FALSE) && (status2 == FALSE) && (status3 == FALSE) 
		&& (status4 == TRUE)) {
		*continues = CONTINUE;
	}
	/* The route is valid when meets status5*/
	else if (status5 == TRUE) {
		*continues = VALID;
	}
	/* Can't be repaired */
	else {
		*continues = FAIL;
	}
}

/* If can be repaired then do repairation */
void
iffound(int **numarray, position_t *size, position_t endposition, 
	int *finalnum_route, route_t **head, route_t *onebefore, char ***twoarray, 
	position_t *block, int num_block) {
	int newnum_route = 0;
	route_t *newhead;
	route_t *newtail;
	/* Find temporary route */
	newnum_route = tmproute(numarray, size, endposition, &newhead, &newtail);
	*finalnum_route = 0;
	/* Update route then update two dimention map */
	*head = newroute(*head, newhead, newtail, finalnum_route, newnum_route, 
		onebefore);			
	*twoarray = twodiarray(*head, size, block, num_block);
}

/* Gather all statements for stage1 */
void
stage1ans(position_t *block, route_t **head, int num_block, char ***twoarray, 
	position_t *size, int ***numarray, int *continues) {
	position_t endposition;
	route_t *onebefore;
	int finalnum_route = 0, finish = FALSE, found = FALSE;
	/* If the route can't be repaired because can't find one before the block */
	printf(SEPERATE1);
	onebefore = findblock(block, *head, num_block);
	notfound(onebefore, &finish, *twoarray, size);
	if (finish != TRUE){
		*numarray = editnumarray(onebefore, size, block, num_block, 
			&endposition, &found);
		/* If can be repaired then do repairation */
		if (found == TRUE) {
			iffound(*numarray, size, endposition, &finalnum_route, head, 
				onebefore, twoarray, block, num_block);
			printarray(*twoarray, size); 
			printf(SEPERATE1);
			printroute(*head, finalnum_route);
			printstatus(*head, size, block, num_block, continues);
		}
		/* If the route can't be repaired because can't find 
		a replacement route */
		endfail(*twoarray, size, *continues, found);
	}
}

/* Initialize the new blocks */
void 
initial(int *loop, int *stage, int *finish, int *num_block, position_t **block, 
	route_t *head, position_t *size, int *continues, char ***twoarray) {
	*loop += 1;
	/* If it's the first further block, print seperate line */ 
	if (*loop == TRUE) {
		printstage(stage);
	}
	*finish = FALSE;
	*num_block = 0;
	/* Find number and spots of new blocks */
	*block = storeblock(num_block);
	continueornot(head, size, *block, *num_block, continues);
	/* Create and print two dimention array */
	*twoarray = twodiarray(head, size, *block, *num_block);
	printarray(*twoarray, size); 
}

/* Check whether the route can be repaired */
void
valid(int finish, int ***numarray, route_t *onebefore, position_t *size, 
	position_t *block, int num_block, int *found, route_t **head, 
	char ***twoarray, int *continues) {
	position_t endposition;
	int finalnum_route = 0;
	if (finish != TRUE) {
		*numarray = editnumarray(onebefore, size, block, num_block, 
			&endposition, found);
		/* The route can be further repaired */
		if (*found == TRUE) {
			iffound(*numarray, size, endposition, &finalnum_route, head, 
				onebefore, twoarray, block, num_block);
			continueornot(*head, size, block, num_block, continues);
		}
		/* Check if failed to repair */
		endfail(*twoarray, size, *continues, *found);
		/* Print out if the route is valid now */
		if (*continues == VALID) {
			end(*twoarray, size, *head, finalnum_route);
			printf(STATUS5);
		}
	}
	/* Print out if it's fair to repair */
	else {
		*continues = FAIL;		
	}
}

/* Gather all stage2 statements */
void
stage2ans(int *loop, int *stage, route_t **head, position_t *size, 
	int *continues, char ***twoarray, int ***numarray) { 
	int inloop = 0, num_block = 0;
	int finish = FALSE, found = FALSE;
	route_t *onebefore;
	position_t *block;
	/* Scan all possible blocks each time */
	while (scanf(" $ ") == FALSE){
		inloop = 0;
		num_block = 0;
		block = NULL;
		/* Find the blocks */
		initial(loop, stage, &finish, &num_block, &block, *head, size, 
			continues, twoarray);
		/* Check whether can be further repaired */
		while (*continues == CONTINUE) {
			found = FALSE;
			*continues = 0;
			inloop += 1;
			/* If it's first time to be repaired, print out the seperate line */
			if (inloop == TRUE) {
				printf(SEPERATE1);
			}
			onebefore = findblock(block, *head, num_block);
			/* Check final status of the route */
			notfound(onebefore, &finish, *twoarray, size);
			valid(finish, numarray, onebefore, size, block, num_block, 
				&found, head, twoarray, continues);
		}
		printf(SEPERATE2);
	}
	freetwoarray(size, *twoarray);
}

/* Free two dimention array */
void
freetwoarray(position_t *size, char **twoarray) {
	int i;
	for (i = 0; i < size[SIZE].row; i++) {
		free(twoarray[i]);
	}
	free(twoarray);
	twoarray = NULL;
}

/* Free link list */
void
freelist(route_t *head) {
	route_t *prev;
	assert(head != NULL);
	while (head != NULL) {
		prev = head;
		head = head -> next;
		free(prev);
		prev = NULL;
	}
}

/* algorithms are fun */