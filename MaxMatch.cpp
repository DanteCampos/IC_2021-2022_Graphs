// C++ Implementation of Edmonds Blossoms' Maximum Matching Algorithm
// Original code by Sadanand Vishwas
// Comments and modifications by Daniel Campos da Silva

#include <iostream>
#include <cstring>
#include <vector>

using namespace std;
int M = 500; // max number of vertices

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
        this->V = V;
        pool.resize(V * V * 2);
        top = &(pool[0]);
        adj.resize(V);
        match.resize(V);
        q.resize(V);
        father.resize(V);
        base.resize(V);
        inq.resize(V);
        inq.resize(V);
        ed.assign(V, vector<bool>(V));
        
        for (int i = 0; i < V; i++)
            ed[i].resize(V);
    }

    void addEdge(int u, int v){
        if (!ed[u][v]){
            top->v = v, top->n = adj[u], adj[u] = top++;
            top->v = u, top->n = adj[v], adj[v] = top++;
            ed[u][v] = ed[v][u] = true;
        }
    }

    // Utility function for blossom contraction
    int LCA(int root, int u, int v){
        vector<bool> inp;
        inp.assign(inp.size(), -1);

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

    void mark_blossom(int lca, int u){
        while (base[u] != lca)
        {
            int v = match[u];
            inb[base[u]] = inb[base[v]] = true;
            u = father[v];
            if (base[u] != lca)
                father[u] = v;
        }
    }

    void blossom_contraction(int s, int u, int v) {
        int lca = LCA(s, u, v);
        inb.assign(inb.size(), 0);

        mark_blossom(lca, u);
        mark_blossom(lca, v);
        if (base[u] != lca)
            father[u] = v;
        if (base[v] != lca)
            father[v] = u;
        for (int u = 0; u < V; u++){
            if (inb[base[u]]) {
                base[u] = lca;
                if (!inq[u])
                    inq[q[++qt] = u] = true;
            }
        }
    }

    int find_augmenting_path(int s){
        inq.assign(inq.size(), 0);
        father.assign(father.size(), -1);

        for (int i = 0; i < V; i++)
            base[i] = i;
        inq[q[qh = qt = 0] = s] = true;
        while (qh <= qt){
            int u = q[qh++];
            for (Edge e = adj[u]; e; e = e->n){
                int v = e->v;
                if (base[u] != base[v] && match[u] != v)
                    if ((v == s) || (match[v] != -1 && father[match[v]] != -1))
                        blossom_contraction(s, u, v);
                    else if (father[v] == -1){
                        father[v] = u;
                        if (match[v] == -1)
                            return v;
                        else if (!inq[match[v]])
                            inq[q[++qt] = match[v]] = true;
                    }
            }
        }
        return -1;
    }

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

    int edmondsBlossomAlgorithm(){ // Converted recursive algorithm to iterative version for simplicity
        int match_counts = 0;
        match.assign(match.size(), -1);

        for (int u = 0; u < V; u++)
            if (match[u] == -1)
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