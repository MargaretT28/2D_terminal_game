# Проект 2D game In Terminal

---
Участники: Гиркало Елизавета, Тураева Маргарита
---

Иногда хочется скоротать время за какой-нибудь простой игрой. В такие моменты игра в терминале может стать хорошим решением.
Огромное количество 2D игр имеют легкую механику. А именно это просто квадратные клетки, по которым двигаются персонажи на ходу, лежат предметы, стоят стены и тд. Тогда задача - написать игру полностью в терминале.

**функционал**
1. строить карту и обновлять её (вместе с доп информацией)
2. добавить игрока и движение (up, down, left, right)
3. добавить "монетки" и их обработку
4. Сделать счет
5. Придумать сюжет/предысторию
   
P.S. В дальнейшем может добавиться другой функционал или даже режимы игры

## Распределение задач
Совместно придумать сюжет игры (предысторию персонажа). Затем 1 человек займётся реализацией карты, её обновлением и счётом, а второй добавит персонажа, монетки и их взаимодействие. 

P.S. может меняться при необходимости в ходе разработки

## Технологии
с/с++ и ncurses

## Проделанная работа

**1) Мы изучили различную информацию по ncurses. Например следующие статьи:**

https://www.nixp.ru/articles/102.html

http://alexber220.narod.ru/ncurses/

https://code-live.ru/post/cpp-ncurses-hello-world/#ncurses

https://skobki.com/yazyk-si-ncurses/

**2) Совместно проработали сюжет**


Студент НИУ ВШЭ Саша участвует в аналоге марафона HSE-RUN. В  марафоне новые правила. Игрок перемещается по локациям и в каждой из них собирает голографический предмет. Саша может перемещаться по комнатам бесконечно, пока ему не надоест.

«Вот кому-то нечего делать!» - скажете Вы, но не все так просто!

Под каждой голограммой спрятано несколько монет, максимум 10, и вся прелесть игры в том, что «пустых» монет не бывает! Таким образом, Саша не только отвлекается от учебы, но и зарабатывает


**3) Написали саму игру**

   Код в отдельном файле

**4) Обновление 30 марта**

   Вместо просто генерации 1 комнаты, теперь генерируется полноценная карта с комнатами и коридорами

**4) Обновление 16 июня**

   Теперь генерируется несколько монет на каждом уровне (для этого была добавлена структура монет и частично изменена логика программы)

   В качестве допольнительного функционала была добавлена система уровней и возможность самостоятельно переходить на следующий уровень. Теперь игрок сам решает когда переходить на следующий уровень

## Инструкция использования


   Скачать файл с кодом программы
   
   Установить библиотеку ncurses. команда для установки из терминала: sudo apt-get install libncurses-dev

   В терминале зайти в папку с игрой
   
   Скомпилировать программу: gcc term_game.c -o term_game -lncurses
   
   Запустить: ./term_game
   
   Можно играть) (управление стрелочками, для перехода на следующий уровень нужно наступить и одновременно нажать на кнопку '>', выйти можно на esc или ctrl+C)
