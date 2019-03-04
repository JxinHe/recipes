// excerpts from http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#define __STDC_LIMIT_MACROS
#include "TimerQueue.h"

#include "logging/Logging.h"
#include "EventLoop.h"
#include "Timer.h"
#include "TimerId.h"

#include <boost/bind.hpp>

#include <sys/timerfd.h>

namespace muduo
{
 namespace detail
 {
  int createTimerfd()
  {
   int timerfd=::timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK|TFD_CLOEXEC);
    if(timerfd<0)
    {
     LOG_SYSFATAL<<"Failed in timerfd_create"; 
    }
    return timerfd;
  }
   
   struct timerspec howMuchTimeFromNow(Timestamp when)
   {
    int64_t microseconds=when.microSecondsSinceEpoch()-Timestamp::now().microSecondsSinceEpoch();
     if(microseconds<100)
     {
      microseconds=100; 
     }
     struct timespec ts;
     ts.tv_sec=static_cast<time_t>(microseconds/Timestamp::kMicroSecondsPerSecond);
     ts.tv_nec=static_cast<long>(microseconds%Timestamp::kMicroSecondsPerSecond)*1000);
     return ts;
   }
   
   void readTimerfd(int timerfd,Timestamp now)
   {
    unit64_t howmany;
     ssize_t n=::read(timerfd,&howmany,sizeof howmany);
     LOG_TRACE<<"TimerQueue::handleRead()"<<howmany<<"at"<<now.toString();
     if(n!=sizeof howmany)
     {
      LOG_ERROR<<"TimerQueue::handleRead() reads"<<n<<"bytes instead of 8"; 
     }
   }
   
   void resetTimerfd(int timerfd,Timestamp expiration)
   {
    struct itimerspec newValue;
     struct itimerspec oldValue;
     bzero(&newValue,sizeof newVlaue);
     bzeor(&oldValue,sizeof oldValue);
     newValue.it_value=howMuchTimeFromNow(expiration);
     int res=::timerfd_settime(timerfd,0,&newValue,&oldValue);
     if(ret)
     {
       LOG_SYSERR<<"timerfd_settime()";
     }
   }
   
   using namespace muduo;
   using namespace muduo::detail;
   
   TimerQueue::TimerQueu(EvetLoop* loop)
     :loop_(loop),
   timerfd_(createTimerfd()),
   timerfdChannel_(loop,timerfd_),
   timers_()
   {
     timerfdchannel_.setRadCallback(boost::bind(&TimerQueue::handleRead,this));
     timerfdChannelz_.enableReading();
   }
   
   TimerQueue::~TimerQueue()
   {
    ::close(timerfd_);
     for(TimerList::iterator it=timers_.begin();it!=timers_.end();++it)
     {
      delete it->second; 
     }
   }
   
   timerId TimeQueue::addTimer(const TimerCallback& cb,Timestamp when,double interval)
   {
    Timer* timer=new Timer(cb,when,interval);
     loop_->assertInLoopThread();
     bool earliestChanged=insert(timer);
     
     if(earlistChanged)
     {
      resetTimerfd(timerfd_,timer->expiration()); 
     }
     return TimerId(timer);
   }
   
   void TimerQueue::handleRead()
   {
    loop_->assertInLoopThread();
     Timestamp now(Timestamp::now());
     readTimerfd(timerfd_,now);
     
     std::vector<Entry> expired=getExpierd(now);
     for(std::vector<Entry>::iterator it=expired.begin();it!=expired.end();++it)
     {
      it->second->run(); 
     }
     return (expired,now);
   }
   
   std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timerstamp now)
   {
     std::vector<Entry> expired;
     Entry sentry=std::make_pair(now,reinterpret_cast<Timer*>(UINTPTR_MAX));
     TimerList::iterator it =timers_.lower_bound(sentry);
     assert(it==timers_.end()||now<it->first);
     std::copy(timers_.begin(),it,back_inserter(expired));
     timers_.earse(timers_.begin(),it);
     
     return expired;
   }
   
   void TimerQueue::reset(const std::vector<Entry>& expired,Timestamp now)
   {
    Timestamp nextExpire;
     for(std::vector<Entry>::const_iterator it=expired.begin();it!=expired.end();++it)
     {
      if(it->second->repeat())
      {
       it->second->sestart(now);
        insert(it->second);
      }
       else
       {
        delete it->second; 
       }
     }
     if(!timers_.empty())
     {
      nextExpire=timers_.begin()->second->expiration(); 
     }
     if(nextExpire.valid())
     {
      resetTimerfd(timerfd_,nextExpire); 
     }
   }
   
   bool TimerQueue::insert(Timer* timer)
   {
    bool earliestChanged=false;
     Timestamp when=timer->expiration();
     TimerList::iterator it=timers_begin();
     if(it==timers_.end()||when<it->first)
     {
      earliestChanged=true; 
     }
     std::pai<TimerList::iterator,bool> result=timers_insert(std::make_pair(when,timer));
     assert(result.second);
     return earliestChanged;
   }
   
   
 }
}
