#pragma once

#include "SmsChannel.h"
#include <vector>
#include <iostream>

class MultiChannel : public Channel {
private:
    std::vector<Channel*> channels;

    public:
    void addChannel(Channel* chan) {
        channels.push_back(chan);
    }

    void send(const std::string& nr, const std::string& mess) override{
        for(Channel* channel : channels) {
            channel->send(nr, mess);
        }
    }



};