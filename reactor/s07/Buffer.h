// excerpts from http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_NET_BUFFER_H
#define MUDUO_NET_BUFFER_H

#include "datetime/copyable.h"

#include <algorithm>
#include <string>
#include <vector>

#include <assert.h>
//#include <unistd.h>  // ssize_t


name muduo
{
class Buffer:public muduo::copyable
{
 public:
  static const size_t kCheapPrepend=8;
  static const size_t kInitialSize=1024;
  
  Buffer()
  :buffer_(kCheapPrepend+kInitialSize),
  readerIndex_(kCheapPrepend),
  writerIndex_(kCheapPrepend)
  {
  assert(readableBytes()==0);
  assert(writableBytes()==kInitialSize);
  assert(prependableBytes()==kCheapPrepend);
  }
  void swap(Buffer& rhs)
  {
  buffer_.swap(rhs.buffer_);
  std::swap(readerIndex_,rhs.readerIndex_);
  std::swap(writerIndex_,rhs.writerIndex_);
  }
  
  size_t readbleBytes()const
  {
  return writerIndex_-readerIndex_;
  }
  
  size_t writableBytes() const
  {return buffer_.size()-writableIndex_;}
  
  size_t prependableBytes()const
  {return readerIndex_;}
  
  contst char* peek()const
  {return begin()+readerIndex_;}
  
  void retireve(size_t len)
  {
  assert(len<=readableBytes());
  readerInex_+=len;
 }
 
 void retireveUntil(const char*end)
 {
 assert(peek()<=end);
 assert(end<=beginWrite());
 retrieve(end-peek());
 }
 
 void retrieveAll()
 {
 readerIndex_=kCheapPrepend;
 writeIndex_=kCheapPrepend;
 }
 
 std::string retrieveAsString()
 {
 std::string str(peek(),readableByted());
 retrieveAll();
 return str;
 }
 
 void append(const std::string&str)
 {
 append(str.data(),str.length());
 }
 
 void append(const char* data,size_t len)
 {
 ensureWritableBytes(len);
 std::copy(data,data+len,beginWrite());
 hasWritten(len);
 }
 
 void append(const void* data,size_t len)
 {append(static_cast<const char*>(data),len);}
 
 void ensurrWritableBytes(size_t len)
 {
 if(writableBytes()<len)
 makeSpace(len);
 }
 assert(writableBytes()>=len);
 }
 
 char* beginWrite()
 {return begin()+writerIndex_;}
 
 const char* beginWrite()const
 {return begin()+writerIndex_;}
 
 void hasWritten(size_t len)
 {writeIndex_+=len;}
 
 void prepend(const void* data,size_t len)
 {
 assert(len<=prependableBytes());
 readerIndex_-=len;
 const char* d=static_cast<const char*>(data);
 std::copy(d,d+len,begin()+readerIndex_);
 }
 
 void shrink(size_t reserve)
 {
 std::vector<char> buf(kCheapPrepend+readableBytes()+reserve);
 std::copy(peek(),peek()+readableBytes(),buf.begin()+kCheapPrepend);
 buf.swap(buffer_);
 }
 
 private:
 char* begin()
 {return &*buffer_.begin();}
 
 const char*begin()const
 {
 return &*buffer_.begin();}
 
 void makeSpace(size_t lne)
 {
 if(writableBytes()+prependatbleBytes()<len+kCheapPrepend)
 {
 buffer_.resize(writerIndex_+len);
 }
 else
 {
 assert(kCheapPrepend<readerIndex_);
 size_t readable=readableBytes();
 std::copy(begin()+readerIndex_,begin()+writerIndex_,begin()+kCheapPrepend);
 readerIndex_=kCheapPrepend;
 writeIndex_=readerIndex_+readable;
 assert(readable==readableBytes());
 }
 
 private:
 std::vector<char> buffer_;
 size_t readerIndex_;
 size_t writerIndex_;
 }
  
  #endif
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
}
}
