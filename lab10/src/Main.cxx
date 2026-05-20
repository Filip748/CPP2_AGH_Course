/*
Najważniejsze informacje:
- nie korzystamy z internetu, notatek, kodu sąsiada, etc.
- nie można w żaden sposób modyfikować maina (chyba, że zawiera tylko komentarz z prośbą o jego wypełnienie :))
- CMake musi mieć flagi generujące warningi, tzn. minimum -Wall -Wextra -Wpedantic
- przypominam o istnieniu valgrinda
- będę się czepiać const correctness

- na upela wrzucamy archiwum z plikami źródłowymi oraz CMakeLists.txt, bez katalogu build
    * w prostych programach proszę się trzymać podziału na include i src, 
    * jeśli pojawi nam się większy projekt, podział na podkatalogi z użyciem include_directory w CMake jest jak najbardziej ok
    * tar -czvf nazwa.tar.gz zad/ :)
- proszę czytać wstępy i feedback
*/


#include <iostream>

#include "SmsChannel.h"

#include "SlackChannel.h"
#include "TeamsChannel.h"

#include "MultiChannel.h"

#include "NotificationService.h"


int main() {

    
    SmsChannel sms;
    NotificationService sms_service(&sms);

    sms_service.alert(
        "+48123123123",
        "Your code is 1234"
    );

    std::cout << std::endl;




    SlackChannel slack;
    NotificationService slack_service(&slack);

    slack_service.alert(
        "#backend",
        "Deployment completed"
    );

    std::cout << std::endl;




    TeamsChannel teams;
    NotificationService teams_service(&teams);

    teams_service.alert(
        "dev-team",
        "Meeting starts in 5 minutes"
    );

    std::cout << std::endl;




    MultiChannel multi;
    multi.addChannel(&sms);
    multi.addChannel(&slack);
    multi.addChannel(&teams);

    NotificationService multi_service(&multi);

    multi_service.alert(
        "global-alert",
        "Production is down"
    );

    std::cout << std::endl;


    return 0;
}

/** 

Skoro już musimy robić hierarchię dziedziczenia, to zróbmy coś z okolic interfejsów/API, 
bo tam przynajmniej granice enkapsulacji, wymuszone przez OOP, pokrywają się w miarę z rozsądkiem (czasem).
A przynajmniej nie sabotują aktywnie wydajności bardziej, niż przez kilka v-table i pointer chasing. :)


Oczekiwany output to (mniej więcej :)):

    SMS to +48123123123: Your code is 1234

    HTTP request to https://slack.example/send: channel=#backend&message=Deployment completed

    HTTP request to https://teams.example/send: group=dev-team&text=Meeting starts in 5 minutes

    SMS to global-alert: Production is down
    HTTP request to https://slack.example/send: channel=global-alert&message=Production is down
    HTTP request to https://teams.example/send: group=global-alert&text=Production is down



Przy okazji użyjemy wzorca strategii. Idea jest taka:

    1) NotificationService nie wie nic o konkretnych implementacjach kanałów.
    Udostępnia tylko zewnętrzne API, które pozwala wysłać alert, przekazując:

        * ścieżkę (np. numer telefonu)
        * wiadomość

    Wysłanie jest realizowane przez kanał przekazany do konstruktora (na zasadzie strategii).

    2) Kanał SMSowy jest prosty i po prostu wysyła wiadomość.
    Kanały slackowy i teamsowy korzystają razem z dodatkowej warstwy, implementującej komunikację przez request HTTP.
    
    Czyli:
        (być może coś :))
            |-- sms
            |-- http channel
                |-- slack
                |-- teams


    3) Na potrzeby zadania, przyjmijmy, że 

        * kanał smsowy wysyła wiadomość przez wypisanie:
            "SMS to {kanał}: {wiadomość}"

        albo czegoś podobnego.

        * kanały slackowy i teamsowy wysyłają wiadomość korzystając z warstwy HTTP,
        tzn. przekazują do niej (poziom wyżej) swój payload, a ona opakowuje go w odpowiedni request i wysyła.

        Przyjmijmy, że to "opakowanie" polega na wypisaniu:

            "HTTP request to {url}: {payload}"

        a sam payload to:

            - dla slacka: "channel={kanał}&message={wiadomość}"
            - dla teamsa: "group={kanał}&text={wiadomość}"

        albo coś podobnego. :)
        
        
        * url używane przez kanały, to odpowiednio: 
            - slack: "https://slack.example/send"
            - teams: "https://teams.example/send"

        Mogą być na sztywno zakodowane w klasach kanałów.


    4) MultiChannel wysyła wiadomość do wszystkich dodanych kanałów.


    Zwracam uwagę, że wszystko ma czas życia równy zakresowi maina. Nie trzeba żadnych delete. :)
*/