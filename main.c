// C program for Dijkstra's single source shortest path
// algorithm. The program is for adjacency matrix
// representation of the graph

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>


//                  Matrix Graph
// Number of vertices in the graph
#define V 700
#define RAND_MAX 1/*implementation defined*/
int RANDINT_MAX = 2147483647;
//typedef enum (true, false) bool;

//int V = 20;
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
void dijkstra_Matrix(int graph[V][V], int src)
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

void makeGraph(int graph[V][V], int size, bool dense){

    for (int i = 0; i < size; ++i){
        for (int j = i; j < size; ++j){
            if (i == j){
                graph[i][j] = 0;
            } else {
                int random = dense ? randint(20) : randSparse(20);

                graph[i][j] = random;
                graph[j][i] = random;
            }
        }
    }
}

int randSparse(int n){
    if (rand() == 1){
        return randint(n);
    } else {
        return 0;
    }
}

int randint(int n) {
  if ((n - 1) == RANDINT_MAX) {
    return rand();
  } else {
    // Supporting larger values for n would requires an even more
    // elaborate implementation that combines multiple calls to rand()
    //assert (n <= RANDINT_MAX);

    // Chop off all of the values that would cause skew...
    int end = RANDINT_MAX / n; // truncate skew
    //assert (end > 0);
    end *= n;

    // ... and ignore results from rand() that fall above that limit.
    // (Worst case the loop condition should succeed 50% of the time,
    // so we can expect to bail out of this loop pretty quickly.)
    int r;
    while ((r = rand()) >= end);

    return r % n;
  }
}

//                  Adjacency Graph
typedef struct {
    int vertex;
    int weight;
} edge_t;

typedef struct {
    edge_t **edges;
    int edges_len;
    int edges_size;
    int dist;
    int prev;
    int visited;
} vertex_t;

typedef struct {
    vertex_t **vertices;
    int vertices_len;
    int vertices_size;
} graph_t;

typedef struct {
    int *data;
    int *prio;
    int *index;
    int len;
    int size;
} heap_t;

void add_vertex (graph_t *g, int i) {
    if (g->vertices_size < i + 1) {
        int size = g->vertices_size * 2 > i ? g->vertices_size * 2 : i + 4;
        g->vertices = realloc(g->vertices, size * sizeof (vertex_t *));
        for (int j = g->vertices_size; j < size; j++)
            g->vertices[j] = NULL;
        g->vertices_size = size;
    }
    if (!g->vertices[i]) {
        g->vertices[i] = calloc(1, sizeof (vertex_t));
        g->vertices_len++;
    }
}

void add_edge (graph_t *g, int a, int b, int w) {
    //a = a - 'a';
    //b = b - 'a';
    add_vertex(g, a);
    add_vertex(g, b);
    vertex_t *v = g->vertices[a];
    if (v->edges_len >= v->edges_size) {
        v->edges_size = v->edges_size ? v->edges_size * 2 : 4;
        v->edges = realloc(v->edges, v->edges_size * sizeof (edge_t *));
    }
    edge_t *e = calloc(1, sizeof (edge_t));
    e->vertex = b;
    e->weight = w;
    v->edges[v->edges_len++] = e;
}

heap_t *create_heap (int n) {
    heap_t *h = calloc(1, sizeof (heap_t));
    h->data = calloc(n + 1, sizeof (int));
    h->prio = calloc(n + 1, sizeof (int));
    h->index = calloc(n, sizeof (int));
    return h;
}

void push_heap (heap_t *h, int v, int p) {
    int i = h->index[v] == 0 ? ++h->len : h->index[v];
    int j = i / 2;
    while (i > 1) {
        if (h->prio[j] < p)
            break;
        h->data[i] = h->data[j];
        h->prio[i] = h->prio[j];
        h->index[h->data[i]] = i;
        i = j;
        j = j / 2;
    }
    h->data[i] = v;
    h->prio[i] = p;
    h->index[v] = i;
}

int min (heap_t *h, int i, int j, int k) {
    int m = i;
    if (j <= h->len && h->prio[j] < h->prio[m])
        m = j;
    if (k <= h->len && h->prio[k] < h->prio[m])
        m = k;
    return m;
}

int pop_heap (heap_t *h) {
    int v = h->data[1];
    int i = 1;
    while (1) {
        int j = min(h, h->len, 2 * i, 2 * i + 1);
        if (j == h->len)
            break;
        h->data[i] = h->data[j];
        h->prio[i] = h->prio[j];
        h->index[h->data[i]] = i;
        i = j;
    }
    h->data[i] = h->data[h->len];
    h->prio[i] = h->prio[h->len];
    h->index[h->data[i]] = i;
    h->len--;
    return v;
}

void dijkstra_Adj (graph_t *g, int a, int b) {
    int i, j;
    //a = a - 'a';
    //b = b - 'a';
    for (i = 0; i < g->vertices_len; i++) {
        vertex_t *v = g->vertices[i];
        v->dist = INT_MAX;
        v->prev = 0;
        v->visited = 0;
    }
    vertex_t *v = g->vertices[a];
    v->dist = 0;
    heap_t *h = create_heap(g->vertices_len);
    push_heap(h, a, v->dist);
    while (h->len) {
        i = pop_heap(h);
        if (i == b)
            break;
        v = g->vertices[i];
        v->visited = 1;
        for (j = 0; j < v->edges_len; j++) {
            edge_t *e = v->edges[j];
            vertex_t *u = g->vertices[e->vertex];
            if (!u->visited && v->dist + e->weight <= u->dist) {
                u->prev = i;
                u->dist = v->dist + e->weight;
                push_heap(h, e->vertex, u->dist);
            }
        }
    }
}

void print_path (graph_t *g, int i) {
    int n, j;
    vertex_t *v, *u;
    i = i - 'a';
    v = g->vertices[i];
    if (v->dist == INT_MAX) {
        printf("no path\n");
        return;
    }
    for (n = 1, u = v; u->dist; u = g->vertices[u->prev], n++)
        ;
    char *path = malloc(n);
    path[n - 1] = 'a' + i;
    for (j = 0, u = v; u->dist; u = g->vertices[u->prev], j++)
        path[n - j - 2] = 'a' + u->prev;
    //printf("%d %.*s\n", v->dist, n, path);
}

void translateMatrixToAdjacency(int graph[V][V], int size, graph_t *g){

    for (int i = 0; i < size; ++i){
        for (int j = 0; j < size; ++j){
            if (graph[i][j] != 0){
                add_edge(g, i, j, graph[i][j]);
            }
        }
    }
}

// driver's code
int main(){
    int graph[V][V];
    makeGraph(graph, V, true);
    timeCalcDijkstra(graph, true);
    timeCalcDijkstra(graph, false);
    makeGraph(graph, V, false);
    timeCalcDijkstra(graph, true);
    timeCalcDijkstra(graph, false);

    return 0;
}

void timeCalcDijkstra(int graph[V][V], bool matrix){
    graph_t *g = calloc(1, sizeof (graph_t));
    translateMatrixToAdjacency(graph, V, g);

    double sumTime = 0;
    for (int i = 0; i < 100; ++i){
        clock_t start = clock();

        if (matrix){
            dijkstra_Matrix(graph, 0);
        } else {
            dijkstra_Adj(g, 'a', 'e');
            print_path(g, 'e');
        }

        clock_t end = clock();
        double timeSpent = (double)(end - start)/CLOCKS_PER_SEC;
        sumTime = sumTime + timeSpent;
    }
    double timeResult = sumTime / 100;
    printf("\nAverage time spent for graph: %f", timeResult);

}

