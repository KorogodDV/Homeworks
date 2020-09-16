#include "TXLib.h"
#include <iostream>
#include "math.h"

void drawSphere(float x, float y, int R, int N, int red, int green, int blue)
{
    COLORREF color = txGetColor();
    COLORREF fillColor = txGetFillColor();
    for (int i = 0; i < N; i++)
    {
        txSetColor(RGB(red * i / N, green * i / N, blue * i / N));
        txSetFillColor(RGB(red * i / N, green * i / N, blue * i / N));
        txCircle(int(x + 0.5*(R - R * i / N)), int(y - 0.5*(R - R * i / N)), R - R * i / N);
    }
    txSetColor(color);
    txSetFillColor(fillColor);
}

void speedBoostForCatchingSpheres(float* v)
{
    *v += 0,005;
}

void moveSphere(float* x, float* y, float vx, float vy, float dt)
{
    *x += vx * dt;
    *y += vy * dt;
}

void DirectSpheresBehindCursor(float x, float y, float* vx, float* vy, POINT p)
{
    float v = sqrt(*vx * *vx + *vy * *vy);
    *vx = v * (p.x - 128 - x) / sqrt((p.x - 128 - x) * (p.x - 128 - x) + (p.y - 85 - y) * (p.y - 85 - y));
    *vy = v * (p.y - 85 - y) / sqrt((p.x - 128 - x) * (p.x - 128 - x) + (p.y - 85 - y) * (p.y - 85 - y));
}

void checkSphereColide(float x, float y, int R, float* vx, float* vy, float dt)
{
    if (x + *vx * dt + R > 1280 || x + *vx * dt - R < 0)
    {
        *vx *= -1;
    }
    if (y + *vy * dt + R > 720 || y + *vy * dt - R < 0)
    {
        *vy *= -1;
    }
}


bool checkCollisionTwoSpheres(float x1, float y1, int R1, float x2, float y2, int R2)
{
    if ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) > (R1 + R2) * (R1 + R2))
    {
        return false;
    }
    else
    {
        return true;
    }
}

void CollideSpheres(int m1, float* vx1, float* vy1, int m2, float* vx2, float* vy2)
{
    float vx10 = *vx1;
    float vy10 = *vy1;
    float vx20 = *vx2;
    float vy20 = *vy2;
    *vx1 = (2 * m2 * vx20 + (m1 - m2) * vx10) / (m1 + m2);
    *vy1 = (2 * m2 * vy20 + (m1 - m2) * vy10) / (m1 + m2);
    *vx2 = (2 * m1 * vx10 + (m2 - m1) * vx20) / (m1 + m2);
    *vy2 = (2 * m1 * vy10 + (m2 - m1) * vy20) / (m1 + m2);
}

int main()
{
    txCreateWindow(1280, 720);
    txSetFillColor(TX_BLACK);

    std::cout << "Вы будете играть красным шариком и ваша цель не дать двум шарикам вас догнать.";

    Sleep(3000);

    int N = 100;
    const float dt = 1.0;

    float x1 = 640.0;
    float y1 = 360.0;
    int R1 = 65;
    float vx1 = 10.0;
    float vy1 = -5.0;
    int m1 = 1;
    int red1 = 255;
    int green1 = 0;
    int blue1 = 0;

    float x2 = 100.0;
    float y2 = 100.0;
    int R2 = 65;
    float vx2 = 2.0;
    float vy2 = -2.0;
    int m2 = 1;
    int red2 = 0;
    int green2 = 255;
    int blue2 = 0;

    float x3 = 100.0;
    float y3 = 600.0;
    int R3 = 65;
    float vx3 = 2.0;
    float vy3 = -2.0;
    int m3 = 1;
    int red3 = 0;
    int green3 = 0;
    int blue3 = 255;

    POINT mousePos;

    while (true)
    {
        txBegin();
        txClear();
        drawSphere(x1, y1, R1, N, red1, green1, blue1);
        drawSphere(x2, y2, R2, N, red2, green2, blue2);
        drawSphere(x3, y3, R3, N, red3, green3, blue3);
        txEnd();

        GetCursorPos(&mousePos);
        speedBoostForCatchingSpheres(&vx2);
        speedBoostForCatchingSpheres(&vx3);

        DirectSpheresBehindCursor(x1, y1, &vx1, &vy1, mousePos);
        DirectSpheresBehindCursor(x2, y2, &vx2, &vy2, mousePos);
        DirectSpheresBehindCursor(x3, y3, &vx3, &vy3, mousePos);

        checkSphereColide(x1, y1, R1, &vx1, &vy1, dt);
        checkSphereColide(x2, y2, R2, &vx2, &vy2, dt);
        checkSphereColide(x3, y3, R3, &vx3, &vy3, dt);
        if (checkCollisionTwoSpheres(x1, y1, R1, x2, y2, R2))
        {
            break;
        }
        if (checkCollisionTwoSpheres(x1, y1, R1, x3, y3, R3))
        {
            break;
        }
        if (checkCollisionTwoSpheres(x3, y3, R3, x2, y2, R2))
        {
            CollideSpheres(m3, &vx3, &vy3, m2, &vx2, &vy2);
        }

        moveSphere(&x1, &y1, vx1, vy1, dt);
        moveSphere(&x2, &y2, vx2, vy2, dt);
        moveSphere(&x3, &y3, vx3, vy3, dt);
    }
    return 0;
}
