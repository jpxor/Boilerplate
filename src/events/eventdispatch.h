
#pragma once

#include <memory>
#include <functional>

namespace Events{

    class Event{
        public:
        int id; 
        Event(int event_id):id(event_id){;;}
    };

    int join_channel(std::string chan_name, std::function<void(std::unique_ptr<Event>&)> callback);
    void leave_channel(std::string chan_name, int id);
    void dispatch_now(std::string chan_name, std::unique_ptr<Event> event);
    void dispatch_async(std::string chan_name, std::unique_ptr<Event>& event);
    void dispatch_later(std::string chan_name, std::unique_ptr<Event> event);
    void dispatch_waiting_events();
}
