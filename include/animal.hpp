#include<iostream>
#include <memory>
#pragma once

using namespace std;


struct Node{
    Node *prev = NULL, *next=NULL;
    int val = 0;
    static int id;
};

template <class T>
class animal{
    public:
    static int b;
    static int a;
    int x, y, z;
    int* li;
    T id;    
    static void total_number() {std::cout<<"Current number of animals: "<<b<<std::endl;}
    virtual void hey(){std::cout<<"Hey, animal";}

    private:
    int m_loglevel = Info;

    public:

    // Constructor

    animal(const T &id): x(0), y(0),z(0){
        li = new int[b];
        for (int i = 0; i < b; i++)
        {
            li[i] = i;
        }
        a++;
        this->id = id;
        std::cout<<"Hello, I'm default animal with id "<<this->id<<std::endl;
        }

    animal(int a, int b, int c, const T &id): x(a), y(b),z(c)
        {
        li = new int[b]; 
        for (int i = 0; i < b; i++)
            {
                li[i] = i;
            }        
        a++;
        this->id = id; 
        std::cout<<"Hello, I'm animal with id "<<this->id<<std::endl;}

    
    // Copy constructor

    template <class T2>
    animal(const animal<T2>& a2):x(a2.x), y(a2.y), z(a2.z){
        li = new int[b]; 
        //memcpy(li, a2.li, sizeof(li));
        for (int i = 0; i < b; i++)
        {
            li[i] = a2.li[i]*2;
        }
        a++; 
        if ( typeid( a2.id ) == typeid(this->id) ) 
        {
            this->id = a2.id;
            std::cout<<"Hello, I'm copied animal with id "<<this->id<<std::endl;}
        else 
        {"ID type different, I'm copied animal with default id";}

    }


    animal operator+(const animal& a2) const {
        return animal(this->x + a2.x,
                    this->y + a2.y,
                    this->z + a2.z,
                    this->id); 
    }

    animal operator-(const animal& a2) const {
        return animal(this->x - a2.x,
                    this->y - a2.y,
                    this->z - a2.z, 
                    this->id); 
    
    }

    bool operator==(const animal& a2) const
    {
        return this->x == a2.x && this->y == a2.y && this->z == a2.z;
    }

    bool operator!=(const animal& a2) const
    {
        return !(*this == a2);
    }

    virtual ~animal(){
        cout<<"Bye, animal id "<< this->id <<endl;
        delete[] li;
        }
    static void add(){b++;}
 

    enum Level: int{Error=0, Warning, Info};
    void set_level(Level level){
        m_loglevel=level;
    }
    void error(const char* message)
    {
        if (m_loglevel >= Error)
        std::cout <<"Error: "<<message<<std::endl;
    }
    void warn(const char* message)
    {
        if (m_loglevel >= Warning)
        std::cout <<"Error: "<<message<<std::endl;
    }
    void info(const char* message)
    {
        if (m_loglevel >= Info)
        std::cout <<"Error: "<<message<<std::endl;
    }
};

template <class T>
int animal<T>::b = 20;
template <class T>
int animal<T>::a = 20;


template <class T>
class fox: public animal<T>{
    public:
    fox(const T &id):animal<T>(id){
        std::cout<<"Hello, I'm default fox with id "<<this->id<<std::endl;
        }

    fox(int a, int b, int c, const T &id):animal<T>(a, b, c, id) 
        {std::cout<<"Hello, I'm fox with id "<<this->id<<std::endl;}

    // Copy constructor
    template <class T2>
    fox(const fox<T2>& a2):animal<T>(a2.x, a2.y, a2.z, a2.id){
        if ( typeid( a2.id ) == typeid(this->id) ) 
        {
            this->id = a2.id;
            std::cout<<"Hello, I'm copied fox with id "<<this->id<<std::endl;}
        else 
        {"ID type different, I'm copied fox with default id";}
    }

    fox operator+(const fox& a2) const {
        return fox(this->x + a2.x,
                   this->y + a2.y,
                   this->z + a2.z,
                   this->id);
    }

    using animal<T>::operator+;

    ~fox();
    void hey() override {cout<<"Hey, fox number "<<this->id<<endl;}
};

template <class T>
fox<T>::~fox()
{
    cout<<"Bye, fox id "<< this->id <<endl;
}


static std::ostream& operator<<(std::ostream &stream, const animal<char>& other)
{
    stream<<other.x<<other.y<<other.z<<std::endl;
    return stream;
}

template <class T>
class dog: public animal<T>{
};


