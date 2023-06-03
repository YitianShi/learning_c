#include<ostream>
#include<vector>
#include<algorithm>
#include<functional>

using namespace std;

void train_sort(){
    vector<int> values ={1,2,3,5,4};
    sort(values.begin(), values.end(), greater<int>());
    sort(values.begin(), values.end(),[](int a, int b)
    {
        if (a==1) return false;
        if(b==1) return true;
        return  a>b;
    });
}