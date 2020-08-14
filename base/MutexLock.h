#ifndef MUTEXLOCK_H
#define MUTEXLOCK_H

#include <pthread.h>
#include <cstdio>
#include <assert.h>
#include "noncopyable.h"
#include "CurrentThread.h"

class MutexLock : noncopyable{
public:
        MutexLock() {pthread_mutex_init(&mutex_, NULL);}
        ~MutexLock(){
          assert(holder_ == 0);
          pthread_mutex_destroy(&mutex_);
        }
        void lock() {
              pthread_mutex_lock(&mutex_);
              assignHolder();
            }
        void unlock() {
              unasssignHolder();
              pthread_mutex_unlock(&mutex_);
            }
        pthread_mutex_t* getPthreadMutex() {return &mutex_;}
private:
    class UnassignGuard : noncopyable{
      public:
            explicit UnassignGuard(MutexLock& owner) : owner_(owner){
                  owner_.assignHolder();
            }
            ~UnassignGuard(){
                  owner_.unasssignHolder();
            }
            
      private:
            MutexLock& owner_;
      };    
      void unasssignHolder(){
            holder_ = 0;
      }
      void assignHolder(){
            holder_ = CurrentThread::tid();
      }
      pthread_mutex_t mutex_;
      pid_t holder_;
};

class MutexLockGuard : noncopyable{
public:
      explicit MutexLockGuard(MutexLock& mutex) : mutex_(mutex){ mutex_.lock();}
      ~MutexLockGuard()   {mutex_.unlock();}
private:
      MutexLock &mutex_;
};



#endif