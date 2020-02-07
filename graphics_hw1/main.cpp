
#include<GLUT/GLUT.h> //includes the opengl, glu, and glut header files
#include <iostream>
#include <vector>
#include <tuple>

float x,y = 0;
bool check = true;
bool filled = false;

struct Coordinates
{
    double x, y;
};

Coordinates temp_coord = {0, 0};

std::vector<Coordinates> coord_vec;

std::tuple<GLint, GLint> frame_buffer_coordinates()
{
    GLint dims[4] = {0};
    glGetIntegerv(GL_VIEWPORT, dims);
    GLint fbWidth = dims[2];
    GLint fbHeight = dims[3];
    
    return std::make_tuple(fbWidth, fbHeight);
}

void mouse(int button, int state, int mousex, int mousey)
{
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
        if (filled)
        {
            glClearColor(1, 1, 1, 0);
            glClear(GL_COLOR_BUFFER_BIT);
            coord_vec.clear();
            
            filled = false;
        }
        
        check=true;
       
        GLint fbWidth;
        GLint fbHeight;
        std::tie(fbWidth, fbHeight) = frame_buffer_coordinates();
        
        x = mousex*2;
        y = fbHeight-(mousey*2);
        
        temp_coord.x = x;
        temp_coord.y = y;
        
        coord_vec.push_back(temp_coord);
        
    }
            
   else if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)//undo(clear)the drawing
   {
        check = false;
    }
    glutPostRedisplay();
}

void display(void)
{
    glClearColor(1, 1, 1, 0); // sets the background color to white light
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);// sets the current matrix to projection
    glLoadIdentity();//multiply the current matrix by identity matrix

    GLint fbWidth;
    GLint fbHeight;
    std::tie(fbWidth, fbHeight) = frame_buffer_coordinates();
    gluOrtho2D(0.0, fbWidth, 0.0, fbHeight);//sets the parallel(orthographic) projection of the full frame buffer

    glPointSize(10); // sets the size of points to be drawn (in pixels)
    glColor3f(0.0, 0.0, 0.0);
    if(check)
    {
        std::cout << coord_vec.size() << std::endl;
        for (int i = 0; i < coord_vec.size(); i++)
        {
            glBegin(GL_POINTS); // writes pixels on the frame buffer with the current drawing color
            glVertex2i(coord_vec[i].x, coord_vec[i].y);   // sets vertex

            glEnd();
        }

        for (int i = 0; i < coord_vec.size(); i++)
        {
            if (i + 1 <= coord_vec.size() - 1)
            {
                glBegin(GL_LINES);
                glVertex2i(coord_vec[i].x, coord_vec[i].y);
                glVertex2i(coord_vec[i+1].x, coord_vec[i+1].y);

                glEnd();
            }
        }
    }
    else
    {
        if (coord_vec.size() > 2) // can make polygon if at least more than two vertices
        {
            glBegin(GL_POLYGON);
            for (int i = 0; i < coord_vec.size(); i++)
            {
                glVertex2i(coord_vec[i].x, coord_vec[i].y);
            }
            glEnd();
            filled = true;
        }
        else
        {
            check = true;
        }
    }
    glFlush();     // flushes the frame buffer to the screen
}

void reshape(int w, int h)
{
    GLint fbWidth;
    GLint fbHeight;
    std::tie(fbWidth, fbHeight) = frame_buffer_coordinates();
    
    GLdouble aspect = (GLdouble)fbWidth / (GLdouble)fbHeight;

    glMatrixMode   (GL_PROJECTION);
    glLoadIdentity ();

    gluOrtho2D     (-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);

    glMatrixMode   (GL_MODELVIEW);

    glViewport     (0, 0, fbWidth, fbHeight);
    
    glutPostRedisplay();
}

int main(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitWindowSize(640,480);   //sets the width and height of the window in pixels
    glutInitWindowPosition(10,10);//sets the position of the window in pixels from top left corner
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//creates a single frame buffer of RGB color capacity.
    glutCreateWindow("Vertex Drawing");//creates the window as specified by the user as above.
  
    glutDisplayFunc(display);//links the display event with the display event handler(display)
    glutReshapeFunc(reshape);
    
    glutMouseFunc(mouse);//keyboard event handler
    
    glutMainLoop();//loops the current event
}

