#include"EventLoop.h"

#include"logging/Logging.h"

#include<assert.h>
#include<poll.h>

using name sapce muduo;

__thread EventLoop* t_loopInThisThread=0;

EventLoop::EventLoop()：looping(false),threadId_(CurrentThread::tid())
{
  if(t_loopInThisThread)
  {  
    LOG_FATAL<<"Another EventLoop"<<this<<"in thread"<<threadId_; 
  }
  else
  {
    t_loopInThisThread=this;
  }
}

EventLoop::~EventLoop()
{
  assert(!looping_);
  t_loopInThisThread=NULL;
}

void EventLoop::loop()
{
  assert(!looping_);
  assertInLoopThread();
  looping_=true;
  
  ::poll(NULL,0,5*1000);
  
  LOG_TRACE<<"EventLoop"<<this<<"stop looping";
  looping_=false;
} 

void EventLoop::abortNotInLoopThread()
{
  LOG_FATAL<<"EventLoop::abortNotInLoopThread-EventLoop"<<this
    <<"was created in threadId_="<<threadId_<<",current thread id="<<CurrentThread::tid();
}
}
