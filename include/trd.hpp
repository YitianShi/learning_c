#pragma once

#include<iostream>
#include<thread>
#include<future>
#include<mutex>
#include<vector>
#include<list>

using namespace std;

static bool stop = false;
static mutex mu;
static int gnum=0;
static mutex mut;


class DLock{
    public:
    mutex m1, m2, m3, m4;
    void func1()
    {
        while (true)
        {
        std::lock(m1, m2);
        lock_guard<mutex> gd1(m2, std::adopt_lock);     
        lock_guard<mutex> gd2(m1, std::adopt_lock);

        //m1.lock();
        //m2.lock();
        cout <<"Hi, 12 \n";
        //m2.unlock();
        //m1.unlock();
        }
    }
    void func2(){
        while (true)
        {
        std::lock(m1, m2);
        lock_guard<mutex> gd1(m2, std::adopt_lock);     
        lock_guard<mutex> gd2(m1, std::adopt_lock); //std::adopt_lock for already locked mutex. Note that m3 should be locked before 
        unique_lock<mutex> gd3(m3, std::try_to_lock); // unique_lock can replace lock_guard
        if (gd3.owns_lock()){;}else{;} // try_to_lock lock the mutex if the lock is not owned by others. Else the function can do something else. 
        //Note that m3 should NOT be locked before
        unique_lock<mutex> gd4(m4, std::defer_lock); 
        //don't lock m3 at first. member function includes lock(), unlock(), try_lock() return bool, 
        // release() decouple mutex with unique_lock (std::mutex* ptr = gd3.release())
        unique_lock<mutex> gd5(move(gd4)); //transfer m4 to gd5

        
        //m2.lock();
        //m1.lock();
        cout <<"Hi, 21 \n";
        //m1.unlock();
        //m2.unlock();
        }
    }
};

void dlock(){
    DLock dl;
    thread th1(&DLock::func1, ref(dl));
    thread th2(&DLock::func2, ref(dl));
    th1.join();
    th2.join();
}


class A{
public:
A(int i):a(i){cout<<"Constructor of A \n";}
A(const A& AA):a(AA.a){cout<<"Copy constructor of A \n";}
~A(){cout<<"Destructor of A"<<endl;};
list<int> vect = {1, 2, 3};

void call (int b){
    while (true){
    cout <<"Hi, I'am A with "<<a<<endl;
    //this_thread::sleep_for(1s);
    unique_lock<mutex> guard(mut);
    //mut.lock();
    //cout <<"Feed with "<<gnum<<endl;
    vect.push_back(gnum);
    //mut.unlock();
    }
    };
void back (int b){
    while (true){
    cout <<"Hello, A with "<<a<<endl;
    //this_thread::sleep_for(1s);
    
    //mut.lock();
    
    if (!vect.empty())
    {//cout <<"Drop "<<vect.front()<<endl;
    unique_lock<mutex> gd1(mut);
    if (!vect.empty()){
    vect.pop_front();
    }
    }
    //mut.unlock(); 
    }
    };


void call_condition (int b){
    while (true){
    unique_lock<mutex> gd1(mut);
    vect.push_back(gnum);
    cond.notify_one(); // use notify_all if multiple threads of back_condition are running
    }
    };
void back_condition (int b){
    while (true){
    
    unique_lock<mutex> gd1(mut);
    cond.wait(gd1, [this]{
         if (!vect.empty())return true;
         return false;
    }); 
    // if lambda function returns false, release the lock and stuck in this line until get notified by "notify_one".
    // if lambda function returns true, just return and continue
    // if get notified, try to get the lock in loop. If get the lock, continue lambda function (two arguments before)
    vect.pop_front();
    gd1.unlock(); //unlock in advance
    /* ... */
    }
    };

condition_variable cond;
int a;
};

