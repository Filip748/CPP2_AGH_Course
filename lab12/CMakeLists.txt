cmake_minimum_required(VERSION 3.10)

# Nazwa projektu i język (C++)
project(DocumentRenderers LANGUAGES CXX)

# Wymuszamy nowoczesny standard C++ (C++17 to bezpieczny i solidny wybór)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Tworzymy plik wykonywalny ze źródeł z katalogu src/
add_executable(DocumentRenderers
    src/Main.cxx
)

# Mówimy kompilatorowi, gdzie ma szukać plików nagłówkowych (.h)
target_include_directories(DocumentRenderers PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Dodajemy wymagane przez prowadzącego flagi z warningami (dla GCC i Clang)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(DocumentRenderers PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()