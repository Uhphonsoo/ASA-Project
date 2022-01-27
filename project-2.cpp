/* v24 */
/* ASA - Projeto 2 - Grupo al082 - Bruna Faria  #86578 e Joao Silva #90398  */

#include <stdio.h>
#include <vector>
#include <stack>

#define indiceX 0

using namespace std;

int nProcessos, nEntradasMatriz, nVertices, indiceY, fluxoMaximo;
bool yEstaNaArvoreDFS = true;
vector<vector<int>> capacidadeResidual;
vector<int> arvoreDFS = vector<int>();
vector<int> pai;

void readInput();
void FordFulkerson();
void DFS();
int  getCapacidadeMinimaArcosCaminho();
bool pushVerticesAdjacentes(int, stack<int>*, vector<bool>*);
void getAdjacentes(int, vector<int>*);


/* O(f*V^2) = O(V^3) */
int main() {

    scanf("%d %d", &nProcessos, &nEntradasMatriz);
    nVertices = nProcessos + 2;
    indiceY = nProcessos + 1;

    capacidadeResidual = vector<vector<int>>(nVertices, vector<int>(nVertices, 0));
    pai = vector<int>(nVertices, -1);

    /* O(E) = O(V^2) */
    readInput();

    /* O(f*V^2) = O(V^3) */
    FordFulkerson();

    printf("%d\n", fluxoMaximo);
}


/* O(E) = O(V^2) */
void readInput() {

    int custoX, custoY, pi, pj, c;

    /* O(V) */
    for (int i = 1; i <= nProcessos; i++) {
        scanf("%d %d", &custoX, &custoY);

        capacidadeResidual[0][i] = custoX;
        capacidadeResidual[i][indiceY] = custoY;
    }

    /* O(E) = O(V^2) */
    for (int i = 1; i <= nEntradasMatriz; i++) {
        scanf("%d %d %d", &pi, &pj, &c);

        capacidadeResidual[pi][pj] = c;
        capacidadeResidual[pj][pi] = c;
    }
}


/* O(f*V^2) = O(V^3) */
void FordFulkerson() {

    /* O(f*V^2) = O(V^3) */
    while (yEstaNaArvoreDFS) { /* O(f*) = O(V) */
        /* O(V^2) */
        DFS();
        /* O(V) */
        fluxoMaximo += getCapacidadeMinimaArcosCaminho();
    }
}


/* O(V^2) */
void DFS() {
    
    stack<int> Stack = stack<int>();
    vector<bool> visitadas = vector<bool>(nVertices, false);
    int vertice;

    if (nVertices <= 0) {
        return;
    }

    arvoreDFS.clear();
    yEstaNaArvoreDFS = false;
    
    Stack.push(0); 

    /* O(V^2) */
    while (!Stack.empty()) { /* O(V) */

        vertice = Stack.top();

        arvoreDFS.push_back(vertice);

        if (vertice == indiceY) {
            yEstaNaArvoreDFS = true;
        }

        Stack.pop();
        visitadas[vertice] = true;

        /* O(V) */
        pushVerticesAdjacentes(vertice, &Stack, &visitadas);

        if (Stack.empty() || vertice == indiceY) {
            break;
        }
    }
}


/* O(V) */
bool pushVerticesAdjacentes(int vertice, stack<int> *Stack, vector<bool> *visitadas) {

    vector<int> adjacentes = vector<int>();

    /* O(V) */
    getAdjacentes(vertice, &adjacentes);
    int i, size = adjacentes.size();

    if (adjacentes.size() == 0) {
        return false;
    }

    /* O(V) */
    for (i = 0; i < size; i++) {

        if (!(*visitadas)[adjacentes[i]]) {

            pai[adjacentes[i]] = vertice;
            Stack->push(adjacentes[i]);
            (*visitadas)[adjacentes[i]] = true;
        }
    }

    return true;
}


/* O(V) */
void getAdjacentes(int vertice, vector<int> *adjacentes) {

    /* O(V) */
    for (int i = 0; i < nVertices; i++) {
        if (capacidadeResidual[vertice][i] != 0) {
            (*adjacentes).push_back(i);
        }
    }
}


/* O(V) */
int getCapacidadeMinimaArcosCaminho() {

    int vertice, anterior, minimo;

    minimo = capacidadeResidual[pai[indiceY]][indiceY];

    /* O(V) */
    for (vertice = indiceY; vertice != indiceX; vertice = pai[vertice]) {
        anterior = pai[vertice];
        if (capacidadeResidual[anterior][vertice] < minimo) {
            minimo = capacidadeResidual[anterior][vertice];
        }
    }

    /* O(V) */
    for (vertice = indiceY; vertice != indiceX; vertice = pai[vertice]) {
        anterior = pai[vertice];
        capacidadeResidual[anterior][vertice] -= minimo;
        capacidadeResidual[vertice][anterior] += minimo;
    }

    return minimo;
}
