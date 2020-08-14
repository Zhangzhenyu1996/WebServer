#include <iostream>
#include <string>
#include <unistd.h>
#include "../CurrentThread.h"
#include "../Thread.h"
using namespace std;
void mysleep(int seconds){
    timespec t = {seconds, 0};
    nanosleep(&t, NULL);
}

void threadFunc(){
    printf("tid=%d\n", CurrentThread::tid());
}

void threadFunc2(int x){
    printf("tid=%d, x=%d\n", CurrentThread::tid(), x);
}

void threadFunc3()
{
    printf("tid=%d\n", CurrentThread::tid());
    mysleep(1);
}

class Test{
public:
        explicit Test(double x) : x_(x){ }
        void mermberFunc(){
            printf("tid=%d, Test::x_=%f\n", CurrentThread::tid(), x_);
        }

        void memberFunc2(const string& text){
            printf("tid=%d, Test::x_=%f, text=%s\n", CurrentThread::tid(), x_, text.c_str());
        }
private:
        double x_;
};


int main()
{
    printf("pid=%d, tid=%d\n", ::getpid(), CurrentThread::tid());
    Thread t1(threadFunc);
    t1.start();
    printf("t1.tid=%d\n", t1.tid());
    t1.join();
    
    Thread t2(bind(threadFunc2, 42), "thread for free function with argument");
    t2.start();
    printf("t2.tid=%d\n", t2.tid());
    t2.join();

    Test test(86.53);
    Thread t3(bind(&Test::mermberFunc, &test),
                   "thread for member function without argument");
    t3.start();
    t3.join();

    Thread t4(bind(&Test::memberFunc2, ref(test), string("zzy")));
    t4.start();
    t4.join();

    printf("Thread nums:%d\n", Thread::numCreated());
}