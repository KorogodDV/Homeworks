#include "TXLib.h"

void drawSphere(int x, int y, int R, int N, int red, int green, int blue)
{
    for (int i = 0; i < N; i++)
    {
        txSetColor(RGB(red * i / N, green * i / N, blue * i / N));
        txSetFillColor(RGB(red * i / N, green * i / N, blue * i / N));
        txCircle(x + 0.5*(R - R * i / N), y - 0.5*(R - R * i / N), R - R * i / N);
    }
}

int main()
{
    txCreateWindow(1280, 720);

    int R = 50;
    int N = 100;
    int x = 400;
    int y = 300;
    int vx = 2;
    int vy = 5;
    int red = 255;
    int green = 0;
    int blue = 0;
    const float dt = 1.0;

    while (true)
    {
        txBegin();
        txSetFillColor(TX_PINK);
        txClear();
        drawSphere(x, y, R, N, red, green, blue);
        txEnd();

        if (x + vx * dt + R > 1280 || x + vx * dt - R < 0) vx = -vx;
        if (y + vy * dt + R > 720 || y + vy * dt - R < 0) vy = -vy;
        x += vx * dt;
        y += vy * dt;
    }
    return 0;
}
