/*
   
*/

#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#include <cstdlib>
#include <iostream>
#include "ToolKit.h"
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;



/****************** Globals ******************/

#define WINDOW_WIDTH	    600	/* window dimensions */
#define WINDOW_HEIGHT		600



static float WinWidth = WINDOW_WIDTH;
static float WinHeight = WINDOW_HEIGHT;
static int icolor = 0;

float White[ ] = { 1., 1., 1., 1.};



const char *WINDOWTITLE = "A Simple Square";
const char *OBJ = "sphere.obj";
const float BOXSIZE = { 400.f };

enum Colors
{
    RED,
    YELLOW,
    GREEN,
    CYAN,
    BLUE,
    MAGENTA,
    WHITE,
    BLACK
};

const GLfloat Colors[ ][3] =
{
   { 1., 0., 0. },
   { 1., 1., 0. },
   { 0., 1., 0. },
   { 0., 1., 1. },
   { 0., 0., 1. },
   { 1., 0., 1. },
   { 1., 1., 1. },
   { 0., 0., 0. },
};



int MainWindow;

int point_count;
int face_count;
Vertex *point = NULL;
Face *face = NULL;
GLuint BoxList;
GLuint Light0List;

struct Vector3d
{
   float x;
   float y;
   float z;
};


///////////////////////////////

#define GRAVITY 9.8

const float g = -GRAVITY;


static int frame = 24;
static int TimeDelay = int ( 1. / 25. * 1000. ); 



Vector3d velocity = { 0., 0., 0. };
Vector3d ball = { 0., 0., 0. };


static float timeStep =  0.4;



/****************** Functions ******************/
void display();
void handleKey(unsigned char key, int x, int y);
void InitGlut();
void doReshape();
void InitList();
void Animation();
void Timer( int t );
void SetMaterial( float r, float g, float b, float shininess );
void PointLight( int ilight, float x, float y, float z, float r, float g, float b );


float *Array3( float a, float b, float c )
{
   float *array = new float [ 4 ];

   array[ 0 ] = a;
   array[ 1 ] = b;
   array[ 2 ] = c;
   array[ 3 ] = 1.;
   return array;

}
float *MulArray3( float factor, float array0[3] )
{
    float *array = new float [ 4 ];

    array[0] = factor * array0[0];
    array[1] = factor * array0[1];
    array[2] = factor * array0[2];
    array[3] = 1.;
    return array;
}



void PointLight( int ilight, float x, float y, float z, float r, float g, float b )
{

    glLightfv( ilight, GL_POSITION, Array3( x, y, z ) );
    glLightfv( ilight, GL_AMBIENT, Array3( 0., 0., 0. ) );
    glLightfv( ilight, GL_DIFFUSE, Array3( r, g, b ) );
    glLightfv( ilight, GL_SPECULAR, Array3( r, g, b ) );
    glLightf ( ilight, GL_CONSTANT_ATTENUATION, 1. );
    glLightf ( ilight, GL_LINEAR_ATTENUATION, 0. );
    glLightf ( ilight, GL_QUADRATIC_ATTENUATION, 0. );
    glEnable( ilight );
}


void SetMaterial( float r, float g, float b, float shininess )
{

    glMaterialfv( GL_BACK, GL_EMISSION, Array3( 0., 0., 0. ) );
    glMaterialfv( GL_BACK, GL_AMBIENT, MulArray3( .4f,  White) );
    glMaterialfv( GL_BACK, GL_DIFFUSE, MulArray3( 1., White ) );
    glMaterialfv( GL_BACK, GL_SPECULAR, Array3( 0., 0., 0. ) );
    glMaterialf ( GL_BACK, GL_SHININESS, 2.f );
    glMaterialfv( GL_FRONT, GL_EMISSION, Array3( 0., 0., 0. ) );
    glMaterialfv( GL_FRONT, GL_AMBIENT, Array3( r, g, b ) );
    glMaterialfv( GL_FRONT, GL_DIFFUSE, Array3( r, g, b ) );
    glMaterialfv( GL_FRONT, GL_SPECULAR, MulArray3( .8f, White ) );
    glMaterialf ( GL_FRONT, GL_SHININESS, shininess );
}

void InitGlut( )
{

 cout << TimeDelay << endl;
  bool load = load_obj_file( OBJ, point, face, point_count, face_count );
  if( !load )
  {
    cout << "Can't open the OBJ" << endl;
    exit(EXIT_FAILURE);
  }

  // make GLUT select a double buffered display that uses RGBA colors(RGBA)

  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH ); //Display needs RGBA, Double Frame, DEPTH
  
  // create the graphics window, giving width, height, and title text
  glutInitWindowSize( WINDOW_WIDTH, WINDOW_WIDTH);
  MainWindow = glutCreateWindow( WINDOWTITLE );

  // specify window clear (background) color to be opaque white
  glClearColor( 0.6, 0.8, 0.7, 1. );
  
  

  // set up the callback routines to be called when glutMainLoop() detects
  // an event
  // display callback
  glutSetWindow( MainWindow );
  glutDisplayFunc(display);
  //glutIdleFunc(Animation);
  glutTimerFunc( TimeDelay, Timer ,0 );



}

