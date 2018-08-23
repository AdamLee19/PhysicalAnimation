/*
   
*/

#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#include <cstdlib>

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


const char *WINDOWTITLE = "A Simple Square";
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







/****************** Functions ******************/
void display();
void handleKey(unsigned char key, int x, int y);
void InitGlut();
void doReshape();




void InitGlut( )
{
  

  // make GLUT select a double buffered display that uses RGBA colors(RGBA)

  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH ); //Display needs RGBA, Double Frame, DEPTH
  
  // create the graphics window, giving width, height, and title text
  glutInitWindowSize( WINDOW_WIDTH, WINDOW_WIDTH);
  MainWindow = glutCreateWindow( WINDOWTITLE );

  // specify window clear (background) color to be opaque white
  glClearColor( 1, 1, 1, 1 );
  
  glutDisplayFunc(display);

  // set up the callback routines to be called when glutMainLoop() detects
  // an event
  // display callback
  






}




/*
   Display Callback Routine: clear the screen and draw a square
   This routine is called every time the window on the screen needs
   to be redrawn, like if the window is iconized and then reopened
   by the user, and when the window is first created. It is also
   called whenever the program calls glutPostRedisplay()
*/
void display(){
  
  glutSetWindow( MainWindow );

  glDrawBuffer( GL_BACK );
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear window to background color
  glEnable( GL_DEPTH_TEST ); //Active Depth

  // define the drawing coordinate system on the viewport
  // lower left is (0, 0), upper right is (WIDTH, HEIGHT), measured in pixels
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();



  //glOrtho is for 3D glOrtho( xl, xr, yb, yt, zn, zf ) 
  glOrtho( -300, 300, -300, 300, 0.1, 1000. );

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  gluLookAt( 0., 0., ( (BOXSIZE / 2.) + 10.) , 0., 0., 0., 0., 1., 0. );
  //gluLookAt( 0., 200., 0., 0., 0., 0., 0., 0., 1. );

  // red, yellow, green, cyan, blue, magenta
  float colors[6][3] = {{1, 0, 0}, {1, 1, 0}, {0, 1, 0},
      {0, 1, 1}, {0, 0, 1}, {1, 0, 1}};
  
  
  // set the drawing color to the currently selected color

  float dx = BOXSIZE / 2.;
  float dy = BOXSIZE / 2.;
  float dz = BOXSIZE / 2.;

  
  //glColor3f(colors[icolor][0], colors[icolor][1], colors[icolor][2]);
  // draw the square

  glBegin(GL_QUADS);
    //Front Side
    glColor3f( Colors[ RED ][ 0 ], Colors[ RED ][ 1 ], Colors[ RED ][ 2 ]);
    glVertex3f( -dx, -dy,  dz );
    glVertex3f(  dx, -dy,  dz );
    glVertex3f(  dx,  dy,  dz );
    glVertex3f( -dx,  dy,  dz );

    //Back Side
    glColor3f( Colors[ YELLOW ][ 0 ], Colors[ YELLOW ][ 1 ], Colors[ YELLOW ][ 2 ]);
    glVertex3f( -dx, -dy, -dz );
    glVertex3f( -dx,  dy, -dz );
    glVertex3f(  dx , dy, -dz );
    glVertex3f(  dx, -dy, -dz );

    //Left Side
    glColor3f( Colors[ GREEN ][ 0 ], Colors[ GREEN ][ 1 ], Colors[ GREEN ][ 2 ]);
    glVertex3f( -dx, -dy,  dz );
    glVertex3f( -dx,  dy,  dz );
    glVertex3f( -dx,  dy, -dz );
    glVertex3f( -dx, -dy, -dz );


    //Right Side
    glColor3f( Colors[ CYAN ][ 0 ], Colors[ CYAN ][ 1 ], Colors[ CYAN ][ 2 ]);
    glVertex3f(  dx, -dy,  dz );
    glVertex3f(  dx, -dy, -dz );
    glVertex3f(  dx,  dy, -dz );
    glVertex3f(  dx,  dy,  dz );

    //Top Side
    glColor3f( Colors[ BLUE ][ 0 ], Colors[ BLUE ][ 1 ], Colors[ BLUE ][ 2 ]);
    glVertex3f( -dx,  dy,  dz );
    glVertex3f(  dx,  dy,  dz );
    glVertex3f(  dx,  dy, -dz );
    glVertex3f( -dx,  dy, -dz );

    //Bottom Side
    glColor3f( Colors[ MAGENTA ][ 0 ], Colors[ MAGENTA ][ 1 ], Colors[ MAGENTA ][ 2 ]);
    glVertex3f( -dx, -dy,  dz );
    glVertex3f( -dx, -dy, -dz );
    glVertex3f(  dx, -dy, -dz );
    glVertex3f(  dx, -dy,  dz );


  glEnd();

  
 

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
  
  //glMatrixMode(GL_MODELVIEW);
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



  
  glutKeyboardFunc(handleKey);    // keyboard callback
  
   
  
  
  glutReshapeFunc(doReshape);
  
  

  // Routine that loops forever looking for events. It calls the registered 
  // callback routine to handle each event that is detected
  glutSetWindow( MainWindow );
  glutMainLoop();

  return 0;
}
