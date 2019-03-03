#include"Channel.h"
#include"EventLoop.h"
#include"logging/Logging.h"

#include<sstream>

#include<poll.h>

using namespace muduo;

const int Channel::kNoneEvent=0;//不关注的事件
const int Channel::kReadEvent=POLLIN|POLLPRI;//关注的读事件
const int Channel::kWriteEvent=POLLOUT;

Channel::Channel(EventLoop* loop,int fdArg)
  :loop_(loop),fd_(fdArg),events_(0),revents_(0),index_(-1)
  {
    
  }
void Channel::update()
{
  loop_->updateChannel(this);//会转而调用Poller::updateChannel() 
}


void Channel::handleEvent()//核心，有Eventloop::loop()调用，根据不同revents_值调用不同用户回调
{
   if(revents_&POLLNVAL)
     LOG_WARN<<"Channel::handle_event() PLOONVAL";
  
   if(revents_&()POLLERR|POLLNVAL)
   {
      if(errorCallback_)
        errorCallback_();
   }
  if(revents_&(POLLING|POLLPRI|POLLRDHUP))
  {
    if(readCallback_)
      readCallback_();
  }
  if(revents_&POLLOUT)
  {
   if(writeCallback_)
     writeCallback_();
  }
}

