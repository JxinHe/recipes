#ifndef MUDUO_NET_EVENTLOOP_H
#define MUDUO_NET_EVENTLOOP_H

#include"thread/Thread.h"

namesapce muduo
{

 class EventLoop:boost::noncopyable
 {
 public:

 EventLoop();
 ~EventLoop();

 void loop();

 void asserInLoopThread()//检查当前线程是否创建了其他EventLoop对象
 {
  if(!isInLoopThread())
  {
   abortNotInLoopThread();
  }
 }
 
 bool isInLoopThread()const
 {
  return threadId_==CurrentThread::tid()；
 }
 
 private:
  bool looping_; //原子操作
  
  void abortNotInLoopThread();
  const pid_t threadId_;
 
};

}
#endif //MUDUO_NET_EVENTLOOP_H
