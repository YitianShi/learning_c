#pragma once

#include<iostream>
#include<thread>
#include<future>
#include<mutex>
#include<vector>

using namespace std;

static bool stop = false;
static mutex mu;


void work(int j){
    while (!stop) {
    cout<<"ID"<<j<<" working ...\n"; 
    this_thread::sleep_for(1s);
    }
    }



void multi_thread(){
    std::thread worker(work, 1);
    cin.get();
    stop = true;
    worker.join();
}


void work2(vector<int>& collect, int j){
    while (!stop) 
        {
        this_thread::sleep_for(1s);
        lock_guard<mutex> lock(mu);
        cout<<"ID"<<j<<" working ...\n"; 
        collect.push_back(j);
        }
    }


void trd_train(){
    vector<future<void>> m_future;
    vector<int> collect;
    for (int j=0; j<20; j++)
    m_future.push_back(async(launch::async, work2, ref(collect), j));
}

