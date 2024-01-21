#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

bool end_game;

const int width = 20;
const int height = 20;

int x, y, fruit_x, fruit_y, score;

int tail_x[100], tail_y[100];
int n_tail;

std::string fruit = "$";

enum e_directions
{
 stop = 0,
 left,
 right,
 up,
 down
};

e_directions dir;

void setup()
{
  end_game = false;
  dir = stop;
  x = width / 2;
  y = height / 2;
  fruit_x = rand() % width + 1;
  fruit_y = rand() % height + 1;
  score = 0;
}

void draw()
{
  system("cls");
  for (int i = 0; i < width + 2; i++)
  {
    std::cout << "||";
  }
  std::cout << std::endl;
  for (int i = 1; i <= height; i++)
  {
    for (int j = 1; j <= width + 1; j++)
    {
      if ( j == 1 || j == height + 1)
        std::cout << "||";
      if (i == y && j == x)
      {
        std::cout << "0";
      }
      else if (j == fruit_x && i == fruit_y)
      {
        std::cout << fruit;
      }
      else
      {
        bool print = false;
        for (int k = 0; k < n_tail; k++)
        {
          if (tail_x[k] == j && tail_y[k] == i)
          {
            std::cout << "o";
            print = true;
          }
        }
        if (!print)
          std::cout << " ";
      }
    }
    std::cout << std::endl;
  }
  for (int i = 0; i < width + 2; i++)
  {
    std::cout << "#";
  }
  std::cout << std::endl;
  std::cout << "Score: " << score << std::endl;
}

void input(char key)
{
  if (read(STDIN_FILENO, &key, 1) > 0)
  {
    switch (key)
    {
      case 'a':
        dir = left;
        break;
      case 'd':
        dir = right;
        break;
      case 'w':
        dir = up;
        break;
      case 's':
        dir = down;
        break;
      case 'x':
        end_game = true;
        break;
    }
  }
}

void logic()
{
  int prev_x = tail_x[0];
  int prev_y = tail_y[0];
  int prev_two_x, prev_two_y;
  tail_x[0] = x;
  tail_y[0] = y;
  for (int i = 1; i < n_tail; i++)
  {
    prev_two_x = tail_x[i];
    prev_two_y = tail_y[i];
    tail_x[i] = prev_x;
    tail_y[i] = prev_y;
    prev_x = prev_two_x;
    prev_y = prev_two_y;
  }
  switch (dir)
  {
    case left:
      x--;
      break;
    case right:
      x++;
      break;
    case up:
      y--;
      break;
    case down:
      y++;
      break;
  }
  if ((x > width || x < 0) || (y > height || y < 0))
  {
    std::cout << "\t\tGame Over" << std::endl;
    std::cout << "\t u hit the wall!";
    end_game = true;
  }
  for (int i = 0; i < n_tail; i++)
  {
    if (tail_x[i] == x && tail_y[i] == y)
    {
      std::cout << "\t\tgame over" << std::endl;
      std::cout << "\t u hit the tail :(";
      end_game = true;
    }
  }
  if (x == fruit_x && y == fruit_y)
  {
    score += 10;
    n_tail++;
    fruit_x = rand() % width + 1;
    fruit_y = rand() % height + 1;
  }
}

int main()
{
  setup();
  char key;
  struct termios old_t, new_t;

  std::cout << "Press a key (q to quit): ";

  tcgetattr(STDIN_FILENO, &old_t);
  new_t = old_t;

  new_t.c_lflag &= ~(ICANON | ECHO);

  tcsetattr(STDIN_FILENO, TCSANOW, &new_t);

  while (!end_game)
  {
    draw();
    input(key);
    logic();
    //sleep(150);
  }
  std::cout << fruit_x << " " << fruit_y;

  tcsetattr(STDIN_FILENO, TCSANOW, &old_t);

  return 0;
}
