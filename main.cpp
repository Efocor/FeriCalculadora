// main.cpp
#include "Calculator.hpp"

int main() {
    try {
        Calculator calc;
        calc.run();
    }
    catch (const std::exception& e) {
        std::abort;
        return 1;
    }
    
    return 0;
}

//g++ -std=c++11 -o calculator main.cpp Calculator.cpp -lsfml-graphics -lsfml-window -lsfml-system

// g++ -o calculator main.cpp calculator.cpp -I /msys64/mingw64/include -L /msys64/mingw64/lib -lsfml-graphics -lsfml-window -lsfml-system