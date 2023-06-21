#include <iostream>;
#include <thread>;
#include <mutex>;


using namespace std;
static mutex m;
once_flag g_flag;
class Singelton{
    static void create_once(){
        singel = new Singelton();
        static Decon decon;
            
    }
    private:
        Singelton(){;}
    private:
        static Singelton* singel;
    public:
        static Singelton* GetInstance()
        {
            if (singel == NULL)
            {
                unique_lock<mutex> mymutex(m);
                if (singel == NULL)
                {
                    singel = new Singelton();
                    static Decon decon;
                }
                
            }
            return singel;
        }

        static Singelton* CallOnce()
        {
            call_once(g_flag, create_once);
            return singel;
        }
        
        class Decon{
            public:
            ~Decon(){
                if (Singelton::singel)
                {
                    delete Singelton::singel;
                    Singelton::singel = NULL;
                }
            }
        };
};

Singelton* Singelton::singel = NULL;


void mythread(){
    cout<<"start"<<endl;
    Singelton* sing = Singelton::GetInstance();
    cout<<"end"<<endl;
}

void test_Singelton(){
    thread job1(mythread);
    thread job2(mythread);
    job1.join();
    job2.join();
}