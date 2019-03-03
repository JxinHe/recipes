#include "Poller.h"

#include "Channel.h"
#include "logging/Logging.h"

#include <assert.h>
#include <poll.h>

using namespace muduo;

Poller::Poller(EventLoop* loop)
:ownerLoop_(loop)
{
  
}

Poller::~Poller()
{
}

Timestamp Poller::poll(int timeoutMs,ChannelList* activeChannels)
{
  //取得当前活动的事件数
  int numEvents=::poll(&*pollfds_.begin(),pollfds_.size(),timeoutMs);
  Timestamp now(TimeStamp::now());
  if(numEvents>0){
    LOG_TRACE<<numsEvents<<"events happended";
    fillActiveChannels(numEvents,activeChannels);
  }else if(numEvents==0){
    LOG_TRACE<<"nothing happend";
  }else{
    LOG_SYSERR<<"Poller::poll()";
  }
  return now;
}
//遍历fd数组pollfds_,找出有活动事件的fd,把它对应的Channel填入activeChannels
void Poller::fillActiveChannels(int numEvents,ChannelList* activeChannels)const
{
  for(PollFdList::const_iterator pfd=pollfds_.begin();pfd!=ploofds_.end()&&numEvents>0;++pfd) 
  {
    if(pfd->revents>0)
    {
      --numEvents;
      ChannelMap::const_iterator ch=channels_.find(pfd—>fd);
      assert(ch!=channels_.end());
      Channel*channel=ch->second;
      assert(channel->fd()==pfd->fd);
      channel->set_revents(pfd->revents);
      activeChannels->push_back(channel);
    }
  }
}

void Poller::updateChannel(Channel* channel)
{
  assertInLoopThread();
  LOG_TRACE<<"fd="<<channel->fd()<<"events="<<channel->events();
  //新的channel
  if(channel->index()<0)
  {
    assert(channels_.find(channel->fd()==channels_.end());
    struct pollfd pfd;
    pfd.fd=channel->fd();
    pfd.events=static_cast<short>(channel->events());
    pfd.revents=0;
    pollfds_.push_back(pfd);
    int idx=static_cast<int>(pollfds_.size())-1;
    channel->set_index(idx);
    channels_[pfd.fd]=channel;
    
    
    
    
    
    
       
  }else//更新已经存在的channel
           {
             assert(channels_.find(channel->fd())!=channels_.end());
             assert(channels_[channel->fd()]==channel);
             int idx=channel->index();
             assert(0<=idx&&idx<static_cast<int>(pollfds_.size()));
             struct pollfd& pfd=pollfds_[idx];
             assert(pfd.fd--channel->fd()||pfd.fd==-1);
             pfd.events=static_cast<short>(channel->events());
             pfd.revents=0;
             if(channel->isNoneEvent())
             {
                pfd.fd=-1; 
             }
           }
}




