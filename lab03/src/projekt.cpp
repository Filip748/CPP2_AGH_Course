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

    // Proszę przetestować nową wersję.
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
    player.pickItem("Lomotacz");

    enemy1.takeDamage(40); 

    bus.processEvents();

    return 0;
}

/*
    Kontynuując wątek z maila, zmienimy implementację na bardziej optymalną.
    A przy okazji mamy szansę przećwiczyć rzeczy, które są często pomijane a warto je mieć w swoim repertuarze. :)

    Proszę:

        1) Zmienić sposób przechowywania wyemitowanych eventów tak, by nie opierał się o dziedziczenie,
        a o unię typów. Proszę użyć klasycznego "tagged union" (enum + unia). **

            * Takie podejście (częściowo) rozwiązuje nasz problem ze skakaniem po wskaźnikach.
            Unia jest już typem, który znajduje się w konkretnym miejscu (ale ma rozmiar mogący pomieścić wszystkie warianty) i nie wymaga ciągłego szukania danych w RAM.
            
            * Częściowo rozwiązuje dlatego, że użyta przez nas kolejka nie jest w pełni ciągła w pamięci. W rzeczywistoście queue jest domyślnie oparte o deque (jest tzw. adapterem),
            które przechowuje dane w ciągłych, ale dość małych, blokach/chunkach, a "główna część" kolejki jest mapą, która indeksuje te bloki.
            Czyli rozwiązujemy problem lokalności danych jedynie na poziomie pojedynczych chunków, co dalej nie jest idealnym rozwiązaniem.  
            
            W praktyce, użylibyśmy raczej cyklicznego bufora (ring buffer), albo tzw. podwójnego buforowania (double buffering),
            tzn. dwóch równoległych std::vector, gdzie do jednego emitujemy eventy, a z drugiego rozsyłamy je do subskrybentów, po czym robimy swap 
            (a ten jest bardzo tani, bo tylko wymieniamy wskaźniki i kilka metadanych - nie musimy w ogóle nic kopiować!). 
            Obydwa rozwiązania dają nam pełną ciągłość w pamięci.

            * Uwaga na std::string. Wkładanie go do unii jest zazwyczaj średnim pomysłem, dlatego, że unia nie wywołuje automatycznie konstruktora/destruktora,
            więc musimy ręcznie zadbać o jego czas życia.

        2) Umożliwić subskrybentom nasłuchiwanie konkretnych typów eventów, które podają przy wywoływaniu metody subscribe.
        Filtrowanie powinno się odbywać na poziomie EventBus, tzn. subskrybenci powinni być informowani jedynie o eventach z kategorii, których nasłuchują.

            * Najbardziej eleganckim rozwiązaniem jest maska bitowa dla każdego subskrybenta, oparta o enum kategorii.
            (ewentualnie std::bitset, ale ma overhead, więc warto umieć się posługiwać klasyczną maską)

            Przypominam, że n-ty bit integera (int maska = 0) można uaktywnić robiąc "maska |= (1 << n)" i sprawdzić "maska & (1 << n)".
            A z kolei by porównać zgodność dwóch masek, wystarczy zrobić maska1 & maska2.

            * Byłoby dobrze mieć zestaw kategorii oddzielny od samych typów eventów i bardziej zbiorczy. Na przykład, eventami mogą być 
            śmierć przeciwnika, zranienie go, leczący się przeciwnik itd., a kategorią do subskrybcji - eventy dotyczące przeciwnika.


        ** W bardziej współczesnym podejściu, użylibyśmy std::variant i std::visit, ale wymagają szablonów i lambdy.
*/