#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <algorithm>

// Structures

// ord(i) has the number of vertex i in the ordenation
// vert(i) has the vertex of number i in the ordenation
typedef struct order_alpha {
	std::vector<int> ord;
	std::vector<int> vert;
} order;


//Global variables
int mat[100][100];

int N_vertices;


// Declarations
bool edge(int i, int j);
bool cmp_second(std::pair<int, int> a, std::pair<int, int> b);
order max_card_search();
bool zero_fill_in(order ord);

//Definitions

// Returns true if there is an edge connecting i and j
// Returns false otherwise
bool edge(int i, int j) {
	return mat[i][j];
}

// Function to sort a vector of pairs by the second value
bool cmp_second(std::pair<int, int> a, std::pair<int, int> b) {
	return a.second < b.second;
}


// Orders the vertices of a graph by maximum cardinality search
order max_card_search() {
	// We use a vector of sets, each one having
	// the vertices with i numbered neighboors
	// sets[i].count(j) = 1 if vertex j is in set i
	// sets[i].count(j) = 0 if it is not
	std::vector<std::set<int>> sets;
	sets.resize(N_vertices);

	// size[i] is the number of numbered neighbors to vertex i
	std::vector <int> size;
	size.resize(N_vertices);

	// ord has the ordenation
	order ord;
	ord.ord.resize(N_vertices);
	ord.vert.resize(N_vertices);

	// All vertices have 0 numbered neighbors in the beginning
	for (int i = 0; i < N_vertices; i++)
		sets[0].insert(i);

	// j is the biggest with set not empty
	int j = 0;

	// For each vertex 
	for (int i = N_vertices - 1; i >= 0; i--) {

		// Gets any vertex with the biggest number of numbered neighbors
		while (sets[j].empty())
			j--;
		int v = *(sets[j].begin());
		sets[j].erase(v);

		// Vertex v receives number i
		ord.ord[v] = i;
		ord.vert[i] = v;
		size[v] = -1;

		// For each edge (v, w) such that w is unumbered
		// adds 1 to the number of numbered vertices of w
		// and places it in the next set
		for (int w = 0; w < N_vertices; w++) {
			if (edge(v, w) && size[w] >= 0) {
				sets[size[w]].erase(w);
				size[w]++;
				sets[size[w]].insert(w);
			}
		}

		// Each time a vertex receives a number, the maximum number
		// of numbered neighbors of a vertex is <= j+1
		j++;
	}
	return ord;
}

// Checks if the fill in based on a vertex ordenation of the graph
// is empty (so the graph is chordal) or not
// Returns true to empty fill in
// Returns false otherwise
bool zero_fill_in(order ord){
	// f[v] is the follower of v, i.e. the neighbor of v
	// with the smallest ordering that is bigger than v's
	std::vector<int> f;
	f.resize(N_vertices);

	// index[v] is the biggest vertex between v and
	// v's already processed neighbors 
	std::vector<int> index;
	index.resize(N_vertices);

	// We begin processing the vertex with the smallest ordering (i)
	for (int i = 0; i < N_vertices; i++) {
		int w = ord.vert[i];
		f[w] = w;
		index[w] = i;

		// For each neighbor v of w with ordering smaller than w
		// that it's already processed neighbors have ordering
		// smaller than w too: connect w to v and to it's followers
		// that respect the same properties
		for (int v = 0; v < N_vertices; v++) {
			if (edge(w, v) && ord.ord[v] < i) {
				int x = v;
				while (index[x] < i) {
					// w is an already processed neighbor of x with
					// bigger ordering, so we update index[x]
					index[x] = i;

					// We would have to connect x to w in the fill in,
					// so if it isn't connected, the fill in is not empty
					// and so we return false
					if (!edge(w, x))
						return false;

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

	// If we made it here, the fill in is empty
	return true;
}

// Input the number of vertices and adjacency matrix
// Prints if graph is chordal or not
int main() {

	std::cin >> N_vertices;

	for (int i = 0; i < N_vertices; i++)
		for (int j = 0; j < N_vertices; j++)
			std::cin >> mat[i][j];

	if (zero_fill_in(max_card_search()))
		std::cout << "The graph is chordal\n";
	else
		std::cout << "The graph is not chordal\n";

	return 0;
}