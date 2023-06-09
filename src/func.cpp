#include<iostream>
#include "animal.hpp"

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

    fox<char> f1('a');
    fox<char>::total_number();
    fox<char> f2 = {2, 3, 4, 'b'};


    auto f3 = f1+f2;
    std::cout<<f2<<std::endl; 
    animal<char>* f4= new fox<char>(2, 3, 4, 'c');
    delete f4; // after deletion, the deconstructor of animal is called but fox isn't. So virtual ~animal() should be defined.

    {
        auto f_s = make_shared<fox<char>>('f');
        weak_ptr<fox<char>> f_w;
        {
            auto f_s2 = f_s;
            f_w = f_s;
            auto f_u=make_unique<animal<char>>(1,2,3,'g');
            // out of this scope, f_w and f_u are deleted
            f_u->hey();
        }
        // out of this scope, f_s is deleted
    }

}

void Func(void(*func)(int)){
    func(10);
}


void func_train(){
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

auto lambda_train(string &name2){
    //lambda
    auto func = [=](int a) mutable {name2 += " yitian";}; // without mutable illegal
    func(5); // name2 still not changed
}

