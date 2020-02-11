/*
 Tobby Lie
 Intro to Graphics HW 1
 2/11/20
*/

#include<GLUT/GLUT.h>
#include <iostream>
#include <vector>
#include <tuple>

/* GLOBAL FLAGS */

float x,y = 0; // float values to hold x and y coordinates
bool display_flag = true; // boolean flag to display_flag for mouse buttons
bool filled = false; // boolean flag for if polygon is filled or not
bool dragging = false; // boolean flag to display_flag if left mouse button is down
bool first_click = true; // boolean flag to display_flag if it is the first click

/* GLOBAL FLAGS */


/* GLOBAL VARIABLES */

// struct to hold x, y coordinates
struct Coordinates
{
    double x, y;
}; // end Coordinates

Coordinates temp_coord = {0, 0}; // temporary coordinates for pushing in to vector

std::vector<Coordinates> coord_vec; // vector to hold coordinates
std::vector<Coordinates> last_line_coord_vec; // will hold values of coordinates that make up last line made

/* GLOBAL VARIABLES */

/*
    Function: frame_buffer_coordinates
 
    Description: From the viewport, gets the frame buffer
    width and height.
 
    Parameters: None
 
    Pre-Conditions: None
 
    Post-Conditions: None
 
    Returns: Tuple of GLint, GLint. The first value being
    frame buffer width, second being frame buffer height.
 */
std::tuple<GLint, GLint> frame_buffer_coordinates()
{
    GLint dims[4] = {0};
    glGetIntegerv(GL_VIEWPORT, dims);
    GLint fbWidth = dims[2];
    GLint fbHeight = dims[3];
    
    return std::make_tuple(fbWidth, fbHeight);
} // end frame_buffer_coordinates

/*
    Function: onMouseClick
 
    Description: Logic for various mouse click actions depending
        on which mouse button and state of mouse button clicks.
 
    Parameters: int button, int state, int mousex, int mousey
 
    Pre-Conditions: None
 
    Post-Conditions: State of coordinate vector updated as well
        as what is displayed.
 
    Returns: Nothing
*/
void onMouseClick(int button, int state, int mousex, int mousey)
{
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN) // left mouse button down
    {
        if (first_click) // if this is the very first click
        {
            if (filled) // if the polygon has already been filled
            {
                 glClearColor(1, 1, 1, 0);
                 glClear(GL_COLOR_BUFFER_BIT);
                 coord_vec.clear(); // clear coordinate vector if first click and polygon filled
                 filled = false; // clear filled flag
            }
             
            display_flag = true; // if first click set display_flag flag

            // get the frame buffer width and height
            GLint fbWidth;
            GLint fbHeight;
            std::tie(fbWidth, fbHeight) = frame_buffer_coordinates();

            // get mouse x and y location from frame buffer
            // need to multiply by 2 because of Macbook screen
            x = mousex*2;
            y = fbHeight-(mousey*2);

            // set temporary coordinates to be pushed into vector
            temp_coord.x = x;
            temp_coord.y = y;

            // push new coordinates into coord_vec
            coord_vec.push_back(temp_coord);
            
            // unset first click false since the first click was already made
            first_click = false;
        }
        dragging = true; // if left mouse button down this is considered as dragging
    }
    
    else if (button==GLUT_LEFT_BUTTON && state==GLUT_UP) // left mouse button released
    {
        if (filled) //  if polygon filled
        {
            glClearColor(1, 1, 1, 0);
            glClear(GL_COLOR_BUFFER_BIT);
            coord_vec.clear(); // clear coord_vec to start over
            filled = false; // unset filled flag
        }

        display_flag = true; // set display_flag flag in order to trigger display

        // get frame buffer width and height
        GLint fbWidth;
        GLint fbHeight;
        std::tie(fbWidth, fbHeight) = frame_buffer_coordinates();

        // get mouse x and y location
        x = mousex*2;
        y = fbHeight-(mousey*2);

        // create temporary coordinate
        temp_coord.x = x;
        temp_coord.y = y;

        // push the temp_coord into the coord_vec
        coord_vec.push_back(temp_coord);
        
        // unset dragging flag
        dragging = false;
    }
            
   else if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN) // if right button is down, fill
   {
        display_flag = false;
   }
    glutPostRedisplay(); // redisplay
} // end onMouseClick