void Timer( int t )
{
  Animation();
  //timeStep = timeStep+1;
  glutPostRedisplay();
  glutTimerFunc( TimeDelay, Timer ,0 );

}

void Animation()
{
  
  float oldV = velocity.y;
  velocity.y = velocity.y + g * timeStep;
  ball.y = ball.y + (oldV+velocity.y) / 2 * timeStep;

  glutSetWindow( MainWindow );
  glutPostRedisplay();

}



/*
   Display Callback Routine: clear the screen and draw a square
   This routine is called every time the window on the screen needs
   to be redrawn, like if the window is iconized and then reopened
   by the user, and when the window is first created. It is also
   called whenever the program calls glutPostRedisplay()
*/
void display(){

  float dx = BOXSIZE / 2.;
  float dy = BOXSIZE / 2.;
  float dz = BOXSIZE / 2.;
  
  glutSetWindow( MainWindow );

  glDrawBuffer( GL_BACK );
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear window to background color
  glEnable( GL_DEPTH_TEST ); //Active Depth
  glDepthFunc( GL_LESS );

  //Specify shading model
  glShadeModel( GL_FLAT );

  //Only Show BACK Face (So Cull Front). FRONT is count clock wise
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  glFrontFace(GL_CCW);

  // define the drawing coordinate system on the viewport
  // lower left is (0, 0), upper right is (WIDTH, HEIGHT), measured in pixels
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();



  //glOrtho is for 3D glOrtho( xl, xr, yb, yt, zn, zf ) 
  glOrtho( -400, 400, -400, 400, 0.1, 1000. );

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  gluLookAt( dx,100, dz, 0., 0., 0., 0., 1., 0. );
 

  // red, yellow, green, cyan, blue, magenta
  float colors[6][3] = {{1, 0, 0}, {1, 1, 0}, {0, 1, 0},
      {0, 1, 1}, {0, 0, 1}, {1, 0, 1}};
  
  

  /************************* My Grawing ****************************/




  

  //Animation();
   
    glPushMatrix();
    //SetMaterial( 0.5, 0.5, 0.5, 1. );
    glColor3f( 0.5, 0.5, 0.5 );
    //glTranslatef( ball.x, ball.y, ball.z );
    glTranslatef( 0., 200., 0. );
    glScalef( 15., 15., 15. );
    glBegin( GL_TRIANGLES );
      for (int i = 0; i < face_count; ++i)
      {
        glVertex3f( point[ face[ i ].v1 ].x, point[ face[ i ].v1 ].y, point[ face[ i ].v1 ].z );
        glVertex3f( point[ face[ i ].v2 ].x, point[ face[ i ].v2 ].y, point[ face[ i ].v2 ].z );
        glVertex3f( point[ face[ i ].v3 ].x, point[ face[ i ].v3 ].y, point[ face[ i ].v3 ].z );

      }
    glEnd();
    glPopMatrix();


   




  /****************************************************************/


  
  

  glEnable( GL_NORMALIZE );

 
  //Call the Box
  glCallList( Light0List );
  glCallList( BoxList );
   
 

  // flush the OpenGL pipeline to the viewport
  glutSwapBuffers();
  glFlush();
}

void doReshape(int w, int h){
  int vpw, vph;
  
  float aspect = float(WINDOW_WIDTH) / float(WINDOW_HEIGHT);
  if(float(w) / float(h) > aspect){
    vph = h;
    vpw = int(aspect * h + 0.5);
  }
  else{
    vpw = w;
    vph = int(w / aspect + 0.5);
  }
  
  glViewport(0, 0, vpw, vph);
  WinWidth = w;
  WinHeight = h;
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho( -300, 300, -300, 300, 0.1, 1000. );
  
  
}

