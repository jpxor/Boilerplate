

#include <unordered_map>
#include <vector>
#include <thread>
#include <mutex>

#include "events/eventdispatch.h"
#include "util/channel.h"

namespace Events{

    static std::mutex mtx;
    static std::unordered_map<std::string, Channel<Event>> named_channels;
    static std::unordered_map<std::string, std::vector<std::function<void(std::unique_ptr<Event>&)>>> stored_callbacks;

    int subscribe(std::string chan_name, std::function<void(std::unique_ptr<Event>&)> callback){
        std::unique_lock<std::mutex> lck(mtx);
        auto& callback_list = stored_callbacks[chan_name];
        callback_list.push_back(callback);
        return callback_list.size()-1;
    }

    void unsubscribe(std::string chan_name, int id){
        std::unique_lock<std::mutex> lck(mtx);
        auto callback_list = stored_callbacks[chan_name];
        callback_list.erase(callback_list.begin()+id);
    }

    void dispatch_now(std::string chan_name, std::unique_ptr<Event> event){
        std::unique_lock<std::mutex> lck(mtx);
        auto callback_list = stored_callbacks[chan_name];
        for( auto callback : callback_list ){
            callback(event);
        }
    }

    void dispatch_async(std::string chan_name, std::unique_ptr<Event>& event){
        std::thread dispatch_thread(dispatch_now, chan_name, std::move(event));
        dispatch_thread.detach();
    }

    void dispatch_later(std::string chan_name, std::unique_ptr<Event> event){
        std::unique_lock<std::mutex> lck(mtx);
        auto& channel = named_channels[chan_name];
        channel.push(event);
    }

    void dispatch_waiting_events(){
        std::unique_lock<std::mutex> lck(mtx);
        for(auto& pair : named_channels){
            std::string chan_name = pair.first;
            Channel<Event>& channel = pair.second;

            auto callback_list = stored_callbacks[chan_name];
            channel.process_all( [callback_list](std::unique_ptr<Event>& event)->void{
                for( auto callback : callback_list ){
                    callback(event);
                }
            } );
        }
    }
}
