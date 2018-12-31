//
//  drawing.c
//  softplot
//
//  Created by András Libál on 8/7/18.
//  Copyright © 2018 András Libál. All rights reserved.
//

#include "drawing.h"
#include "globaldata.h"

#include <math.h>
//#include <unistd.h>
#include <windows.h>

#include <GLUT/glut.h>
//#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>



//initialize OpenGl
void init_OpenGl()
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
}


//initialize everything related to glut
void init_glut(int argc, char **argv)
{
//int argc;
//char **argv;
//
//    argc = 0;
//    argv = NULL;
    
    // Initializes glut
    glutInit(&argc, argv);

    // Sets up a double buffer with RGBA components and a depth component
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);

    // Sets the window size to 512*512 square pixels
    glutInitWindowSize(global.Windowsize_x, global.Windowsize_y);

    // Sets the window position to the upper left
    glutInitWindowPosition(0, 0);

    // Creates a window using internal glut functionality
    glutCreateWindow("Softplot - Soft Matter Plotter");

    // passes reshape and display functions to the OpenGL machine for callback
    glutReshapeFunc(reshape_function);
    glutDisplayFunc(display_function);
    glutIdleFunc(display_function);
}

//Draws a circle with triangle segments
void DrawCircle(float cx, float cy, float r, int num_segments,int filled)
{
    float theta,c,s,t;

    theta = 2 * 3.1415926 / float(num_segments);
    c = cosf(theta);//precalculate the sine and cosine
    s = sinf(theta);

    //we start at angle = 0
    float x = r;
    float y = 0;
    
    //draw a filled circle
    if (filled==1)
        {
        glBegin(GL_TRIANGLE_FAN);
    
        for(int ii = 0; ii < num_segments; ii++)
            {
            //output vertex
            glVertex3f(x + cx, y + cy, -10);
        
            //apply the rotation matrix
            t = x;
            x = c * x - s * y;
            y = s * t + c * y;
            }
        glEnd();
        }

    //draw a circle without fill
    if (filled==0)
        {
        glBegin(GL_LINE_LOOP);
    
        for(int ii = 0; ii < num_segments; ii++)
            {
            glVertex3f(x + cx, y + cy, -10);//output vertex
        
            //apply the rotation matrix
            t = x;
            x = c * x - s * y;
            y = s * t + c * y;
            }
        glEnd();
        }
}

