#include <bits/stdc++.h>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#define MAXN 305
#define MAXM 10005
#define PRIO 130

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
    const int mf_s = 0;
    const int mf_t = mf_s + 1;
    const int mf_time = mf_t + 1;
    int n_stops, m_missions, l_max_train_agents;
    cin >> n_stops >> m_missions >> l_max_train_agents;
    graph g;
    edge_adder ea(g);
    ea.add_edge(mf_s, mf_time + 0, l_max_train_agents, 0);
    for (int i = 0; i < n_stops; i++) {
        // do nothing, the cost of doing nothing is PRIO
        ea.add_edge(mf_time + i, mf_time + i + 1, l_max_train_agents, PRIO);
    }
    for (int i = 0; i < m_missions; i++) {
        int x, y, prio;
        cin >> x >> y >> prio;
        // take the mission, with cost (y - x) * PRIO - prio
        // cerr << "can take mission from " << x << " to " << y << " -> " << prio << "\n";
        ea.add_edge(mf_time + x, mf_time + y, 1, (y - x) * PRIO - prio);
    }
    // report completed missions to HQ, free of charge
    ea.add_edge(mf_time + n_stops, mf_t, l_max_train_agents, 0);
    boost::successive_shortest_path_nonnegative_weights(g, mf_s, mf_t);
    int mf_cost = boost::find_flow_cost(g);
    // calculate priorities of taken missions
    cout << l_max_train_agents * n_stops * PRIO - mf_cost << "\n";
}

int main () {
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) tc();
}
