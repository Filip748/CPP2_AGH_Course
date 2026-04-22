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

#include "Tensor.h"

int main() {

    Tensor t({2, 3, 4}, true); // wymiary + zerowanie pamięci

    std::cout << "Przypisanie i dostęp przez operator()\n" << std::endl;
    t({1, 2, 3}) = 42;
    std::cout << "t({1, 2, 3}) = 42: " <<  t({1, 2, 3}) << std::endl;

    // Wypełnienie całej pamięci wzorem
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            for (size_t k = 0; k < 4; ++k) {
                t({i,j,k}) = i * 100 + j * 10 + k;
            }
        }
    }


    std::cout << "\n\nTest wypełnienia\n" << std::endl;
    std::cout << "Powinniśmy zobaczyć (z dowolnym formatowaniem :)):" << std::endl;
    std::cout << "wymiary - 2, 3, 4" << std::endl;
    std::cout << "stridy - 12, 4, 1" << std::endl;
    std::cout << "dane (pionowe kreski oznaczają stridy, pomijając ten jednostkowy):" << std::endl;
    std::cout << "| |000, 001, 002, 003|, |010, 011, 012, 013|, |020, 021, 022, 023| |" << std::endl;
    std::cout << "| |100, 101, 102, 103|, |110, 111, 112, 113|, |120, 121, 122, 123| |" << std::endl;
    std::cout << "\n\nTest printDebugInfo()\n" << std::endl;

    t.printDebugInfo();


    std::cout << "\n\nTest widoku\n" << std::endl;
    std::cout << "Utworzenie, wypisanie i nadpisanie przez widok (jedna redukcja)" << std::endl;

    auto t_view2D = t[1];
    std::cout << "t_view2D({2, 3}) = 100 + 20 + 3 = 123? : " << t_view2D({2, 3}) << std::endl;
    
    t_view2D({2, 3}) = 24;
    std::cout << "t_view2D({2, 3}) = 24? : " << t_view2D({2, 3}) << std::endl;
    std::cout << "t({1, 2, 3}) = 24? : " << t({1, 2, 3}) << std::endl;

    
    std::cout << "\n\nTest widoku - redukcja do 1 wymiaru\n" << std::endl;

    auto t_view1D = t[1][2];
    std::cout << "t_view1D({1}) = 100 + 20 + 1 = 121? : " << t_view1D({1}) << std::endl;
    std::cout << "t_view1D(1) = 100 + 20 + 1 = 121? : " << t_view1D(1) << std::endl;

    t_view1D(1) = 42;
    std::cout << "t_view1D(1) = 42? : " << t_view1D(1) << std::endl;
    std::cout << "t({1, 2, 1}) = 42? : " << t({1, 2, 1}) << std::endl;


    std::cout << "\n\nTest konwersji widoku na tensor 2D\n" << std::endl;

    auto t_2D_from_view = t[0].asTensor(); 
    std::cout << "Powinniśmy zobaczyć (z dowolnym formatowaniem :)):" << std::endl;
    std::cout << "wymiary - 3, 4" << std::endl;
    std::cout << "stridy - 4, 1" << std::endl;
    std::cout << "dane (pionowe kreski oznaczają stridy, pomijając ten jednostkowy):" << std::endl;
    std::cout << "| |000, 001, 002, 003|, |010, 011, 012, 013|, |020, 021, 022, 023| |" << std::endl;
    std::cout << "\n\nprintDebugInfo()\n" << std::endl;
    t_2D_from_view.printDebugInfo();


    std::cout << "\n\nTest konwersji widoku na tensor 1D\n" << std::endl;
    auto t_1D_from_view = t[0][1].asTensor();
    std::cout << "Powinniśmy zobaczyć (z dowolnym formatowaniem :)):" << std::endl;
    std::cout << "wymiary - 4" << std::endl;
    std::cout << "stridy - 1" << std::endl;
    std::cout << "dane: 010, 011, 012, 013" << std::endl;
    std::cout << "\n\nprintDebugInfo()\n" << std::endl;
    t_1D_from_view.printDebugInfo();

}

