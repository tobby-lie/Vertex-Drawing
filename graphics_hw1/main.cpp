
#include<GLUT/GLUT.h> //includes the opengl, glu, and glut header files
#include <iostream>
#include <vector>
#include <tuple>

float x,y = 0;
bool check = true;
bool filled = false;
bool g_bDragging = false;
bool first_click = true;

struct Coordinates
{
    double x, y;
};

Coordinates temp_coord = {0, 0};

std::vector<Coordinates> coord_vec;
std::vector<Coordinates> last_line_coord_vec; // will hold values of coordinates that make up last line made

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
        if (first_click)
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
            
            first_click = false;
        }
        g_bDragging = true;
    }
    
    else if (button==GLUT_LEFT_BUTTON && state==GLUT_UP)
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
        
        g_bDragging = false;
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
    
    if (g_bDragging)
    {
        glColor3f(0.0, 0.0, 0.0);
        glEnable(GL_COLOR_LOGIC_OP);
        
        glBegin(GL_LINES);
        
        glVertex2i(coord_vec.back().x, coord_vec.back().y);
        glVertex2i(x, y);
        
        glEnd();
    }

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
                if (i + 1 > coord_vec.size() - 1) // at last vertex so need to save the last two coordinate points
                {
                    last_line_coord_vec.clear();
                    last_line_coord_vec.push_back(coord_vec[i]);
                    last_line_coord_vec.push_back(coord_vec[i-1]);
                }
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
            first_click = true;
        }
        else
        {
            check = true;
        }
    }
    glFlush();     // flushes the frame buffer to the screen
    
    glutSwapBuffers();
}

void myidle()
{
    
}

void onMouseMove(int mousex, int mousey)
{
    if (g_bDragging)
    {
        GLint fbWidth;
        GLint fbHeight;
        std::tie(fbWidth, fbHeight) = frame_buffer_coordinates();
        
        // x and y of current postion of cursor
        x = mousex*2;
        y = fbHeight-(mousey*2);
        
        std::cout << x << std::endl;
        glColor3f(0.0, 0.0, 0.0);
        glEnable(GL_COLOR_LOGIC_OP);
        
        glBegin(GL_LINES);
        
        glVertex2i(coord_vec.end()[-1].x, coord_vec.end()[-1].y);
        glVertex2i(coord_vec.back().x, coord_vec.back().y);
        
        glVertex2i(coord_vec.back().x, coord_vec.back().y);
        glVertex2i(x, y);
        glutSwapBuffers();
        glutPostRedisplay();
        
        glEnd();
        
        glFlush();
        
        glDisable(GL_COLOR_LOGIC_OP);
    }
}

void reshape(int w, int h)
{
    GLint fbWidth;
    GLint fbHeight;
    std::tie(fbWidth, fbHeight) = frame_buffer_coordinates();
    
    GLdouble aspect = (GLdouble)fbWidth / (GLdouble)fbHeight;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);

    glViewport(0, 0, fbWidth, fbHeight);
    glutPostRedisplay();
}

int main(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitWindowSize(640,480);   //sets the width and height of the window in pixels
    glutInitWindowPosition(10,10);//sets the position of the window in pixels from top left corner
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);//creates a single frame buffer of RGB color capacity.
    glutCreateWindow("Vertex Drawing");//creates the window as specified by the user as above.
  
    glutDisplayFunc(display);//links the display event with the display event handler(display)
    glutReshapeFunc(reshape);
    
    glutMouseFunc(mouse);//keyboard event handler
    glutMotionFunc(onMouseMove);
    
    glutMainLoop();//loops the current event
}

