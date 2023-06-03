#include <iostream>
#include <fstream>

using namespace std;
#include<array>
#include<vector>
#include<tuple>
#include<cmath>
#include<utility>
#include<functional>
#include<optional>
#include<unordered_map>


#define DEBUG 0

#if DEBUG==0
#define LOG(x)
#else
#define LOG(x) cout << "Hi, Debuger "<<x<<endl
#endif


typedef struct Vertex{
    union {struct {float a, b, c;};
    struct {float x, y, z;};};
    Vertex(float x, float y, float z):
    x(x), y(y), z(z)
    {}

    Vertex(const Vertex& v):
    x(v.x), y(v.y), z(v.z)
    {cout<<"Copy"<<endl;}

    float* give_a()
    {
        cout << "x=a="<<a<<endl;
        cout << "y=b="<<b<<endl;
        cout << "z=c="<<c<<endl;
        return &a;
    }
   
} V;

ostream& operator<<(ostream& stream, const Vertex& vertex)
{
    stream<<"x: " << vertex.x<<"y: " <<vertex.y<<"z: " <<vertex.z<<endl;
    return stream;
}

auto punning_train(){
    V v1={2, 3, 19};

    int* b=(int*)&v1;

    float c = *(float*)((char*)&v1 + 4); // print the b's value
    
    cout << c << endl;
    
    auto d = v1.give_a();

    d[0] = 33;
    d[1] = 44;
    d[2] = 50;
    
    }

auto arr_train(){
    int a[7];
    int* b= new int[7];
    *(b+2) = 10;
    *(int*)((char*)b+sizeof(int)/sizeof(char)*2) = 20;
    delete[] b;
    array<int, 20> c;

    c[0] = 2;


}

auto string_train(){
      // String
    char* name = "Shi";
    string name2="Shi";
    int name_size = name2.size();
    name2 += "Hello"; 
    string name3 = "S"s + "hi";

    return tuple<string, string, string>(name, name2, name3);
}

auto lambda_train(string &name2){
    //lambda
    auto func = [=](int a) mutable {name2 += " yitian";}; // without mutable illegal
    func(5); // name2 still not changed
}

auto vector_train(){

    //vector
    
    vector<V> vt;
    vt.reserve(9);

    vt.emplace_back(2, 3, 4);
    vt.push_back({2, 3, 4});
    vt.push_back({2, 3, 4});
    vt.erase(vt.begin()+1); //remove second element

    for (V& v: vt)
    cout<<v<<endl; 

    for (auto k = vt.begin(); k!= vt.end(); k++)
    cout<<*k<<endl; 

}

optional<string> data(int a){
    if (a==1) return "found";
    else return {};
}

bool train_optional(){
    auto opt=data(0);
    string val = opt.value_or("not found");
    return opt.has_value();
}



