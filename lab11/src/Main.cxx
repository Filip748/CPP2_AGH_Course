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

#include "DocumentRenderers.h"


int main() {

    // plain text renderer

    std::cout << "\n\nPlain text renderer:\n";

    std::cout << "Oczekujemy:\n\nhello, world!\n\n"; // znormalizowane do lowercase

    PlainTextRenderer plainTextRenderer;
    std::cout << plainTextRenderer.render("Hello, World!") << std::endl << std::endl;


    // html renderer

    std::cout << "\n\nHTML renderer:\n";

    std::cout << "Oczekujemy:\n\n";

    std::cout << "<html>\n";
    std::cout << "<body>\n";
    std::cout << "moja pierwsza strona o oop\n"; // znormalizowane i opakowane
    std::cout << "</body>\n";
    std::cout << "</html>\n\n";


    HtmlRenderer htmlRenderer;
    std::cout << htmlRenderer.render("Moja pierwsza strona o OOP") << std::endl << std::endl;


    // header decorator

    std::cout << "\n\nHeader decorator:\n";

    std::cout << "Oczekujemy:\n\n";

    std::cout << "<!-- Wolałbym proceduralnie -->\n";
    std::cout << "<html>\n";
    std::cout << "<body>\n";
    std::cout << "moja pierwsza strona o oop\n"; // znormalizowany + opakowany + header
    std::cout << "</body>\n";
    std::cout << "</html>\n\n";


    HeaderDecorator headerDecorator("<!-- Wolałbym proceduralnie -->", htmlRenderer);
    std::cout << headerDecorator.render("Moja pierwsza strona o OOP") << std::endl << std::endl;


    // logging decorator

    std::cout << "\n\nLogging decorator:\n";

    std::cout << "Oczekujemy (mniej więcej):\n\n";

    std::cout << "[LOG] Rendering document with content:\nHello, World!\n"; // przekazany kontent
    std::cout << "[LOG] Result is: \nhello, world!\n\n"; // wyrenderowany do tego etapu

    std::cout << "hello, world!\n\n";

    LoggingDecorator loggingDecorator(plainTextRenderer);
    std::cout << loggingDecorator.render("Hello, World!") << std::endl << std::endl;


    // zaszalejmy

    std::cout << "\n\nHeader + logging decorator:\n";
    std::cout << "Oczekujemy (mniej więcej):\n\n";

    std::cout << "[LOG] Rendering document with content:\nHello, World!\n"; // logger jest najbardziej zewnętrzny, dostaje kontent z maina
    std::cout << "[LOG] Result is: \nHello, \nhello, world!\n\n"; // Header nie przechodzi przez normalizację

    std::cout << "Hello, \nhello, world!\n\n";

    HeaderDecorator combinedHeader("Hello,", plainTextRenderer);
    LoggingDecorator combinedLogging(combinedHeader);
    std::cout << combinedLogging.render("Hello, World!") << std::endl << std::endl;
}



