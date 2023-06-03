#include<iostream>
#include<chrono>
using namespace std;
class Timer
{
private:
    chrono::time_point<chrono::high_resolution_clock> start;
    
public:

Timer(){
    start = chrono::high_resolution_clock::now();
}
~Timer(){
    stop();
}

void stop(){
    auto end = chrono::high_resolution_clock::now();
    auto st = chrono::time_point_cast<chrono::microseconds>(start).time_since_epoch().count();
    auto ed = chrono::time_point_cast<chrono::microseconds>(end).time_since_epoch().count();
    double ms = (ed-st) * 0.001;

    cout << ms << "us"<< endl;
}
};