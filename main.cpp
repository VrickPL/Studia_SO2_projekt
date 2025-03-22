#include <iostream>

using namespace std;

struct Philosopher {
    static int next_id;
    int id;
    bool isEating;
    
    Philosopher() : id(next_id++), isEating(false) { }

    void eat() {
        isEating = true;
        cout << "Philosopher " << id << " is eating." << endl;
    }

    void think() {
        isEating = false;
        cout << "Philosopher " << id << " is thinking." << endl;
    }
};

int Philosopher::next_id = 0;

int main() {
    cout << "Hello, World!" << endl;
    int numPhilosophers = 5;

    vector<Philosopher> philosophers;
    for (int i = 0; i < numPhilosophers; i++) {
        philosophers.push_back(Philosopher());
    }

    return 0;
}