class Work{
public:
Work(int& a): j(a){cout<<"Constructor thread id: "<<this_thread::get_id()<<endl;};
void operator()(const int& i, const string &str_buf) 
// i is fake reference, still copy in thread, after detach still exist
// std::ref give the true reference, even "const" will not be needed if you want to change its value in the thread

// char* to string
// both i and str_buf use reference, otherwise waste the copy in thread
// even str_buf (general class) has mutable member and it get changed, str_buf in main function does not change. 
{
 cout<<"Operator thread id: "<<this_thread::get_id()<<endl;
 while (true)//(!stop) 
 {
    // lock_guard<mutex> lock(mu);
    // cout<<"j is "<<j<<endl;

    //cout<<"ID"<<j<<" Number"<<i<<" working ... Gnum become "<<gnum<<"\n"; 
    this_thread::sleep_for(1s);
    }
    cout<<"finish\n"; 
}
Work(const Work& wk):j(wk.j){cout<<"copy"<<endl;}
int& j;
};

void work_fun(const int& i, char* str_buf, unique_ptr<int> ptr, A& aa) 
// i is fake reference, still copy in thread, after detach still exit
// std::ref give the true reference, even "const" will not be needed if you want to change its value in the thread
{
 while (true)//(!stop) 
 {
    mut.lock();
    gnum++;
    //cout<<"Func"<<" Number "<<i<<" working ..."<<gnum<<"\n"; 
    // aa.call(20);
    // cout<<str_buf<<" in "<<&str_buf<<endl;
    // cout<<this_thread::get_id()<<endl;
    //this_thread::sleep_for(1s);
    mut.unlock();
    }
    cout<<"finish\n"; 
}



void multi_thread(){
    dlock();
    {
    int num = 1;
    int num2 = 2;
    int id1 = 10;
    int id2 = 200;
    char strbuf[] = "Hi, Yitian";
    auto aa = A(55);
    // cout<<"j in "<<&num<<endl;
    // cout<<strbuf<<" in "<<&strbuf<<endl;

    unique_ptr<int> ptr(new int(100));

    auto work = Work(num);
    auto work2 = Work(num2);
    
    std::thread worker(work, ref(id1), string(strbuf)); // generate thread, 
    // copy work function to the thread so not removed even the main function ends, it calls the copy constructor
    // id1 here is fake reference
    // don't use implicit conversion of strbuf
    // std::ref give the true reference
    std::thread worker2(&A::call, aa, 100); // use member function to generate thread
    std::thread worker25(&A::back, aa, 100); // use member function to generate thread

    vector<thread> worker3;
    for (int i = 0; i<5;i++){
        worker3.push_back(thread(work_fun, id2, strbuf, std::move(ptr), ref(aa))); // std::move for smart pointer
        id2++;
        }

    this_thread::sleep_for(2s);

    aa.a = 40; // change aa.a in thread if ref(aa) given into thread
    num = 10; // change Work::j
    id1 = 100;


    cin.get();
    stop = true;


    if (worker.joinable())
    worker.detach();
    for(auto wk=worker3.begin();wk!=worker3.end();wk++){wk->detach();}    
    worker2.detach();

    }
    cout<<"main thread end"<<endl;
    while (true)
    {
        /* code */
    }
    
}


static bool stop2 = true;

int work2(vector<int>& collect, int j){
    while (!stop2) 
        {
        this_thread::sleep_for(1s);
        lock_guard<mutex> lock(mu);
        cout<<"ID "<<j<<" working ...\n"; 
        collect.push_back(j);
        }
        return j;
    }


void trd_train(){
    vector<future<int>> m_future;
    vector<future<void>> m_future2;

    vector<int> collect;
    for (int j=0; j<20; j++)
    m_future.push_back(async(launch::async, work2, ref(collect), j)); 
    // default async
    // launch::deferred don't create thread, runs when get()/wait() is called

    A a(5);
    m_future2.push_back(async(&A::call, &a, 21));
    m_future2.push_back(async(&A::back, &a, 22));

    stop2 = true;
    for (int j=0; j<20; j++)
    {m_future.at(j).wait();
    cout<<"Result "<<j<<": "<<m_future.at(j).get()<<endl;}
    // get will wait until the thread return
}

