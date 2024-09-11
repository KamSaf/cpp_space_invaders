#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <iostream>
#include <tuple>
#include <vector>

struct Point {
  int x, y;
};

void DrawShip(WINDOW *win, int x, const int y) {
  mvwprintw(win, y, x, "^");
  wrefresh(win);
}

void DrawBullets(WINDOW *win, std::vector<std::tuple<int, int>> &bullets) {
  for (std::tuple<int, int> bullet : bullets) {
    mvwprintw(win, std::get<0>(bullet), std::get<1>(bullet), "o");
  }
  wrefresh(win);
}

void UpdateBullets(WINDOW *win, std::vector<std::tuple<int, int>> &bullets) {
  for (int i = 0; i < bullets.size(); i++) {
    int x = std::get<0>(bullets[i])--;
    if (x < 5) {
      bullets.erase(bullets.begin() + i);
      continue;
    }
  }
  wrefresh(win);
}

WINDOW *CreateGameWindow(int height, int width) {
  WINDOW *win = newwin(height, width, 0, 0);
  box(win, 0, 0);
  wrefresh(win);
  return win;
}

bool inputLeft(int ch) { return ch == KEY_LEFT || ch == 'a'; }

bool inputRight(int ch) { return ch == KEY_RIGHT || ch == 'd'; }

bool inputSpace(int ch) { return ch == ' '; }

int main() {
  const int WIDTH = 60;
  const int HEIGHT = 20;
  int shipX = WIDTH / 2;
  const int shipY = 16;
  std::vector<std::tuple<int, int>> bullets;
  initscr();
  noecho();
  curs_set(0);
  cbreak();
  srand(time(0));
  WINDOW *mainWin = CreateGameWindow(HEIGHT, WIDTH);

  WINDOW *gameWind = newwin(HEIGHT - 2, WIDTH - 2, 1, 1);
  nodelay(gameWind, TRUE);
  wrefresh(gameWind);

  while (true) {
    wclear(gameWind);
    DrawShip(gameWind, shipX, shipY);
    DrawBullets(gameWind, bullets);
    int moveX = 0;
    int ch = wgetch(gameWind);

    if (inputLeft(ch)) {
      moveX = shipX > 2 ? -3 : 0;
    }
    if (inputRight(ch)) {
      moveX = shipX < 55 ? 3 : 0;
    }
    if (inputSpace(ch)) {
      bullets.push_back(std::make_tuple(shipY - 1, shipX));
    }
    shipX += moveX;
    UpdateBullets(gameWind, bullets);
    usleep(45000);
  }
  delwin(gameWind);
  delwin(mainWin);

  endwin();
  return 0;
}