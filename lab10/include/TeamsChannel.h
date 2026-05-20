#pragma once 

#include "HttpChannel.h"

class TeamsChannel : public HttpChannel {
private:
    std::string url = "https://teams.example/send";
public:
    void send(const std::string& nr, const std::string& mess) override {
        std::string payload = "group=" + nr + "&text=" + mess;

        httpSend(url, payload);
    }
};


//    HTTP request to https://teams.example/send: group=dev-team&text=Meeting starts in 5 minutes