//Here is sth will never change
void InitList()
{
  float dx = BOXSIZE / 2.;
  float dy = BOXSIZE / 2.;
  float dz = BOXSIZE / 2.;

  glutSetWindow( MainWindow );

  BoxList = glGenLists( 1 );
  glNewList( BoxList, GL_COMPILE );

  //Front Side
  glPushMatrix();
  glBegin(GL_QUADS);
    SetMaterial( Colors[ RED ][ 0 ], Colors[ RED ][ 1 ], Colors[ RED ][ 2 ], 1. );
    glColor3f( Colors[ RED ][ 0 ], Colors[ RED ][ 1 ], Colors[ RED ][ 2 ]);
    glNormal3f( 0., 0., -1. );
    glVertex3f( -dx, -dy,  dz );
    glVertex3f(  dx, -dy,  dz );
    glVertex3f(  dx,  dy,  dz );
    glVertex3f( -dx,  dy,  dz );

  glEnd();
  glPopMatrix();

  glPushMatrix();
  glBegin(GL_QUADS);
     //Back Side
    SetMaterial( Colors[ YELLOW ][ 0 ], Colors[ YELLOW ][ 1 ], Colors[ YELLOW ][ 2 ], 1. );
    glColor3f( Colors[ YELLOW ][ 0 ], Colors[ YELLOW ][ 1 ], Colors[ YELLOW ][ 2 ]);
    glNormal3f( 0., 0., 1. );
    glVertex3f( -dx, -dy, -dz );
    glVertex3f( -dx,  dy, -dz );
    glVertex3f(  dx , dy, -dz );
    glVertex3f(  dx, -dy, -dz );
    

  glEnd();
  glPopMatrix();

  glPushMatrix();
  glBegin(GL_QUADS);
    //Left Side
    SetMaterial( Colors[ GREEN ][ 0 ], Colors[ GREEN ][ 1 ], Colors[ GREEN ][ 2 ], 1. );
    glColor3f( Colors[ GREEN ][ 0 ], Colors[ GREEN ][ 1 ], Colors[ GREEN ][ 2 ]);
    glNormal3f( 1., 0., 0. );
    glVertex3f( -dx, -dy,  dz );
    glVertex3f( -dx,  dy,  dz );
    glVertex3f( -dx,  dy, -dz );
    glVertex3f( -dx, -dy, -dz );
    

  glEnd();
  glPopMatrix();


  glPushMatrix();
  glBegin(GL_QUADS);
    //Right Side
    SetMaterial( Colors[ CYAN ][ 0 ], Colors[ CYAN ][ 1 ], Colors[ CYAN ][ 2 ], 1. );
    glColor3f( Colors[ CYAN ][ 0 ], Colors[ CYAN ][ 1 ], Colors[ CYAN ][ 2 ]);
    glNormal3f( -1., 0., 0. );
    glVertex3f(  dx, -dy,  dz );
    glVertex3f(  dx, -dy, -dz );
    glVertex3f(  dx,  dy, -dz );
    glVertex3f(  dx,  dy,  dz );
    

  glEnd();
  glPopMatrix();



  glPushMatrix();
  glBegin(GL_QUADS);
    //Top Side
    SetMaterial( Colors[ BLUE ][ 0 ], Colors[ BLUE ][ 1 ], Colors[ BLUE ][ 2 ], 1. );
    glColor3f( Colors[ BLUE ][ 0 ], Colors[ BLUE ][ 1 ], Colors[ BLUE ][ 2 ]);
    glNormal3f( 0., -1., 0. );
    glVertex3f( -dx,  dy,  dz );
    glVertex3f(  dx,  dy,  dz );
    glVertex3f(  dx,  dy, -dz );
    glVertex3f( -dx,  dy, -dz );
    

  glEnd();
  glPopMatrix();


  glPushMatrix();
  glBegin(GL_QUADS);
    //Top Side
    glColor3f( Colors[ MAGENTA ][ 0 ], Colors[ MAGENTA ][ 1 ], Colors[ MAGENTA ][ 2 ]);
    glNormal3f( 0., 1., 0. );
    SetMaterial( Colors[ MAGENTA ][ 0 ], Colors[ MAGENTA ][ 1 ], Colors[ MAGENTA ][ 2 ], 1. );
    glVertex3f( -dx, -dy,  dz );
    glVertex3f( -dx, -dy, -dz );
    glVertex3f(  dx, -dy, -dz );
    glVertex3f(  dx, -dy,  dz );
    

  glEnd();
  glPopMatrix();
  /*
  glPushMatrix();
  glBegin(GL_QUADS);
    //Front Side
    glPushMatrix();
    SetMaterial( Colors[ RED ][ 0 ], Colors[ RED ][ 1 ], Colors[ RED ][ 2 ], 1. );
    glColor3f( Colors[ RED ][ 0 ], Colors[ RED ][ 1 ], Colors[ RED ][ 2 ]);
    glNormal3f( 0., 0., -1. );
    glVertex3f( -dx, -dy,  dz );
    glVertex3f(  dx, -dy,  dz );
    glVertex3f(  dx,  dy,  dz );
    glVertex3f( -dx,  dy,  dz );
    glPopMatrix();

    //Back Side
    glPushMatrix();
    SetMaterial( Colors[ YELLOW ][ 0 ], Colors[ YELLOW ][ 1 ], Colors[ YELLOW ][ 2 ], 1. );
    glColor3f( Colors[ YELLOW ][ 0 ], Colors[ YELLOW ][ 1 ], Colors[ YELLOW ][ 2 ]);
    glNormal3f( 0., 0., 1. );
    glVertex3f( -dx, -dy, -dz );
    glVertex3f( -dx,  dy, -dz );
    glVertex3f(  dx , dy, -dz );
    glVertex3f(  dx, -dy, -dz );
    glPopMatrix();

    //Left Side
    glPushMatrix();
    SetMaterial( Colors[ GREEN ][ 0 ], Colors[ GREEN ][ 1 ], Colors[ GREEN ][ 2 ], 1. );
    glColor3f( Colors[ GREEN ][ 0 ], Colors[ GREEN ][ 1 ], Colors[ GREEN ][ 2 ]);
    glNormal3f( 1., 0., 0. );
    glVertex3f( -dx, -dy,  dz );
    glVertex3f( -dx,  dy,  dz );
    glVertex3f( -dx,  dy, -dz );
    glVertex3f( -dx, -dy, -dz );
    glPopMatrix();


    //Right Side
    glPushMatrix();

    SetMaterial( Colors[ CYAN ][ 0 ], Colors[ CYAN ][ 1 ], Colors[ CYAN ][ 2 ], 1. );
    glColor3f( Colors[ CYAN ][ 0 ], Colors[ CYAN ][ 1 ], Colors[ CYAN ][ 2 ]);
    glNormal3f( -1., 0., 0. );
    glVertex3f(  dx, -dy,  dz );
    glVertex3f(  dx, -dy, -dz );
    glVertex3f(  dx,  dy, -dz );
    glVertex3f(  dx,  dy,  dz );
    glPopMatrix();

    //Top Side
    glPushMatrix();
    SetMaterial( Colors[ BLUE ][ 0 ], Colors[ BLUE ][ 1 ], Colors[ BLUE ][ 2 ], 1. );
    glColor3f( Colors[ BLUE ][ 0 ], Colors[ BLUE ][ 1 ], Colors[ BLUE ][ 2 ]);
    glNormal3f( 0., -1., 0. );
    glVertex3f( -dx,  dy,  dz );
    glVertex3f(  dx,  dy,  dz );
    glVertex3f(  dx,  dy, -dz );
    glVertex3f( -dx,  dy, -dz );
    glPopMatrix();

    //Bottom Side
    glPushMatrix();
   
    glColor3f( Colors[ MAGENTA ][ 0 ], Colors[ MAGENTA ][ 1 ], Colors[ MAGENTA ][ 2 ]);
    glNormal3f( 0., 1., 0. );
    SetMaterial( Colors[ MAGENTA ][ 0 ], Colors[ MAGENTA ][ 1 ], Colors[ MAGENTA ][ 2 ], 1. );
    glVertex3f( -dx, -dy,  dz );
    glVertex3f( -dx, -dy, -dz );
    glVertex3f(  dx, -dy, -dz );
    glVertex3f(  dx, -dy,  dz );
    glPopMatrix();


  glEnd();
  glPopMatrix();*/
  glEndList();

   //Set the first light
    
  Light0List = glGenLists( 1 );
  glNewList( Light0List, GL_COMPILE );

  glPushMatrix();
  glDisable(GL_LIGHTING);
  PointLight(GL_LIGHT0, 0., 0., 0., 1.,1.,1.);
  glEnable( GL_LIGHT0 );
  glEnable(GL_LIGHTING);
  
  glPopMatrix();

  glEndList();
  

}

/*
  Keyboard Callback Routine: 'c' cycle through colors, 'q' or ESC quit
  This routine is called every time a key is pressed on the keyboard
*/
void handleKey(unsigned char key, int x, int y){
  
  switch(key){
    case 'c':   // 'c' - cycle to next color
    case 'C':
      icolor = (icolor + 1) % 6;
      glutPostRedisplay();
      break;
      
    case 'q':   // q - quit
    case 'Q':
    case 27:    // esc - quit
      exit(0);
      
    default:    // not a valid key -- just ignore it
      return;
  }
}




/*
   Main program to draw the square, change colors, and wait for quit
*/
int main(int argc, char* argv[]){

  glutInit(&argc, argv);

  InitGlut();      //Initialize all graphics

  InitList();


  
  glutKeyboardFunc(handleKey);    // keyboard callback
  
   
  
  
  glutReshapeFunc(doReshape);
  
  

  // Routine that loops forever looking for events. It calls the registered 
  // callback routine to handle each event that is detected
  glutSetWindow( MainWindow );
  glutMainLoop();

  return 0;
}
