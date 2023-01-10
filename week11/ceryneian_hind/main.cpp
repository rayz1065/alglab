#include <bits/stdc++.h>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/adjacency_list.hpp>
#define MAXN 205

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

void tc () {
    int n, m;
    cin >> n >> m;
    const int mf_s = n;
    const int mf_t = n + 1;
    graph g(mf_t + 1);
    edge_adder ea(g);
    int pos_sum = 0;
    for (int i = 0; i < n; i++) {
        int conv;
        cin >> conv;
        if (conv > 0) {
            ea.add_edge(mf_s, i, conv, 0);
            pos_sum += conv;
        }
        else ea.add_edge(i, mf_t, -conv, 0);
    }
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        ea.add_edge(a, b, INT_MAX >> 1, 0);
    }
    int mf = boost::push_relabel_max_flow(g, mf_s, mf_t);
    int sol = pos_sum - mf;
    if (sol == 0) cout << "impossible\n";
    else cout << pos_sum - mf << "\n";
}

int main () {
    int t;
    cin >> t;
    while (t--) tc();
}