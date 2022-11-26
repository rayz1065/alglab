#include <bits/stdc++.h>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#define MAXN 105

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

int best_offers[MAXN];

void tc () {
    int n_buy, m_sites, s_states;
    cin >> n_buy >> m_sites >> s_states;
    const int s = 0;
    const int t = 1;
    const int buy_s = 2;
    const int site_s = buy_s + n_buy;
    const int state_s = site_s + m_sites;
    fill(best_offers, best_offers + m_sites, 0);
    graph g(n_buy);
    edge_adder ea(g);
    for (int i = 0; i < s_states; i++) { // fourth layer, state -> sink
        int state_lim;
        cin >> state_lim; // at most state_lim flow through this state
        ea.add_edge(i + state_s, t, state_lim, 0);
    }
    for (int i = 0; i < m_sites; i++) { // third layer, site -> state
        int state;
        cin >> state; // uses 1 of the capacity of the state
        state--;
        ea.add_edge(i + site_s, state + state_s, 1, 0);
    }
    for (int i = 0; i < n_buy; i++) { // first layer, source -> buyer
        ea.add_edge(s, buy_s + i, 1, 0);
        for (int j = 0; j < m_sites; j++) { // second layer, buyer -> site
            int offer;
            cin >> offer;
            ea.add_edge(buy_s + i, site_s + j, 1, MAXN - offer);
        }
    }
    int flow = boost::push_relabel_max_flow(g, s, t);
    boost::successive_shortest_path_nonnegative_weights(g, s, t);
    int cost = boost::find_flow_cost(g);
    cout << flow << " " << MAXN * flow - cost << "\n";
}

int main () {
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) tc();
}
