#include <ncurses.h>
#include <stdlib.h> //для ранд функции, чтобы когда мы находим на предмет он появлялся в др месте
#include <time.h>
#include <stdbool.h>

int py, px; // корды игрока
int ty, tx; //корды предмета
int p_gold = 0; //голда
bool t_placed = 0; //флаг расположения предмета
bool p_placed = 0; //флаг расположения игрока
int r_placed = 0; //комнаты


int dungeon(int c, int rows, int cols, char (*map)[cols]) { //рисуем комнату
    srand(time(NULL)); //каждый раз число будет разным изза времени. srand потому что seed random

    //заполнение карты
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
    }


    //отрисовка подземелья
    for (int y = 0; y <= rows; y++){ //рисуем стены
        for (int x = 0; x <= cols; x++){
	    if (y == rows) {
	        mvaddch(y, x, ' ');
            } else if (map[y][x] == '%') {
                mvaddch(y, x, '%');
            } else if (map[y][x] == ' ') {
                mvaddch(y, x, ' ');
            } else {
                mvaddch(y, x, ':');
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
