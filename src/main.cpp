#include <iostream>
#include "iou.hpp"
#include "animal.hpp"
#include "arr.hpp"
#include "timer.hpp"
#include "trd.hpp"
#include "opencv_train.hpp"


using namespace std;

extern int sta;
extern void forest(int a=20);


int Node1::id;
int main(int, char**) {
    Timer timer;
    forest();
    auto[name1, name2, name3]=string_train();
    Node1 n1;
    n1.val = 10;
    Node1 n2;
    n2.prev = &n1;
    Node1::id = 50;
    cout<<n2.prev->val;
    cout<<sta<<endl;


   
    LOG("Shi");
    //multi_thread();
    trd_train();
    }


