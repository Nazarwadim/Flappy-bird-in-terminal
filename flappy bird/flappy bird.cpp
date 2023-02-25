#include <iostream>
#include <thread>
#include <conio.h>
#include <time.h>
#include "../Header1.h"
#pragma comment(lib, "winmm.lib")
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
    void render_bird(Resl& display)
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
        anim_time += anim_speed;
    }
    void set_Phisics()
    {
        float g = 9.8;
        speedY += g / 140;
        float temp_y = o.get_y();
        temp_y += speedY;
        o.set_y(temp_y);
    }
};
void generate_sth(Resl* display, Point peregoroda[], int n);
void set_peregoroga_x(Point peregoroda[], float value, int number_of_element);
void draw_peregoroda(Point peregoroda[], int number_of_element, Resl* display);
void draw_peregoroda(Point peregoroda[], int number_of_element, Resl& display);
void show_frame(int frame, Resl &display);
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
            PlaySound(TEXT("arkadnaya-igra-power-power-sound-41566.wav"), NULL, SND_FILENAME | SND_ASYNC);
            bird->time = 0;
            bird->set_anim_time(3);
            bird->speedY = -0.8;
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

    bool light = true;
    //zastavka
    for (int i = 0; i < 1000; i++) {
        display.cleardisplay();
        char c = light == true ? 177 : ' ';
        if (!(i % 50))light = !light;

        memset(display.display, c, R - 1);
        display.setdisplay();
        puts(display.display);

    }

    //update

    Point peregoroda[12];
    generate_sth(&display, peregoroda, 0);
    generate_sth(&display, peregoroda, 1);
    generate_sth(&display, peregoroda, 2);
    std::thread t(check_key, &key, &bird);
    for (float i = display.resx, m = display.resx + 40, l = display.resx + 80;; )
    {
        if (i < 0) {
            i = display.resx;
            generate_sth(&display, peregoroda, 0);
        }
        if (m < 0) {
            m = display.resx;
            generate_sth(&display, peregoroda, 1);
        }
        if (l < 0) {
            l = display.resx;
            generate_sth(&display, peregoroda, 2);
        }
        if (score < 20) { i -= 0.5; m -= 0.5; l -= 0.5; }
        else { i -= 1.5; m -= 1.5; l -= 1.5; }         //speed of blocks

        short frame = 1000 / (clock() - time + 1);
        time = clock();
        display.cleardisplay();
        show_frame(frame, display);
        bird.render_bird(display);
        bird.set_Phisics();

        set_peregoroga_x(peregoroda, i, 0);
        set_peregoroga_x(peregoroda, m, 1);
        set_peregoroga_x(peregoroda, l, 2);
        draw_peregoroda(peregoroda, 0, display);
        draw_peregoroda(peregoroda, 1, display);
        draw_peregoroda(peregoroda, 2, display);
        if (key == 'q') {
            bird.alive = false;
            t.detach();
            break;
        }
        int point_on_display = (int)bird.o.get_x() + (int)bird.o.get_y() * display.resx;
        if (bird.o.get_y() > display.resy || bird.o.get_y() < 0 || display.display[point_on_display] == '$')
        {
            std::cout << "You die" << '\n';
            PlaySound(TEXT("lkpkpgougp.wav"), NULL, SND_FILENAME | SND_ASYNC);

            std::cout << "Play again? Y/N  (double tap) or P to pay 10492348237409$ to continue" << '\n';

            char c = getchar();
            fflush(stdin);
            if (c == 'p') {
                PlaySound(TEXT("povezlo-povezlo.wav"), NULL, SND_SYNC);
                bird.o.set_y(3);
                bird.speedY = 0;
                i--;
                m--;
                l--;
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
        //score

        for (int j = 0; j < 12; j += 4)
        {
            if ((int)peregoroda[j].get_x() == 14)
            {
                score++;
            }
        }
        
        display.setdisplay();
        puts(display.display);
        Sleep(20);  // 60 frames per sec // 20 for 30 fps     50 for 20  Warming !!! fps break`s phisics
    }
    t.detach();
    return 0;
}
void generate_sth(Resl* display, Point peregoroda[], int n)
{
    float y2[] = { 0,(float)display->resy };
    bool up = rand() % 2;
    float y1 = rand() % (display->resy - 8);
    float ytd = y1 > 10 ? y1 : y1 + 8;
    Point A(display->resx - 11, ytd);
    Point B(display->resx - 11, y2[up]);
    Point C(display->resx - 1, y2[up]);
    Point D(display->resx - 1, ytd);
    peregoroda[4 * n] = A;
    peregoroda[4 * n + 1] = B;
    peregoroda[4 * n + 2] = C;
    peregoroda[4 * n + 3] = D;
}
void set_peregoroga_x(Point peregoroda[], float value, int number_of_element)
{
    float dx = peregoroda[4 * number_of_element + 2].get_x() - peregoroda[4 * number_of_element].get_x();
    for (int i = number_of_element * 4; i < number_of_element * 4 + 2; i++)
    {
        peregoroda[i].set_x(value - dx / 2);
    }
    for (int i = number_of_element * 4 + 2; i < number_of_element * 4 + 4; i++)
    {
        peregoroda[i].set_x(value + dx / 2);
    }
}
void draw_peregoroda(Point peregoroda[], int number_of_element, Resl& display)
{
    drawline(peregoroda[4 * number_of_element + 0], peregoroda[4 * number_of_element + 1], display);
    drawline(peregoroda[4 * number_of_element + 2], peregoroda[4 * number_of_element + 3], display);
    drawline(peregoroda[4 * number_of_element + 3], peregoroda[4 * number_of_element + 0], display);
}
void show_frame(int frame, Resl &display) {
    if (frame < 10) display.display[10 + display.resx] = frame + '0';
    if (frame > 10 && frame < 100)
    {
        display.display[10 + display.resx] = frame / 10 + '0';
        display.display[11 + display.resx] = frame - (frame / 10) * 10 + '0';
    }
}