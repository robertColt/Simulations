//
//percolation simulation


#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define N_grid 35

int grid[N_grid][N_grid];
int cluster_number[N_grid][N_grid];

int actual_cluster;

double  p; ///probability of picking the site

FILE *moviefile;
FILE *statsFile;
int t, runs;

void initialize_system()
{
	int i, j;
	for (i = 0; i < N_grid; i++)
		for (j = 0; j < N_grid; j++)
		{
			grid[i][j] = 0; //have not picked any sites
			cluster_number[i][j] = -1;//nobody is in a cluster
		}
}

void fill_system_with_probability(double p)
{
	int i, j;
	double r;
	int N_particles;

	N_particles = 0;

	for (i = 0; i < N_grid; i++)
		for (j = 0; j < N_grid; j++)
		{
			cluster_number[i][j] = -1;

			r = rand() / (RAND_MAX + 1.0);
			//if r<p, this site will be filled
			if (r < p)
			{
				grid[i][j] = 1;
				N_particles++;
			}
		}
	printf("Filled up the system.Ended up with %d particles \n", N_particles);
}

int stackCalls = 0;
int maxStackCalls = 0;

int isWithinBounds(int index) {
	if (index >= 0 && index < N_grid) {
		return 1;
	}
	return 0;
}

void clusternumber(int i, int j)
{
	int stacki[50000];
	int stackj[50000];
	int stackIndex = 0, finished = 0;

	stacki[stackIndex] = i;
	stackj[stackIndex] = j;
	stackIndex++;
	int index1 = 0, index2 = 0;

	while (!finished) {
		//process current i and j and pop from stack
		cluster_number[i][j] = actual_cluster;
		stackIndex--;

		index1 = j;
		if (i % 2 == 0) {
			index2 = j - 1;
		}
		else {
			index2 = j + 1;
		}

		//DOWN
		if ((i + 1 < N_grid)) {
			//DOWN RIGHT
			if ((cluster_number[i + 1][index1] == -1) && (grid[i + 1][index1] == 1)) {
				stacki[stackIndex] = i + 1;
				stackj[stackIndex] = index1;
				stackIndex++;
			}
			//DOWN LEFT
			if (isWithinBounds(index2) && (cluster_number[i + 1][index2] == -1) && (grid[i + 1][index2] == 1)) {
				stacki[stackIndex] = i + 1;
				stackj[stackIndex] = index2;
				stackIndex++;
			}
		}

		//UP
		if ((i - 1 >= 0)) {
			//UP RIGHT
			if ((cluster_number[i - 1][index1] == -1) && (grid[i - 1][index1] == 1)) {
				stacki[stackIndex] = i - 1;
				stackj[stackIndex] = index1;
				stackIndex++;
			}
			//UP LEFT
			if (isWithinBounds(index2) && (cluster_number[i - 1][index2] == -1) && (grid[i - 1][index2] == 1)) {
				stacki[stackIndex] = i - 1;
				stackj[stackIndex] = index2;
				stackIndex++;
			}

		}
		//SAME ROW
		//RIGHT
		if ((j + 1 < N_grid))
			if ((cluster_number[i][j + 1] == -1) && (grid[i][j + 1] == 1)) {
				stacki[stackIndex] = i;
				stackj[stackIndex] = j + 1;
				stackIndex++;
			}
		//LEFT
		if ((j - 1 >= 0))
			if ((cluster_number[i][j - 1] == -1) && (grid[i][j - 1] == 1)) {
				stacki[stackIndex] = i;
				stackj[stackIndex] = j - 1;
				stackIndex++;
			}

		if (stackIndex == 0) {
			finished = 1;
		}
		else {
			if (stackIndex + 1 > maxStackCalls) maxStackCalls = stackIndex;

			if (stackIndex >= 50000) {
				printf("STACK CALLS TOO MUCH");
				getchar();
			}
			//printf("Max Stack calls %d\n", maxStackCalls);
			i = stacki[stackIndex - 1];
			j = stackj[stackIndex - 1];
		}
	}

}

void clusterize_system()
{
	int i, j;

	actual_cluster = 1;

	for (i = 0; i < N_grid; i++)
		for (j = 0; j < N_grid; j++)
		{
			//filled position, it's not in any cluser
			if ((grid[i][j] == 1) && (cluster_number[i][j] == -1))
			{
				//start the recursive algorithm
				//to find everybody connected to this point

				//recursive_clusternumber(i, j);
				clusternumber(i, j);

				//the next cluster is going to
				//be a bigger number
				actual_cluster++;
			}
		}
	actual_cluster--;

}

void write_moview_text() {
	int i, j;
	for (i = 0; i < N_grid; i++) {
		for (j = 0; j < N_grid; j++) {
			fprintf(moviefile, "%d", cluster_number[i][j]);
			if (j != N_grid - 1) fprintf(moviefile, " ");
		}
		fprintf(moviefile, "\n");
	}
	//if (t != runs-1) fprintf(moviefile, "\n");
}


