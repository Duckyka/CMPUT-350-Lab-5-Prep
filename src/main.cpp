#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include <cmath>

#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int FPS_LIMIT = 30;
const float ANIMATION_FRAME_DURATION = 30;
float FrameCounter = 0;
bool isMovingRight = true;
std::function<float()> t;
std::function<float(float)> functionT;

// global tween function
std::function<float(float, float, float)> tween = [](float a, float b, float t) {
    return (1 - t) * a + t * b;
};
//Easing Function 0
float t0()
{ return FrameCounter / ANIMATION_FRAME_DURATION; }

//Easing function 1
float t1()
{ return sqrt(FrameCounter / ANIMATION_FRAME_DURATION); }

//Easing function 2
float t2()
{ return (FrameCounter / ANIMATION_FRAME_DURATION) * (FrameCounter / ANIMATION_FRAME_DURATION); }

//Easing function 3
float t3()
{ return (FrameCounter / ANIMATION_FRAME_DURATION) * (FrameCounter / ANIMATION_FRAME_DURATION) * (FrameCounter / ANIMATION_FRAME_DURATION); }

//Easing function 4
float t4()
{   float x = (FrameCounter / ANIMATION_FRAME_DURATION);
    return (x < 0.5 ? 4 * x * x * x : 1 - std::pow(-2 * x + 2, 3) / 2); }

