#include <iostream>
using namespace std;

const int SIZE = 10; // حجم الـ Grid
const int MAX_STATES = 10000; //اقصى عدد حالات ممكن نخزنوها
const int PATH_LIMIT = 500; // اقصى طول للمسار 
struct State {
    int x, y; // موقع الـ AGENT
    int fuel;  // محطة البنزين 
    char c1, c2, c3, c4; // 't' or 'f'الكوينز
};
// العقدة الحالية و الاب متاعها
struct Node {
    State state;
    int parent;
};
// مصفوفة للحالات
Node nodes[MAX_STATES];
int nodeCount; // متغير لمعرفة عدد الحالات

// موقع العملات 
int coinX[4] = {1, 2, 5, 6}; 
int coinY[4] = {2, 5, 5, 9};

// موقع محطة الوقود
int fuelX = 6;
int fuelY = 6;

//اماكن الحواجز
bool isBlocked(int x, int y){
    if((x==2 && y==2) || (x==3 && y==3) || (x==4 && y==4))
        return true;
    return false;
}

// باش نتحقق من الهدف في حال وصلتله
bool isGoal(State s) {
    return (s.x == 1 && s.y == 1 &&
            s.c1 == 't' && s.c2 == 't' &&
            s.c3 == 't' && s.c4 == 't');
}
// نقارنوا بين الحالات 
bool sameState(State a, State b) {
    return (a.x == b.x && a.y == b.y && a.fuel == b.fuel &&
            a.c1 == b.c1 && a.c2 == b.c2 &&
            a.c3 == b.c3 && a.c4 == b.c4);
}
// التأكد من ان زرنا نفس الحالة او لا
bool visited(State s) {
    for (int i = 0; i < nodeCount; i++) {
        if (sameState(nodes[i].state, s))
            return true;
    }
    return false;

}
// دالة تحدث الكوينز في حالة ان تحركنا الى مكان وكان فيه احد الكوينز
void updateCoins(State &s) {
    if (s.x == coinX[0] && s.y == coinY[0]) s.c1 = 't';
    if (s.x == coinX[1] && s.y == coinY[1]) s.c2 = 't';
    if (s.x == coinX[2] && s.y == coinY[2]) s.c3 = 't';
    if (s.x == coinX[3] && s.y == coinY[3]) s.c4 = 't';
}
// دالة تعبي الوقود في حال وصلت للمحطة
void refillFuel(State &s) {
    if (s.x == fuelX && s.y == fuelY)
        s.fuel = 20;
}
// دالة طباعة الحالة
void printState(State s) {
    cout << "(" << s.x << "," << s.y << "," << s.fuel << ",";
    cout << s.c1 << "," << s.c2 << "," << s.c3 << "," << s.c4 << ")";
}
// دالة لطباعة المسار في حال وصلنا للهدف
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

    cout << "\nCost: " << length - 1 << endl;
}
// BFS
bool BFS(State start) {
    nodeCount = 0;

    int queue[MAX_STATES];
    int front = 0, rear = 0;

    nodes[nodeCount].state = start;
    nodes[nodeCount].parent = -1;
    queue[rear++] = nodeCount;
    nodeCount++;

    while (front < rear) {
        int current = queue[front++];
        State s = nodes[current].state;

        cout << "Visited: ";
        printState(s);
        cout << endl;

        if (isGoal(s)) {
            cout << "\nGoal Found Using BFS!\n";
            printPath(current);
            return true;
        }

        int dx[4] = {-1, 1, 0, 0};
        int dy[4] = {0, 0, -1, 1};

        for (int i = 0; i < 4; i++) {
            State next = s;
            next.x += dx[i];
            next.y += dy[i];
            next.fuel--;

            if (next.x < 0  || next.x >= SIZE || next.y < 0 || next.y >= SIZE)
                continue;

            if (isBlocked(next.x, next.y))
                continue;    

            if (next.fuel < 0)
                continue;

            updateCoins(next);
            refillFuel(next);

            if (!visited(next)) {
                nodes[nodeCount].state = next;
                nodes[nodeCount].parent = current;
                queue[rear++] = nodeCount;
                nodeCount++;
            }
        }
    }

    cout << "No Solution Using BFS\n";
    return false;
}
//DFS 
bool DFS(State start) {
    nodeCount = 0;

    int stack[MAX_STATES];
    int top = -1;

    nodes[nodeCount].state = start;
    nodes[nodeCount].parent = -1;
    stack[++top] = nodeCount;
    nodeCount++;

    while (top >= 0) {
        int current = stack[top--];
        State s = nodes[current].state;

        cout << "Visited: ";
        printState(s);
        cout << endl;

        if (isGoal(s)) {
            cout << "\nGoal Found Using DFS!\n";
            printPath(current);
            return true;
        }

        int dx[4] = {0, 0, 1, -1};
        int dy[4] = {1, -1, 0, 0};

        for (int i = 0; i < 4; i++) {
            State next = s;
            next.x += dx[i];
            next.y += dy[i];
            next.fuel--;

            if (next.x < 0 || next.x >= SIZE || next.y < 0 || next.y >= SIZE)
                continue;
            
            if (isBlocked(next.x, next.y))
                continue;

            if (next.fuel < 0)
                continue;

            updateCoins(next);
            refillFuel(next);

            if (!visited(next)) {
                nodes[nodeCount].state = next;
                nodes[nodeCount].parent = current;
                stack[++top] = nodeCount;
                nodeCount++;
            }
        }
    }

    cout << "No Solution Using DFS\n";
    return false;
}
// IDS
bool DLS(State s, int depth, int limit, int parentIndex) {
    cout << "Visited: ";
    printState(s);
    cout << endl;

    nodes[nodeCount].state = s;
    nodes[nodeCount].parent = parentIndex;
    int currentIndex = nodeCount;
    nodeCount++;

    if (isGoal(s)) {
        cout << "\nGoal Found Using IDS!\n";
        printPath(currentIndex);
        return true;
    }

    if (depth == limit)
        return false;

    int dx[4] = {-1, 1, 0, 0};
    int dy[4] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++) {
        State next = s;
        next.x += dx[i];
        next.y += dy[i];
        next.fuel--;

        if (next.x < 0 || next.x >= SIZE || next.y < 0 || next.y >= SIZE)
            continue;

        if (isBlocked(next.x, next.y))
            continue;

        if (next.fuel < 0)
            continue;

        updateCoins(next);
        refillFuel(next);

        bool repeated = false;
        for (int j = 0; j < nodeCount; j++) {
            if (sameState(nodes[j].state, next)) {
                repeated = true;
                break;
            }
        }

        if (!repeated) {
            if (DLS(next, depth + 1, limit, currentIndex))
                return true;
        }
    }

    return false;
}

bool IDS(State start) {
    for (int limit = 0; limit <= 50; limit++) {
        cout << "\n--- Depth Limit = " << limit << " ---\n";
        nodeCount = 0;

        if (DLS(start, 0, limit, -1))
            return true;
    }

    cout << "No Solution Using IDS\n";
    return false;
}
//MAIN
int main() {
    State start;

    cout << "Enter start x: ";
    cin >> start.x;

    cout << "Enter start y: ";
    cin >> start.y;

    cout << "Enter start fuel: ";
    cin >> start.fuel;

    start.c1 = 'f';
    start.c2 = 'f';
    start.c3 = 'f';
    start.c4 = 'f';

    updateCoins(start);
    refillFuel(start);

    int choice;

    cout << "\n1. BFS\n2. DFS\n3. IDS\n";
    cout << "Choose algorithm: ";
    cin >> choice;

    if (choice == 1)
        BFS(start);
    else if (choice == 2)
        DFS(start);
    else if (choice == 3)
        IDS(start);
    else
        cout << "Invalid Choice\n";

}
