
#include<SFML\Graphics.hpp>
#include<string>
#include<iostream>
#include<string.h>

using namespace std;
int main()
{

    sf::RenderWindow window(sf::VideoMode(500, 200), "winner", sf::Style::Close | sf::Style::Titlebar);
    sf::RectangleShape box(sf::Vector2f(200, 60));
    box.setFillColor(sf::Color::Green);
    box.setPosition(150, 90);
    box.setFillColor(sf::Color::White);

    sf::Font font;
    font.loadFromFile("font.ttf");
    sf::Text texture;
    texture.setString("please enter your name:");
    texture.setCharacterSize(30);
    texture.setFont(font);
    texture.setFillColor(sf::Color::Magenta);
    texture.setStyle(sf::Text::Bold);
    texture.setPosition(100, 20);




    sf::Event evnt;

    //    sf::Font font;
    font.loadFromFile("font.ttf");
    sf::Text texture1;

    texture1.setCharacterSize(30);
    texture1.setFont(font);
    texture1.setFillColor(sf::Color::Magenta);
    texture1.setStyle(sf::Text::Bold);
    texture1.setPosition(180, 95);


    string st;
    //st.pop_back();
    //st.pop_back();
    while (window.isOpen())
    {
        sf::Keyboard::Key key;
        sf::Event evnt;
        while (window.pollEvent(evnt))
        {


            while (window.isOpen())
            {
                while (window.pollEvent(evnt))
                {
                    if (evnt.type == sf::Event::Closed)
                        window.close();
                    //else if (evnt.Type == sf::Event::KeyPressed)
                    //{
                    //    if (evnt.Key.Code == sf::Key::Back)
                    //    {
                    //        texture1.erase(text.size() - 1);
                    //    }
                    //    else if (evnt.Key.Code == sf::Key::Return)
                    //    {
                    //        texture1 += "\n";
                    //    }
                    //}
                    //else if (evnt.type == sf::Event::TextEntered)
                    //{
                    //    texture1 += (char)evnt.text.unicode;
                    //}
                    if (evnt.type == sf::Event::TextEntered)
                    {

                         st = sf::Event::TextEntered;
                        texture1.setString(st);

                        /*if (evnt.text.unicode < 128) {
                            //std::cout << "ASCII character typed: " << static_cast<char>(evnt.text.unicode) << std::endl;
                            st += (evnt.text.unicode);
                            texture1.setString(st);



                        }*/
                    }

                    /*else if (evnt.type == sf::Event::TextEntered)
                    {
                        st += ("sf::Event::TextEntered");
                        texture1.setString(st);

                    }*/
                    /*else if (evnt.type == sf::Event::TextEntered)
                    {

                        if (evnt.text.unicode < 128)
                        {
                            printf("%c", evnt.text.unicode);

                            playerText.setString(sf::Event::TextEntered);
                        }
                    }*/



                }

            }

        }

        window.clear();

        window.draw(texture1);
        window.draw(texture);
        window.draw(box);
        window.display();

    }




    system("pause");
    return 0;
}