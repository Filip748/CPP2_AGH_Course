#pragma once

#include <iostream>
#include "SmsChannel.h"

class NotificationService {
private:
    Channel* channel;
public:
    NotificationService(Channel* channel) : channel(channel) {}

    void alert(const std::string& nr, const std::string& mess) {
        channel->send(nr, mess);
    }
};