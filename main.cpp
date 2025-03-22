#include <iostream>
#include <vector>
#include <atomic>

using namespace std;

struct Fork {
private:
    std::atomic<bool> available;

public:
    Fork() : available(true) { }

    bool acquire() {
        bool expected = true;
        return available.compare_exchange_strong(expected, false);
    }

    void release() {
        available.store(true);
    }
};

struct Philosopher {
    static int next_id;
    int id;
    Fork* leftFork;
    Fork* rightFork;

    Philosopher(Fork* left, Fork* right)
        : id(next_id++), leftFork(left), rightFork(right) { }

    void eat() {
        cout << "Philosopher " << id << " starts eating." << endl;
        sleep();
        cout << "Philosopher " << id << " finishes eating." << endl;
    }

    void think() {
        cout << "Philosopher " << id << " is thinking." << endl;
        sleep();
    }

    void sleep() {
        // TODO: implement
    }
};

int Philosopher::next_id = 0;

int main() {
    cout << "Hello, World!" << endl;
    int numPhilosophers = 5;
    
    vector<Fork> forks(numPhilosophers);

    vector<Philosopher> philosophers;
    for (int i = 0; i < numPhilosophers; i++) {
        int leftForkIndex = i;
        int rightForkIndex = (i + 1) % numPhilosophers;
        Philosopher philosopher = Philosopher(&forks[leftForkIndex], &forks[rightForkIndex]);

        philosophers.push_back(philosopher);
    }

    philosophers[0].eat();
    philosophers[0].think();

    return 0;
}