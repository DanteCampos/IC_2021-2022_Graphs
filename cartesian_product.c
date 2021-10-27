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


// Calculates the cartesian product G x H of the
// adjacency matrices G and H with VG and VH vertices, respectively
int* cartProd (int* G, int *H, int VG, int VH){
	int VP = VG*VH;
	int i, j, m, line, column;
	int *P;
	
	// Calculating the cartesian product G x H
	
	// P is a VP x VP adj. matrix with aij = (vi, uj) as its vertexes, a pair of vi in G and uj in H
	// Each element elc in P is a (a l/VH l%VH, a c/VG c%VG) edge
	P = calloc (VP*VP, sizeof(int));
	if (!P) return NULL;
	
	// For each edge (vi, vj) in G, make (aim, ajm) for m from 0 to VH:
	for (i = 0; i < VG; i++){
		for (j = 0; j < VG; j++){
			if ( *(G+i*VG+j) ){ // (vi, vj) is in EG, equal to G[i][j] != 0
				for (m = 0; m < VH; m++){
					line = i*VH + m; //aim
					column = j*VH + m; //ajm
					*(P + line*VP + column) = 1;
				}
			}
		}
	}
	
	// For each edge (ui, uj) in H, make (ami, amj) for m from 0 to VG:
	for (i = 0; i < VH; i++){
		for (j = 0; j < VH; j++){
			if ( *(H+i*VH+j)){ // (ui, uj) is in EH, equal to H[i][j] != 0
				for (m = 0; m < VG; m++){
					line = m*VH + i; //ami
					column = m*VH + j; //amj
					*(P + line*VP + column) = 1;
				}
			}
		}
	}
	
	return P;
}

int main(){
	int VH = 3, VG = 3;
	int G[3][3] = {{0,1,1},{1,0,1},{1,1,0}}, H[3][3] = {{0,1,0},{1,0,1},{0,1,0}};
	int *P;
	
	printf("G:\n");
	printAdjMat (G, VG);
	
	printf("H:\n");
	printAdjMat (H, VH);
	
	P = cartProd (G, H, VG, VH);
	
	printf("P:\n");
	printAdjMat (P, VG*VH);
	
	free (P);
	return 0;
}
