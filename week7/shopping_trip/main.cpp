#include <bits/stdc++.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#define MAXN 5005

using namespace std;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor>>>> Graph;

typedef Traits::vertex_descriptor VertexDesc;
typedef Traits::edge_descriptor EdgeDesc;

class EdgeAdder {
  Graph &G;

 public:
    explicit EdgeAdder(Graph &G) : G(G) {}

    void add_edge(int from, int to, long capacity) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        const auto e = boost::add_edge(from, to, G).first;
        const auto rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
    }
};

void tc() {
    int n, m, s;
    cin >> n >> m >> s;
    Graph g(n + 1);
    EdgeAdder ea = EdgeAdder(g);
    int flow_s = 0;
    int flow_t = n;
    for (int i = 0; i < s; i++) {
        int store;
        cin >> store;
        ea.add_edge(store, flow_t, 1);
    }
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        ea.add_edge(a, b, 1);
        ea.add_edge(b, a, 1);
    }
    int flow = boost::push_relabel_max_flow(g, flow_s, flow_t);

    cout << (flow == s ? "yes" : "no") << "\n";
}

int main() {
    int t;
    cin >> t;
    while (t--) tc();
}
