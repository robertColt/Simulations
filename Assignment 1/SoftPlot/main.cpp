//
//  main.cpp
//  softplot
//
//  Created by András Libál on 7/20/18.
//  Copyright © 2018 András Libál. All rights reserved.
//

// The OpenGL libraries, make sure to include the GLUT and OpenGL frameworks

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "globaldata.h"
#include "drawing.h"


int main(int argc, char **argv)
{

    initialize_global_data();
    
    read_moviefile_data();
    
    write_frame_data_to_file();
    
    init_glut(argc, argv);
    init_OpenGl();

    start_main_loop_glut();
    
    return 0;
}
