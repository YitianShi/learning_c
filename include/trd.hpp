#include<iostream>
#include<thread>
using namespace std;
static bool stop = false;
void work(){while (!stop) {
    cout<<"Working ...\n"; 
    this_thread::sleep_for(1s);}}

void multi_thread(){
    std::thread worker(work);
    cin.get();
    stop = true;
    worker.join();
}

