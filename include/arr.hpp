#include <iostream>
using namespace std;
#include<array>
#include<vector>
#include<tuple>

typedef struct Vertex{
    float x, y, z;
    Vertex(float x, float y, float z):
    x(x), y(y), z(z)
    {}

    Vertex(const Vertex& v):
    x(v.x), y(v.y), z(v.z)
    {cout<<"Copy"<<endl;}

} V;

ostream& operator<<(ostream& stream, const Vertex& vertex)
{
    stream<<"x: " << vertex.x<<"y: " <<vertex.y<<"z: " <<vertex.z<<endl;
    return stream;
}

void arr_train(){
    int a[7];
    int* b= new int[7];
    *(b+2) = 10;
    *(int*)((char*)b+sizeof(int)/sizeof(char)*2) = 20;
    delete[] b;
    array<int, 20> c;

}

void string_train(){
      // String
    char* name = "Shi";
    string name2="Shi";
    int name_size = name2.size();
    name2 += "Hello"; 
    string name3 = "S"s + "hi";
}

void lambda_train(string &name2){
    //lambda
    auto func = [=](int a) mutable {name2 += " yitian";}; // without mutable illegal
    func(5); // name2 still not changed
}

void vector_train(){

    //vector
    
    vector<V> vt;
    vt.reserve(9);

    vt.emplace_back(2, 3, 4);
    vt.push_back({2, 3, 4});
    vt.push_back({2, 3, 4});

    for (V& v: vt)
    cout<<v<<endl; 
}

/*
static tuple<string, string> func2()
{return ;}*/
