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
        // cerr << "\t" << from << " -> " << to << " [label=" << capacity << "]\n";
    }
};

int n, m, k_knights, c_inters;
int flow_s;
int flow_inters;
int flow_tunnel;
int flow_t;

bool in_range (int x, int y) {
    return 0 <= x && x < n && 0 <= y && y < m;
}

int flow_trash;
int grid[MAXN][MAXN];
int v_indeces[MAXN][MAXN];
int next_v_idx;

pair<int, int> mov[] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

int get_idx (int i, int j) {
    if (i < 0 || j < 0) {
        return flow_trash;
    }
    if ((i == 0 || j == 0 || i == n * 2 + 2 || j == m * 2 + 2) && !(i == 0 && j == 0)) {
        return get_idx(0, 0);
    }
    int v_idx = v_indeces[i][j];
    if (!v_idx) {
        v_idx = v_indeces[i][j] = ++next_v_idx;
        next_v_idx += 1;
    }
    return v_idx;
}

void tc() {
    cin >> n >> m >> k_knights >> c_inters;
    for (int i = 0; i < 2 * n + 5; i++) {
        fill(grid[i], grid[i] + m * 2 + 5, 0);
        fill(v_indeces[i], v_indeces[i] + m * 2 + 5, 0);
    }
    next_v_idx = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            grid[i * 2 + 2][j * 2 + 2] = c_inters;
            for (int dire = 0; dire < 4; dire++) {
                grid[i * 2 + 2 + mov[dire].first][j * 2 + 2 + mov[dire].second] = 1;
            }
        }
    }
    for (int i = 0; i < n * 2 + 5; i++) {
        for (int j = 0; j < m * 2 + 5; j++) {
            // cout << get_idx(i, j) << " ";
            get_idx(i, j);
        }
        // cout << "\n";
    }
    // fill(grid[0], grid[0] + 2 * m + 5, k_knights);
    // fill(grid[2 * n + 2], grid[2 * n + 2] + 2 * m + 5, k_knights);
    // for (int j = 0; j < m * 2 + 5; j++) {
    //     grid[j][0] = grid[j][m * 2 + 2] = k_knights;
    // }
    grid[0][0] = k_knights;
    for (int i = 0; i < n * 2 + 5; i++) {
        for (int j = 0; j < m * 2 + 5; j++) {
            cerr << grid[i][j] << " ";
        }
        cerr << "\n";
    }
    Graph g((n * 2 + 5) * (m * 2 + 5) * 2);
    int flow_t = get_idx(0, 0) + 1;
    int flow_s = boost::add_vertex(g);
    flow_trash = boost::add_vertex(g);
    EdgeAdder ea(g);

    for (int i = 0; i < n * 2 + 5; i++) {
        for (int j = 0; j < m * 2 + 5; j++) {
            int v_idx = get_idx(i, j);
            if (!grid[i][j]) continue ;
            ea.add_edge(v_idx, v_idx + 1, grid[i][j]);
            for (int dire = 0; dire < 4; dire++) {
                int next_r = i + mov[dire].first, next_c = j + mov[dire].second;
                int nxt = get_idx(next_r, next_c);
                if (nxt == flow_trash) continue ;
                if (nxt != flow_t - 1 && !grid[next_r][next_c]) continue ;
                ea.add_edge(v_idx + 1, nxt, k_knights);
            }
        }
    }
    for (int i = 0; i < k_knights; i++) {
        int x, y;
        cin >> x >> y;
        // 1 knight
        int v_idx = get_idx(x * 2 + 2, y * 2 + 2);
        ea.add_edge(flow_s, v_idx, 1);
        // cout << "going to " << x << "," << y << " -> " << x * 2 + 2 << "," << y * 2 + 2 << " (" << v_idx << ")\n";
        cout.flush();
        assert(v_idx != 1);
        // cout << "from " << flow_s << " to " << v_idx << " -> 1\n";
    }

    int flow = boost::push_relabel_max_flow(g, flow_s, flow_t);
    cout << flow << "\n";


    // const auto c_map = boost::get(boost::edge_capacity, g);
    // const auto rc_map = boost::get(boost::edge_residual_capacity, g);
    // auto edge_iters = boost::edges(g);
    // for (auto edge_it = edge_iters.first; edge_it != edge_iters.second; ++edge_it) {
    //     auto edge = *edge_it;
    //     const long flow_through_edge = c_map[edge] - rc_map[edge];
    //     if (flow_through_edge > 0)
    //         std::cout << boost::source(edge, g) << " -> " << boost::target(edge, g) << ": " << flow_through_edge << "\t";
    // }
}

int main() {
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) tc();
}
