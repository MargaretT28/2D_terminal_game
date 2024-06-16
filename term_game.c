#include <ncurses.h>
#include <stdlib.h> //для ранд функции
#include <time.h>
#include <stdbool.h>

int py, px; // корды игрока
int p_gold = 0; //голда
bool t_placed = 0; //флаг расположения предмета
bool p_placed = 0; //флаг расположения игрока
int r_placed = 0; //комнаты
int lvl = 1;

struct money {
    int y;
    int x;
};

struct money money[10];

int grab_money(int cols, char (*map)[cols], int dir_y, int dir_x) {
    for (int m = 0; m < 10; m++) {
	if (dir_y == money[m].y && dir_x == money[m].x) {
	    map[dir_y][dir_x] = ' ';
	    p_gold += rand() % 10 + 1;
	    break;
	}
    }
    return 0;
}

int p_action(int c, int cols, char (*map)[cols]) {
    int dir_y = py, dir_x = px;
    if (c == KEY_UP)
        dir_y--;
    else if (c == KEY_DOWN)
        dir_y++;
    else if (c == KEY_LEFT)
    	dir_x--;
    else if (c == KEY_RIGHT)
	dir_x++;
    else if (c == '>' && map[py][px] == '>'){
	t_placed = 0; //флаг расположения предмета
	p_placed = 0; //флаг расположения игрока
	r_placed = 0; //комнаты
	return 1;
    }

    if (map[dir_y][dir_x] == ' ' || map[dir_y][dir_x] == '>') {
	py = dir_y;
	px = dir_x;
    } else if (map[dir_y][dir_x] == 't') 
	grab_money(cols, map, dir_y, dir_x);
    return 0;
}

int map_draw(int rows, int cols, char (*map)[cols]) {
    for (int y = 0; y <= rows; y++){ //рисуем стены
        for (int x = 0; x <= cols; x++){
	    if (y == rows) {
	        mvaddch(y, x, ' ');
	    } else if (map[y][x] == 't') {
	        mvaddch(y, x, 't');
	    } else if (map[y][x] == '>') {
	        mvaddch(y, x, '>');
            } else if (map[y][x] == '%') {
                mvaddch(y, x, '%');
            } else if (map[y][x] == ' ') {
                mvaddch(y, x, ' ');
            } else {
                mvaddch(y, x, ':');
            }
        }
    }
    mvprintw(rows, 0, "Gold: %d \t Lvl: %d", p_gold, lvl);

    return 0;
}

int respawn_creatures(int rows, int cols, char (*map)[cols]) {
    if (!t_placed){
	int my, mx;
	for (int m = 0; m < 10; m++) {
            do {
                my = rand() % rows;
                mx = rand() % cols;
            } while (map[my][mx] != ' '); //пока попадаем в стены генерим новые, чтобы попасть в окно
	    money[m].y = my;
	    money[m].x = mx;
	    map[money[m].y][money[m].x] = 't';

        }
	t_placed = 1;
    }

    if (!p_placed){
        do {
            py = rand() % rows;
            px = rand() % cols;
        } while (map[py][px] == ':' || map[py][px] == '%'); //пока попадаем в стены генерим новые, чтобы попасть в окно
        p_placed = 1;
    }
    return 0;
}

int map_gen(int rows, int cols, char (*map)[cols]) {
    if (!r_placed) {
	int ry, rx, r_size_y, r_size_x; //корды и размер комнат
	int r_old_center_y, r_old_center_x;
        int r_center_y, r_center_x;
	int room_num = rand() % 5 + 5; //количество комнат
        bool collision;

	for (int y = 0; y <= rows; y++){ //рисуем стены
            for (int x = 0; x <= cols; x++){
                if (y == 0 || y == rows - 1 || x ==0 || x == cols || y == rows) {
                    map[y][x] = '%'; //границы
                } else {
                    map[y][x] = ':'; //стены
                }
            }
        }

        while (r_placed < room_num) {
	    int try_cnt = 0; //кол-во попыток генерации
	    //генерим комнаты пока не будет коллизий
	    do {
		collision = 0;

                //генерим координаты комнаты
                ry = rand() % (rows - 4) + 1;
                rx = rand() % (cols - 4) + 1;

                //размер комнаты
                r_size_y = rand() % 5 + 4;
                r_size_x = rand() % 10 + 8;

		//предохранитель (если комнаты некуда расставить -> вечный цикл)
		try_cnt++;
		if (try_cnt > 100) {   
		    ry = rx = 3;
		    r_size_y = r_size_x = 3;	
		    break;
		}

                //добавляем комнаты
                for (int y = ry; y <= ry + r_size_y; y++) { //рисуем комнату
                    for (int x = rx; x <= rx + r_size_x; x++){
                        if (map[y][x] == '%' || map[y][x] == ' ' ||
			    map[y + 2][x] == ' ' || map[y - 2][x] == ' ' ||
			    map[y][x + 2] == ' ' || map[y][x - 2] == ' '){   //всегда между комнатами минимум 2 стенки
			    collision = 1;
                            y = ry + r_size_y; //выход из внешнего цикла
                            break;
                        }
                    }
                }

	    } while (collision);

            //добавляем комнаты
            for (int y = ry; y <= ry + r_size_y; y++) { //рисуем комнату
                for (int x = rx; x <= rx + r_size_x; x++){
                    if(map[y][x] == '%'){
                        y = ry + r_size_y; //выход из внешнего цикла
                        break;
                    } else {
                        map[y][x] = ' ';
                    }
                }
            }
            r_placed++;

	    //корридоры
	    if (r_placed > 1) {
	        r_old_center_y = r_center_y;
	        r_old_center_x = r_center_x;
	    }

	    r_center_y = ry + (r_size_y/2);
	    r_center_x = rx + (r_size_x/2);

	    if (r_placed > 1) {
		int temp_y;
                for (temp_y = r_old_center_y; temp_y != r_center_y; ) {
		    map[temp_y][r_old_center_x] = ' ';
		    if (r_old_center_y < r_center_y) {
		        temp_y++;
		    } else if (r_old_center_y > r_center_y){
		        temp_y--;
		    }
	        }

	        for (int x = r_old_center_x; x != r_center_x; ) {
		    map[temp_y][x] = ' ';
		    if (r_old_center_x < r_center_x) {
		        x++;
		    } else if (r_old_center_x > r_center_x){
		        x--;
		    }
	        }
    	    }
	} 
	//переход между уровнями
	int sy, sx; 
	do {
            sy = rand() % rows;
            sx = rand() % cols;
	} while (map[sy][sx] != ' ');
	map[sy][sx] = '>';
    }

    return 0;
}

int game_loop(int rows, int cols, char (*map)[cols]) { //рисуем комнату
    int c;
    int new_lvl = 0;
    srand(time(NULL)); //каждый раз число будет разным изза времени. srand потому что seed random

    //заполнение карты
    map_gen(rows, cols, map);

    respawn_creatures(rows, cols, map);

    if (c != 0) {
	new_lvl = p_action(c, cols, map); //действия игрока, включая подбор денег
    }

    //отрисовка карты
    map_draw(rows, cols, map);

    mvaddch(py, px, 'O');  //расположить игрока

    if (new_lvl) {
	clear();
	mvprintw(rows/2 - 1, cols/2 - 10, "Welcome to level %d", ++lvl);
        mvprintw(rows/2 + 1, cols/2 - 10, "Press Enter to continue");
    }

    c = getch();

    return c;
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
        c = game_loop(rows -1, cols -1, map);
    } while (c != 27); //пока не esc

    getch();

    endwin();
    return 0;
}
