// Calculator.cpp
#include "Calculator.hpp"
#include <sstream>

Calculator::Calculator() : window(sf::VideoMode(800, 600), "Scientific Calculator") {
    if (!font.loadFromFile("arial.ttf")) {
        throw std::runtime_error("No se pudo cargar la fuente");
    }
    
    initWindow();
    initDisplay();
    initButtons();
    initMathFunctions();
    isNewCalculation = true;
}

void Calculator::initWindow() {
    window.setFramerateLimit(60);
}

void Calculator::initDisplay() {
    displayBox.setSize(sf::Vector2f(700, 80));
    displayBox.setPosition(50, 50);
    displayBox.setFillColor(sf::Color(30, 30, 30));
    displayBox.setOutlineThickness(2);
    displayBox.setOutlineColor(sf::Color::White);
    
    displayText.setFont(font);
    displayText.setCharacterSize(24);
    displayText.setFillColor(sf::Color::White);
    displayText.setPosition(60, 65);
}

void Calculator::initButtons() {
    const float buttonWidth = 80.f;
    const float buttonHeight = 60.f;
    const float spacing = 10.f;
    const float startX = 50.f;
    const float startY = 150.f;
    
    // Números
    std::vector<std::string> numbers = {"7", "8", "9", "4", "5", "6", "1", "2", "3", "0", ".", "="};
    for (size_t i = 0; i < numbers.size(); ++i) {
        Button btn;
        btn.shape.setSize(sf::Vector2f(buttonWidth, buttonHeight));
        btn.shape.setPosition(
            startX + (i % 3) * (buttonWidth + spacing),
            startY + (i / 3) * (buttonHeight + spacing)
        );
        btn.shape.setFillColor(sf::Color(50, 50, 50));
        
        btn.text.setFont(font);
        btn.text.setString(numbers[i]);
        btn.text.setCharacterSize(20);
        btn.text.setFillColor(sf::Color::White);
        btn.text.setPosition(
            btn.shape.getPosition().x + buttonWidth/2 - btn.text.getGlobalBounds().width/2,
            btn.shape.getPosition().y + buttonHeight/2 - btn.text.getGlobalBounds().height/2
        );
        
        btn.value = numbers[i];
        
        if (numbers[i] == "=") {
            btn.action = [this]() { calculateResult(); };
        } else {
            btn.action = [this, i, numbers]() {
                if (isNewCalculation) {
                    currentExpression.clear();
                    isNewCalculation = false;
                }
                currentExpression += numbers[i];
                displayText.setString(currentExpression);
            };
        }
        
        buttons.push_back(btn);
    }
    
    // Operadores básicos
    std::vector<std::string> operators = {"+", "-", "*", "/", "^", "√"};
    float operatorStartX = startX + 4 * (buttonWidth + spacing);
    
    for (size_t i = 0; i < operators.size(); ++i) {
        Button btn;
        btn.shape.setSize(sf::Vector2f(buttonWidth, buttonHeight));
        btn.shape.setPosition(operatorStartX, startY + i * (buttonHeight + spacing));
        btn.shape.setFillColor(sf::Color(70, 70, 70));
        
        btn.text.setFont(font);
        btn.text.setString(operators[i]);
        btn.text.setCharacterSize(20);
        btn.text.setFillColor(sf::Color::White);
        btn.text.setPosition(
            btn.shape.getPosition().x + buttonWidth/2 - btn.text.getGlobalBounds().width/2,
            btn.shape.getPosition().y + buttonHeight/2 - btn.text.getGlobalBounds().height/2
        );
        
        btn.value = operators[i];
        btn.action = [this, i, operators]() {
            if (!currentExpression.empty()) {
                currentExpression += " " + operators[i] + " ";
                displayText.setString(currentExpression);
            }
        };
        
        buttons.push_back(btn);
    }
    
    // Funciones matemáticas
    std::vector<std::string> functions = {"sin", "cos", "tan", "ln", "exp", "sqrt"};
    float functionStartX = operatorStartX + buttonWidth + spacing;
    
    for (size_t i = 0; i < functions.size(); ++i) {
        Button btn;
        btn.shape.setSize(sf::Vector2f(buttonWidth, buttonHeight));
        btn.shape.setPosition(functionStartX, startY + i * (buttonHeight + spacing));
        btn.shape.setFillColor(sf::Color(70, 70, 70));
        
        btn.text.setFont(font);
        btn.text.setString(functions[i]);
        btn.text.setCharacterSize(20);
        btn.text.setFillColor(sf::Color::White);
        btn.text.setPosition(
            btn.shape.getPosition().x + buttonWidth/2 - btn.text.getGlobalBounds().width/2,
            btn.shape.getPosition().y + buttonHeight/2 - btn.text.getGlobalBounds().height/2
        );
        
        btn.value = functions[i];
        btn.action = [this, i, functions]() {
            currentExpression += functions[i] + "(";
            displayText.setString(currentExpression);
        };
        
        buttons.push_back(btn);
    }
}

void Calculator::initMathFunctions() {
    mathFunctions["sin"] = [](double x) { return std::sin(x); };
    mathFunctions["cos"] = [](double x) { return std::cos(x); };
    mathFunctions["tan"] = [](double x) { return std::tan(x); };
    mathFunctions["ln"] = [](double x) { return std::log(x); };
    mathFunctions["exp"] = [](double x) { return std::exp(x); };
    mathFunctions["sqrt"] = [](double x) { return std::sqrt(x); };
}

void Calculator::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            
            for (auto& button : buttons) {
                if (button.shape.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    button.action();
                    break;
                }
            }
        }
    }
}

void Calculator::update() {
    // Actualizar posición del texto en el display
    displayText.setPosition(
        displayBox.getPosition().x + 10,
        displayBox.getPosition().y + displayBox.getSize().y/2 - displayText.getGlobalBounds().height/2
    );
}

void Calculator::render() {
    window.clear(sf::Color(20, 20, 20));
    
    // Renderizar display
    window.draw(displayBox);
    window.draw(displayText);
    
    // Renderizar botones
    for (const auto& button : buttons) {
        window.draw(button.shape);
        window.draw(button.text);
    }
    
    window.display();
}

double Calculator::evaluateExpression(const std::string& expr) {
    // Esta es una implementación básica. Para una calculadora real,
    // necesitarías un parser más sofisticado
    std::istringstream iss(expr);
    std::vector<std::string> tokens;
    std::string token;
    
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    // Evaluación simple para demostración
    if (tokens.size() == 1) {
        return std::stod(tokens[0]);
    }
    else if (tokens.size() == 3) {
        double a = std::stod(tokens[0]);
        double b = std::stod(tokens[2]);
        std::string op = tokens[1];
        
        if (op == "+") return a + b;
        if (op == "-") return a - b;
        if (op == "*") return a * b;
        if (op == "/") return a / b;
        if (op == "^") return std::pow(a, b);
    }
    
    return 0.0;
}

void Calculator::calculateResult() {
    try {
        double resultValue = evaluateExpression(currentExpression);
        result = std::to_string(resultValue);
        currentExpression = result;
        displayText.setString(result);
        isNewCalculation = true;
    }
    catch (std::exception& e) {
        currentExpression = "Error";
        displayText.setString(currentExpression);
        isNewCalculation = true;
    }
}

void Calculator::run() {
    while (window.isOpen()) {
        handleInput();
        update();
        render();
    }
}