//Easing function 5
float t5()
{
    float x = (FrameCounter / ANIMATION_FRAME_DURATION);
    float c1 = 1.70158;
    float c2 = c1 * 1.525;

    return x < 0.5
    ? (std::pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
    : (std::pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
}

//Easing function 6
float t6()
{
    float x = (FrameCounter / ANIMATION_FRAME_DURATION);
    return 1 - std::pow(1 - x, 5);
}

//Easing function 7
float t7()
{
    float x = (FrameCounter / ANIMATION_FRAME_DURATION);
    float c5 = (2 * 3.141592653589793) / 4.5;

    return x == 0
    ? 0
    : x == 1
    ? 1
    : x < 0.5
    ? -(std::pow(2, 20 * x - 10) * std::sin((20 * x - 11.125) * c5)) / 2
    : (std::pow(2, -20 * x + 10) * std::sin((20 * x - 11.125) * c5)) / 2 + 1;
}

//Easing function 8
float t8()
{
    float x = (FrameCounter / ANIMATION_FRAME_DURATION);
    float c4 = (2 * 3.141592653589793) / 3;

    return x == 0
    ? 0
    : x == 1
    ? 1
    : -std::pow(2, 10 * x - 10) * std::sin((x * 10 - 10.75) * c4);
}

//Easing function 9
float t9()
{
    float x = (FrameCounter / ANIMATION_FRAME_DURATION);
    return 1 - std::sqrt(1 - std::pow(x, 2));
}

float function0(float x)
{
    return x;
}

float function1(float x)
{
    return sqrt(x);
}

float function2(float x)
{
    return x * x;
}

float function3(float x)
{
    return x * x * x;
}

float function4(float x)
{
    return (x < 0.5 ? 4 * x * x * x : 1 - std::pow(-2 * x + 2, 3) / 2);
}

float function5(float x)
{
    float c1 = 1.70158;
    float c2 = c1 * 1.525;

    return x < 0.5
    ? (std::pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
    : (std::pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
}

float function6(float x)
{
    return 1 - std::pow(1 - x, 5);
}

float function7(float x)
{
    float c5 = (2 * 3.141592653589793) / 4.5;

    return x == 0
    ? 0
    : x == 1
    ? 1
    : x < 0.5
    ? -(std::pow(2, 20 * x - 10) * std::sin((20 * x - 11.125) * c5)) / 2
    : (std::pow(2, -20 * x + 10) * std::sin((20 * x - 11.125) * c5)) / 2 + 1;
}

float function8(float x)
{
    float c4 = (2 * 3.141592653589793) / 3;

    return x == 0
    ? 0
    : x == 1
    ? 1
    : -std::pow(2, 10 * x - 10) * std::sin((x * 10 - 10.75) * c4);
}

float function9(float x)
{
    return 1 - std::sqrt(1 - std::pow(x, 2));
}


struct GridMapping {
    sf::FloatRect miniBounds;
    sf::FloatRect mathBounds;

    sf::Vector2f map(float mathX, float mathY) const {
        float normX = (mathX - mathBounds.position.x) / mathBounds.size.x;
        float normY = 1.0f - ((mathY - mathBounds.position.y) / mathBounds.size.y);

        float screenX = miniBounds.position.x + (normX * miniBounds.size.x);
        float screenY = miniBounds.position.y + (normY * miniBounds.size.y);

        return sf::Vector2f(screenX, screenY);
    }
};


void handleInput(sf::Window& window, bool& shouldQuit) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            shouldQuit = true;
        }

        // ====== ====== ======
        // TODO: (Q2)
        //  implement key presses (1-9) that replace the tween function
        //  with different alternate tween functions.
        //  Functions can be from lecture or from https://easings.net/#
        // ====== ====== ======
        if (const auto* keyPressed = event->getIf<sf::Event::TextEntered>())
        {
            if (keyPressed->unicode == '0') { t = t0; functionT = function0; }
            else if (keyPressed->unicode == '1') { t = t1; functionT = function1; }
            else if (keyPressed->unicode == '2') { t = t2; functionT = function2; }
            else if (keyPressed->unicode == '3') { t = t3; functionT = function3; }
            else if (keyPressed->unicode == '4') { t = t4; functionT = function4; }
            else if (keyPressed->unicode == '5') { t = t5; functionT = function5; }
            else if (keyPressed->unicode == '6') { t = t6; functionT = function6; }
            else if (keyPressed->unicode == '7') { t = t7; functionT = function7; }
            else if (keyPressed->unicode == '8') { t = t8; functionT = function8; }
            else if (keyPressed->unicode == '9') { t = t9; functionT = function9; }
        }
    }
}

void render(sf::RenderWindow& window) {
    // Clear with blue background (sky)
    window.clear(sf::Color::Black);
    // ====== ====== ======
    // TODO: (Q1) Draw circle that moves between
    // the left/right half of the screen.
    // Movement should be governed by the tween function.
    // ====== ====== ======

    sf::CircleShape circle;
    circle.setRadius(40);
    circle.setFillColor(sf::Color::Red);
    
    if (isMovingRight)
    {
        circle.setPosition({tween(50, WINDOW_WIDTH-100, t()), WINDOW_HEIGHT/4});
    }
    else if (!isMovingRight)
    {
        circle.setPosition({tween(WINDOW_WIDTH-100, 50, t()), WINDOW_HEIGHT/4});
    }

    window.draw(circle);

    // ====== ====== ======
    // TODO: (Q3) Draw tween function graph with a dot
    // on the current portion of the curve
    // ====== ====== ======

    //Lines
    sf::RectangleShape lineX;
    lineX.setOutlineColor(sf::Color::White);
    lineX.setOutlineThickness(1);
    lineX.setSize(sf::Vector2f(1, 200));
    lineX.setPosition({WINDOW_HEIGHT-750, 510});

    sf::RectangleShape lineY;
    lineY.setOutlineColor(sf::Color::White);
    lineY.setOutlineThickness(1);
    lineY.setSize(sf::Vector2f(650, 1));
    lineY.setPosition({50, WINDOW_HEIGHT-90});

    window.draw(lineX);
    window.draw(lineY);

    //Function Line
    sf::CircleShape dot(5.0f);
    GridMapping grid;
    grid.miniBounds = sf::FloatRect({50.f, 500.f}, {650.f, 200.f});
    grid.mathBounds = sf::FloatRect({0.f, 0.f}, {1.f, 1.f});

    sf::RectangleShape gridVisual(sf::Vector2f(grid.miniBounds.size.x, grid.miniBounds.size.y));

    int resolution = 300;
    sf::VertexArray functionLine(sf::PrimitiveType::LineStrip, resolution);

    for (int i = 0; i < resolution; ++i) {
        float x = static_cast<float>(i) / (resolution - 1);
        float y = functionT(x); 

        functionLine[i].position = grid.map(x, y);
        functionLine[i].color = sf::Color::Cyan;
        dot.setFillColor(sf::Color::Blue);
        if (fabs(t() - x) <= 0.01)
        {
            if (isMovingRight)
            {
                dot.setPosition(functionLine[i].position);
            }
            else
            {
                if (functionLine[resolution - i - 1].position.x == 0 || functionLine[resolution - i - 1].position.y == 0)
                {
                    float tempx = static_cast<float>(resolution - i - 1) / (resolution - 1);
                    functionLine[resolution - i - 1].position = grid.map((tempx), (functionT(tempx)));
                }
                dot.setPosition(functionLine[resolution - i - 1].position);
            }
        }
    }
    window.draw(functionLine);
    window.draw(dot);

    window.display();
}

int main() {
    sf::RenderWindow window;
    t = t0;
    functionT = function0;

    try {
        // Initialize window
        window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Tween");
        window.setFramerateLimit(FPS_LIMIT);
        // Prevent key repeats.
        window.setKeyRepeatEnabled(false);
        
        bool shouldQuit = false;
        // Main game loop
        while (window.isOpen()) {
            handleInput(window, shouldQuit);
            if (shouldQuit) {
                break;
            }
            if (FrameCounter > ANIMATION_FRAME_DURATION)
            {
                FrameCounter = 0;
                isMovingRight = !isMovingRight;
            }

            render(window);
            FrameCounter++;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
