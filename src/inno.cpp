#include<iostream>
#include <algorithm> 
using namespace std;



typedef struct stone {
float weight;
};

bool comp(stone st1, stone st2){
    return st1.weight<st2.weight;
}
void stones()
{
float diff;
stone st[] = {5, 2, 1};
int n = sizeof(st)/sizeof(st[0]);
cout <<sizeof(st)<<endl;
sort(st, st+n, comp);


}

