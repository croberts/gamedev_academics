/*
   Nathan Cooper
   Christopher Roberts
   Adel Azzam

   CSCI 4631 - Fall 2010

   Lighting a Sphere

*/

#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>

static GLfloat theta[] = {0.0, 0.0, 0.0};

GLfloat mat_specular[]={1.0, 1.0, 1.0, 1.0};
GLfloat mat_diffuse[]={1.0, 0.7, 1.0, 1.0};
GLfloat mat_ambient[]={0.7, 1.0, 1.0, 1.0};
GLfloat mat_shininess={100};

void modifyProperty(int property[], int y);


typedef float point[4];

/* initial tetrahedron */

point v[]={{0.0, 0.0, 1.0},  {0.0, 0.942809, -0.33333},
          {-0.816497, -0.471405, -0.333333}, {0.816497, -0.471405, -0.333333}};

int n;
//int mode;


void triangle( point a, point b, point c)

/* display one triangle using a line loop for wire frame, a single
normal for constant shading, or three normals for interpolative shading */
{

     glBegin(GL_POLYGON);

     glNormal3fv(a);

     glVertex3fv(a);

     glNormal3fv(b);
     glVertex3fv(b);

     glNormal3fv(c);
     glVertex3fv(c);
     glEnd();
}


void normal(point p)
{

/* normalize a vector */

    double sqrt();
    float d =0.0;
    int i;
    for(i=0; i<3; i++) d+=p[i]*p[i];
    d=sqrt(d);
    if(d>0.0) for(i=0; i<3; i++) p[i]/=d;
}

void divide_triangle(point a, point b, point c, int m)
{

/* triangle subdivision using vertex numbers
righthand rule applied to create outward pointing faces */

    point v1, v2, v3;
    int j;
    if(m>0)
    {
        for(j=0; j<3; j++) v1[j]=a[j]+b[j];
        normal(v1);
        for(j=0; j<3; j++) v2[j]=a[j]+c[j];
        normal(v2);
        for(j=0; j<3; j++) v3[j]=b[j]+c[j];
         normal(v3);
        divide_triangle(a, v1, v2, m-1);
        divide_triangle(c, v2, v3, m-1);
        divide_triangle(b, v3, v1, m-1);
        divide_triangle(v1, v3, v2, m-1);
    }
    else(triangle(a,b,c)); /* draw triangle at end of recursion */
}



void tetrahedron( int m)
{

/* Apply triangle subdivision to faces of tetrahedron */

    divide_triangle(v[0], v[1], v[2], m);
    divide_triangle(v[3], v[2], v[1], m);
    divide_triangle(v[0], v[3], v[1], m);
    divide_triangle(v[0], v[2], v[3], m);
}




void spin(int axis, GLboolean left)
{
 if (left)
 {
    theta[axis]  =  theta[axis] -  2;
     printf("%f\n", theta[axis]);
}
 else
 {
    theta[axis]  =  theta[axis] +  2;
     printf("%f\n", theta[axis]);
}

 if (theta[axis] > 360.0 )
 {
    theta[axis] =  theta[axis] - 360.0;
     printf("%f\n", theta[axis]);
}

 glutPostRedisplay();

}// end spin



/*
This function is for rotating by using up/down/left/right keyboard keys
*/
void processSpecialKeys(int key, int x, int y)
     {

      GLint mod = 0;
      mod = glutGetModifiers();
     switch(key)
     {


                case GLUT_KEY_LEFT:
                     if (mod == GLUT_ACTIVE_SHIFT)
                     {
                     spin(2, GL_TRUE);
                     }
                     else
                     {
                     spin(1, GL_TRUE);
                     }
                     break;

                //modify y


                case GLUT_KEY_RIGHT:

                  if (mod == GLUT_ACTIVE_SHIFT)
                {
                     spin(2, GL_FALSE);
                     }
                     else
                     {
                     spin(1, GL_FALSE);
                     }

                     break;

                case GLUT_KEY_UP:
                 spin(0, GL_FALSE);
                 break;

                case GLUT_KEY_DOWN:
                 spin(0, GL_TRUE);
                 break;
                }

     }// end processSpecialKeys




void display(void)
{

/* Displays all three modes, side by side */

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   glLoadIdentity();

    glRotatef(theta[0], 1.0, 0.0, 0.0);
    glRotatef(theta[1], 0.0, 1.0, 0.0);
    glRotatef(theta[2], 0.0, 0.0, 1.0);

    tetrahedron(n);
// Testing moon.
//    glScalef(.1, .1, .1);
//    glTranslatef(-30, 0, 0);
//
//    tetrahedron(n);

    glFlush();
}


