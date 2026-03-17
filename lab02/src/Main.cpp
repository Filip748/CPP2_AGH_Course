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
#include <cstdlib>
#include <ctime>

#include "Image.h"

int main() {

    std::srand(std::time(nullptr));

    Image A(3, 3);
    Image B(3, 3);

    for(int y=0; y<3; y++) {
        for(int x=0; x<3; x++) {
            A(x,y) = {static_cast<unsigned char>(rand() % 256), static_cast<unsigned char>(rand() % 256), static_cast<unsigned char>(rand() % 256), 255};
            B(x,y) = {static_cast<unsigned char>(rand() % 256), static_cast<unsigned char>(rand() % 256), static_cast<unsigned char>(rand() % 256), 255};
        }
    }

    std::cout << "Obraz A:\n" << A << "\n";
    std::cout << "Obraz B:\n" << B << "\n";



    std::cout << "=== Konstruktor kopiujący ===\n";
    Image C = A;
    std::cout << "C jako kopia A:\n" << C << "\n";
    std::cout << "C == A?" << (C == A ? " tak" : " nie") << "\n";

    A(0,0) = {255,0,0,255};
    std::cout << "Po zmianie A(0,0) na czerwony:\n";
    std::cout << "A(0,0): " << A(0,0) << "\n";
    std::cout << "A:\n" << A << "\n";
    std::cout << "C:\n" << C << "\n";    
    std::cout << "C == A?" << (C == A ? " tak" : " nie") << "\n";



    std::cout << "=== Operator kopiujący ===\n";
    Image D(5,5);
    D = B;
    std::cout << "D jako kopia B:\n" << D << "\n";
    std::cout << "D == B?" << (D == B ? " tak" : " nie") << "\n";

    B(0,0) = {255,0,0,255};
    std::cout << "Po zmianie B(0,0) na czerwony:\n";
    std::cout << "B(0,0): " << B(0,0) << "\n";
    std::cout << "B:\n" << B << "\n";
    std::cout << "D:\n" << D << "\n";      
    std::cout << "D == B?" << (D == B ? " tak" : " nie") << "\n";



    std::cout << "=== Test dodawania i odejmowania ===\n";
    Image G = A + B;
    std::cout << "A + B:\n" << G << "\n";

    Image H = A - B;
    std::cout << "A - B:\n" << H << "\n";

    G += H;
    std::cout << "G += H\n" << G << "\n";

    H -= G;
    std::cout << "H -= G\n" << H << "\n";


    std::cout << "=== Test skalowania jasności ===\n";
    Image I = A * 0.5;
    std::cout << "A * 0.5:\n" << I << "\n";

    I *= 5;
    std::cout << "I *= 5:\n" << I << "\n";

    std::cout << "=== Test konkatenacji ===\n";
    Image J = A | B;
    std::cout << "A | B:\n" << J << "\n";
    
    J |= A;
    std::cout << "J |= A\n" << J << "\n";

    /*  // opcjonalnie :)
        Image Obraz(25, 25);
        Obraz.pprint();
    */

    return 0;
}

/** 
Napiszemy klasę reprezentującą obraz 2D o dowolnym rozmiarze (w pikselach).

Założenia: 

1) Piksel reprezentowany jest przez cztery liczby (od 0 do 255) - r, g, b, a.
2) Obraz przechowywany jest wewnętrznie jako 1-wymiarowa tablica (data locality! dynamiczna tablica 2D -> ciągłe skakanie po pamięci)
    * można przyjąć dowolną konwencję przechowywania (tzn. row-major albo column-major)
    * podobnie można przyjąć dowolną konwencję indeksowania (chociaż najczęściej robi się to od lewego dolnego rogu)
3) Obraz implementuje:

    Uwaga - jako, że wyjątki się jeszcze oficjalnie nie pojawiły, to zakładamy, że rozmiary zawsze mają sens.

    * kopiowanie i przenoszenie
    * dostęp do piksela po podaniu indeksów 
        - na dwa sposoby - umożliwiający i nieumożliwiający edycję
    * porównanie dwóch obrazów (true jeśli wszystkie piksele są identyczne)
    * dodawanie i odejmowanie dwóch obrazów 
        - clamp na 0-255
        - na dwa sposoby - zwracający nowy obraz i "w miejscu"
    * skalowanie jasności (tzn. wszystkich składowych pikseli) przez współczynnik
        - clamp na 0-255
        - na dwa sposoby - zwracający nowy obraz i "w miejscu"
    * tzw. konkatenacje poziomą, czyli ustawienie dwóch obrazów obok siebie
        - nowy obraz ma szerokość równą szerokość1 + szerokość2 oraz wysokość max(wysokość1, wysokość2)
        - lewa część jest wypełniona przez obraz1, prawa przez obraz2
        - na dwa sposoby - zwracający nowy obraz i "w miejscu"
    * wypisanie na strumień standardowy
        - wystarczy wypisanie "(r, g, b, a)" wszystkich pikseli
    
        
    *** gdyby ktoś się bardzo nudził :), to może doimplementować metodę konwencjonalnie nazywaną pprint (od pretty print)
        kolorowe ASCII można wypisywać stosując tzw. ASCII escape codes (zaczynające się od \033[, czyli ESC[);
        u nas byłoby to:
            std::cout << "\033[48;2;"
                      << (int)r << ";"
                      << (int)g << ";"
                      << (int)b
                      << "m  ";      
                      
            gdzie "\033[48;2;R;G;Bm" ustawia kolor tła (background color; 48), a spacja oznacza puste pole, czyli efektywnie kwadracik w danym kolorze.
            2 oznacza 24-bitowy format koloru (tzw. truecolor)

            Gdybyśmy chcieli rysować gwiazdki * w danym kolorze, musimy ustawić foreground color (38) - "\033[38;2;R;G;Bm*" 

            Na koniec wypisywania powinniśmy jeszcze zresetować całość do domyślnych wartości (inaczej terminal zostanie w ostatnim użytym trybie):
            std::cout << "\033[0m\n";
*/