/*
* Chris Roberts
* Graphics 4361
* October 2010
*/


#include <stdlib.h>
#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define windowsize 1000
#define div_x 1000
#define div_y 1000
#define gap 1
#define ROWS 75
#define COLS 75
#define SIZE ROWS*COLS

float width = (windowsize - 75.0*gap) / 74;

int matrix[ROWS][COLS];
int active = 0;

int delay = 200;
int numNbrs(int, int);

// starts all cells off as dead.
void initmatrix()
{

  int i;
  int j;
	for(i = 0; i < ROWS; i++)
	{
		for(j = 0; j < COLS; j++)
		{
			matrix[i][j] = 0;
		}
	}
}

// Applies the dead to alive rule.
void checkRevive(int i, int j)
{
  int aliveNbrs = numNbrs(i, j);
  if(aliveNbrs == 3)
    matrix[i][j] = -1;
}


// Applies the kill rule.
void checkKill(int i, int j)
{
  int aliveNbrs = numNbrs(i, j);
  if(aliveNbrs > 3 || aliveNbrs < 2){
    matrix[i][j] = 2;
  }
}

/*
* Checks the current cell for
* alive neighbors.
*/
int numNbrs(int i, int j)
{
  int nbrs = 0;
  if (i > 0 && j > 0){
    if(matrix[(i-1)][j] >= 1)
     nbrs++;
    if(matrix[(i+1)%ROWS][j] >= 1)
     nbrs++;
    if(matrix[i][(j+1)%COLS] >= 1)
     nbrs++;
    if(matrix[i][(j-1)] >= 1)
     nbrs++;
    if(matrix[(i-1)][(j+1)%COLS] >= 1)
     nbrs++;
    if(matrix[(i+1)%ROWS][(j+1)%COLS] >= 1)
     nbrs++;
    if(matrix[(i-1)][(j-1)] >= 1)
     nbrs++;
    if(matrix[(i+1)%ROWS][(j-1)] >= 1)
     nbrs++;
  }

  else if (i > 0){
    if(matrix[(i-1)][j] >= 1)
     nbrs++;
    if(matrix[(i+1)%ROWS][j] >= 1)
     nbrs++;
    if(matrix[i][(j+1)%COLS] >= 1)
     nbrs++;
    if(matrix[i][COLS-1] >= 1)
     nbrs++;
    if(matrix[(i-1)][(j+1)%COLS] >= 1)
     nbrs++;
    if(matrix[(i+1)%ROWS][(j+1)%COLS] >= 1)
     nbrs++;
    if(matrix[(i-1)][COLS-1] >= 1)
     nbrs++;
    if(matrix[(i+1)%ROWS][COLS-1] >= 1)
     nbrs++;
  }

  else if (j > 0){
    if(matrix[ROWS-1][j] >= 1)
     nbrs++;
    if(matrix[(i+1)%ROWS][j] >= 1)
     nbrs++;
    if(matrix[i][(j+1)%COLS] >= 1)
     nbrs++;
    if(matrix[i][(j-1)] >= 1)
     nbrs++;
    if(matrix[ROWS-1][(j+1)%COLS] >= 1)
     nbrs++;
    if(matrix[(i+1)%ROWS][(j+1)%COLS] >= 1)
     nbrs++;
    if(matrix[ROWS-1][(j-1)] >= 1)
     nbrs++;
    if(matrix[(i+1)%ROWS][(j-1)] >= 1)
     nbrs++;
  }

  else {
    if(matrix[ROWS-1][j] >= 1)
     nbrs++;
    if(matrix[(i+1)%ROWS][j] >= 1)
     nbrs++;
    if(matrix[i][(j+1)%COLS] >= 1)
     nbrs++;
    if(matrix[i][COLS-1] >= 1)
     nbrs++;
    if(matrix[ROWS-1][(j+1)%COLS] >= 1)
     nbrs++;
    if(matrix[(i+1)%ROWS][(j+1)%COLS] >= 1)
     nbrs++;
    if(matrix[ROWS-1][COLS-1] >= 1)
     nbrs++;
    if(matrix[(i+1)%ROWS][COLS-1] >= 1)
     nbrs++;
  }

  return nbrs;
}


