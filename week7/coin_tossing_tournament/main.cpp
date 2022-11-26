#include <bits/stdc++.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#define MAXN 205

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

int scores[MAXN];

void tc() {
    int n, m;
    cin >> n >> m;
    const int flow_m = 0;  // [0..m - 1]: games
    const int flow_p = flow_m + m;  // [m..m + n - 1]: players
    Graph g(flow_p + n);
    const VertexDesc flow_s = boost::add_vertex(g);
    const VertexDesc flow_t = boost::add_vertex(g);
    int vertex_type[flow_t + 1];
    fill(vertex_type, vertex_type + flow_t + 1, 0);
    EdgeAdder ea(g);
    for (int i = 0; i < m; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        // c = 1 -> a won, c = 2 -> b won, c = 0 -> ?
        ea.add_edge(flow_s, flow_m + i, 1);
        if (c == 1) {
            ea.add_edge(flow_m + i, flow_p + a, 1);
        } else if (c == 2) {
            ea.add_edge(flow_m + i, flow_p + b, 1);
        } else {
            // branch from matches[i] to a and to b, w = 1
            ea.add_edge(flow_m + i, flow_p + a, 1);
            ea.add_edge(flow_m + i, flow_p + b, 1);
        }
    }
    int tot_score = 0;
    for (int i = 0; i < n; i++) {
        cin >> scores[i];
        tot_score += scores[i];
        ea.add_edge(flow_p + i, flow_t, scores[i]);
    }
    if (tot_score > m) {
        cout << "no\n";
        return ;
    }
    int flow = boost::push_relabel_max_flow(g, flow_s, flow_t);

    cout << (flow == m ? "yes" : "no") << "\n";
}

int main() {
    int t;
    cin >> t;
    while (t--) tc();
}
