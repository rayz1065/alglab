#include <bits/stdc++.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#define MAXN 1005
#define MAXA 105
#define MAXS 105

using namespace std;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > Graph; // new! weightmap corresponds to costs

class EdgeAdder {
    Graph &g;

    public:
    explicit EdgeAdder(Graph &g) : g(g) {}
    void add_edge(int from, int to, long capacity, long cost) {
        auto c_map = boost::get(boost::edge_capacity, g);
        auto r_map = boost::get(boost::edge_reverse, g);
        auto w_map = boost::get(boost::edge_weight, g); // new!
        const auto e = boost::add_edge(from, to, g).first;
        const auto rev_e = boost::add_edge(to, from, g).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
        w_map[e] = cost;   // new assign cost
        w_map[rev_e] = -cost;   // new negative cost
    }
};

int n_nodes, a_agents, s_shelters, c_capacity, d_enter_time;
vector<pair<int, int>> graph[MAXN];
int agents_loc[MAXS];
int shelters_loc[MAXS];
vector<int> agents[MAXN];
vector<int> shelters[MAXN];

int dist[MAXN];
int assp[MAXN][MAXN];

void sssp (int node) {
    fill(dist, dist + n_nodes, INT_MAX >> 1);
    set<pair<int, int>> ff;
    ff.insert({ dist[node] = 0, node });
    while (!ff.empty()) {
        node = ff.begin()->second;
        ff.erase(ff.begin());
        for (auto b: graph[node]) {
            int new_dist = dist[node] + b.second;
            if (dist[b.first] > new_dist) {
                ff.insert({ dist[b.first] = new_dist, b.first });
            }
        }
    }
}

bool is_feasible (int max_time) {
    Graph g;
    EdgeAdder ea(g);
    const int mf_s = 0;
    const int mf_t = 1;
    const int mf_agent = 2;
    const int mf_shelter = mf_agent + a_agents;
    const int mf_last_min = mf_shelter + s_shelters;
    for (int i = 0; i < a_agents; i++) {
        // source to agent
        ea.add_edge(mf_s, mf_agent + i, 1, 0);
    }
    for (int i = 0; i < a_agents; i++) {
        for (int j = 0; j < s_shelters; j++) {
            // agent to shelter
            int enter_time = assp[agents_loc[i]][shelters_loc[j]] + d_enter_time;
            if (enter_time + d_enter_time <= max_time) {
                ea.add_edge(mf_agent + i, mf_shelter + j, 1, 0);
            } else if (enter_time <= max_time) {
                // enters last minute
                ea.add_edge(mf_agent + i, mf_last_min + j, 1, 0);
            }
        }
    }
    for (int j = 0; j < s_shelters; j++) {
        // last minute to shelter
        ea.add_edge(mf_last_min + j, mf_shelter + j, 1, 0);
        // shelter to well
        ea.add_edge(mf_shelter + j, mf_t, c_capacity, 0);
    }
    int mf = boost::push_relabel_max_flow(g, mf_s, mf_t);
    return mf == a_agents;
}

int solve_bs () {
    // binary search
    int i = 0, j = 1 << 30, s = -1;
    while (i <= j) {
        int m = (i + j) / 2;
        if (is_feasible(m)) {
            s = m;
            j = m - 1;
        } else {
            i = m + 1;
        }
    }
    return s;
}

void tc () {
    int m_edges;
    cin >> n_nodes >> m_edges >> a_agents
        >> s_shelters >> c_capacity >> d_enter_time;
    for (int i = 0; i < n_nodes; i++) {
        agents[i].clear();
        shelters[i].clear();
        graph[i].clear();
    }
    for (int i = 0; i < m_edges; i++) {
        char edge_type;
        int x, y, z;
        cin >> edge_type >> x >> y >> z;
        graph[x].push_back({ y, z });
        if (edge_type == 'L') {
            graph[y].push_back({ x, z });
        }
    }
    for (int i = 0; i < a_agents; i++) {
        cin >> agents_loc[i];
        agents[agents_loc[i]].push_back(i);
    }
    for (int i = 0; i < s_shelters; i++) {
        cin >> shelters_loc[i];
        shelters[shelters_loc[i]].push_back(i);
    }
    for (int i = 0; i < n_nodes; i++) {
        if (agents[i].size() > 0) {
            sssp(i);
            for (int j = 0; j < n_nodes; j++) {
                assp[i][j] = dist[j];
            }
        }
    }
    cout << solve_bs() << "\n";
}

int main () {
    int t;
    cin >> t;
    while (t--) tc();
}