/*
    Function: display_vertices
 
    Description: Iterates through coord_vec and displays points.
 
    Parameters: None
 
    Pre-Conditions: coord_vec should be filled but this is not
        required.
 
    Post-Conditions: Diplayed points are updated.
 
    Returns: Nothing
*/
void display_vertices()
{
    for (int i = 0; i < coord_vec.size(); i++)
    {
        glBegin(GL_POINTS);
        glVertex2i(coord_vec[i].x, coord_vec[i].y); // draws point at x, y

        glEnd();
    }
} // end display_vertices

/*
    Function: display_lines
 
    Description: Iterates through coord_vec and displays lines
        between points.
 
    Parameters: None
 
    Pre-Conditions: coord_vec should be filled but this is not
        required.
 
    Post-Conditions: Displayed lines are updated.
 
    Returns: Nothing
*/
void display_lines()
{
    for (int i = 0; i < coord_vec.size(); i++) // iterate through all coordinates
    {
        if (i + 1 <= coord_vec.size() - 1) // check bounds
        {
            glBegin(GL_LINES); // draw line between point and point + 1
            glVertex2i(coord_vec[i].x, coord_vec[i].y);
            glVertex2i(coord_vec[i+1].x, coord_vec[i+1].y);
            
            glEnd();
            
            if (i + 1 > coord_vec.size() - 1) // at last vertex so need to save the last two coordinate points
            {
                last_line_coord_vec.clear(); // make sure the last line is cleared first before loading
                last_line_coord_vec.push_back(coord_vec[i]);
                last_line_coord_vec.push_back(coord_vec[i-1]);
            }
        }
    }
} // end display_lines

/*
    Function: fill_polygon
 
    Description: Iterates through coord_vec and creates polygon
        from these point.
 
    Parameters: None
 
    Pre-Conditions: coord_vec should be filled but this is not
        required.
 
    Post-Conditions: Displayed polygon is updated.
 
    Returns: Nothing
*/
void fill_polygon()
{
    glColor3f(0.7f, 0.18f, 0.28f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < coord_vec.size(); i++)
    {
        glVertex2i(coord_vec[i].x, coord_vec[i].y);
    }
    glEnd();
    filled = true;
    first_click = true;
} // end fill_polygon

/*
    Function: display
 
    Description: Based on flags that are set, display certain
    information to the window.
 
    Parameters: None
 
    Pre-Conditions: Different flags should carry information as
        to what needs to be executed.
 
    Post-Conditions: Display on window is updated.
 
    Returns: Nothing
*/
void display(void)
{
    glClearColor(1, 1, 1, 0); // sets the clear color to white
    glClear(GL_COLOR_BUFFER_BIT); // clear
    
    if (dragging) // if dragging, need to draw rubberband line
    {
        glColor3f(0.0, 0.0, 0.0); // set color to black
        glEnable(GL_COLOR_LOGIC_OP); // set GL_COLOR_LOGIC_OP for XOR
        
        glBegin(GL_LINES); // draw line from last to newest point
        glVertex2i(coord_vec.back().x, coord_vec.back().y);
        glVertex2i(x, y);
        
        glEnd();
        
        glDisable(GL_COLOR_LOGIC_OP); // disable XOR
    }

    glMatrixMode(GL_PROJECTION); // sets the current matrix to projection
    glLoadIdentity(); //multiply the current matrix by identity matrix

    // get frame buffer width and height
    GLint fbWidth;
    GLint fbHeight;
    std::tie(fbWidth, fbHeight) = frame_buffer_coordinates();
    gluOrtho2D(0.0, fbWidth, 0.0, fbHeight); //sets the parallel(orthographic) projection of the full frame buffer

    glPointSize(10); // sets the size of points to be drawn
    glColor3f(0.0, 0.0, 0.0); // set color of points to black
    
    if(display_flag) // if display flag is set
    {
        display_vertices(); // display points

        display_lines(); // display lines between points
    }
    else
    {
        if (coord_vec.size() > 2) // can make polygon if at least more than two vertices
        {
            display_vertices(); // display points again

            display_lines(); // display lines again
            
            fill_polygon(); // display polygon again
        }
        else
        {
            display_flag = true; // otherwise unset display flag
        }
    }
    glFlush(); // flushes the frame buffer to the screen
    
    glutSwapBuffers(); // swap buffers for double buffer for smoother animations
} // end display

