#ifndef MUDUO_NET_POLLER_H
#define MUDUO_NET_POLLER_H

#include<map>
#include<vector>

#include"datetime/Timestamp.h"
#include"EventLoop.h"

struct pollfd;

namesapce muduo
{
 class Channel;
 
 class Poller:boost::noncopyable
 {
  public:
   typedef std::vector<Channel*> ChannelList;//存放每个Channel
   Poller(EventLoop* loop);
   ~Poller();
   
   Timestamps poll(int timeoutMs,ChannelList* activeChannels);//poll IO活动，必须在当前循环线程中
   
   void assertInLoopThread(){ownerLoop_->assertInLoopThread();}
   
   private:
    void fillActiveChannels(int numEvents,ChannelList* activeChannels)const;
    
    typedef std::vector<struct pollfd> PollFdList;//pollfd数组
    typedef std::map<int,Channel*> ChannelMap;//fd到Channel*的映射
    
    EventLoop* ownerLoop_;
    PollFdList pollfds_;
    ChannelMap channels_;
 };
 
}
#endif
