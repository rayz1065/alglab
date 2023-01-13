#include <bits/stdc++.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#define MAXM 5005
#define MAXN 255
#define BAD_SCHEDULE "No schedule!\n"

using namespace std;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > Graph;

class EdgeAdder {
    Graph &g;

    public:
    explicit EdgeAdder(Graph &g) : g(g) {}
    void add_edge(int from, int to, long capacity, long cost) {
        auto c_map = boost::get(boost::edge_capacity, g);
        auto r_map = boost::get(boost::edge_reverse, g);
        auto w_map = boost::get(boost::edge_weight, g);
        const auto e = boost::add_edge(from, to, g).first;
        const auto rev_e = boost::add_edge(to, from, g).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0;
        r_map[e] = rev_e;
        r_map[rev_e] = e;
        w_map[e] = cost;
        w_map[rev_e] = -cost;
    }
};

struct Match {
    int east;
    int west;
    int risk;
} matches[MAXM];

void tc () {
    int e_east_teams, w_west_teams, m_easy_matches, d_difficult_matches;
    int p_req_matches, l_min_team_matches;
    cin >> e_east_teams >> w_west_teams >> m_easy_matches >> d_difficult_matches;
    cin >> p_req_matches >> l_min_team_matches;
    int n_teams = e_east_teams + w_west_teams;
    for (int i = 0; i < m_easy_matches + d_difficult_matches; i++) {
        int east, west, risk;
        cin >> east >> west >> risk;
        matches[i] = { east, west, risk };
    }

    Graph g;
    EdgeAdder ea(g);
    const int mf_s = 0;
    const int mf_t = 1;
    const int mf_east_team = 2; // handles initial l flow
    const int mf_west_team = mf_east_team + e_east_teams;
    const int mf_east_extra_matches = mf_west_team + w_west_teams; // additional matches
    const int mf_west_extra_matches = mf_east_extra_matches + 1;
    const int mf_east_team_hard = mf_west_extra_matches + 1; // does not include initial l flow
    const int mf_west_team_hard = mf_east_team_hard + e_east_teams;
    int east_extra_matches = p_req_matches - l_min_team_matches * e_east_teams;
    int west_extra_matches = p_req_matches - l_min_team_matches * w_west_teams;

    ea.add_edge(mf_s, mf_east_extra_matches, east_extra_matches, 0); // extra matches
    // source to east team
    for (int i = 0; i < e_east_teams; i++) {
        ea.add_edge(mf_s, mf_east_team + i, l_min_team_matches, 0);
        ea.add_edge(mf_east_extra_matches, mf_east_team + i, INT_MAX >> 1, 0);
        ea.add_edge(mf_east_extra_matches, mf_east_team_hard + i, INT_MAX >> 1, 0);
    }
    // initial matches can use initial l flow
    for (int i = 0; i < m_easy_matches; i++) {
        ea.add_edge(mf_east_team + matches[i].east,
                    mf_west_team + matches[i].west,
                    1, matches[i].risk);
    }
    // hard matches cannot use initial l flow
    for (int i = m_easy_matches; i < m_easy_matches + d_difficult_matches; i++) {
        ea.add_edge(mf_east_team_hard + matches[i].east,
                    mf_west_team_hard + matches[i].west,
                    1, matches[i].risk);
    }
    // west team to well
    for (int i = 0; i < w_west_teams; i++) {
        ea.add_edge(mf_west_team + i, mf_t, l_min_team_matches, 0);
        ea.add_edge(mf_west_team + i, mf_west_extra_matches, INT_MAX >> 1, 0);
        ea.add_edge(mf_west_team_hard + i, mf_west_extra_matches, INT_MAX >> 1, 0);
    }
    ea.add_edge(mf_west_extra_matches, mf_t, west_extra_matches, 0); // extra matches


    int mf = boost::push_relabel_max_flow(g, mf_s, mf_t);
    assert (mf <= p_req_matches);
    if (mf == p_req_matches) {
        boost::successive_shortest_path_nonnegative_weights(g, mf_s, mf_t);
        int risk = boost::find_flow_cost(g);
        cout << risk << "\n";
    } else {
        cout << BAD_SCHEDULE;
    }
}

int main () {
    int t;
    cin >> t;
    while (t--) tc();
}
