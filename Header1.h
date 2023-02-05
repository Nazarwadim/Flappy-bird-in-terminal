#include <cmath>
#include <windows.h>
const int R = 120 * 33;
const char chars[13] = ".,-~:;=!*#$@";
class Resl {
public:
    const int resx = sqrt(R * 3.64);
    const int resy = sqrt(R / 3.63);
    char display[R];
    void setdisplay() {
        for (int i = resx; i < R; i += resx)display[i] = '\n';
        display[R - 1] = '\0';
    }
    void cleardisplay()
    {
        memset(display, ' ', R);
        setdisplay();
    }
};
class Point {
private:
    float x = 0;
    float y = 0;
public:
    inline float get_x()
    {
        return this->x;
    }
    inline float get_y()
    {
        return this->y;
    }
    void set_x(float x)
    {
        this->x = x;
    }
    void set_y(float y)
    {
        this->y = y;
    }
    Point(float x, float y)
    {
        set_x(x);
        set_y(y);
    }
    Point(){}
};
template<typename T>
int sign(T a)
{
    if (a > 0) return 1;
    if (a == 0) return 0;
    if (a < 0) return -1;
    return 1230043013;
}
void draw_pixel(Resl& display, float x, float y, int N)
{
    int pointTodraw = (int)x + (int)y * display.resx;
    if ( pointTodraw < R && x < display.resx && x > 0) {
        display.display[pointTodraw] = chars[N];
    }
}
void draw_pixel(Resl& display, Point P, int N)
{
    int pointTodraw = (int)P.get_x() + (int)P.get_y() * display.resx;
    if (pointTodraw < R && P.get_x() < display.resx && P.get_x() > 0) {
        display.display[pointTodraw] = chars[N];
    }
}
// using Bresenham's line algorithm
void drawline(Point A, Point B, Resl& display)
{
    float x1 = A.get_x();
    float y1 = A.get_y();
    float x2 = B.get_x();
    float y2 = B.get_y();
    float y = y1;
    float x = x1;
    float dx = fabs(x2 - x1);
    float dy = fabs(y2 - y1);
    int s1 = sign(x2 - x1);
    int s2 = sign(y2 - y1);
    bool interchange;
    if (dy > dx)
    {
        float T = dx;
        dx = dy;
        dy = T;
        interchange = true;
    }
    else {
        interchange = false;
    }
    float e = 2 * dy - dx,
        a = 2 * dy,
        b = 2 * dy - 2 * dx;
    draw_pixel(display, x1, y, 10);

    for (float i = 1; i < dx; i++)
    {
        if (e < 0)
        {
            if (interchange == 1)y = y + s2;
            else x = x + s1;
            e = e + a;
        }
        else {
            y = y + s2;
            x = x + s1;
            e = e + b;
        }
        draw_pixel(display, x, y, 10);
    }

}