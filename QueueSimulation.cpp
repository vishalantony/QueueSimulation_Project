/*
 *	Computer Graphics Project.
 *	Objective: Simulation of Linear Queue Using OpenGL.
 *	Author: Vishal Antony
 *			REG No.: 12GAEC8076
 *			B.E. 5th Sem Computer Science and Engineering.
 *			U.V.C.E., Bangalore University
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <ctype.h>
#include <time.h>
#include "queueSimula.h"

void myinit() {
	glClearColor(BACKGROUND_R, BACKGROUND_G, BACKGROUND_B, BACKGROUND_A);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0,SCREEN_X,0.0,SCREEN_Y);

	int i;
	for(i = 0; i < 15; i++) {
		queue[i].r = 0;
		queue[i].g = 0.5;
		queue[i].b = 0.95;
		queue[i].rl = 0;
		queue[i].gl = 0;
		queue[i].bl = 0;
	}

	float step = DIST/MAX;
	queue[0].x1 = OFFSET_X;
	queue[0].x2 = queue[0].x1+step;

	for(i = 1; i <= 15; i++) {
		queue[i].x1 = queue[i-1].x1+step;
		queue[i].x2 = queue[i].x1+step;
	}

}

void square(int x1, int y1, int x2, int y2) {
	glBegin(GL_POLYGON);
		glVertex2f(x1, y1);
		glVertex2f(x1, y2);
		glVertex2f(x2, y2);
		glVertex2f(x2, y1);
	glEnd();
}

void drawOutline(int x1, int y1, int x2, int y2) {
	int temp;
	if(x1 < x2) {
		temp = x1;
		x1 = x2;
		x2 = temp;
	}
	if(y1 < y2) {
		temp = y1;
		y1 = y2;
		y2 = temp;
	}
	glBegin(GL_LINES);
		glVertex2f(x1, y1);
		glVertex2f(x1, y2);
		glVertex2f(x2, y2);
		glVertex2f(x2, y1);
	glEnd();
}

void drawString(const char *str, double x=0, double y=0, double size=1.0) { 
	glPushMatrix(); 
	glTranslatef(x,y,0); 
	glScalef(size,size,1.0); 
	glColor3f(0, 0, 0);
	int itemCt = 0; 
	int len = strlen(str); 
	for (int i = 0; i < len; i++) { 
		glutStrokeCharacter(GLUT_STROKE_ROMAN,str[i]);  
	} 
	glPopMatrix(); 
}


void drawArrow(int x1, const char* s) {
	glBegin(GL_LINES);
		glVertex2f(x1, 3*SCREEN_Y/4);
		glVertex2f(x1, 3*SCREEN_Y/4-ARROW_LENGTH);
	glEnd();
	glBegin(GL_TRIANGLES);
		glVertex2f(x1, 3*SCREEN_Y/4-ARROW_LENGTH);
		glVertex2f(x1-ARROW_LENGTH/4, 3*SCREEN_Y/4-ARROW_LENGTH+ARROW_LENGTH/4);
		glVertex2f(x1+ARROW_LENGTH/4, 3*SCREEN_Y/4-ARROW_LENGTH+ARROW_LENGTH/4);
	glEnd();
	if(strcmp(s, "BACK"))
		drawString(s, x1-ARROW_LENGTH/3, 3*SCREEN_Y/4+ARROW_LENGTH/2, 1.0/(2*FONT_RATIO));
	else 
		drawString(s, x1-ARROW_LENGTH/3, 3*SCREEN_Y/4+ARROW_LENGTH/4, 1.0/(2*FONT_RATIO));
}

void blink(void) {
	if(blinking[0] == '_')
		blinking[0] = '|';
	else
		blinking[0] = '_';
	clock_t start = clock();
	while((clock() - start)/CLOCKS_PER_SEC < 0.01);
	glutPostRedisplay();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	strcpy(displayString, enter_str);
	strcat(displayString, blinking);
	
	drawString(displayString, ENTER_POSITION_X, ENTER_POSITION_Y,1.0/(FONT_RATIO));
	//blink();
	
	for(int i = 0; i < b; i++) {
		glColor3f(queue[i].r,queue[i].g,queue[i].b);
		square(queue[i].x1, SCREEN_Y/2, queue[i].x2, SCREEN_Y/2+YD);
		glColor3f(queue[i].rl,queue[i].gl,queue[i].bl);
		drawOutline(queue[i].x1, SCREEN_Y/2, queue[i].x2, SCREEN_Y/2+YD);
		if(queue[i].r != 1.0 || queue[i].b != 1.0 || queue[i].g != 1.0) {
			double ln = strlen(queue[i].num);
			double width = queue[i].x2-queue[i].x1;
			double step = width/(ln+1);
			double step_y = YD/4;
			drawString(queue[i].num, queue[i].x1+step, SCREEN_Y/2+step_y, 1.0/FONT_RATIO);
		}
	}

	drawArrow(queue[f].x1, "FRONT");
	drawArrow(queue[b].x1+OFFSET_X/2, "BACK");
	if(enqORdq == ENQUEUE)
		drawString("LAST OPERATION: ENQUEUE", OPERATION_POSITION_X, OPERATION_POSITION_Y+50, 1.0/(FONT_RATIO));
	else if(enqORdq == DEQUEUE)
		drawString("LAST OPERATION: DEQUEUE", OPERATION_POSITION_X, OPERATION_POSITION_Y+50, 1.0/(FONT_RATIO));
	else
		drawString("LAST OPERATION: NO OPERATION YET", OPERATION_POSITION_X, OPERATION_POSITION_Y+50, 1.0/(FONT_RATIO));
		
	//message
	if(message == EMPTY) {
		drawString("QUEUE EMPTY!", OPERATION_POSITION_X+80, OPERATION_POSITION_Y, 1.0/(FONT_RATIO));
	}
	else if(message == FULL) {
		drawString("QUEUE FULL!", OPERATION_POSITION_X+80, OPERATION_POSITION_Y, 1.0/(FONT_RATIO));
	}
	
	//instructions
	drawString("Instructions:", OPERATION_POSITION_X-280, OPERATION_POSITION_Y+30, 1.0/(1.5*FONT_RATIO));
	drawString("Press D or d", OPERATION_POSITION_X-280, OPERATION_POSITION_Y, 1.0/(2*FONT_RATIO));
	drawString("to dequeue.", OPERATION_POSITION_X-280, OPERATION_POSITION_Y-20, 1.0/(2*FONT_RATIO));
	glFlush();
}


void enqueue(char *s) {
	int len = strlen(s);
	int i;
	if(b < MAX) {
		len = strlen(s);
		for(i = 0; i < len-1 && s[i] == '0'; i++);
		strcpy(queue[b].num, s+i);
		b++;
		enqORdq = ENQUEUE;	// indicates what the last operation was.
		message = NO_MESSAGE;
	}
	if(b == MAX)
		message = FULL;
}


void dequeue(void) {
	if(f >= MAX || f >= b) {
		message = EMPTY;
		return;
	}

	// set all as background color
	queue[f].r = BACKGROUND_R;
	queue[f].g = BACKGROUND_G;
	queue[f].b = BACKGROUND_B;

	// outline colors. set all as same as background color.
	queue[f].rl = BACKGROUND_R;
	queue[f].gl = BACKGROUND_G;
	queue[f].bl = BACKGROUND_B;

	f++;
	enqORdq = DEQUEUE;	// indicates what the last operation was.
	
	if(f >= b) {
		message = EMPTY;
	}
	if(b >= MAX)
		message = FULL;
}

int max(int a, int b) {
	return a?(a>b):b;
}

void mykey(unsigned char key, int x, int y) {
	if(f == MAX && b == MAX) exit(0);
	
	int len = strlen(enter_str);
	if((key == 'd' || key == 'D') && (cnt_of_chars == 0)) 
		dequeue();
	else if(isdigit(key) && strlen(enter_str) <= 28) {
		enter_str[len+1] = '\0';
		enter_str[len] = key;
		cnt_of_chars++;
	}
	else if(key == '\b' && len > start_of_num) {
		enter_str[len -1] = '\0';
		cnt_of_chars--;
	}
	else if(!isgraph(key)) {
		char newint[4];
		strncpy(newint, enter_str+start_of_num, 3);
		if(newint[0]) {
			enqueue(newint);
			enter_str[start_of_num] = '\0';
		}
		cnt_of_chars = 0;
	}
	glutPostRedisplay();
}


int  main(int argc,char **argv) {	
	char number[1000];
	int i, j, len;
	char c;
	strcpy(enter_str, "Enter Element to Enqueue: ");
	start_of_num = strlen("Enter Element to Enqueue: ");
	printf("\n\n\n");
	printf("------------------------------------\n");
	printf("Simulation of Linear Queue in OpenGL\n");
	printf("------------------------------------\n\n");
	printf("Enter the number of elements in the Queue\n(Not greater than 15 and not lesser than 3):\n");
	scanf("%d", &MAX);

	while(MAX > 15 || MAX < 3) {
		printf("ERROR: Invalid value! \nEnter again: ");
		scanf("%d", &MAX);
	}
	printf("\n\nVALUE ACCEPTED! Program Running...\n");

	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(SCREEN_X,SCREEN_Y);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Queue Simulation");
	glutDisplayFunc(display);
	glutKeyboardFunc(mykey);
	//glutIdleFunc(blink);
	myinit();
	glutMainLoop();
	return 0;
}
