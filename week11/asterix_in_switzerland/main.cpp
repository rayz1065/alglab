#include <bits/stdc++.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

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
    graph g(n);
    edge_adder ea(g);
    const int mf_s = n;
    const int mf_t = mf_s + 1;
    int pos_balance = 0;
    for (int i = 0; i < n; i++) {
        int b;
        cin >> b;
        if (b > 0) {
            ea.add_edge(mf_s, i, b, 0);
            pos_balance += b;
        }
        else if (b < 0) ea.add_edge(i, mf_t, -b, 0);
    }
    for (int i = 0; i < m; i++) {
        int a, b, debt;
        cin >> a >> b >> debt;
        ea.add_edge(a, b, debt, 0);
    }
    int mf = boost::push_relabel_max_flow(g, mf_s, mf_t);
    cout << (mf < pos_balance ? "yes" : "no") << "\n";
    // cout << mf << " / " << pos_balance << "\n";
}

int main () {
    int t;
    cin >> t;
    while (t--) tc();
}
