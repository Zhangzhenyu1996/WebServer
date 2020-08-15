#include <stdio.h>
#include <unistd.h>
#include "ThreadPool.h"
#include "CountDownLatch.h"
#include "CurrentThread.h"
#include "../dbg.h"
using namespace std;
void print()
{
  printf("tid=%d\n", CurrentThread::tid());
}

void printString(const std::string& str)
{   
  printf("%s\n", str.c_str());
  usleep(100*1000);
}

void test(int maxSize)
{
    log_warn("threadPoll with max queue size:%d", maxSize);
    ThreadPool pool("MainThreadPool");
    pool.setMaxQueueSize(maxSize);
    pool.start(5);

    for(int i=0; i<100; ++i)
    {
        char buf[32];
        snprintf(buf, sizeof(buf), "task:%d", i);
        pool.run(bind(printString, string(buf)));
    }
    CountDownLatch latch(1);
    pool.run(bind(&CountDownLatch::countDown, &latch));
    latch.wait();
    pool.stop();
}

int main()
{
    test(0);
    test(1);
    test(5);
}