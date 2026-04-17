#include <iostream>
using namespace std;

const int SIZE = 10;
const int MAX_STATES = 10000;
const int PATH_LIMIT = 500;
struct State {
    int x, y;
    int fuel;
    char c1, c2, c3, c4; // 't' or 'f'
};

struct Node {
    State state;
    int parent;
};

Node nodes[MAX_STATES];
int nodeCount;

// Coins positions
int coinX[4] = {2, 3, 5, 8};
int coinY[4] = {2, 5, 7, 8};

// Fuel station
int fuelX = 4;
int fuelY = 9;

// goal check
bool isGoal(State s) {
    return (s.x == 1 && s.y == 1 &&
            s.c1 == 't' && s.c2 == 't' &&
            s.c3 == 't' && s.c4 == 't');
}
// Compare between states 
bool sameState(State a, State b) {
    return (a.x == b.x && a.y == b.y && a.fuel == b.fuel &&
            a.c1 == b.c1 && a.c2 == b.c2 &&
            a.c3 == b.c3 && a.c4 == b.c4);
}
// the place u vidited between all states
bool visited(State s) {
    for (int i = 0; i < nodeCount; i++) {
        if (sameState(nodes[i].state, s))
            return true;
    }
    return false;
}

void updateCoins(State &s) {
    if (s.x == coinX[0] && s.y == coinY[0]) s.c1 = 't';
    if (s.x == coinX[1] && s.y == coinY[1]) s.c2 = 't';
    if (s.x == coinX[2] && s.y == coinY[2]) s.c3 = 't';
    if (s.x == coinX[3] && s.y == coinY[3]) s.c4 = 't';
}

void refillFuel(State &s) {
    if (s.x == fuelX && s.y == fuelY)
        s.fuel = 20;
}

void printState(State s) {
    cout << "(" << s.x << "," << s.y << "," << s.fuel << ",";
    cout << s.c1 << "," << s.c2 << "," << s.c3 << "," << s.c4 << ")";
}

void printPath(int index) {
    int path[PATH_LIMIT];
    int length = 0;

    while (index != -1) {
        path[length++] = index;
        index = nodes[index].parent;
    }

    cout << "\nPath:\n";
    for (int i = length - 1; i >= 0; i--) {
        printState(nodes[path[i]].state);
        cout << endl;
    }

    cout << "\nNumber of visited states: " << nodeCount << endl;
}