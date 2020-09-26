#include "math.h"
#include <SFML/Graphics.hpp>
#include "windows.h"

const float DT = 1.0;
const float speedboost = 0.001;
const int window_length = 1280;
const int window_width = 720;

struct vector2f
{
    float x;
    float y;
};

struct Sphere
{
    vector2f pos;
    int R;
    vector2f speed;
    int m;
    int red;
    int green;
    int blue;
};

void drawSphere(sf::RenderWindow* window, Sphere sphere, int lighting_detailing = 100)
{
    sf::CircleShape circle(sphere.R);
    for (int i = 0; i < lighting_detailing; i++)
    {
        circle.setRadius(sphere.R - sphere.R * i / lighting_detailing);
        circle.setPosition(sphere.pos.x - sphere.R + 1.4 * sphere.R * i / lighting_detailing, sphere.pos.y - sphere.R + 0.6 * sphere.R * i / lighting_detailing);
        circle.setFillColor(sf::Color(sphere.red * i / lighting_detailing, sphere.green * i / lighting_detailing, sphere.blue * i / lighting_detailing));
        (*window).draw(circle);
    }
}

void speedBoostForCatchingSpheres(Sphere* sphere)
{
    sphere->speed.x += speedboost;
}

void moveSphere(Sphere* sphere, float DT)
{
    sphere->pos.x += sphere->speed.x * DT;
    sphere->pos.y += sphere->speed.y * DT;
}

void directSpheresBehindCursor(Sphere* sphere, POINT p)
{
    float v = sqrt(pow(sphere->speed.x, 2) + pow(sphere->speed.y, 2));
    sphere->speed.x = v * (p.x - sphere->pos.x) / sqrt(pow(p.x - sphere->pos.x, 2) + pow(p.y - sphere->pos.y, 2));
    sphere->speed.y = v * (p.y - sphere->pos.y) / sqrt(pow(p.x - sphere->pos.x, 2) + pow(p.y - sphere->pos.y, 2));
}

void checkSphereColideWithWalls(Sphere* sphere, float DT)
{
    if (sphere->pos.x + sphere->speed.x * DT + sphere->R > window_length || sphere->pos.x + sphere->speed.x * DT - sphere->R < 0)
    {
        sphere->speed.x *= -1;
    }
    if (sphere->pos.y + sphere->speed.y * DT + sphere->R > window_width || sphere->pos.y + sphere->speed.y * DT - sphere->R < 0)
    {
        sphere->speed.y *= -1;
    }
}

bool checkCollisionTwoSpheres(const Sphere* sphere1, const Sphere* sphere2)
{
    return pow(sphere1->pos.x - sphere2->pos.x, 2) + pow(sphere1->pos.y - sphere2->pos.y, 2) < pow(sphere1->R + sphere2->R, 2);
}

void collideSpheres(Sphere* sphere1, Sphere* sphere2)
{
    float vx10 = sphere1->speed.x;
    float vy10 = sphere1->speed.y;
    float vx20 = sphere2->speed.x;
    float vy20 = sphere2->speed.y;

    sphere1->speed.x = (2 * sphere2->m * vx20 + (sphere1->m - sphere2->m) * vx10) / (sphere1->m + sphere2->m);
    sphere1->speed.y = (2 * sphere2->m * vy20 + (sphere1->m - sphere2->m) * vy10) / (sphere1->m + sphere2->m);
    sphere2->speed.x = (2 * sphere1->m * vx10 + (sphere2->m - sphere1->m) * vx20) / (sphere1->m + sphere2->m);
    sphere2->speed.y = (2 * sphere1->m * vy10 + (sphere2->m - sphere1->m) * vy20) / (sphere1->m + sphere2->m);
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(window_length, window_width), "Chasing");

    Sphere sphere1 = { 640.0, 360.0, 65, 3.0, -1.0, 1, 255, 0, 0 };
    Sphere sphere2 = { 100.0, 100.0, 65, 1.0, -0.4, 2, 0, 255, 0 };
    Sphere sphere3 = { 100.0, 500.0, 65, 1.4, -0.4, 5, 0, 0, 255 };

    POINT mousePos = { 640.0, 360.0 };

    while (window.isOpen())
    {
        sf::Event event;
        window.clear();
        drawSphere(&window, sphere1);
        drawSphere(&window, sphere2);
        drawSphere(&window, sphere3);
        window.display();

        while (window.pollEvent(event))
        {
            if ((event.type == sf::Event::Closed) || (event.key.code == sf::Keyboard::Escape))
            {
                window.close();
                break;
            }
            if (event.type == sf::Event::MouseMoved)
            {
                mousePos.x = event.mouseMove.x;
                mousePos.y = event.mouseMove.y;
            }
        }

        speedBoostForCatchingSpheres(&sphere2);
        speedBoostForCatchingSpheres(&sphere3);

        directSpheresBehindCursor(&sphere1, mousePos);
        directSpheresBehindCursor(&sphere2, mousePos);
        directSpheresBehindCursor(&sphere3, mousePos);

        checkSphereColideWithWalls(&sphere1, DT);
        checkSphereColideWithWalls(&sphere2, DT);
        checkSphereColideWithWalls(&sphere3, DT);

        if (checkCollisionTwoSpheres(&sphere1, &sphere2))
        {
            break;
        }

        if (checkCollisionTwoSpheres(&sphere1, &sphere3))
        {
            break;
        }

        if (checkCollisionTwoSpheres(&sphere2, &sphere3))
        {
            collideSpheres(&sphere2, &sphere3);
        }

        moveSphere(&sphere1, DT);
        moveSphere(&sphere2, DT);
        moveSphere(&sphere3, DT);
    }
    return 0;
}
