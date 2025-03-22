#include <iostream>
#include <vector>
#include <atomic>
#include <thread>

using namespace std;

class Lock {
    atomic_flag flag = ATOMIC_FLAG_INIT;

public:
    void lock() {
        while(flag.test_and_set(memory_order_acquire)) {
            this_thread::sleep_for(1ms);
        }
    }

    void unlock() {
        flag.clear(memory_order_release);
    }
};

Lock coutLock;

struct Fork {
private:
    atomic<bool> available;

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

    void dine() {
        for (int i = 0; i < 3; i++) {
            think();

            while (true) {
                {
                    coutLock.lock();
                    cout << "Philosopher " << id << " is trying to acquire forks." << endl;
                    coutLock.unlock();
                }

                //avoiding deadlock by assigning the forks in a different order for each philosopher
                bool gotLeft = false, gotRight = false;
                if (id % 2 == 0) {
                    gotLeft = leftFork->acquire();
                    gotRight = rightFork->acquire();
                } else {
                    gotRight = rightFork->acquire();
                    gotLeft = leftFork->acquire();
                }

                if (gotLeft && gotRight) {
                    eat();
                    leftFork->release();
                    rightFork->release();
                    break;
                } else {
                    if (gotLeft) {
                        leftFork->release();
                    }
                    if (gotRight) {
                        rightFork->release();
                    }
                    this_thread::sleep_for(500ms);
                }
            }
        }
    }

    void eat() {
        {
            coutLock.lock();
            cout << "Philosopher " << id << " starts eating." << endl;
            coutLock.unlock();
        }
        sleep();
        {
            coutLock.lock();
            cout << "Philosopher " << id << " finishes eating." << endl;
            coutLock.unlock();
        }
    }

    void think() {
        {
            coutLock.lock();
            cout << "Philosopher " << id << " is thinking." << endl;
            coutLock.unlock();
        }
        sleep();
    }

    void sleep() {
        this_thread::sleep_for(2000ms);
    }
};

int Philosopher::next_id = 0;

int main() {
    int numPhilosophers = 3;
    
    vector<Fork> forks(numPhilosophers);

    vector<Philosopher> philosophers;
    for (int i = 0; i < numPhilosophers; i++) {
        int leftForkIndex = i;
        int rightForkIndex = (i + 1) % numPhilosophers;
        Philosopher philosopher = Philosopher(&forks[leftForkIndex], &forks[rightForkIndex]);

        philosophers.push_back(philosopher);
    }

    vector<thread> threads;
    for (int i = 0; i < numPhilosophers; i++) {
        threads.push_back(thread(&Philosopher::dine, &philosophers[i]));
    }

    for (thread &t : threads) {
        t.join();
    }

    return 0;
}