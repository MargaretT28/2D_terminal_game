#include <ncurses.h>
#include <stdlib.h> //для ранд функции, чтобы когда мы находим на предмет он появлялся в др месте
#include <time.h>
#include <stdbool.h>

int py, px; // корды игрока
int ty, tx; //корды предмета
int p_gold = 0; //голда
bool t_placed = 0; //флаг расположения предмета
bool p_placed = 0; //флаг расположения игрока
bool r_placed = 0; //флаг расположения комнаты
int ry, rx, r_size_y, r_size_x;

int dungeon(int c, int rows, int cols, char (*map)[cols]) { //рисуем комнату
    srand(time(NULL)); //каждый раз число будет разным изза времени. srand потому что seed random

    if (!r_placed) {
        ry = rand() % (rows - 4) + 1;
        rx = rand() % (cols - 4) + 1;

        r_size_y = rand() % 5 + 4;
        r_size_x = rand() % 10 + 8;

        r_placed = 1;
    }

    for (int y = 0; y <= rows; y++){ //рисуем стены
        for (int x = 0; x <= cols; x++){
            if (y == 0 || y == rows || x ==0 || x == cols) {
                map[y][x] = '%';
                mvaddch(y, x, '%');
            } else {
                map[y][x] = ':';
                mvaddch(y, x, ':');
            }
        }
    }
    for (int y = ry; y <= ry + r_size_y; y++) { //рисуем комнату
        for (int x = rx; x <= rx + r_size_x; x++){
            if (map[y][x] == '%'){
                y = ry + r_size_y; //выход из внешнего цикла
                break;
            } else {
                map[y][x] = ' ';
                mvaddch(y, x, ' ');
            }
        }
    }
    if (c == KEY_UP && map[py-1][px] == ' ') py--;
    else if (c == KEY_DOWN && map[py+1][px] == ' ') py++;
    else if (c == KEY_LEFT && map[py][px - 1] == ' ') px--;
    else if (c == KEY_RIGHT && map[py][px + 1] == ' ') px++;

    if (!t_placed){
        do {
            ty = rand() % rows;
            tx = rand() % cols;
        } while (map[ty][tx] == ':' || map[ty][tx] == '%'); //пока попадаем в стены генерим новые, чтобы попасть в окно
        t_placed = 1;
    }

    if (!p_placed){
        do {
            py = rand() % rows;
            px = rand() % cols;
        } while (map[py][px] == ':' || map[py][px] == '%'); //пока попадаем в стены генерим новые, чтобы попасть в окно
        p_placed = 1;
    }

    if (py == ty && px == tx) {
        t_placed = 0;
        r_placed = 0;
        p_placed = 0;
        p_gold += rand() % 10 + 1;

    }

    mvaddch(ty, tx, 't'); //расположить предмет
    mvaddch(py, px, 'o');  //расположить игрока
    mvprintw(rows, 0, "Gold: %d ", p_gold);

    return 0;
}

int main () {
    int c = 0;
    int cols, rows;

    initscr();
    keypad(stdscr ,1); //считываем стрелки

    noecho(); //не выводим ввод
    curs_set(0); //не мигает курсор

    getmaxyx(stdscr, rows, cols); //достаем границы
    printw("rows: %d, cols: %d", rows, cols);

    char map[rows][cols];

    do{ //основной наш цикл
        dungeon(c, rows -1, cols -1, map);
    } while ((c = getch()) != 27); //пока не esc

    getch();

    endwin();
    return 0;
}