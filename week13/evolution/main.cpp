#include <bits/stdc++.h>
#define MAXN 50005

using namespace std;

int n_nodes;
string species[MAXN];
int specie_age[MAXN];
int ancestor[MAXN];
int anc[20][MAXN]; // binary lifting
map<string, int> species_map;

int get_ancestor (int specie_idx, int max_age) {
    // finds oldest ancestor with age at most max_age
    for (int k = 19; k >= 0; k--) {
        if (anc[k][specie_idx] == -1) continue ;
        int curr = anc[k][specie_idx];
        if (specie_age[curr] > max_age) {
            continue ;
        }
        specie_idx = curr;
    }
    return specie_idx;
}

void tc () {
    int q_queries;
    cin >> n_nodes >> q_queries;
    species_map.clear();
    fill(ancestor, ancestor + n_nodes, -1);
    for (int i = 0; i < n_nodes; i++) {
        string specie;
        int age;
        cin >> specie >> age;
        species_map[specie] = i;
        specie_age[i] = age;
        species[i] = specie;
    }
    // ancestor relations
    for (int i = 0; i < n_nodes - 1; i++) {
        string specie, parent;
        cin >> specie >> parent;
        int s_idx = species_map[specie];
        int p_idx = species_map[parent];
        ancestor[s_idx] = p_idx;
    }

    // construct binary lifting
    for (int i = 0; i < n_nodes; i++) {
        anc[0][i] = ancestor[i];
    }
    for (int k = 1; k < 20; k++) {
        for (int i = 0; i < n_nodes; i++) {
            anc[k][i] = -1;
            if (anc[k - 1][i] == -1) continue ;
            int curr = anc[k - 1][i];
            if (anc[k - 1][curr] == -1) continue ;
            anc[k][i] = anc[k - 1][curr];
        }
    }

    for (int q = 0; q < q_queries; q++) {
        string specie;
        int max_age;
        cin >> specie >> max_age;
        int specie_idx = species_map[specie];
        cout << species[get_ancestor(specie_idx, max_age)] << " \n"[q + 1 == q_queries];
    }
}

int main () {
    int t;
    cin >> t;
    while (t--) tc();
}
