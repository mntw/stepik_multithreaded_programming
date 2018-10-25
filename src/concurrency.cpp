#include <list>
#include <cmath>
#include <iostream>
using namespace std;

int TimeSlice = 0;
int ticks = 0;
std::list <int> query;

void scheduler_setup(int timeslice) {
    query.clear();
    TimeSlice = timeslice;
}

void new_thread(int thread_id){
    query.push_back(thread_id + 1);
}

void exit_thread() {
    if (!query.empty()) query.pop_front();
}

void block_thread() {
    if (!query.empty()) {
	    int current = query.front();
	    query.pop_front();
	    query.push_front(-1 * current);
    }	
}

void wake_thread(int thread_id) {
    if (!query.empty()) {
        int current = 0;
        for (std::list <int> :: iterator i = query.begin(); i != query.end(); i++) {
            if (*i * -1 == thread_id + 1 ) {
                current = *i * -1;
                i = query.erase(i);
                query.insert(i, current);
            }
        }
    }
}

void timer_tick() {
    int current;
    int i = 0;
    ticks += 1;
    if (ticks == TimeSlice) {
        ticks = 0;
        current = -1;
      	while (current < 0  and i <= query.size() and !query.empty()) {
      	    current = query.front();
      	    query.pop_front();
      	    query.push_back(current);
      	    i++;
      	}
    } else if (!query.empty()){
      	current = query.front();
      	while (current < 0  and i <= query.size()) {
      	    current = query.front();
      	    query.pop_front();
      	    query.push_back(current);
      	    i++;
      	}
    }
}

int current_thread() {
    int flag = 0;
    for (std::list <int> :: iterator i = query.begin(); i != query.end(); ++i) {
        if (*i > 0 ) flag = 1;
    }
    if (query.empty() or !flag) {
        return -1;
    } else {
        int ret = query.front();
        return std::abs(ret) - 1;
    }
}


int main () {
  scheduler_setup(2);
  cout << "current thread: " << current_thread() << endl;
  new_thread(1);
  new_thread(2);
  new_thread(3);

  cout << "================" << endl;
  cout << "current thread: " << current_thread() << endl;
  cout << "================" << endl;

  for (int i = 0; i < 4; i++) {
    timer_tick();
    cout << "TICK: current thread: " << current_thread() << endl;
  }

  int block = current_thread();
  cout << "================" << endl;
  cout << "block thread: " << block << endl;
  block_thread();
  cout << "================" << endl;
  cout << "current thread: " << current_thread() << endl;
  cout << "================" << endl;

  for (int i = 0; i < 4; i++) {
    timer_tick();
    cout << "TICK: current thread: " << current_thread() << endl;
  }

  cout << "================" << endl;
  wake_thread(block);
  cout << "wake thread: " << block << endl;
  cout << "================" << endl;
  cout << "current thread: " << current_thread() << endl;
  cout << "================" << endl;
  for (int i = 0; i < 4; i++) {
    timer_tick();
    cout << "TICK: current thread: " << current_thread() << endl;
  }

  cout << "================" << endl;
  cout << "exit thread: " << current_thread() << endl;
  cout << "================" << endl;
  exit_thread();
  cout << "current thread: " << current_thread() << endl;
  cout << "================" << endl;

  for (int i = 0; i < 4; i++) {
    timer_tick();
    cout << "TICK: current thread: " << current_thread() << endl;
  }

  cout << "================" << endl;
  cout << "exit thread: " << current_thread() << endl;
  cout << "================" << endl;

  exit_thread();

  cout << "current thread: " << current_thread() << endl;
  cout << "================" << endl;

  for (int i = 0; i < 4; i++){
    timer_tick();
    cout << "TICK: current thread: " << current_thread() << endl;
  }

  cout << "exit thread: " << current_thread() << endl;
  exit_thread();
  cout << "current thread: " << current_thread() << endl;

  return 0;
} 
