// C program for Dijkstra's single source shortest path
// algorithm. The program is for adjacency matrix
// representation of the graph

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

// Number of vertices in the graph
#define V 9

// A utility function to find the vertex with minimum
// distance value, from the set of vertices not yet included
// in shortest path tree
int minDistance(int dist[], bool sptSet[])
{
    // Initialize min value
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

// A utility function to print the constructed distance
// array
void printSolution(int dist[])
{
    printf("Vertex \t\t Distance from Source\n");
    for (int i = 0; i < V; i++)
        printf("%d \t\t\t\t %d\n", i, dist[i]);
}

// Function that implements Dijkstra's single source
// shortest path algorithm for a graph represented using
// adjacency matrix representation
void dijkstra(int graph[V][V], int src)
{
    int dist[V]; // The output array.  dist[i] will hold the
                 // shortest
    // distance from src to i

    bool sptSet[V]; // sptSet[i] will be true if vertex i is
                    // included in shortest
    // path tree or shortest distance from src to i is
    // finalized

    // Initialize all distances as INFINITE and stpSet[] as
    // false
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, sptSet[i] = false;

    // Distance of source vertex from itself is always 0
    dist[src] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < V - 1; count++) {
        // Pick the minimum distance vertex from the set of
        // vertices not yet processed. u is always equal to
        // src in the first iteration.
        int u = minDistance(dist, sptSet);

        // Mark the picked vertex as processed
        sptSet[u] = true;

        // Update dist value of the adjacent vertices of the
        // picked vertex.
        for (int v = 0; v < V; v++)

            // Update dist[v] only if is not in sptSet,
            // there is an edge from u to v, and total
            // weight of path from src to  v through u is
            // smaller than current value of dist[v]
            if (!sptSet[v] && graph[u][v]
                && dist[u] != INT_MAX
                && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }

    // print the constructed distance array
    //printSolution(dist);
}

// driver's code
int main()
{
    /* Let us create the example graph discussed above */
    //                    a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p  q  r  s  t
    int graph[V][V] = makeGraph(20); /*{ { 0, 4, 5, 8, 2,11, 9, 8, 1,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  },
                        { 4, 0, 8, 3, 6, 6,13,11, 7 },
                        { 5, 8, 0, 7, 4, 4, 5, 4, 2 },
                        { 8, 3, 7, 0, 9,14, 7, 3, 4 },
                        { 2, 6, 4, 9, 0,10,14, 6, 7 },
                        {11, 6, 4,14,10, 0, 2, 9, 4 },
                        { 9,13, 5, 7,14, 2, 0, 1, 6 },
                        { 8,11, 8, 9,10, 7, 1, 0, 7 },
                        { 1, 7, 2, 4, 7, 4, 6, 7, 0 } };*/

    // Function call
    double sumTime = 0;
    for (int i = 0; i < 100; ++i){
        clock_t start = clock();
        dijkstra(graph, 0);
        clock_t end = clock();
        double timeSpent = (double)(end - start)/CLOCKS_PER_SEC;
        sumTime = sumTime + timeSpent;
    }
    double timeResult = sumTime / 100;
    printf("Average time spent: %f", timeResult);
    return 0;
}

int* makeGraph(int size){
    int graph[size][size];


    for (int i = 0; i < size; ++i){
        for (int j = i; j < size; ++j){
            if (i == j){
                graph[i][j] = 0;
            } else {
                int random = randint(15);
                graph[i][j] = random;
                graph[j][i] = random;
            }
        }
    }
    return graph*;
}

int randint(int n) {
  if ((n - 1) == RAND_MAX) {
    return rand();
  } else {
    // Supporting larger values for n would requires an even more
    // elaborate implementation that combines multiple calls to rand()
    assert (n <= RAND_MAX)

    // Chop off all of the values that would cause skew...
    int end = RAND_MAX / n; // truncate skew
    assert (end > 0);
    end *= n;

    // ... and ignore results from rand() that fall above that limit.
    // (Worst case the loop condition should succeed 50% of the time,
    // so we can expect to bail out of this loop pretty quickly.)
    int r;
    while ((r = rand()) >= end);

    return r % n;
  }
}
