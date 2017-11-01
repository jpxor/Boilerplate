
#pragma once

#include <queue>
#include <functional>
#include <memory>
#include <mutex>
#include <condition_variable>

template <class T>
class Channel{
    private:
    bool nonblocking = false;
    std::deque<std::unique_ptr<T>> channel;
    std::condition_variable cv;
    std::mutex mtx;
    
    public:
    //push item into channel
    void push( std::unique_ptr<T>& datum ){
        std::unique_lock<std::mutex> lck(mtx);
        channel.push_back( std::move(datum) );
        cv.notify_all();
    }
    //pop item from channel.
    //if empty, channel blocks or returns nullptr if non-blocking
    std::unique_ptr<T> pop(){
        std::unique_lock<std::mutex> lck(mtx);
        while (channel.empty()){
            if (nonblocking) {
                return nullptr;
            }
            cv.wait(lck);
        }
        std::unique_ptr<T> datum = std::move(channel.front());
        channel.pop_front(); //removes nullptr created by the above move
        return datum;
    }
    //processes all items in the channel, and clears them.
    void process_all( std::function<void(std::unique_ptr<T>&)> process){
        std::unique_lock<std::mutex> lck(mtx);
        for (std::unique_ptr<T> &item : channel){
            process(item);
        }
        std::deque<std::unique_ptr<T>> empty;
        std::swap(channel, empty);
    }
    //release all threads blocked on pop.
    //future calls to pop will not block. 
    void set_nonblocking(){
        nonblocking = true;
        std::unique_lock<std::mutex> lck(mtx);
        cv.notify_all();
    }
    //inline size
    inline int size(){return (int)channel.size();}
};