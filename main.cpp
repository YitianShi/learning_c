#include <iostream>
#include "iou.hpp"
#include "1.hpp"
#include "arr.hpp"
#include "opencv2/opencv.hpp"



using namespace std;

extern int sta;
extern void forest(int a=20);


int Node::id;
int main(int, char**) {
   
    forest();
    Node n1;
    n1.val = 10;
    Node n2;
    n2.prev = &n1;
    Node::id = 50;
    cout<<n2.prev->val;
    cout<<sta<<endl;



    {
        auto f_s = make_shared<fox<int>>(10);
        weak_ptr<fox<int>> f_w;
        {
            auto f_s2 = f_s;
            f_w = f_s;
            auto f_u=make_unique<animal<char>>(1,2,3,'c');
            // out of this scope, f_w and f_u are deleted
            f_u->hey();
        }
        // out of this scope, f_s is deleted
    }
    LOG("Shi");

    }


