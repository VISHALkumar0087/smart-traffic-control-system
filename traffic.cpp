// Smart Traffic Control System (simulation)
// Idea: sensors count vehicles in each lane. The lane with the highest
// priority gets the green light, and the green time grows with density.
// A "waiting" bonus stops a quiet lane from being ignored forever (starvation).
//
// Build: g++ -std=c++17 traffic.cpp -o traffic
// Run:   ./traffic [number_of_steps]     (on Windows: traffic.exe 12)
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>
using namespace std;

const int MIN_GREEN = 10;           // seconds, even for an almost empty lane
const int MAX_GREEN = 60;           // seconds, upper safety limit
const int SECONDS_PER_VEHICLE = 2;  // time one vehicle needs to cross
const int WAIT_BONUS = 3;           // priority added per step a lane has waited

struct Lane {
    string name;
    int vehicles;  // value read from the "sensor"
    int waited;    // steps since this lane last had green
};

int greenTime(int vehicles) {
    // More vehicles -> longer green, limited between MIN and MAX.
    return clamp(MIN_GREEN + vehicles * SECONDS_PER_VEHICLE / 2, MIN_GREEN, MAX_GREEN);
}

int priority(const Lane& l) { return l.vehicles + WAIT_BONUS * l.waited; }

int main(int argc, char** argv) {
    int steps = argc > 1 ? atoi(argv[1]) : 10;
    mt19937 rng(42);                              // fixed seed -> same output each run
    uniform_int_distribution<int> arrivals(0, 8); // new vehicles per step (sensor input)

    vector<Lane> lanes = {{"North", 8, 0}, {"South", 3, 0}, {"East", 15, 0}, {"West", 5, 0}};

    cout << left << setw(6) << "Step" << setw(8) << "Green" << setw(10) << "Vehicles"
         << setw(10) << "Time(s)" << "Passed\n";

    for (int s = 1; s <= steps; ++s) {
        for (auto& l : lanes) l.vehicles += arrivals(rng);  // read sensors

        // Choose the lane with the highest priority.
        auto best = max_element(lanes.begin(), lanes.end(),
                                [](const Lane& a, const Lane& b) { return priority(a) < priority(b); });

        int g = greenTime(best->vehicles);
        int passed = min(best->vehicles, g / SECONDS_PER_VEHICLE);

        cout << left << setw(6) << s << setw(8) << best->name << setw(10) << best->vehicles
             << setw(10) << g << passed << "\n";

        best->vehicles -= passed;
        for (auto& l : lanes) l.waited = (&l == &*best) ? 0 : l.waited + 1;
    }

    cout << "\nVehicles still waiting:\n";
    for (auto& l : lanes) cout << "  " << l.name << ": " << l.vehicles << "\n";
    return 0;
}
