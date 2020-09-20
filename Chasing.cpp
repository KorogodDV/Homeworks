#include <iostream>
#include "math.h"
#include <SFML/Graphics.hpp>
#include "TXLib.h"

struct Sphere
{
    float x;
    float y;
    int R;
    float vx;
    float vy;
    int m;
    int red;
    int green;
    int blue;
};

void drawSphere(Sphere sphere)
{
    COLORREF color = txGetColor();
    COLORREF fillColor = txGetFillColor();
    int N = 100;
    for (int i = 0; i < N; i++)
    {
        txSetColor(RGB(sphere.red * i / N, sphere.green * i / N, sphere.blue * i / N));
        txSetFillColor(RGB(sphere.red * i / N, sphere.green * i / N, sphere.blue * i / N));
        txCircle(int(sphere.x + 0.5 * (sphere.R - sphere.R * i / N)), int(sphere.y - 0.5 * (sphere.R - sphere.R * i / N)), sphere.R - sphere.R * i / N);
    }
    txSetColor(color);
    txSetFillColor(fillColor);
}

void speedBoostForCatchingSpheres(Sphere* sphere)
{
    sphere->vx += 0, 005;
}

void moveSphere(Sphere* sphere, float dt)
{
    sphere->x += sphere->vx * dt;
    sphere->y += sphere->vy * dt;
}

void DirectSpheresBehindCursor(Sphere* sphere, POINT p)
{
    float v = sqrt(pow(sphere->vx, 2) + pow(sphere->vy, 2));
    sphere->vx = v * (p.x - sphere->x) / sqrt(pow(p.x - sphere->x, 2) + pow(p.y - sphere->y, 2));
    sphere->vy = v * (p.y - sphere->y) / sqrt(pow(p.x - sphere->x, 2) + pow(p.y - sphere->y, 2));
}

void checkSphereColide(Sphere* sphere, float dt)
{
    if (sphere->x + sphere->vx * dt + sphere->R > 1280 || sphere->x + sphere->vx * dt - sphere->R < 0)
    {
        sphere->vx *= -1;
    }
    if (sphere->y + sphere->vy * dt + sphere->R > 720 || sphere->y + sphere->vy * dt - sphere->R < 0)
    {
        sphere->vy *= -1;
    }
}


bool checkCollisionTwoSpheres(Sphere* sphere1, Sphere* sphere2)
{
    return pow(sphere1->x - sphere2->x, 2) + pow(sphere1->y - sphere2->y, 2) < pow(sphere1->R + sphere2->R, 2);
}

void CollideSpheres(Sphere* sphere1, Sphere* sphere2)
{
    float vx10 = sphere1->vx;
    float vy10 = sphere1->vy;
    float vx20 = sphere2->vx;
    float vy20 = sphere2->vy;
    sphere1->vx = (2 * sphere2->m * vx20 + (sphere1->m - sphere2->m) * vx10) / (sphere1->m + sphere2->m);
    sphere1->vy = (2 * sphere2->m * vy20 + (sphere1->m - sphere2->m) * vy10) / (sphere1->m + sphere2->m);
    sphere2->vx = (2 * sphere1->m * vx10 + (sphere2->m - sphere1->m) * vx20) / (sphere1->m + sphere2->m);
    sphere2->vy = (2 * sphere1->m * vy10 + (sphere2->m - sphere1->m) * vy20) / (sphere1->m + sphere2->m);
}

int main()
{
    txCreateWindow(1280, 720);
    txSetFillColor(TX_BLACK);

    std::cout << "Вы будете играть красным шариком и ваша цель не дать двум шарикам вас догнать.";

    Sleep(3000);

    int N = 100;
    const float dt = 1.0;

    Sphere sphere1 = { 640.0, 360.0, 65, 10.0, -5.0, 1, 255, 0, 0 };
    Sphere sphere2 = { 100.0, 100.0, 65, 2.0, -2.0, 1, 0, 255, 0 };
    Sphere sphere3 = { 100.0, 600.0, 65, 2.0, -2.0, 1, 0, 0, 255 };

    POINT mousePos;

    while (true)
    {
        txBegin();
        txClear();
        drawSphere(sphere1);
        drawSphere(sphere2);
        drawSphere(sphere3);
        txEnd();

        mousePos = txMousePos();
        speedBoostForCatchingSpheres(&sphere2);
        speedBoostForCatchingSpheres(&sphere3);

        DirectSpheresBehindCursor(&sphere1, mousePos);
        DirectSpheresBehindCursor(&sphere2, mousePos);
        DirectSpheresBehindCursor(&sphere3, mousePos);

        checkSphereColide(&sphere1, dt);
        checkSphereColide(&sphere2, dt);
        checkSphereColide(&sphere3, dt);
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
            CollideSpheres(&sphere2, &sphere3);
        }

        moveSphere(&sphere1, dt);
        moveSphere(&sphere2, dt);
        moveSphere(&sphere3, dt);
    }
    return 0;
}
