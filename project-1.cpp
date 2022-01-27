/* v17.10 (L) */
/* ASA - Projeto 1 - Bruna Faria e Joao Silva  */

#include <stdio.h>
#include <vector>
#include <queue>

using namespace std;

int nPecas, nDependencias, nIntervencoes = 1, tamanhoMaiorSequencia = 0;

vector<vector<int>> grafo;
vector<vector<int>> grafoCopia;
vector<vector<int>> grafoReverse;

vector<int> ordenacaoTopologica = vector<int>();
vector<int> ordenacaoBFS = vector<int>();
vector<int> nArcosConvergentes;
vector<int> nArcosConvergentesCopia;
vector<int> raizes = vector<int>();

void topologicalSort();
void eliminaArcosDivergentes(int, vector<vector<int>>*);

void BFS();
void pushQueuePecasAdjacentes(vector<int>, queue<int>*, vector<bool>*);

int caminhoLongo();
void reverseGraph();

int main() {

    int x, y, i;

    scanf("%d %d", &nPecas, &nDependencias);

    grafo = vector<vector<int>>(nPecas, vector<int>());
    grafoCopia = vector<vector<int>>(nPecas, vector<int>());
    grafoReverse = vector<vector<int>>(nPecas, vector<int>());

    nArcosConvergentes = vector<int>(nPecas, 0);
    nArcosConvergentesCopia = vector<int>(nPecas, 0);

    /* O(E) */
    for (i = 0; i < nDependencias; i++) {
        scanf("%d %d", &x, &y);

        grafo[x-1].push_back(y-1);
        grafoCopia[x-1].push_back(y-1);

        /* marca pecas com arcos convergentes no processo de insercao */
        nArcosConvergentes[y-1]++;
        nArcosConvergentesCopia[y-1]++;
    }

    /* O(V+E) */
    topologicalSort();
    /* O(V+E) */
    BFS();

    /* O(V+E) */
    int distMax = caminhoLongo();

    printf("%d %d\n", nIntervencoes, distMax);
}


/* O(V+E) */
void topologicalSort() {
    vector<int> pecas = vector<int>();
    vector<vector<int>> arcosEliminados = vector<vector<int>>();
    queue<int> Queue = queue<int>();
    int i, u, nArcosEliminados;

    /* O(V) */
    for (i = 0; i < nPecas; i++) {
        if (nArcosConvergentes[i] == 0) {
            Queue.push(i);
        } 
    }

    /* O(V+E) */
    while (!Queue.empty()) {
        u = Queue.front();

        /* O(E) */
        eliminaArcosDivergentes(u, &arcosEliminados);
        nArcosEliminados = arcosEliminados.size();

        /* O(E) */
        for (i = 0; i < nArcosEliminados; i++) {
            if (nArcosConvergentes[arcosEliminados[i][1]] == 0) {
                Queue.push(arcosEliminados[i][1]);
            }
        }
        
        arcosEliminados.clear();
        Queue.pop();
        ordenacaoTopologica.push_back(u);
    }
}


/* O(E) */
void eliminaArcosDivergentes(int peca, vector<vector<int>> *arcosEliminados) {
    vector<int> arcoEliminado = vector<int>(2, -1);
    int i, nArcosDivergentes = grafo[peca].size();

    /* O(E) */
    for (i = 0; i < nArcosDivergentes; i++) {
        arcoEliminado[0] = peca;
        arcoEliminado[1] = grafo[peca][i];
        
        nArcosConvergentes[grafo[peca][i]]--;
        arcosEliminados->push_back(arcoEliminado);
    }
    grafoCopia[peca].clear();
}


/* O(V+E) */
void BFS() {
    queue<int> Queue = queue<int>();
    vector<bool> visitadas = vector<bool>(nPecas, false);
    int peca, nPopped = 0, aux = 0;

    if (nPecas <= 0) {
        return;
    }

    nIntervencoes = 1;
    Queue.push(ordenacaoTopologica[0]);
    raizes.push_back(ordenacaoTopologica[0]);

    /* O(V+E) */
    while(!Queue.empty()) {
        peca = Queue.front();
        ordenacaoBFS.push_back(peca);

        Queue.pop();
        nPopped++;
        visitadas[peca] = true;

        /* O(E) */
        pushQueuePecasAdjacentes(grafo[peca], &Queue, &visitadas);

        /* 1) se nao ha pecas na queue mas ainda ha pecas por visitar */
        if (Queue.empty() && nPopped < nPecas) {
            Queue.push(ordenacaoTopologica[++aux]);
            nIntervencoes++;
            raizes.push_back(ordenacaoTopologica[aux]);
        }
        /* 2) se nao ha pecas na queue e nao ha mais pecas por visitar */
        else if (Queue.empty() && nPopped == nPecas) {
            break;
        }
    }
}


/* O(E) */
void pushQueuePecasAdjacentes(vector<int> adjacentes, queue<int> *Queue, vector<bool> *visitadas) {
    int i, size = adjacentes.size(), peca;

    /* O(E) */
    for (i = 0; i < size; i++) {
        peca = adjacentes[i];

        if (!(*visitadas)[peca]) {
            Queue->push(peca);
            (*visitadas)[peca] = true;
        }
    }
}


/* O(V+E) */
int caminhoLongo() {
    int distMax = 1, size, peca, i, k, l;
    vector<int> distancia = vector<int>(nPecas, 1);

    /* O(V+E) */
    reverseGraph();

    /* O(V+E) */
    for (i = 0; i < nPecas; i++){        
        peca = ordenacaoTopologica[i];
        size = grafoReverse[peca].size();

        /* O(E) */
        for (k = 0; k < size; k++) {
            if (distancia[peca] < distancia[grafoReverse[peca][k]] +1) {
                distancia[peca] = distancia[grafoReverse[peca][k]] +1;
            }
        }
    }

    /* O(V) */
    for (l = 0; l < nPecas; l++) {
        if (distMax < distancia[l]){
            distMax = distancia[l];
        }
    }

    return distMax;
}

/* O(V+E) */
void reverseGraph() {
    int size, i, k;

    /* O(V+E) */
    for (i = 0; i < nPecas; i++){
        size = grafo[i].size();

        /* O(E) */
        for (k = 0; k < size; k++){
            grafoReverse[grafo[i][k]].push_back(i);
        }
    }
}
