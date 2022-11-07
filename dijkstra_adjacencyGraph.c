#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

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
    a = a - 'a';
    b = b - 'a';
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

void dijkstra (graph_t *g, int a, int b) {
    int i, j;
    a = a - 'a';
    b = b - 'a';
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

int main () {

    graph_t *g = calloc(1, sizeof (graph_t));
    add_edge(g, 'a', 'b', 4);
    add_edge(g, 'a', 'c', 5);
    add_edge(g, 'a', 'd', 8);
    add_edge(g, 'a', 'e', 2);
    add_edge(g, 'a', 'f', 11);
    add_edge(g, 'a', 'g', 9);
    add_edge(g, 'a', 'h', 8);
    add_edge(g, 'a', 'i', 4);

    add_edge(g, 'b', 'a', 4);
    add_edge(g, 'b', 'c', 8);
    add_edge(g, 'b', 'd', 3);
    add_edge(g, 'b', 'e', 6);
    add_edge(g, 'b', 'f', 6);
    add_edge(g, 'b', 'g', 13);
    add_edge(g, 'b', 'h', 11);
    add_edge(g, 'b', 'i', 7);

    add_edge(g, 'c', 'a', 5);
    add_edge(g, 'c', 'b', 8);
    add_edge(g, 'c', 'd', 7);
    add_edge(g, 'c', 'e', 4);
    add_edge(g, 'c', 'f', 4);
    add_edge(g, 'c', 'g', 5);
    add_edge(g, 'c', 'h', 8);
    add_edge(g, 'c', 'i', 2);

    add_edge(g, 'd', 'a', 8);
    add_edge(g, 'd', 'b', 3);
    add_edge(g, 'd', 'c', 7);
    add_edge(g, 'd', 'e', 9);
    add_edge(g, 'd', 'f', 14);
    add_edge(g, 'd', 'g', 7);
    add_edge(g, 'd', 'h', 9);
    add_edge(g, 'd', 'i', 4);

    add_edge(g, 'e', 'a', 2);
    add_edge(g, 'e', 'b', 6);
    add_edge(g, 'e', 'c', 4);
    add_edge(g, 'e', 'd', 9);
    add_edge(g, 'e', 'f', 10);
    add_edge(g, 'e', 'g', 14);
    add_edge(g, 'e', 'h', 10);
    add_edge(g, 'e', 'i', 7);

    add_edge(g, 'f', 'a', 11);
    add_edge(g, 'f', 'b', 6);
    add_edge(g, 'f', 'c', 4);
    add_edge(g, 'f', 'd', 14);
    add_edge(g, 'f', 'e', 10);
    add_edge(g, 'f', 'g', 2);
    add_edge(g, 'f', 'h', 7);
    add_edge(g, 'f', 'i', 4);

    add_edge(g, 'g', 'a', 9);
    add_edge(g, 'g', 'b', 13);
    add_edge(g, 'g', 'c', 5);
    add_edge(g, 'g', 'd', 7);
    add_edge(g, 'g', 'e', 14);
    add_edge(g, 'g', 'f', 2);
    add_edge(g, 'g', 'h', 1);
    add_edge(g, 'g', 'i', 6);

    add_edge(g, 'h', 'a', 8);
    add_edge(g, 'h', 'b', 11);
    add_edge(g, 'h', 'c', 4);
    add_edge(g, 'h', 'd', 3);
    add_edge(g, 'h', 'e', 6);
    add_edge(g, 'h', 'f', 9);
    add_edge(g, 'h', 'g', 1);
    add_edge(g, 'h', 'i', 7);

    add_edge(g, 'i', 'a', 1);
    add_edge(g, 'i', 'b', 7);
    add_edge(g, 'i', 'c', 2);
    add_edge(g, 'i', 'd', 4);
    add_edge(g, 'i', 'e', 7);
    add_edge(g, 'i', 'f', 4);
    add_edge(g, 'i', 'g', 6);
    add_edge(g, 'i', 'h', 7);

    double sum = 0;
    for(int i = 0; i<100; i++){
        clock_t  start = clock();
        dijkstra(g, 'a', 'e');
        print_path(g, 'e');
        clock_t  end = clock();
        double timeSpent = (double)(end - start)/CLOCKS_PER_SEC;
        sum += timeSpent;
    }

    double avgTime = sum/100;
    printf("time spent: %f \n",avgTime);
    return 0;
}
