#include <stdio.h>
#include <stdlib.h>

// Print the adjacency matrix M with V vertices
void printAdjMat (int * M, int V){
	int i, j;
	
	for (i = 0; i < V; i++){
		for (j = 0; j < V; j++)
			printf ("%d ", *(M + i*V + j) );
		printf("\n");
	}
	
	printf("\n");
	
	return;	
}

// ord(i) has the number of vertex i in the ordenation
// vert(i) has the vertex of number i in the ordenation
typedef struct order_alpha{
	int* ord;
	int* vert;
} order;

// Adds vertex n_elem in the set n_set
void add_in_set_n (int n_set, int n_elem, int* set, int V){
	*(set + n_set*V + n_elem) = 1;
	return;
}

// Removes vertex n_elem of the set n_set
// Returns n_elem if vertex is in the set
// Returns -1 if n_elem is not in the set
int remove_from_set_n (int n_set, int n_elem, int* set, int V){
	if (*(set + n_set*V + n_elem) == 1){
		*(set + n_set*V + n_elem) = 0;
		return n_elem;
	} else return -1;
}

// Removes and returns the first vertex in set n_set
// Returns -1 if set is empty
int remove_any_from_set_n (int n_set, int* set, int V){
	int i;
	
	for (i = 0; i < V; i++){
		if ( *(set + n_set*V + i) == 1){
			*(set + n_set*V + i) = 0;
			return i;
		}
	}
	
	return -1;
}

// Orders V vertices of the adjacency matrix G by maximum cardinality search
order max_card_search (int* G, int V){
	int i, j, v, w;
	int* set;
	int* size;
	order ord;
	
	// Each line is a set
	// Each column is a vertex
	// set(i,j) = 1 if vertex j is in set i
	// set(i,j) = 0 if it is not
	set = calloc (V*V, sizeof(int));
	if (!set) return ord;
	
	// size(i) is the number of numbered neighbors to vertex i
	size = calloc (V, sizeof(int));
	if (!size) return ord;
	
	// ord has the ordenation
	ord.ord = calloc (V, sizeof(int));
	ord.vert = calloc (V, sizeof(int));
	
	// All vertices have 0 numbered neighbors in the beginning
	for (i=0; i < V; i++)
		add_in_set_n(0, i, set, V);
	
	// j is the biggest with set(j) not empty
	j=0;
	
	// For each vertex 
	for (i=V-1; i>=0; i--){
		
		// Gets any vertex with the biggest number of numbered neighbors
		while(1==1){
			v = remove_any_from_set_n ( j, set, V);
			if (v == -1) j--;
			else break;
		}
		
		// Vertex v is receives number i
		ord.ord[v] = i;
		ord.vert[i] = v;
		size[v] = -1;
		
		// For each edge (v, w) such that w is unumbered
		// adds 1 to the number of numbered vertices of w
		// and places it in the next set
		for (w=0; w<V; w++){
			if ( *(G + v*V + w) != 0 && size[w] >= 0){
				remove_from_set_n ( size[w], w, set, V);
				size[w]++;
				add_in_set_n (size[w], w, set, V);
			}
		}
		
		// Each time a vertex receives a number, the maximum number
		// of numbered neighbors of a vertex is <= j+1
		j++;
	}
	
	free(size);
	free(set);
	
	return ord;
}

// Creates an adjacency matrix with the fill in edges based on
// a vertex ordenation and an adjacency matrix
int* fill_in(int* G, int V, order ord){
	int v, w, x, i;
	int *f, *index, *H;
	
	// f(v) is the follower of v, i.e. the neighbor of v
	// with the smallest ordering that is bigger than v's
	f = calloc(V, sizeof(int));
	if (!f) return NULL;
	
	// index(v) is the biggest vertex between v and
	// v's already processed neighbors 
	index = calloc(V, sizeof(int));
	if (!index) return NULL;
	
	// The adjacency matrix which will receive the fill in
	H  = calloc (V*V, sizeof(int));
	if (!H) return NULL;
	
	// H receives a copy of G
	for (i = 0; i < V; i++)
		for (w = 0; w < V; w++)
			*(H + i*V + w) = *(G + i*V + w);
	
	// We begin processing the vertex with the smallest ordering (i)
	for (i = 0; i < V; i++){
		w = ord.vert[i];
		f[w] = w;
		index[w] = i;
		
		// For each neighbor v of w with ordering smaller than w
		// that it's already processed neighbors have ordering
		// smaller than w too: connect w to v and to it's followers
		// that respect the same properties
		for (v = 0; v < V; v++){
			if ( *(G + w*V + v) != 0 && ord.ord[v] < i){
				x = v;
				while(index[x] < i){
					// w is an already processed neighbor of x with
					// bigger ordering, so we update index(x)
					index[x] = i;
					
					// Connect x to w in the fill in
					*(H + w*V + x) = 1;
					*(H + x*V + w) = 1;
					
					// We repeat with the follower of x
					x = f[x];
				}
				
				// If the last follower of v has itself as a follower,
				// it shall follow w
				if (f[x] == x)
					f[x] = w;
			}
		}
	}
	
	free(f);
	free(index);
	
	return H;
}



int main(){
	int *G, *H;
	int V = 9, i;
	order ord;
	
	int ex[9][9] = 	{{0,0,1,1,0,0,0,0,0},
					{0,0,1,0,0,0,1,0,0},
					{1,1,0,0,1,1,0,0,0},
					{1,0,0,0,1,0,0,1,0},
					{0,0,1,1,0,1,0,1,0},
					{0,0,1,0,1,0,1,0,1},
					{0,1,0,0,0,1,0,0,1},
					{0,0,0,1,1,0,0,0,1},
					{0,0,0,0,0,1,1,1,0}};
	
	/*
	G = calloc(V*V, sizeof(int));
	if (!G) return 0;
	
	// Making a V-path
	for (i=0; i < V-1; i++)
		*(G + i*V + i+1) = *(G + (i+1)*V + i) = 1;
	
	// Connecting the extremes so it becomes a V-cicle
	*(G + (V-1)*V + 0) = *(G + 0*V + (V-1)) = 1;
	*/
	
	G = (int*) ex;
	
	ord = max_card_search (G, V);
	H = fill_in(G, V, ord);
	
	printf("Before fill in:\n");
	printAdjMat (G, V);
	printf("After fill in:\n");
	printAdjMat (H, V);
	
	free(ord.ord);
	free(ord.vert);
	//free(G);
	free(H);
	return 0;
}
