#pragma once
#include <deque>
#include <vector>
#include <memory>
#include <assert.h>
#include <stdio.h>
#include "Condition.h"
#include "MutexLock.h"
#include "Thread.h"
using namespace std;
class ThreadPool : noncopyable
{
public:
    typedef function<void()> Task;
    explicit ThreadPool(const string& name = string("ThreadPool"));
    ~ThreadPool();
    void setMaxQueueSize(int maxSize) {maxQueueSize_ = maxSize;}
    void setThreadInitCallback(const Task& cb){
        threadInitCallback_ = cb;
    }
    void start(int numsThreads);
    void stop();
    const string& naem() const{
        return name_;
    }
    size_t queueSize() const;
    void run(Task f);

private:
    bool isFull() const;
    void threadWorker();
    Task take();
    mutable MutexLock mutex_;
    Condition notEmpty_;
    Condition notFull_;
    string name_;
    Task threadInitCallback_;
    vector<unique_ptr<Thread>> threads_;
    deque<Task> queue_;
    size_t maxQueueSize_;
    bool running_;
};