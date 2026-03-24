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
#include <string>

#include "EventBus.h"
#include "Enemy.h"
#include "Player.h"
#include "ScoreSystem.h"
#include "LootSystem.h"
#include "SoundSystem.h"

int main() {

    EventBus bus;

    bus.subscribe(&ScoreSystem::handleEvent);
    bus.subscribe(&LootSystem::handleEvent);
    bus.subscribe(&SoundSystem::handleEvent);

    Enemy enemy1(1, 50, bus);
    Enemy enemy2(2, 30, bus);
    Player player(1, 100, bus);

    enemy1.takeDamage(20);
    enemy2.takeDamage(40);

    player.heal(25);
    player.pickItem("Łomotacz");

    enemy1.takeDamage(40);


    bus.processEvents();

    return 0;
}

/*
    Z uwagi na charakter projektów, skupimy się raczej na architekturze i mniej standardowych rzeczach,
    których (ciągle) nie zrobi za Was GPT. :)

    Napiszemy system eventów, który oparty jest o tzw. wzorzec obserwatora (Observer Pattern) i callbacki,
    czyli funkcje, które rejestrujemy by w odpowiednim momencie mogły zostać wywołane i dać nam informację zwrotną (stąd nazwa :)).

    Jako, że nie znamy jeszcze funkcji lambda, w którą można byłoby opakować wywołanie odpowiedniej metody,
    to musimy rozwiązać to w trochę mniej elegancki sposób, jak powyżej. :)
    W bardziej produkcyjnej wersji moglibśmy zrobić, np.

        EventBus bus;
        ScoreSystem score;
        
        bus.subscribe([&](IEvent* e){ score.handleEvent(e); });

    Założenia:

        0) Player i Enemy są tylko tłem, którym (być może) zajmiemy się później.
        1) Event Bus:
            * przechowuje kolejkę eventów (std::queue)
                - trzeba się zastanowić jak sobie zorganizować dziedziczenie i ich kategorie
            * przechowuje kontener zarejestrowanych callbacków
            * pozwala na zarejestrowanie callbacka (konwencjonalnie nazywamy to subscribe, a nasłuchującego subscriber)
            * pozwala na wyemitowanie eventu do wszystkich nasłuchujących
            * pozwala na przeprocesowanie wszystkich eventów w kolejce, czyli poinformowanie o nich wszystkich nasłuchujących

        2) Systemy - tutaj przyjmujemy tylko, że mają wypisywać jakąś informację, że otrzymały event.
*/