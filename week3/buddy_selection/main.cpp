#include <bits/stdc++.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
#define MAXN 405
#define MAXM 105

using namespace std;

int n, m;
string carat[MAXN][MAXM];

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor                       VertexDesc;

int maximum_matching(const Graph &G) {
    int n = boost::num_vertices(G);
    std::vector<VertexDesc> mate_map(n);  // exterior property map

    boost::edmonds_maximum_cardinality_matching(G,
        boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
    int matching_size = boost::matching_size(G,
        boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
    return matching_size;
}

int count_common (int a, int b) {
    int i = 0, j = 0;
    int s = 0;
    while (i < m && j < m) {
        if (carat[a][i] < carat[b][j]) i++;
        else if (carat[a][i] > carat[b][j]) j++;
        else i++, j++, s++;
    }
    return s;
}

void solve_tc () {
    int f;
    cin >> n >> m >> f;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> carat[i][j];
        }
    }
    for (int i = 0; i < n; i++) {
        sort(carat[i], carat[i] + m);
    }

    Graph g;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (count_common(i, j) > f) {
                boost::add_edge(i, j, g);
            }
        }
    }
    int match = maximum_matching(g);
    cout << ((match == n / 2) ? "not optimal" : "optimal") << "\n";
}

int main () {
    int t;
    cin >> t;
    while (t--) solve_tc();
}
