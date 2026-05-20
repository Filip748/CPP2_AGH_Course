#pragma once

#include "HttpChannel.h"

class SlackChannel : public HttpChannel {
private:
    std::string url = "https://teams.example/send";
public:
    void send(const std::string& nr, const std::string& mess) override {
        std::string payload = "channel=" + nr + "&message=" + mess;

        httpSend(url, payload);
    }
};

//HTTP request to https://slack.example/send: channel=#backend&message=Deployment completed