/*
    Function: onMouseMove
 
    Description: Handles logic for mouse motion
 
    Parameters: int mousex, int mousey
 
    Pre-Conditions: Should have at least one point to start from.
 
    Post-Conditions: Rubberband line is drawn.
 
    Returns: Nothing
*/
void onMouseMove(int mousex, int mousey)
{
    if (dragging) // if dragging
    {
        // get frame buffer width and height
        GLint fbWidth;
        GLint fbHeight;
        std::tie(fbWidth, fbHeight) = frame_buffer_coordinates();
        
        // x and y of current postion of cursor
        x = mousex*2;
        y = fbHeight-(mousey*2);
        
        glColor3f(0.0, 0.0, 0.0); // make color black
        glEnable(GL_COLOR_LOGIC_OP); // enable XOR
        
        glBegin(GL_LINES); // draw from last line to to new point
        glVertex2i(coord_vec.back().x, coord_vec.back().y);
        glVertex2i(x, y); // new point to be drawn to
        glutSwapBuffers(); // swap buffers for double buffering, faster animations
        glutPostRedisplay(); // redisplay
        
        glEnd();
        
        glFlush(); // flushes the frame buffer to the screen
        
        glDisable(GL_COLOR_LOGIC_OP); // disable XOR
    }
} // end onMouseMove

/*
    Function: reshape
 
    Description: Handles logic for reshaping window
 
    Parameters: int w, int h
 
    Pre-Conditions: None
 
    Post-Conditions: Window is scaled while pixels anchored to
        bottom left corner.
 
    Returns: Nothing
*/
void reshape(int w, int h)
{
    // get frame buffer width and height
    GLint fbWidth;
    GLint fbHeight;
    std::tie(fbWidth, fbHeight) = frame_buffer_coordinates();
    
    // get aspect from fbWidth and fbHeight
    GLdouble aspect = (GLdouble)fbWidth / (GLdouble)fbHeight;

    // set viewport
    glViewport(0, 0, fbWidth, fbHeight);
    
    glMatrixMode(GL_PROJECTION); // sets the current matrix to projection
    glLoadIdentity(); //multiply the current matrix by identity matrix

    gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW); // sets the current matrix to modelview
    glLoadIdentity(); // multiply the current matrix by identity matrix
    
    glutPostRedisplay(); // redisplay
} // end reshape

/*
    Function: init
 
    Description: Initializes all settings.
 
    Parameters: None
 
    Pre-Conditions: None
 
    Post-Conditions: Window settings should be initialized.
 
    Returns: Nothing
*/
void init(void)
{
    glClearColor(1, 1, 1, 0); // sets the background color to white light

    glMatrixMode(GL_PROJECTION); // sets the current matrix to projection
    glLoadIdentity(); //multiply the current matrix by identity matrix

    // get frame buffer width and height
    GLint fbWidth;
    GLint fbHeight;
    std::tie(fbWidth, fbHeight) = frame_buffer_coordinates();
    gluOrtho2D(0.0, fbWidth, 0.0, fbHeight);//sets the parallel(orthographic) projection of the full frame buffer
} // end init

int main(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitWindowSize(640,480); // sets the width and height of the window in pixels
    glutInitWindowPosition(10,10); // sets the position of the window in pixels from top left corner
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // creates a single frame buffer of RGB color capacity.
    glutCreateWindow("Vertex Drawing"); // creates the window as specified by the user as above.
    
    init(); // call init function
  
    glutDisplayFunc(display); // links the display function with display event handler
    glutReshapeFunc(reshape); // links the reshape function with reshape event handler
    
    glutMouseFunc(onMouseClick); // mouse event handler
    glutMotionFunc(onMouseMove); // motion event handler
    
    glutMainLoop(); // loops the current event
    
    return 0;
}
