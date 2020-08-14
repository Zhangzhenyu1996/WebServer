#include <iostream>
#include <unistd.h>
#include <sys/prctl.h>
#include <assert.h>
#include "Thread.h"
#include "CurrentThread.h"
#include "dbg.h"
using namespace std;



namespace CurrentThread{
  //__thread修饰的变量是线程局部存储的
  __thread int t_cachedTid = 0; //线程真实pid(tid)缓存
  __thread char t_tidString[32];// tid字符串表现形式
  __thread int t_tidStringLength = 6;
  __thread const char* t_threadName = "default"; 
};

pid_t gettid(){return static_cast<pid_t>(::syscall(SYS_gettid));}

void CurrentThread::cacheTid(){
    if (t_cachedTid == 0) {
    t_cachedTid = gettid();
    t_tidStringLength =
        snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
  }
}

struct ThreadData{
    typedef Thread::ThreadFunc ThreadFunc;
    ThreadFunc func_;
    string name_;
    pid_t* tid_;
    CountDownLatch* latch_;
    ThreadData(const ThreadFunc&  func, const string& name,
               pid_t* tid, CountDownLatch* latch)
        : func_(func), name_(name), tid_(tid), latch_(latch){

    }
    void runInThread(){
      *tid_ = CurrentThread::tid();
      tid_ = NULL;
      latch_->countDown();
      latch_ = NULL;
      CurrentThread::t_threadName = name_.empty() ? "Thread" : name_.c_str();
      prctl(PR_SET_NAME, CurrentThread::t_threadName);
      func_();
      CurrentThread::t_threadName = "finished";
    }
};


void* startThread(void* arg)
{
  ThreadData* data = static_cast<ThreadData*>(arg);
  data->runInThread();
  delete data;
  return NULL;
}

atomic_int32_t Thread::numCreated_;

Thread::Thread(const ThreadFunc& func, const string& name)
    :started_(false),
     joined_(false),
     pthreadId_(0),
     tid_(0),
     func_(func),
     name_(name),
     latch_(1)
{
  setDefaultName();
}

Thread::~Thread(){
  if(started_ && !joined_) 
    pthread_detach(pthreadId_);
}

void Thread::setDefaultName(){
  int num = numCreated_++;
  if(name_.empty()){
    char buf[32];
    snprintf(buf, sizeof(buf), "Thread%d", num);
    name_ = buf;
  }
}


void Thread::start(){
  assert(!started_);
  started_ = true;
  ThreadData* data = new ThreadData(func_, name_, &tid_, &latch_);
  if(pthread_create(&pthreadId_, NULL, &startThread, data)){
      started_ = false;
      delete data;
      log_warn("failed int pthread_create");
  }else{
    latch_.wait();
    assert(tid_ > 0);
  }
}

int Thread::join(){
  assert(started_);
  assert(!joined_);
  joined_ = true;
  return pthread_join(pthreadId_, NULL);
}



