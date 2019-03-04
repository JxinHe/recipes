
#ifndef MUDUO_NET_TIMERQUEUE_H
#define MUDUO_NET_TIMERQUEUE_H

#include <set>
#include <vector>

#include <boost/noncopyable.hpp>

#include "datetime/Timestamp.h"
#include "thread/Mutex.h"
#include "Callbacks.h"
#include "Channel.h"
namespace muduo
{
 class EventLoop;
  class Timer;
  class TimerId;
  
  class TimerQueue:boost::noncopyable
  {
  public:
  TimerQueue(Eventloop* loop);
  ~TimerQueue();
  
  TimerId addTimer(const TimerCallback& cb,Timerstamp when,double interval);
  
  private:
    typedef std::pair<Timerstamp,Timer*> Entry;
    typedef std::set<Entry> TimerList;
    void handleRead();
    
    std::vector<Entry> getExpired(Timestamp now);
    void reset(const std::vector<Entry>& expired,Timerstamp now);
    
    bool insert(Timer* timer);
    
    EventLoop* loop_;
    const int timerfd;
    Channel timerfdChannel_;
    TimerList timers_;
  };
  
}
#endif
