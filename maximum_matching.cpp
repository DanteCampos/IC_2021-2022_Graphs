// C++ Implementation of Edmonds Blossoms' Maximum Matching Algorithm
// Original code by Sadanand Vishwas, available in https://iq.opengenus.org/blossom-maximum-matching-algorithm/
// Comments and modifications by Daniel Campos da Silva

#include <iostream>
#include <cstring>
#include <vector>

using namespace std;

struct StructEdge {
    int v;
    StructEdge* n;
};
typedef StructEdge* Edge;

class Blossom{
    vector<StructEdge> pool;
    Edge top;
    vector<Edge> adj;
    int V, qh, qt;
    vector<int> match, q, father, base;
    vector<bool> inq, inb;
    vector<vector<bool>> ed;

public:
    Blossom(int V){
        this->V = V; // Number of vertices
        pool.resize(V * V * 2); // Area to place edges
        top = &(pool[0]); // Most recent edge
        adj.resize(V); // adj[v] = {v, u}, start of a linked list of edges
        match.resize(V); // mate[v] = u if {u, v} is in matching
        q.resize(V); // queue
        father.resize(V); // father[v] = u if v came from u in the forest
        base.resize(V); // Base[v]=u if v is in a contracted blossom of base u
        inq.resize(V); // inq[v] = true if v is in queue
        ed.assign(V, vector<bool>(V)); // ed[i][j] = true if {i, j} is in E
    }

    void addEdge(int u, int v){
        if (!ed[u][v]){
            top->v = v, top->n = adj[u], adj[u] = top++;
            top->v = u, top->n = adj[v], adj[v] = top++;
            ed[u][v] = ed[v][u] = true;
        }
    }

    // Returns the least commom ancestor of u and v in a tree of same root
    int LCA(int root, int u, int v){
        vector<bool> inp; // inp[v] = true if v is in the path
        inp.assign(inp.size(), false);

        while (true){
            u = base[u];
            inp[u] = true;
            if (u == root)
                break;
            u = father[match[u]];
        }

        while (true){
            if (inp[v = base[v]])
                return v;
            else
                v = father[match[v]];
        }
    }
    
    // Marks vertices from u to LCA as in blossom
    void mark_blossom(int lca, int u){
        while (base[u] != lca){
            int v = match[u];
            inb[base[u]] = inb[base[v]] = true;
            u = father[v];
            if (base[u] != lca)
                father[u] = v;
        }
    }
    
    // Adjust the new graph with the contracted blossom
    void blossom_contraction(int s, int u, int v) {
        int lca = LCA(s, u, v); // least commom ancestor of u and v in tree s
        inb.assign(inb.size(), 0);

        mark_blossom(lca, u);
        mark_blossom(lca, v);
        
        // All vertices v that must be contracted have inb[v] = true
        // Adjusting bases and fathers to the new graph with the contracted blossom
        
        if (base[u] != lca)
            father[u] = v;
        if (base[v] != lca)
            father[v] = u;
        
        for (int u = 0; u < V; u++){
            if (inb[base[u]]) {
                base[u] = lca;
                if (!inq[u]) // Puts u in queue after the contraction
                    inq[q[++qt] = u] = true;
            }
        }
    }
    
    // Returns v if there is an augmanting path from s to v
    // Returns -1 if there is not
    int find_augmenting_path(int s){
        inq.assign(inq.size(), 0);
        father.assign(father.size(), -1); // In tree structure
           
        // Initialization
        for (int i = 0; i < V; i++)
            base[i] = i;
        inq[q[qh = qt = 0] = s] = true; // Puts every vertex in queue
        
        while (qh <= qt){ // While didn't pass queue's top
            int u = q[qh++]; // Actual vertex
            for (Edge e = adj[u]; e; e = e->n){
                int v = e->v; // Analysing edge {u, v}
                if (base[u] != base[v] && match[u] != v) // Not in the same blossom nor mates
                    if ((v == s) || (match[v] != -1 && father[match[v]] != -1)) // Found a cycle
                        blossom_contraction(s, u, v);
                    else if (father[v] == -1){ // v is not in the forest
                        father[v] = u; // v came from u
                        if (match[v] == -1) // v is a free vertex, we found an augmanting path
                            return v;
                        else if (!inq[match[v]]) // Puts mate of v in queue
                            inq[q[++qt] = match[v]] = true;
                    }
            }
        }
        return -1;
    }

    // Unrolls path from t to s (both free vertexes) assigning new matches
    // If there is no path, t = -1
    // Returns 1 if there is an augmanting path or else returns 0 
    int augment_path(int s, int t){
        int u = t, v, w;
        while (u != -1){
            v = father[u];
            w = match[v];
            match[v] = u;
            match[u] = v;
            u = w;
        }

        return t != -1;
    }

    // Converted recursive algorithm to iterative version for simplicity
    // Returns number of matches
    int edmondsBlossomAlgorithm(){ 
        // Every vertex begins unmatched
        int match_counts = 0;
        match.assign(match.size(), -1);

        for (int u = 0; u < V; u++)
            if (match[u] == -1) // If u is a free vertex
                match_counts += augment_path(u, find_augmenting_path(u));
        
        return match_counts;
    }

    void printMatching(){
        for (int i = 0; i < V; i++)
            if (i < match[i])
                cout << i + 1 << " " << match[i] + 1 << "\n";
    }

};

int main(){
    int graph[8][8] = {
        {0, 1, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 1, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 1, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 0, 0}
     };
    
    int V = 8;
    Blossom bm(V);

    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            if (graph[i][j] == 1)
                bm.addEdge(i, j);

    int res = bm.edmondsBlossomAlgorithm();
    if (!res)
        cout << "No Matching found\n";
    else{
        cout << "Total Matching = " << res << "\n";
        bm.printMatching();
    }

    return 0;
}
