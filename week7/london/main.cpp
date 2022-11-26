#include <bits/stdc++.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#define MAXN 5005
#define MAXH 3005
#define MANX 1000005
#define ALF 26

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

string pages[2][MAXH];
int freqs[ALF][ALF];
int note_freqs[ALF];

void tc() {
    int h, w;
    string note;
    cin >> h >> w >> note;
    for (int i = 0; i < ALF; i++) fill(freqs[i], freqs[i] + ALF, 0);
    fill(note_freqs, note_freqs + ALF, 0);
    for (auto i: note) note_freqs[i - 'A'] += 1;
    for (int page_idx = 0; page_idx < 2; page_idx++) {
        for (int i = 0; i < h; i++) {
            cin >> pages[page_idx][i];
        }
    }
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            char a = pages[0][i][j];
            char b = pages[1][i][w - j - 1];
            freqs[a - 'A'][b - 'A']++;
        }
    }
    const int flow_s = 0;
    const int flow_freqs = 1;
    const int flow_note = flow_freqs + ALF * ALF;
    const int flow_t = flow_note + ALF;
    Graph g(flow_t + 1);
    EdgeAdder ea = EdgeAdder(g);

    for (int i = 0; i < ALF; i++) {
        for (int j = 0; j < ALF; j++) {
            if (freqs[i][j]) {
                ea.add_edge(flow_s, i * 26 + j + flow_freqs, freqs[i][j]);
                ea.add_edge(i * 26 + j + flow_freqs, i + flow_note, freqs[i][j]);
                ea.add_edge(i * 26 + j + flow_freqs, j + flow_note, freqs[i][j]);
            }
        }
    }
    for (int i = 0; i < ALF; i++) {
        ea.add_edge(i + flow_note, flow_t, note_freqs[i]);
    }

    int flow = boost::push_relabel_max_flow(g, flow_s, flow_t);

    cout << (flow == note.size() ? "Yes" : "No") << "\n";
}

int main() {
    int t;
    cin >> t;
    while (t--) tc();
}
