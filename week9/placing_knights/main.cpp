#include <bits/stdc++.h>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/adjacency_list.hpp>
#define MAXN 70
#define MOV 8

using namespace std;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor   edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it; // Iterator

class edge_adder {
    graph &G;

    public:
    explicit edge_adder(graph &G) : G(G) {}
    void add_edge(int from, int to, long capacity, long cost) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        auto w_map = boost::get(boost::edge_weight, G); // new!
        const edge_desc e = boost::add_edge(from, to, G).first;
        const edge_desc rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
        w_map[e] = cost;   // new assign cost
        w_map[rev_e] = -cost;   // new negative cost
    }
};

int n;
bool grid[MAXN][MAXN];
int movr[] = { -1, -1, +1, +1, -2, -2, +2, +2, };
int movc[] = { -2, +2, -2, +2, -1, +1, -1, +1, };

int cell_id (int r, int c) {
    return r * n + c;
}

bool in_range (int r, int c) {
    return 0 <= r && r < n && 0 <= c && c < n;
}

void tc () {
    cin >> n;
    graph g(n * n);
    int s = boost::add_vertex(g);
    int t = boost::add_vertex(g);
    edge_adder ea(g);
    auto c_map = boost::get(boost::edge_capacity, g);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> grid[i][j];
        }
    }
    int lft_s = 0, rgt_s = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i % 2; j < n; j += 2) {
            if (!grid[i][j]) continue ;
            lft_s++;
            int cell = cell_id(i, j);
            ea.add_edge(s, cell, 1, 1);
            for (int mov = 0; mov < MOV; mov++) {
                int i2 = i + movr[mov], j2 = j + movc[mov];
                if (!in_range(i2, j2) || !grid[i2][j2]) continue ;
                int cell2 = cell_id(i2, j2);
                ea.add_edge(cell, cell2, 1, 1);
            }
        }
        for (int j = (i + 1) % 2; j < n; j += 2) {
            if (!grid[i][j]) continue ;
            int cell = cell_id(i, j);
            ea.add_edge(cell, t, 1, 1);
            rgt_s++;
        }
    }
    int mcbm = boost::push_relabel_max_flow(g, s, t);
    cout << lft_s + rgt_s - mcbm << "\n";
}

int main () {
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) tc();
}
