#include <iostream> // std::cout
#include <fstream>  // std::ofstream
#include <unistd.h> // getpid
#include <thread>   // std::thread
#include <sstream>  //std::stringstream
#include <mutex>    // std::mutex


std::mutex mx;
void thread_f (int i, double d, const std::string s) {
    std::stringstream ss;
    ss << "Copy constructor with values: " << i << " " << d << " "  << s << "\n";
    std::cout << ss.str();
}
void thread_f_ref (int & i, double & d, const std::string & s) {
    std::stringstream ss;
    ss << "Copy constructor with values: " << i << " " << d << " "  << s << "\n";
    std::cout << ss.str();
}
void thread_under_mutex() {
    std::stringstream ss;
    ss << "Try to lock....\n";
    std::cout << ss.str();
    //mx.lock();
    std::unique_lock<std::mutex> l(mx);
    ss << "Unlocked!" << std::endl;
    std::cout << ss.str();
    //mx.unlock();

}

int main() {
    //in this case we can put values, since std::thread can use copy constructor
    std::thread t1 (thread_f, 16, 3.14, "Hello world!");


    int i = 16; double d = 3.14; std::string s = "Hello world!";
    std::thread t2 (thread_f_ref, std::ref(i), std::ref(d), s); //in this case we can use references


    std::thread t3 ([]() {printf("This is LAMBDA!\n");});

    mx.lock();
    std::thread t4 (thread_under_mutex);
    sleep(5);
    mx.unlock();

    t1.join();
    t2.detach();
    t3.join();
    t4.join();

    return 0;
}
