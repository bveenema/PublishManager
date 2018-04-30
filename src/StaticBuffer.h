// StaticBuffer.h
// Static Size Circular Buffer - Adapted from https://embeddedartistry.com/blog/2017/4/6/circular-buffers-in-cc

#include <memory>
#include <mutex>

template <class T>
class StaticBuffer {
public:
    explicit StaticBuffer(size_t size) :
        buf_(std::unique_ptr<T[]>(new T[size])),
        size_(size)
    {
        //empty constructor
    }

    void put(T item){
      std::lock_guard<std::mutex> lock(mutex_);

      buf_[head_] = item;
      head_ = (head_ + 1) % size_; // same as h=h+1; if(h==s) h=0; ie. auto-reset at size

      if(head_ == tail_){
        tail_ = (tail_ + 1) % size_;
      }

      //mutex_ is automatically released when lock goes out of scope (method returns)
    }

    T get(void){
      std::lock_guard<std::mutex> lock(mutex_);

      if(empty()){
        return T(); // return null
      }

      //Read data nd advance the tail_

      auto val = buf_[tail_];
      tail_ = (tail_ +1) % size_;

      return val;
    }

    void reset(void)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        head_ = tail_;
    }

    bool empty(void){
      // if head and tail are equal, we are empty
      return head_ == tail_;
    }

    bool full(void){
      // if tail is ahead of the head by 1, we are full
      return ((head_ + 1) % size_) == tail_;
    }

    size_t size(void){
      return tail_ > head_ ? (size_ - tail_ + head_) : (head_ - tail_);
    }

private:
    std::mutex mutex_;
    std::unique_ptr<T[]> buf_;
    size_t head_ = 0;
    size_t tail_ = 0;
    size_t size_;
};
