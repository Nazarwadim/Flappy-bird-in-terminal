#include <cmath>
#include <windows.h>
#include <iostream>
#include <thread>
#include <conio.h>
#include <time.h>
const int R = 120 * 33;
const char chars[13] = ".,-~:;=!*#$@";
template<typename T>
int sign(T a);
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
    float x;
    float y;
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
    Point() {};
};
void draw_pixel(Resl& display, Point P, int N);
void draw_pixel(Resl& display, float x, float y, int N);
class Bird {
private:
    float anim_time = 0;
    float anim_speed = 1;
public:
    bool alive = true;
    int time = 0;
    Point o;
    float speedY = 0;
    Bird(float x, float y) {
        o.set_x(x);
        o.set_y(y);
    }
    void set_anim_time(float anim_time)
    {
        this->anim_time = anim_time;
    }
    void render_bird(Resl &display)
    {   
        if (anim_time > 2) {
            anim_speed = -0.25;
        }
        if (anim_time < 0) anim_speed = 0.25;
        draw_pixel(display, o, 7);
        draw_pixel(display, o.get_x() + 1, o.get_y(), 11);
        draw_pixel(display, o.get_x() - 1, o.get_y(), 9);
        draw_pixel(display, o.get_x() - 2, o.get_y(), 9);
        draw_pixel(display, o.get_x(), o.get_y() + (int)anim_time - 1, 6); // wing 
        anim_time+=anim_speed;
    }
    void set_Phisics()
    {
        float g = 9.8;
        speedY += g /320;
        float temp_y = o.get_y();
        temp_y += speedY;
        o.set_y(temp_y);
    }
};
void drawline(Point A, Point B, Resl& display);
void generate_sth(Resl* display, Point peregoroda[], int n);
void check_key(char* key, Bird* bird)
{
    int point_on_display = 0;
    while (bird->alive) {
        Sleep(20);
        *key = _getch();
        if (*key == 'q')return;
        switch (*key)
        {
        case ' ':
            //move up
            bird->time = 0;
            bird->set_anim_time(3);
            bird->speedY = -0.6;
            break;
        }
    }
}


int main()
{

    //start
    int score = 0;
    system("color 5");
    srand(time(NULL));
    char key;
    Resl display;
    Bird bird(15, 10);    
    int time = -1000;
    display.cleardisplay();
    for (int i = 0; i < 33 * 120 - 1; i++) {
        if (i % 120) display.display[i] = 177;
        if(i % 2)puts(display.display);
    }

    //update

    Point peregoroda[12];
    generate_sth(&display, peregoroda, 1);
    generate_sth(&display, peregoroda, 2);
    std::thread t(check_key, &key, &bird);
    for (int i = 0, mj = 0;; mj--)
    {
        if(score <= 20)
            if (mj % 3)i--; // i is speed of barriers
        if (score > 20 && score <= 120)
            if (mj % 3)i--; 
        if (score > 120)
            i--;
        short frame = 1000 / (clock() - time + 1);
        time = clock();
        display.cleardisplay();
        bird.render_bird(display);
        bird.set_Phisics();
        if (!i % display.resx) {
            generate_sth(&display, peregoroda,0);
            score++;
        }
        if (i % display.resx == (2*display.resx)/3) {
            generate_sth(&display, peregoroda, 1);
            score++;
        }
        if (i % display.resy == display.resx/3) {
            generate_sth(&display, peregoroda, 2);
            score++;
        }
        peregoroda[0].set_x(i % 120 );
        peregoroda[1].set_x(i % 120 );
        peregoroda[2].set_x(i % 120 + 40);
        peregoroda[3].set_x(i % 120 + 40);
        peregoroda[4].set_x(i % 120 + 80);
        peregoroda[5].set_x(i % 120 + 80);
        drawline(peregoroda[0], peregoroda[1], display);
        drawline(peregoroda[2], peregoroda[3], display);
        drawline(peregoroda[4], peregoroda[5], display);

        if (i == 0) i = display.resx;
        if (key == 'q') {
            bird.alive = false;
            t.detach();
            break;
        }
        int point_on_display = (int)bird.o.get_x() + (int)bird.o.get_y() * display.resx;
        if (bird.o.get_y() > display.resy || bird.o.get_y() < 0 || display.display[point_on_display] == '$')
        {
            std::cout << "You die" << '\n';
            std::cout << "Play again? Y/N  (double tap) or P to pay 10492348237409$ to continue" << '\n';

            char c = getchar();
            fflush(stdin);
            if (c == 'p') {
                bird.o.set_y(10);
                bird.speedY = 0;
                i--;
                continue;
            }
            if (c == 'y')
            {
                bird.alive = false;
                main();
            }
            bird.alive = false;
            break;
        }
        display.setdisplay();
        std::cout <<"FPS: "<< frame << '\n' <<"Score: " << score << '\n';
        puts(display.display);
        Sleep(1);// 60 frames per sec
    }
    t.detach();
    return 0;
} 
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
    if (pointTodraw > 0 && pointTodraw < R && pointTodraw % display.resx != 0) {
        display.display[pointTodraw] = chars[N];
    }
}
void draw_pixel(Resl& display, Point P, int N)
{
    int pointTodraw = (int)P.get_x() + (int)P.get_y() * display.resx;
    if (pointTodraw > 0 && pointTodraw < R && pointTodraw % display.resx != 0) {
        display.display[pointTodraw] = chars[N];
    }
}
void drawline(Point A, Point B, Resl& display)
{
    // using Bresenham's line algorithm
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
void generate_sth(Resl* display, Point peregoroda[], int n)
{
    float y2[] = { 0,33 };
    bool up = rand() % 2;
    float y1 = rand() % (display->resy - 10);
    float ytd = y1 > 10 ? y1 : y1 + 10;
    Point A(display->resx - 10, ytd);
    Point B(display->resx - 10, y2[up]);
    peregoroda[2 * n] = A;
    peregoroda[2 * n + 1] = B;
}