/** 
Ponieważ ustaliliśmy, że będzie wektor i/lub macierz (a to byłoby jednak za dużo pisania), 
to zrobimy wydajną reprezentację tensora, którego wektor i macierz są szczególnymi przypadkami.

A tak całkiem poważnie, wydajną (liniową w pamięci) reprezentację dowolnych wielowymiarowych danych,
ale żeby nie bawić się już strukturami, niech nazywa się tensor i przechowuje integery, bo będą się ładniej wypisywać. *

Nie będziemy, oczywiście, implementować żadnych działań, ale skupimy się na widoku, który pozwala "zdejmować" kolejne wymiary (przy użyciu operatora []) tak,
żebyśmy mieli naturalne indeksowanie. Nie da się tego zrobić w pełni idealnie, bo operator[] nie może zwracać widoku albo elementu,
więc jako kompromis przyjmiemy, że operator() służy do wyciągania konkretnego elementu, a operator[] do redukcji wymiaru o 1.


Na przykład, zakładając 3 wymiary, {i, j, k},

    tensor({1,2,3})

zwróci referencję do elementu pod indeksami 1,2,3, ale już:

    tensor[i][j][k]

uznamy za undefined behavior w ramach tego interfejsu.
Innymi słowy, jesteśmy w stanie zredukować całość do 1 wymiaru, ale nie do 1 elementu.

Z drugiej strony,

    tensor[i][j],

zwróci view pojedynczej kolumny/wektora, gdzie konkretny element możemy wyciągnąć przez:

    view({5})
    view(5) // zdefiniowane tylko dla wymiaru 1D

lub, w połączeniu,

    tensor[1][2](3)

a

    tensor[i]

zwróci view macierzy 2D, gdzie analogicznie zadziała:

    view({1, 3})
    tensor[1]({1, 3})
    
ale 

    view(5)

będzie niezdefiniowane.




Nie przejmujemy się potencjalnymi problemami z zakresami w finalnej wersji (tzn. release),
ale w kodzie powinny pojawić się odpowiednie asserty, które uniemożliwiają wykorzystanie niezdefiniowanych ścieżek.


Potrzebujemy:

    1) Klasy tensora, która:

        * reprezentuje obiekt o dowolnej ilości wymiarów, przechowujący dane typu int
        * dane są przechowywane w liniowy sposób, w jednowymiarowej tablicy

            Ponieważ potrzebujemy ogólności pod względem ilości wymiarów, tensor musi wiedzieć dwie rzeczy:

                * ile jest wymiarów i ile indeksów przyjmuje każdy z nich (od 0 do jakiegoś n_i),
                * jak daleko powinniśmy się przemieścić w bloku pamięci, jeżeli i-ty indeks zmieni się o 1
            
            Taki dystans w pamięci nazywa się "stride". Robiliśmy to już w przypadku 2D, gdzie też mieliśmy dwa takie "stride":
                - stride drugiego (ostatniego) wymiaru było równe 1 - bo zmiana najbardziej zewnętrznego indeksu (kolumny) o 1 przesuwała nas o 1 element w pamięci
                - stride pierwszego wymiaru wynosiło "liczba kolumn" - bo zmiana pierwszego indeksu (wiersza), przesuwała nas w pamięci o "liczba kolumn" elementów

            Tutaj, musimy to pojęcie uogólnić, domnażając wielkości kolejnych wymiarów. 
            Załóżmy, że mamy tensor o wymiarach {2, 5, 4, 3} - wtedy, idąc od tyłu, mamy kolejne stridy:

                - 1 dla ostatniego wymiaru
                - 1 * 3 dla przedostatniego
                - 1 * 3 * 4 dla wcześniejszego
                - 1 * 3 * 4 * 5 dla jeszcze wcześniejszego

            Zbierając wszystko (kolejność indeksów jest wszędzie ta sama):

                tensor[i][j][k][l] - wywołanie
                {i, j, k, l} - indeksy
                {2, 5, 4, 3} - odpowiadające im wymiary
                {60, 12, 3, 1} - wielkość kroku w pamięci, po zmianie indeksu o 1

            Innymi słowy, zakładamy, że indeks najbardziej z prawej robi najmniejszy krok w pamięci (a więc ten wymiar jest ułożony liniowo).

        
        Interfejs:

            * Konstrukcja wymaga podania wymiarów. Można zdecydować o zerowaniu pamięci flagą.

            * Metoda printDebugInfo() do testów, która powinna wypisać:
                * rozmiary wszystkich wymiarów i ich ilość
                * wszystkie stridy
                * cały ciągły blok pamięci (jednym ciągiem, bez przejmowania się wymiarami) 

            * Operator() pozwala nam uzyskać dostęp do konkretnego elementu (typu int), po podaniu jego indeksów we wszystkich wymiarach (jako wektor)
                - Zakładamy, że ilość przekazanych indeksów zawsze będzie poprawna. 
                - Czyli,
                    tensor({1,2,3})
                zwraca nam referencję do wartości pod indeksami 1, 2, 3.

            * Operator() można też wywołać z jednym indeksem, ale operacja jest prawidłowa tylko, jeśli tensor ma jeden wymiar.

            * Operator[] przyjmuje jeden indeks, który jest podstawiony za ten najbardziej z lewej (robiący największe skoki w pamięci) i efektywnie redukuje ten wymiar,
            To znaczy, tworzy i zwraca obiekt TensorView, któy widzi tensor jakby wymiar był o 1 mniejszy i wie jak się po nim przemieszczać.
            
                - Czyli jeśli tensor ma indeksy i,j,k, i wywołamy na nim tensor[2], to dostaniemy view reprezentujący tensor[2][j][k]
                
            Tyle jest nam potrzebne, żeby utworzyć pierwszy widok oraz mieć możliwość bezpośredniego wyciągania wartości z tensora.
            View napiszemy tak, żeby dało się też zrobić tensor[2][1] (czyli tensor[2][1][k]), itd.
            
            
    2) Klasy TensorView ^

        1) Jak zwykle, widzi dane tensora, ale nie ma nad nimi własności.

        2) Pod wszystkimi względami, zachowuje się dokładnie tak jak tensor, z którego powstał, ale gdzie pierwszy wymiar przyjął konkretną wartość.

        3) Posiada metodę asTensor(), która konwertuje to, co widzi dany view, w nowy tensor i zwraca go.





============== to na później :) =================


^ Jeszcze krótki komentarz - dlaczego view, a nie zwracanie mniejszego tensora?
Widok z definicji nie posiada kontroli nad danymi, które pokazuje (mówi się że jest non-owning). Z czego automatycznie wynika też pewien kontrakt:
"dam ci wydajny dostęp do danych, pozwolę nawet zmodyfikować konkretne elementy, ale nie pozwolę na zmianę samego tensora jako obiektu (np. jego wymiarów)".


* Trochę C++ lore, bo wcześniej zapomniałem. :)
Oczywiście, jeśli zadajemy sobie trud trzymania wszystkiego w jednowymiarowej tablicy, to nie chcemy w przechowywanej strukturze niczego, co robiłoby dodatkową alokację dla każdego elementu.

Ale std::string jest dość ciekawym przypadkiem, bo niekoniecznie musi się znaleźć na stercie. Większość implementacji używa tzw. SSO (Small String Optimization),
które sprowadza się do tego (w dużym uproszczeniu, bo są tam różne inne tricki), że string jest zaimplementowany jako unia takiego klasycznego {size_t size, size_t capacity, char* data} i małego "statycznego" bufora {size_t size, char data[20]}.
Dynamiczna alokacja jest używana dopiero, kiedy zawartość nie mieści się w buforze na stosie. Przy czym, 20 jest tu losową liczbą i w rzeczywistości zależy od implementacji/kompilatora/platformy.

Natomiast, umieszczenie klasy/struktury wewnątrz naszej przechowywanej struktury ma jeszcze jedną, dość poważną, konwekwencję - każde pole, będące klasą/strukturą, musi zostać skonstruowane. Zatem, jeżeli przechowujemy tak string,
to niezależnie od tego czy struktura ma dedykowany konstruktor, konstruktor domyślny, czy konstruktor domniemany, ten string musi być skonstruowany. Jeżeli nie zrobimy tego ręcznie, to wywoła się jego domyślny konstruktor. 

Z drugiej strony, jeżeli mamy klasę/strukturę która ma tylko trywialne pola (czyli struktury/klasy, które same też mają tylko trywialne pola, albo w ogóle nie ma struktur/klas), nie ma żadnego konstruktora, 
ani metody wirtualnej (bo wtedy potrzebne byłoby vtable),
na przykład:

struct Pixel {

    double r;
    double g;
    double b;
};

to mówimy, że jest "trivially constructible". W tym sensie, że kompilator wygeneruje konstruktor domniemany, który nie robi nic (bo, z jego definicji, pozostawia pola trywialnych typów niezainicjalizowanymi).
A skoro nie robi nic, to nie wyemituje też żadnego kodu maszynowego w czasie kompilacji.

Czasem ma to znaczenie, jeżeli niekoniecznie chcemy inicjalizować pamięć, bo new wywołuje konstruktory domyślne elementów.
Przy strukturze zdefiniowanej jak wyżej,

    new Pixel[N]

to efektywnie tylko malloc, bo konstrukcja zostanie pominięta (albo nie tyle pominięta - bo formalnie konstruktor będzie wywołany - co nie będzie skutkować żadnym kodem maszynowym).
A jeśli potrzebujemy, pamięć możemy zawsze wyzerować lub nadpisać sami.

Z trzeciej strony :), nawet jeśli zdefiniujemy prosty konstruktor, np.

struct Pixel {

    double r = 0;
    double g = 0;
    double b = 0;

    Pixel() = default;
};

albo

struct Pixel {

    double r;
    double g;
    double b;

    Pixel() : r(0), g(0), b(0) {}
};

to kompilator powinien zdać sobie sprawę, że to zwykłe zerowanie pamięci i zwektoryzować kod (chociaż nie dam ani gwarancji, ani nic sobie uciąć :D).
Tym bardziej, że taki konstruktor zostanie prawie na pewno zinlinowany.


Problem może się pojawić, gdyby konstruktor był bardziej skomplikowany i nie został zamieniony na inline,
bo wtedy dochodzi nam bariera wywołania funkcji i kompilator może sobie nie poradzić z optymalizacją.
Może się w takiej sytuacji okazać, że lepiej nie pisać konstruktora, tylko zrobić to samo w pętli, już po utworzeniu tablicy.
*/