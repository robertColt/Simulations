//
//  main.c
//  softsim
//
//  Created by András Libál on 7/18/18.
//  Copyright © 2018 András Libál. All rights reserved.
//
// first problem: tim,e crystal simulation

#ifdef _WIN32
#define WINPAUSE system("pause")
#endif

#include <stdio.h>
#include "initializer.h"
#include "running.h"
#include <time.h>
#include "linked_list.h"


void test() {
	struct linked_list list = { NULL };
	struct node head = { -1, NULL };
	struct node n1 = { 1, NULL };
	struct node n2 = { 2, NULL };
	struct node n3 = { 3, NULL };


	add_node(&head, &head);
	add_node(&head, &n1);
	add_node(&head, &n2);
	add_node(&head, &n3);

	/*add_node_list(&list, &head);
	add_node_list(&list, &n1);
	add_node_list(&list, &n2);
	add_node_list(&list, &n3);*/
	print(&head);
	clear_list(&head);
	print(&head);
}

int main(int argc, const char * argv[]) {
    
    printf("Soft Matter Simulator\n");

    //read_init_file("parameters/parameters.txt");
   
    init_simulation();
    init_simulation_box();
    //init_pinningsites();
    init_particles();
    init_files();
	init_grid();
    
	clock_t timeElapsed;
	timeElapsed = clock();

	run_simulation();

	timeElapsed = clock() - timeElapsed;
	double time_taken = ((double)timeElapsed) / CLOCKS_PER_SEC; //time taken by the simulation

	printf("Simulation took %f seconds !\n", time_taken); //takes about 151s for total time = 100000 particles 800

	WINPAUSE;
    
    return 0;
}
