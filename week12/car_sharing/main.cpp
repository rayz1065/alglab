#include <bits/stdc++.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#define MAXN 10005
#define MAXS 11
#define MAXT 100005

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

int n_bookings, s_stations;
int stations[MAXS];
struct Booking {
    int start;
    int target;
    int departure;
    int arrival;
    int profit;
} bookings[MAXN];
int time_map[MAXT]; // for index compression
set<int> departures_at_station[MAXS];

int time_idx (int time, int station) {
    // an index for the station at the desired time
    return time * s_stations + station;
}

int index_compress () {
    set<int> times;
    times.insert(MAXT - 1);
    for (int i = 0; i < n_bookings; i++) {
        times.insert(bookings[i].departure);
    }
    // align arrival times with next departure
    for (int i = 0; i < n_bookings; i++) {
        bookings[i].arrival = *times.lower_bound(bookings[i].arrival);
    }
    // assign the new indices
    int new_idx = 0;
    for (auto t: times) {
        time_map[t] = new_idx++;
    }
    for (int i = 0; i < n_bookings; i++) {
        bookings[i].departure = time_map[bookings[i].departure];
        bookings[i].arrival = time_map[bookings[i].arrival];
    }
    return new_idx;
}

int next_departure (int station, int time) {
    return *departures_at_station[station].lower_bound(time);
}

void tc () {
    cin >> n_bookings >> s_stations;
    for (int i = 0; i < s_stations; i++) {
        cin >> stations[i];
    }
    for (int i = 0; i < n_bookings; i++) {
        int s, t, d, a, p;
        cin >> s >> t >> d >> a >> p;
        s--, t--;
        bookings[i] = { s, t, d, a, p };
    }
    int maxt = index_compress();
    for (int i = 0; i < s_stations; i++) {
        departures_at_station[i].clear();
        departures_at_station[i].insert(maxt - 1);
    }
    for (int i = 0; i < n_bookings; i++) {
        departures_at_station[bookings[i].start].insert(bookings[i].departure);
    }

    // constructs the graph
    Graph g;
    EdgeAdder ea(g);
    const int mf_s = 0;
    const int mf_t = 1;
    const int mf_book = 2;
    for (int i = 0; i < s_stations; i++) {
        // starting positions
        int arrival = next_departure(i, 0);
        ea.add_edge(mf_s, mf_book + time_idx(arrival, i), stations[i], arrival * 100);
    }
    for (int i = 0; i < n_bookings; i++) {
        // from current station to new station
        int arrival = next_departure(bookings[i].target, bookings[i].arrival);
        int start_idx = mf_book + time_idx(bookings[i].departure, bookings[i].start);
        int end_idx = mf_book + time_idx(arrival, bookings[i].target);
        ea.add_edge(start_idx, end_idx, 1, (arrival - bookings[i].departure) * 100 - bookings[i].profit);
    }
    for (int t = 0; t < maxt - 1; t++) {
        for (int j = 0; j < s_stations; j++) {
            if (next_departure(j, t) != t) {
                // no car starts at this station at the current time
                continue ;
            }
            // move forward in time, skipping the booking
            int start_idx = mf_book + time_idx(t, j);
            int arrival = next_departure(j, t + 1); // go to next
            int end_idx = mf_book + time_idx(arrival, j);
            ea.add_edge(start_idx, end_idx, INT_MAX >> 1, (arrival - t) * 100);
        }
    }
    for (int j = 0; j < s_stations; j++) {
        // reach the well
        int start_idx = mf_book + time_idx((maxt - 1), j);
        ea.add_edge(start_idx, mf_t, INT_MAX >> 1, 0);
    }
    int total_cars = 0; // = max flow
    for (int i = 0; i < s_stations; i++) {
        total_cars += stations[i];
    }
    boost::successive_shortest_path_nonnegative_weights(g, mf_s, mf_t);
    int cost = boost::find_flow_cost(g);
    cout << ((maxt - 1) * 100) * total_cars - cost << "\n";
}

int main () {
    cin.tie(NULL);
    cout.tie(NULL);
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) tc();
}
