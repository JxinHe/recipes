#ifndef MUDUO_NET_CHANNEL_H
#define MUDUO_NET_CHANNEL_H

#include<boost/function.cpp>
#include<boost/noncopyable.hpp>

namesapce muduo
{

 class EventLoop;

 class Channel:boost::noncopyalbe
 {
  public:
   typedef boost::function<void()> EventCallback;
   
   Channel(EventLop* loop,int fd);
   
   void handleEvent();//核心，根据不同的revents_值调用不同的用户回调
   void setReadCallback(const EventCallback& cb)
   {
    readCallback_=cb;
   }
   void setWriteCallback(const EventCallback& cb)
   {
    writeCallback_=cb;
   }
   void setErrorCallback(const EventCallback& cb)
   {
    errorCallback_=cb;
   }
   
   int fd()const
   {
    return fd_;
   }
   int events()const
   {
    return events_;
   }
   void set_revents(int revt)
   {
    revents_=revt;
   }
   bool isNoneEvent()const
   {
    return events_ ==kNoneEvent;
   }
   
   void enableReading()
   {
    events_|=kReadEvent;
    update();
    }
    
    
    int index()
    {
     return index_;
    }
    
    void set_index(int idx)
    {
     index_=idx;
    }
    
    EventLoop* ownerLoop()
    {
     return loop_;
    }
    
    private:
    void update();
    
    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;
    
    EventLoop* loop_;
    const int fd_;
    int events_;//关注的
    int revents_;//活动的
    int index_;//used by Poller
    
    EventCallback readCallback_;
    EventCallback writecCallback_;
    EventCallback errorCallback_;
   };
 }
 #endif

}
