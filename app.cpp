#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <iostream>
#include <tuple>
#include <vector>

const int X_POSITION = 0;
const int WIDTH = 60;
const int HEIGHT = 23;
const int HEADER_HIGHT = 3;
const int FOOTER_HEIGHT = 3;
const int MAIN_WINDOW_HEIGHT = HEIGHT - FOOTER_HEIGHT - HEADER_HIGHT + 1;
const int GAME_BOARD_HEIGHT = MAIN_WINDOW_HEIGHT - 2;
const int MAIN_WINDOW_Y = HEADER_HIGHT - 1;
const int GAME_BOARD_Y = MAIN_WINDOW_Y + 1;
const int FOOTER_Y = GAME_BOARD_Y + GAME_BOARD_HEIGHT;
const int SHIP_Y = 15;

std::tuple<WINDOW *, WINDOW *, WINDOW *, WINDOW *> CreateGameWindows() {
  WINDOW *header = newwin(HEADER_HIGHT, WIDTH, 0, X_POSITION);
  wborder(header, 0, 0, 0, ' ', 0, 0, 0, 0);
  mvwprintw(header, 1, WIDTH / 2 - 8, "Space Invaders");
  WINDOW *footer = newwin(FOOTER_HEIGHT, WIDTH, FOOTER_Y, X_POSITION);
  wborder(footer, 0, 0, ' ', 0, 0, 0, 0, 0);
  mvwprintw(footer, 1, 1, "Score: ");
  WINDOW *mainWin =
      newwin(MAIN_WINDOW_HEIGHT, WIDTH, MAIN_WINDOW_Y, X_POSITION);
  box(mainWin, 0, 0);
  WINDOW *gameWin =
      newwin(GAME_BOARD_HEIGHT, WIDTH - 2, GAME_BOARD_Y, X_POSITION + 1);
  nodelay(gameWin, TRUE);

  for (WINDOW *win : {header, footer, mainWin, gameWin}) {
    wrefresh(win);
  }
  return std::make_tuple(mainWin, gameWin, header, footer);
}

void DrawShip(WINDOW *win, int x) { mvwprintw(win, SHIP_Y, x, "^"); }

void DrawBullets(WINDOW *win, std::vector<std::tuple<int, int>> &bullets) {
  for (std::tuple<int, int> bullet : bullets) {
    mvwprintw(win, std::get<0>(bullet), std::get<1>(bullet), "o");
  }
}

void UpdateBullets(WINDOW *win, std::vector<std::tuple<int, int>> &bullets) {
  for (int i = 0; i < bullets.size(); i++) {
    int x = std::get<0>(bullets[i])--;
    if (x < 1) {
      bullets.erase(bullets.begin() + i);
      continue;
    }
  }
}

bool inputLeft(int ch) { return ch == KEY_LEFT || ch == 'a'; }

bool inputRight(int ch) { return ch == KEY_RIGHT || ch == 'd'; }

bool inputSpace(int ch) { return ch == ' '; }

bool inputAuthor(int ch) { return ch == 'k'; }

int main() {
  int shipX = WIDTH / 2;
  std::vector<std::tuple<int, int>> bullets;
  initscr();
  noecho();
  curs_set(0);
  cbreak();
  srand(time(0));

  auto [mainWin, gameWin, header, footer] = CreateGameWindows();

  while (true) {
    wclear(gameWin);
    DrawShip(gameWin, shipX);
    DrawBullets(gameWin, bullets);
    wrefresh(gameWin);
    int moveX = 0;
    int ch = wgetch(gameWin);

    if (inputLeft(ch)) {
      moveX = shipX > 2 ? -3 : 0;
    }
    if (inputRight(ch)) {
      moveX = shipX < WIDTH - 5 ? 3 : 0;
    }
    if (inputSpace(ch)) {
      bullets.push_back(std::make_tuple(SHIP_Y - 1, shipX));
    }
    if (inputAuthor(ch)) {
      mvwprintw(footer, 1, WIDTH - 28, "created by Kamil Safaryjski");
      wrefresh(footer);
    }

    shipX += moveX;
    UpdateBullets(gameWin, bullets);
    usleep(45000);
  }
  for (WINDOW *win : {header, footer, mainWin, gameWin}) {
    delwin(win);
  }
  endwin();
  return 0;
}