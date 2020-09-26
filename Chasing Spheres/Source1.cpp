//#include <SFML/Graphics.hpp>
//#include <iostream>
//
//int main()
//{
//    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");
//    sf::CircleShape shape(100.f);
//    shape.setPosition(0, 520);
//    std::cout << shape.getPosition().x << shape.getPosition().y;
//    shape.setFillColor(sf::Color::Green);
//
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::MouseWheelScrolled)
//                window.close();
//        }
//
//        window.clear();
//        window.draw(shape);
//        window.display();
//    }
//
//    return 0;
//}