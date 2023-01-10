#include <bits/stdc++.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#define MAXN 505
#define MAXP 5005

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
    const int mf_source = 0;
    const int mf_target = 1;
    const int mf_boats = 2;
    const int mf_sailors = mf_boats + MAXN;
    int b_boats, s_sailors;
    int m_branches;
    cin >> b_boats >> s_sailors >> m_branches;
    graph g;
    edge_adder ea(g);
    for (int i = 0; i < b_boats; i++) { // first layer, source -> boat
        ea.add_edge(mf_source, i + mf_boats, 1, 0);
        ea.add_edge(i + mf_boats, mf_target, 1, MAXP); // skip it
    }
    for (int i = 0; i < m_branches; i++) { // second layer, boat -> sailor
        int boat, sail, w;
        cin >> boat >> sail >> w;
        ea.add_edge(boat + mf_boats, sail + mf_sailors, 1, MAXP - w);
    }
    for (int i = 0; i < s_sailors; i++) { // third layer, sailor -> well
        ea.add_edge(mf_sailors + i, mf_target, 1, 0);
    }

    boost::successive_shortest_path_nonnegative_weights(g, mf_source, mf_target);
    int sol = b_boats * MAXP - boost::find_flow_cost(g);
    cout << sol << "\n";
}

int main () {
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) tc();
}
