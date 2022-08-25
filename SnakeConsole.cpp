// SnakeConsole.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <queue>
#include <windows.h>
#include <conio.h>

struct position {
    int x, y;
    position() {
        x = y = 0;
    }
    position(int x, int y) {
        this->x = x;
        this->y = y;
    }
    position operator+(position& pos) {
        x += pos.x;
        y += pos.y;
        return *this;
    }
    position operator-(position& pos) {
        x -= pos.x;
        y -= pos.y;
        return *this;
    }
};
struct find_pos : std::unary_function<position, bool> {
    int x, y;
    find_pos(position pos) { x = pos.x; y = pos.y; }
    bool operator()(position const& pos) {
        if (pos.x == x && pos.y == y) return true;
        else return false;
    }
};
enum directions{
    right=0,left,down,up
};
void  ClearScreen() {
    char fill = ' ';
    COORD cursorPosition;
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO s;
    GetConsoleScreenBufferInfo(console,&s);
    DWORD written, cells = s.dwSize.X * s.dwSize.Y;
    cursorPosition.X = 0;
    cursorPosition.Y = 0;
    FillConsoleOutputCharacter(console,fill,cells,cursorPosition,&written);
    FillConsoleOutputAttribute(console,s.wAttributes,cells, cursorPosition,&written);
    SetConsoleCursorPosition(console, cursorPosition);
}

void InputDirection(directions& dirs) {
    if (_kbhit()) {
        switch (_getch()) {
        case 230: {if(dirs != down)dirs = up; break; }
        case 235: {if(dirs != up)dirs = down; break; }
        case 162: {if(dirs!=left)dirs = right; break; }
        case 228: {if(dirs!=right)dirs = left; break; }
        }
    }
}

int main()
{
    bool game_over = false;
    int score = 0;
    directions direction = right;
    int width = 25;
    int height = 50;
    int lastFoodTime = 0;
    int foodDissapearTime = 8000;
    int negativePoints = 0;
    position right_pos(0, 1);
    position* dirs = new position[4]{position(0,1),position(0,-1),position(1,0),position(-1,0)};
    position food;
    std::vector<position> obstacles;
    std::vector<position> snake;
    obstacles.push_back(position(12,12));
    obstacles.push_back(position(14, 20));
    obstacles.push_back(position(7, 7));
    obstacles.push_back(position(19, 19));
    obstacles.push_back(position(6, 9));
    for(int i=0;i<=5;i++)
        snake.push_back(position(0,i));
    do {
        int posX = rand() % width,posY = rand() % height;
        food.x = 1+posX;
        food.y = 1+posY;
    } while (std::find_if(snake.begin(), snake.end(), find_pos(food)) != snake.end()  && std::find_if(obstacles.begin(), obstacles.end(), find_pos(food)) != obstacles.end());
    //borders
    for (int i = 0; i < height; i++) {
        std::cout << '-';
    }
    std::cout << std::endl;
    while (!game_over) {
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                if (j == 0 || j == height - 1)
                    std::cout << '|';
                else {
                    if (std::find_if(snake.begin(), snake.end(), find_pos(position(i, j))) != snake.end()) {
                        position last = snake.at(snake.size() - 1);
                        if (last.x == i && last.y == j) {
                            if (direction == right) std::cout << '>';
                            if (direction == left) std::cout << '<';
                            if (direction == up) std::cout << '^';
                            if (direction == down) std::cout << 'v';
                        }
                        else std::cout << '*';
                    }
                    else if (std::find_if(obstacles.begin(), obstacles.end(), find_pos(position(i, j))) != obstacles.end()) {
                        std::cout << '=';
                    }
                    else if (food.x == i && food.y == j) {
                        std::cout << '@';
                    }
                    else {
                        std::cout << ' ';
                    }
                }
            }
            std::cout << std::endl;
        }
        for (int i = 0; i < height; i++) {
            std::cout << '-';
        }
       /* if (std::find_if(snake.begin(), snake.end(), find_pos(snake[snake.size() - 1] + dirs[direction])) != snake.end()) {
            game_over = true;
        }*/
        snake.push_back(snake[snake.size() - 1] + dirs[direction]);
        snake.erase(snake.begin());
        for (int i = snake.size() - 1; i > 0 ; i--) {
            if (snake[i].x == width) {
                snake[i].x = 1;
                break;
            }
            if (snake[i].y == height) {
                snake[i].y = 1;
                break;
            }
            if (snake[i].x == 1) {
                snake[i].x = width - 1;
                break;;
            }
            if (snake[i].y == 1) {
                snake[i].y = height - 1;
                break;
            }
        }
        for (int i = 0; i < obstacles.size(); i++)
        {
            if(std::find_if(snake.begin(), snake.end(), find_pos(obstacles[i])) != snake.end()){
                game_over = true;
            }
        }
        if (snake[snake.size()-1].x == food.x && snake[snake.size() - 1].y == food.y) {
            score += 10;
            snake.insert(snake.begin(), 1, snake[0] - dirs[direction]);
        }
        Sleep(50);
        ClearScreen();
        InputDirection(direction);
    }
    std::cout << "your score " << score << std::endl;
}