/** 
    Więcej OOPowych śmiesznostek. Dekoratory przez dziedziczenie. Brace yourselves.
    Na końcu pokażę implementację bez OOP.
    

    Mamy bardzo klasyczny dla OOP setup (już przestaję, przepraszam):
    
        1) Wspólny interfejs do renderowania dokumentów.
        Niech będzie IDocumentRenderer.

            * U nas będzie służył do wymuszenia implementacji metody render,
            która przyjmuje jakiś kontent do wyrenderowania.


        2) Nad nim, mamy bazę do renderowania, która dostarcza metodę normalizującą
        kontent do formy zjadliwej dla wszystkich pochodnych rendererów.

            * Na zasadzie podobnej do warstwy HTTP ostatnio
            * Na potrzeby zadania, przyjmijmy, że normalizacja oznacza konwersję na lowercase

            * Sama klasa ciągle jest abstrakcyjna


        3) Nad bazą, mamy dwa różne renderery, które dostarczają konkretne implementacje metody render.

            - PlainTextRenderer, który tylko zwraca znormalizowany kontent
            - HtmlRenderer, który oprócz normalizacji, dodaje na początku i końcu tagi:
                <html>
                <body>

                </body>
                </html>


        Czyli:

            IDocumentRenderer
                ^
                |
            BaseDocumentRenderer
                ^               ^
                |               |
        PlainTextRenderer   HtmlRenderer



    Dołożymy teraz do tej hierarchii dwa proste dekoratory:

        - dodający header na początku dokumentu
        - dodający logowanie renderowania na terminal


    Musimy się oczywiście wpiąć w hierarchię, więc potrzebujemy też bazy, np. RendererDecorator.
    Wpięcie musi być na poziomie IDocumentRenderer, żebyśmy mogli dowolnie łączyć dekoratory i renderery.

    Nasza baza musi jedynie przechować wskaźnik/referencję do dekorowanego renderera.
    Ponieważ dziedziczymy z IDocumentRenderer, kwestię abstrakcyjności i wymuszania implementacji render mamy załatwioną.


    Dalej w hierarchii, musimy mieć jeden dekorator, który do zwracanego wyrenderowanego tekstu dodaje
    na początku przekazany header. Oraz drugi, który przed zwróceniem wyrenderowanego tekstu, wypisze log na konsolę.


    Czyli, ignorując poprzednie renderery, mamy dodatkowo:

        IDocumentRenderer
                ^
                |
                RendererDecorator
                ^               ^
                |               |
    HeaderDecorator   LoggingDecorator





Szkic (uproszczony :)) wersji funkcyjnej, bez OOP 


    using Renderer = std::function<std::string(std::string const&)>;

    Renderer plainTextRenderer() {
        return [](std::string const& content) {
            return content;
        };
    }

    Renderer htmlRenderer() {
        return [](std::string const& content) {
            return "<html>\n<body>\n" + content + "\n</body>\n</html>";
        };
    }


dekoratory:

    Renderer addHeader(std::string const& header, Renderer renderer) {
        return [header, renderer](std::string const& content) {
            return header + "\n" + renderer(content);
        };
    }

    Renderer addLogging(Renderer renderer) {
        return [renderer](std::string const& content) {
            std::cout << "[LOG] Rendering document with content:\n '" << content << "'\n";
            return renderer(content);
        };
    }

użycie:

    auto udekorowany = addLogging(addHeader("Hello,", plainTextRenderer()));
    std::cout << udekorowany("Hello, World!") << std::endl;


std::function można też podpiąć wszędzie, nawet jeśli pochodzi z pluginów.
Wystarczy, żeby sygnatura się zgadzała.



Swoją drogą, w czystym C też się da, tylko trzeba zrobić closure samemu.
Nie będę pisał całości, bo tam zdecydowanie za dużo zabawy ze stringiem :), ale renderer wyglądałby jakoś tak:

typedef char* (*RenderFunction)(void* context, const char* content);

typedef struct Renderer {

    RenderFunction render;
    void* context;
    
} Renderer;

Wtedy pod render mamy wskaźnik na funkcję ostatniego poziomu, a jako context całą wcześniejszą historię.
Z wywołaniem na zasadzie:

ren.render(ren.context, "Hello, World!")


Przy czym, każdy dektorator musi też mieć swoją wersję kontekstu i trzymać ją na heapie,
np.

typedef struct HeaderContext{

    char* header;
    Renderer renderer;

} HeaderContext;


A że każdy ma swoją własną funkcję, to każdy wie na co rzutować void*.


char* renderHeader(void* context, const char* content) {

    HeaderContext* head = context;

    char* inner = head->renderer.render( 
        head->renderer.context, content
    );

    // 50 linijek obrabiania stringa, ponieważ C

    return result;
}

Renderer addHeader(const char* header, Renderer renderer) {

    HeaderContext* head = malloc(sizeof(HeaderContext));

    head->header = header;
    head->renderer = renderer;

    Renderer r = {
        .render = renderHeader,
        .context = head
    };

    return r;
}

Zresztą, std::function to pod spodem mniej więcej to samo co Renderer wyżej.
*/