void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-4.0, 4.0, -4.0 * (GLfloat) h / (GLfloat) w,
            4.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
    else
        glOrtho(-4.0 * (GLfloat) w / (GLfloat) h,
            4.0 * (GLfloat) w / (GLfloat) h, -4.0, 4.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    display();
}

void updateMaterials()
{
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
}

/*
This function handles the event processing.
*/
void processMenuEvents(int selection)
{
     int x = selection;

if( x < 30)
{
    modifyProperty(mat_specular, x);
    }
else if( x < 60)
{
     modifyProperty(mat_diffuse, x - 30);
     }
else if( x < 90)
{
     modifyProperty(mat_ambient, x - 60);
     }
else
{
    mat_shininess = (x - 90)*10;
     updateMaterials();
glutPostRedisplay();
}

  if (selection==100)
     exit(EXIT_SUCCESS);


}//end processMenuEvents

void modifyProperty(int property[], int y)
{

     float value = 0;

     if( y < 10) // red
     {
         value = y/10.0;
         property[0] = value;

         }
     else if( y < 20) // green
     {
          value = (y - 10.0)/10.0;
          property[1] = value;

          }
     else // blue
     {
         value = (y - 20.0)/10.0;
         property[2] = value;

          }

updateMaterials();
glutPostRedisplay();
     }


void myinit()
{

   int menu;//, submenuSpecular, submenuDiffuse, submenuAmbient;
   int valueMenus[10], colorSubMenus[3];
   int i, j;

   //Create the value menu items.
   for (i = 0; i < 10; i++){
       valueMenus[i] = glutCreateMenu(processMenuEvents);
       for (j = 0; j < 10; j++){
           char label[7];
           sprintf(label, "%d0%%", j);
           glutAddMenuEntry(label,i*10+j);
       }
   }

   //Create the color submenus.
   j = 0;
   for (i = 0; i < 3; i++){
       colorSubMenus[i] = glutCreateMenu(processMenuEvents);
       glutAddSubMenu("Red",valueMenus[j++]);
       glutAddSubMenu("Green",valueMenus[j++]);
       glutAddSubMenu("Blue",valueMenus[j++]);
   }

   //Create the main menu.
   menu = glutCreateMenu(processMenuEvents);//
         glutAddSubMenu("S p e c u l a r ", colorSubMenus[0]);
         glutAddSubMenu("D i f f u s e ", colorSubMenus[1]);
         glutAddSubMenu("A m b i e n t ", colorSubMenus[2]);
         glutAddSubMenu("S h i n i n e s s ", valueMenus[9]);
   	     glutAddMenuEntry("Exit ", 100);
   glutAttachMenu(GLUT_RIGHT_BUTTON);


    GLfloat light_ambient[]={0.1, 0.1, 0.1, 1.0}; //affects color
    GLfloat light_diffuse[]={1.0, 1.0, 1.0, 1.0}; //affects color
    GLfloat light_specular[]={1.0, 1.0, 1.0, 1.0};

//Light positioned top-left, behind viewer
GLfloat light_pos[] = {-7, 10.0, 10, 1.0};

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient );
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

/* define material proerties for front face of all polygons */

    glShadeModel(GL_SMOOTH); /*enable smooth shading */
    glEnable(GL_LIGHTING); /* enable lighting */
    glEnable(GL_LIGHT0);  /* enable light 0 */
    glEnable(GL_DEPTH_TEST); /* enable z buffer */

    updateMaterials();

    glClearColor (1.0, 1.0, 1.0, 1.0);
    glColor3f (0.0, 0.0, 0.0);

}//end myinit function



void main(int argc, char **argv)
{
    n=5;
    glutInit(&argc, argv);
    printf("Welcome to the sphere model!\n");
     printf("Right click to change the properties of the sphere,\n");
      printf("and use the arrow keys with or without shift to rotate.\n");
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("~~ Lighting and shading / fractal spheres ~~");
    myinit();
    glutReshapeFunc(myReshape);
    glutSpecialFunc(processSpecialKeys);
    glutDisplayFunc(display);
    glutMainLoop();

}



Lighting_Shading_sphere_backup.c
Open with
Displaying Lighting_Shading_sphere_backup.c.