//draws the particles from a frame
void draw_all_particles(int frame)
{
    int i,j;
    float x,y;
    float r;
    int color;

    float x0,y0,x1,y1;
    float dx,dy;

    //transform simulation box size
    //to Opengl display coordinates

    x0 = 0.0625 - 2.0;
    y0 = 0.0625 - 1.25;
    x1 = 2.5 - 0.0625 - 2.0;
    y1 = 2.5 - 0.0625 - 1.25;
    //printf("%f %f\n",x0,y0);
    //printf("%f %f\n",x1,y1);

    //simulation box
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(1.0);
    glBegin(GL_LINE_LOOP);
        glVertex3f(x0,y0,-10);
        glVertex3f(x0,y1,-10);
        glVertex3f(x1,y1,-10);
        glVertex3f(x1,y0,-10);
    glEnd();

//draw a line to indicate non-square box
//SY is smaller I suppose
/*
glEnable(GL_LINE_STIPPLE);
glLineStipple(4, 0xAAAA);
glBegin(GL_LINES);
    x0 = 0.0;
    y0 = 62.35;
    transform_to_opengl(&x0,&y0);
    glVertex3f(x0,y0,-10);
    //printf("%f %f\n",x0,y0);
    x0 = SX;
    y0 = 62.35;
    transform_to_opengl(&x0,&y0);
    glVertex3f(x0,y0,-10);
    //printf("%f %f\n",x0,y0);
glEnd();
glDisable(GL_LINE_STIPPLE);
glFlush();
*/


    //draw all the particles

    for(i=0;i<global.N_objects;i++)
        {
        x = global.objects[frame][i].x;
        y = global.objects[frame][i].y;
        
        if (inside_zoomed_box(x,y))
            {
            color  = global.objects[frame][i].color;
            //default: black
            
            
            if (color==2) glColor3f(1.0, 0.0, 0.0);
                else if (color==3) glColor3f(0.0, 0.0, 1.0);
                    else glColor3f(0.7, 0.7, 0.7);
            /*
            switch (color)
                {
                case 2: {glColor3f(0.0, 0.0, 0.0);break;} //particles
                case 4: {glColor3f(0.25, 0.25, 0.588);break;} //0-in
                case 5: {glColor3f(0.322, 0.616, 0.718);break;} //1in
                case 6: {glColor3f(0.49, 0.722, 0.455);break;}   //2in
                case 7: {glColor3f(0.89, 0.612, 0.216);break;}   //3in
                case 8: {glColor3f(0.85, 0.129, 0.125);break;}   //4in
                
            
                default: {glColor3f(1.0, 1.0, 1.0);break;} //should be only gs
                }
            */
            
            //remap to OpenGl coord
        
            //main window is an 1280x800
            //I want to be in the right side 800x800 for the plot
            //max coordinates are -2,-2 (x) and -1.25,1.25 (y)
        
            transform_to_opengl(&x,&y);
        
            if (color==2) DrawCircle(x, y, global.radius_particle, 36,1);
            else DrawCircle(x, y, global.radius_vertex, 36,1);
        
            /*
            if ((color==2)||(color==3))
                {
                r = objects[frame][i].R;
                transform_distance_to_opengl(&r);
                DrawCircle(x, y, r, 36,0);
                }*/
            //else DrawCircle(x, y, 0.02, 36,1);
            }
        }
    
    
    //draw the trace
    
   if (frame>100)
   for(i=0;i<global.N_objects;i++)
    {
    color  = global.objects[frame][i].color;
        //default: black
        glColor3f(0.0, 0.0, 0.0);
        switch (color)
            {
            case 2: {glColor3f(1.0, 0.0, 0.0);break;}
            case 3: {glColor3f(0.0, 0.5, 0.0);break;}
            case 4: {glColor3f(0.0, 0.0, 1.0);break;}
            //else: {glColor3f(0.0, 0.0, 0.0);break;}
            }
    
    //only for particles draw traces
    
    if (global.trajectories_on)
        if (global.objects[frame][i].color>1)
            {
            glLineWidth(3.0);
            glBegin(GL_LINES);
            for(j=10;j<100;j=j+10)
                {
                //how long will the line segment be
                dx = fabs(global.objects[frame-j+10][i].x - global.objects[frame-j][i].x);
                dy = fabs(global.objects[frame-j+10][i].y - global.objects[frame-j][i].y);
                
                //does it skip over boundaries
                //if it does stop drawing the trace from this point on
                if ((dx>(global.SX/2.0))||(dy>(global.SY/2.0))) break;
                    
                x0 = global.objects[frame-j][i].x;
                y0 = global.objects[frame-j][i].y;
                transform_to_opengl(&x0,&y0);
                glVertex3f(x0,y0,-10);
                
                x0 = global.objects[frame-j+10][i].x;
                y0 = global.objects[frame-j+10][i].y;
                transform_to_opengl(&x0,&y0);
                glVertex3f(x0,y0,-10);
                //printf("%f %f\n",x0,y0);
                
                }
            glEnd();
            }
    
    }
}

int inside_zoomed_box(float x, float y)
{

if ((x>=global.zoom_x0)&&(x<=global.zoom_x1)&&(y>=global.zoom_y0)&&(y<=global.zoom_y1))
    return 1;
else return 0;

}



//transforms from simulation units to Opengl units
void transform_to_opengl(float *x,float *y)
{

*x = *x - global.zoom_x0;
*y = *y - global.zoom_y0;

*x = *x/global.zoom_deltax * 2.375 - 2.0 + 0.0625;
*y = *y/global.zoom_deltay * 2.375 - 1.25 + 0.0625;
}

//transforms from simulation unit distances to Opengl units
void transform_distance_to_opengl(float *r)
{
*r = *r/global.SX * 2.375;
}

void display_function()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    draw_all_particles(global.current_frame);
    glutSwapBuffers();
    
    //global.current_frame = 20;
   
    if (global.current_frame<global.N_frames-10)
        {
        global.current_frame +=10;
        printf("%d/%d\n",global.current_frame,global.N_frames);
        fflush(stdout);
        }
   else global.current_frame = 0;

   //usleep(400000); //microseconds
   Sleep(400); //milliseconds
}

void reshape_function(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.1, 0.1, -float(h)/(10.0*float(w)), float(h)/(10.0*float(w)), 0.5, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void start_main_loop_glut()
{
      glutMainLoop();
}



