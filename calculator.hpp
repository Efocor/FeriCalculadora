// Calculator.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <functional>
#include <map>
#include <cmath>

class Calculator {
private:
    struct Button {
        sf::RectangleShape shape;
        sf::Text text;
        std::string value;
        std::function<void()> action;
    };

    // Ventana principal
    sf::RenderWindow window;
    sf::Font font;
    
    // Display
    sf::RectangleShape displayBox;
    sf::Text displayText;
    std::string currentExpression;
    std::string result;
    
    // Botones
    std::vector<Button> buttons;
    
    // Estado
    bool isNewCalculation;
    
    // Mapa de funciones matemáticas
    std::map<std::string, std::function<double(double)>> mathFunctions;
    
    void initWindow();
    void initDisplay();
    void initButtons();
    void initMathFunctions();
    void handleInput();
    void update();
    void render();
    void calculateResult();
    double evaluateExpression(const std::string& expr);
    
public:
    Calculator();
    void run();
};
