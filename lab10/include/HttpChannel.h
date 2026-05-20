#pragma once

#include <iostream>
#include "SmsChannel.h"

class HttpChannel : public Channel {
public:
    void httpSend(const std::string& url, const std::string& payload) {
        std::cout << "HTTP request to" << url << ": " << payload << "\n";
    }
};