int largest_cluster_size() {
	int cluster_sizes[5000];
	for (int k = 0; k < 5000; k++) cluster_sizes[k] = 0;

	if (actual_cluster > 5000) {
		printf("WARNING more clusters than expected!!!");
		getchar();
	}

	int i, j, cluster_num;
	for (i = 0; i < N_grid; i++) {
		for (j = 0; j < N_grid; j++) {
			cluster_num = cluster_number[i][j];
			if (cluster_num != -1) {
				cluster_sizes[cluster_num] += 1;
			}
		}
	}

	int max = 0;
	for (i = 0; i < 5000; i++) {
		if (cluster_sizes[i] > max) {
			max = cluster_sizes[i];
			cluster_num = i;
		}
	}
	printf("Largest cluster: %d (%d)\n", max, cluster_num);

	return max;
}

double p_spanning_cluster() {
	int spanning_clusters = 0,
		i, j,
		numberTop, numberBottom,
		numberLeft, numberRight;

	int already_found[2000];
	int index = 0, foundLeft = 0, foundTop = 0;

	//iterate over top part and left part
	for (j = 0; j < N_grid; j++) {
		numberTop = cluster_number[0][j];
		numberLeft = cluster_number[j][0];
		if (numberTop == -1 && numberLeft == -1) continue;

		//check if the current number has been already found
		for (int k = 0; k < index; k++) {
			if (numberTop == already_found[k]) {
				foundTop = 1;
			}
			if (numberLeft == already_found[k]) {
				foundLeft = 1;
			}
			if (foundLeft && foundTop) break;
		}

		//if both numbers have already been found continue
		if (foundLeft && foundTop) continue;

		//iterate over bottom and right part to find pairs
		for (i = 0; i < N_grid; i++) {
			numberBottom = cluster_number[N_grid - 1][i];
			numberRight = cluster_number[i][N_grid - 1];

			if (numberTop != -1 && !foundLeft && !foundTop && numberTop == numberLeft &&
				(numberTop == numberBottom || numberLeft == numberRight)) {
				spanning_clusters++;
				already_found[index] = numberTop;
				index++;
				break;
			}
			else {
				if (numberTop != -1 && numberTop == numberBottom && !foundTop) {
					spanning_clusters++;
					already_found[index] = numberTop;
					index++;
					foundTop = 1;
				}
				if (numberLeft != -1 && numberLeft == numberRight && !foundLeft) {
					spanning_clusters++;
					already_found[index] = numberLeft;
					index++;
					foundLeft = 1;
				}
			}
			if (foundTop && foundLeft) break;
		}
	}

	double p_spanning = (double)spanning_clusters / actual_cluster;
	printf("Spanning clusters found: %d/%d p(%.2f)\n ", spanning_clusters, actual_cluster, p_spanning);

	return p_spanning;

}
void write_statistics(double, double);

void write_statistics(double biggest_cluster, double p_span) {
	fprintf(statsFile, "%.2f %.2f %.2f\n", p, biggest_cluster, p_span);
}

void openFiles() {
	char* statsFileName[13];
	char* percFileName[14];

	sprintf(statsFileName, "statistics%d", N_grid);
	sprintf(percFileName, "percolation%d", N_grid);

	statsFile = fopen(statsFileName, "at");
	moviefile = fopen(percFileName, "wt");
}

void closeFiles() {
	fclose(moviefile);
	fclose(statsFile);
}

int max_clusters = 0;
int main(int argc, const char * argv[]) {
	printf("Percolation calculation \n");

	openFiles();

	int onlyOneP = 1;
	double pmin = 0.9,
		pmax = 0.99;
	double avg_biggest_cluster = 0;
	double avg_p_span = 0;

	if (onlyOneP) pmax = pmin;
	for (p = pmin; p <= pmax; p += 0.01) {
		runs = 20;
		avg_biggest_cluster = 0;
		avg_p_span = 0;
		fprintf(moviefile, "%d %d\n", N_grid, runs);

		for (t = 0; t < runs; t++)
		{
			//srand(1446742268);
			int seed = (int)time(NULL) + t * 10;
			printf("%d\n", t);
			srand(seed);

			initialize_system();
			fill_system_with_probability(p);

			clusterize_system();
			printf("Max stack calls:%d\n", maxStackCalls);
			maxStackCalls = 0;

			//biggest cluster
			avg_biggest_cluster += largest_cluster_size();
			// - probability of a spanning cluster in the system
			avg_p_span += p_spanning_cluster();

			printf("Actual clusters: %d\n", actual_cluster);

			if (actual_cluster > max_clusters) {
				max_clusters = actual_cluster;
			}
			write_moview_text();
			printf("\n");
		}
		printf("MaxCluster : %d\n", max_clusters);

		avg_biggest_cluster /= runs;
		avg_p_span /= runs;
		write_statistics(avg_biggest_cluster, avg_p_span);
	}

	closeFiles();
	//getchar();
	return 0;
}
