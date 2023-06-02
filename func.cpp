#include<iostream>
#include "1.hpp"

int sta = 1;

namespace n2{
    void f(){
        cout << 2<< endl;
        }
}

namespace n1{
    void f(){
        cout << 1<< endl;
        }
}

template <typename T1, class T2>
void animal_voice(animal<T1> &a, T2 &b){
    a.hey();
    cout << "it says "<< b << endl;
}

void forest(int def = 20){

     for (int i=0; i<5; i++)
    {
    static int st=0;
    fox<char> f1('v');
    auto f2 = make_shared<fox<int>>(st);
    auto f3 = make_unique<fox<int>>(st + 10);
    fox<int>::total_number();
    fox<char>::total_number();
    animal_voice(f1, "fuck");
    f1.set_level(animal<char>::Error);
    f1.warn("shit");
    f1.error("shit");
    f1.info("shit");
    st++;
    }

    fox<char> f1('c');
    fox<char>::total_number();
    fox<char> f2 = {2, 3, 4, 'd'};

    auto f3 = f1+f2;
    std::cout<<f2<<std::endl; 

}

void Func(void(*func)(int)){
    func(10);
}

void train_func(){
    auto fo = forest;
    void(*func)(int) = forest;
    typedef void(*forest_func)(int);
    forest_func func2 = forest;

    Func([](int a){forest(a);}); // give Func a function, which calls forest function

    using n1::f;
    f();
    {
        using n2::f;
        f();
    }
}