// applies the rules to the current matrix.
void updateMatrix(int doit)
{
  if(doit == 1)
  {
    int i;
    int j;
    for(i = 0; i < ROWS; i++)
	    {
		  for(j = 0; j < COLS; j++)
		    {
			  if(matrix[i][j] == 0)
			   checkRevive(i,j);
			  if(matrix[i][j] == 1)
			   checkKill(i,j);
		    }
	    }
    for(i = 0; i < ROWS; i++)
	    {
		  for(j = 0; j < COLS; j++)
		    {
		  	switch(matrix[i][j])
		    	{
		   		case 2:
		   			matrix[i][j] = 0;
		  			break;
		  		case -1:
		  			matrix[i][j] = 1;
		  			break;
		  	}
		  }
	}

  glutPostRedisplay();

// determines how fast you want the simulation
// to run.
	glutTimerFunc(delay,updateMatrix,active);

}

}


void init()
{
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,div_x,div_y,0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}


/*
* Draws orange squares on a black background.
* Also loads a new name on the name stack
* for each row.
*/
void drawObjects(GLenum mode)
{
int i = 0;
int j = 0;
float x = gap;
float y = gap;

for(i = 0 ; i < 75; i++)
{
		if (mode == GL_SELECT)
         glLoadName (i); // new row

for(j = 0; j < 75; j ++)
{


		if(matrix[i][j] == 1) // alive
		  glColor3f(0.0, 0.0, 0.0);
		else
		  glColor3f(1.0, 165.0/255.0, 0.0);  //dead

    if(mode == GL_SELECT)
		glPushName(j);

    glRectf(x, y, x+width, y+width);

	  x = x +width+gap;

		if (mode == GL_SELECT)
				glPopName ();
}


x = gap;
y = y + width + gap;

}


}

/*
* processHits prints out the contents of the
*  selection array and processes clicks
*/
void processHits (GLint hits, GLuint buffer[])
{
   unsigned int i, j;
   GLuint names, *ptr;
		int x, y;

 //  printf ("hits = %d\n", hits); // diagnostic tool
   ptr = (GLuint *) buffer;
   for (i = 0; i < hits; i++)
   { /*  for each hit  */
      names = *ptr;
      ptr+=3;
      for (j = 0; j < names; j++)
      { /*  for each name */
         if(j==0) x = (int)*ptr;
         else y = (int)*ptr;
         ptr++;
      }
    //  printf ("%i %i\n",x,y); // diagnostic tool
   }

	//flip the value of the cell
	matrix[x][y] = !matrix[x][y];

}

/*
* s starts the simulation,
* e ends the simulation.
* ESC quits.
*/
void keyboard(unsigned char key,
int x, int y)
{


   switch (key)
   {
      case 27:
         exit(0);
         break;
			case 's':
			   active = 1;
				 updateMatrix(active);
				 break;
			case 'e':
				 active = 0;
				 break;
			case '-': // slow
				 delay = delay + 20;
				 break;
			case '=': // speed up
		if(delay >= 20)
				 delay = delay - 20;
				 break;
   }
}


/*
* Standard mouse callback function
*/
void mouse(int button, int state, int x, int y)
{
   GLuint selectBuf[SIZE];
   GLint hits;
   GLint viewport[4];

   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
   {
   glGetIntegerv (GL_VIEWPORT, viewport);

   glSelectBuffer (SIZE, selectBuf);
   glRenderMode(GL_SELECT);

   glInitNames();
   glPushName(0);

   glMatrixMode (GL_PROJECTION);
   glPushMatrix ();
   glLoadIdentity ();
/* create 5x5 pixel picking region near cursor location */
   gluPickMatrix ((GLdouble) x, (GLdouble) (viewport[3] - y),
                  5.0, 5.0, viewport);
   glOrtho(0,div_x,div_y,0, -1, 1);
   drawObjects(GL_SELECT);


   glMatrixMode (GL_PROJECTION);
   glPopMatrix ();
   glFlush ();

   hits = glRenderMode (GL_RENDER);
   processHits (hits, selectBuf);

   glutPostRedisplay();
   }
}



/*
* Display loop, double buffered.
*/
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawObjects(GL_RENDER);
    glutSwapBuffers ();
    glFlush();
}



// main loop
 int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (windowsize, windowsize);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display);
	 glutMouseFunc(mouse);
	 glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
