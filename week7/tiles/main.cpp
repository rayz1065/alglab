#include <bits/stdc++.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#define MAXN 55

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
        // cerr << from << " -> " << to << "\n";
    }
};

bool tiles[MAXN][MAXN];
int tiles_freqs[MAXN][MAXN];
int movr[4] = {-1, 0, 1, 0};
int movc[4] = {0, 1, 0, -1};
int n, m;

bool in_range (int i, int j) {
    return 0 <= i && i < n && 0 <= j && j < m;
}

void tc() {
    cin >> m >> n;
    int tot_cells = 0;
    for (int i = 0; i < n; i++) {
        string line;
        cin >> line;
        for (int j = 0; j < m; j++) {
            tiles[i][j] = line[j] == '.';
            tot_cells += tiles[i][j];
            tiles_freqs[i][j] = 0;
        }
    }
    const int flow_s = 0;
    const int flow_t = flow_s + 1;
    const int flow_tiles = flow_t + 1;
    const int flow_event = flow_tiles + n * m;
    Graph g(flow_event + n * m);
    EdgeAdder ea = EdgeAdder(g);

    int exp_flow = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i % 2; j < m; j += 2) {
            if (!tiles[i][j]) continue ;
            ea.add_edge(flow_s, flow_event + i * m + j, 1);
            int f = 0;
            for (int dire = 0; dire < 4; dire++) {
                int newi = i + movr[dire], newj = j + movc[dire];
                if (!in_range(newi, newj) || !tiles[newi][newj]) continue ;
                ea.add_edge(flow_event + i * m + j, flow_tiles + newi * m + newj, 1);
                tiles_freqs[newi][newj]++;
                f++;
            }
            // if (!f) exp_flow = INT_MAX >> 1; // impossible
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = (i + 1) % 2; j < m; j += 2) {
            if (tiles[i][j]) {
                ea.add_edge(flow_tiles + i * m + j, flow_t, 1);
                exp_flow++;
            }
        }
    }
    if (tot_cells != exp_flow * 2) {
        cout << "no\n";
        return ;
    }

    int flow = boost::push_relabel_max_flow(g, flow_s, flow_t);

    cout << (flow == exp_flow ? "yes" : "no") << "\n";
}

int main() {
    int t;
    cin >> t;
    while (t--) tc();
}
