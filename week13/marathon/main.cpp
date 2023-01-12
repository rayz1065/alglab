#include <bits/stdc++.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#define MAXN 1005

using namespace std;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor>>>> Graph;

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
        c_map[rev_e] = 0;
        r_map[e] = rev_e;
        r_map[rev_e] = e;
    }
};

int n_nodes;
struct Branch {
    int to;
    int width;
    int length;
};
vector<Branch> graph[MAXN];
int dist_from_start[MAXN];
int dist_to_end[MAXN];

void dijkstra (int node, int *dist) {
    set<pair<int, int>> ff;
    fill(dist, dist + n_nodes, INT_MAX >> 1);
    ff.insert({ dist[node] = 0, node });
    while (!ff.empty()) {
        node = ff.begin()->second;
        ff.erase(ff.begin());
        for (auto branch: graph[node]) {
            int new_dist = dist[node] + branch.length;
            if (new_dist < dist[branch.to]) {
                ff.insert({ dist[branch.to] = new_dist, branch.to });
            }
        }
    }
}

void tc () {
    int m_edges;
    int s_start, f_end;
    cin >> n_nodes >> m_edges >> s_start >> f_end;
    for (int i = 0; i < n_nodes; i++) {
        graph[i].clear();
    }
    for (int i = 0; i < m_edges; i++) {
        int a, b, width, length;
        cin >> a >> b >> width >> length;
        graph[a].push_back({ b, width, length });
        graph[b].push_back({ a, width, length });
    }
    dijkstra(s_start, dist_from_start);
    dijkstra(f_end, dist_to_end);

    // removes every branch not in shortest path start -> end
    Graph g;
    EdgeAdder ea(g);
    int target_dist = dist_from_start[f_end];

    for (int i = 0; i < n_nodes; i++) {
        for (auto branch: graph[i]) {
            int dist = dist_from_start[i] + branch.length + dist_to_end[branch.to];
            if (dist != target_dist) {
                continue ;
            }
            // of desired length
            ea.add_edge(i, branch.to, branch.width);
        }
    }
    int mf = boost::push_relabel_max_flow(g, s_start, f_end);
    cout << mf << "\n";
}

int main () {
    int t;
    cin >> t;
    while (t--) tc();
}
