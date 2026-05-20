#pragma once

#include <string>

class Channel {
public:
    virtual void send(const std::string& nr, const std::string& mess) = 0;
    virtual ~Channel() {}
};

class SmsChannel : public Channel {
public:
    void send(const std::string& nr, const std::string& mess) override {
        std::cout << "SMS to " << nr << ": " << mess << "\n";
    }
};

//SMS to +48123123123: Your code is 1234