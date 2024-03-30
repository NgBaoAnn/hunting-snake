#include <iostream>
#include <conio.h>
#include <thread>
#include <ctime>
#include "mylib.h"
#include "cstring"
#include <stdlib.h>
#include <string>
using namespace std;
#define MAX_SIZE_SNAKE 40
#define MAX_SIZE_FOOD 40
#define MAX_SPEED 3
struct Point
{
    int x;
    int y;
};
Point snake[40];
Point food[40];
Point wall_1[40];
Point wall_2[62];
int COUNT = 0;
int START = 0;
int CHAR_LOCK; // được sử dụng để xác định hướng mà con rắn của tôi không thể di chuyển đến 
// (Tại một thời điểm, có một hướng mà con rắn của tôi không thể di chuyển đến)
int MOVING; // được sử dụng để xác định hướng mà con rắn của tôi có thể di chuyển đến 
// (Tại một thời điểm, có ba hướng mà con rắn của tôi có thể di chuyển đến)
int SPEED; // Đại diện cho cấp độ, càng cao cấp độ, tốc độ càng nhanh
int HEIGH_CONSOLE;
int WIDTH_CONSOLE; // Độ rộng và chiều cao của màn hình console
int FOOD_INDEX; // chỉ số thức ăn hiện tại
int SIZE_SNAKE; // kích thước của con rắn, ban đầu có thể là 5 đơn vị và kích thước tối đa có thể là 40
int STATE; // Trạng thái của con rắn: chết hoặc sống
int STOP_DRAW_FOOD = 0; // Bật tắt chế độ tạo đồ ăn
int LEVEL; // Chỉ số lưu màn chơi
int NEW_LEVEL = 1; // Bật tắt chế độ chuyển màn chơi
int TIMEBIGFOOD = 0;
int SCORE = 0;
void FixConsoleWindow() {
    HWND consoleWindow = GetConsoleWindow();
    LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
    style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
    SetWindowLong(consoleWindow, GWL_STYLE, style);
}
void GetDesktopResolution(int& horizontal, int& vertical) {
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    horizontal = desktop.right;
    vertical = desktop.bottom;
}
void Hide_Console_Cursor()
{
    CONSOLE_CURSOR_INFO cursor_info;
    cursor_info.dwSize = 1;
    cursor_info.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
void DeleteSnakeAndFood()
{
    gotoXY(food[FOOD_INDEX].x, food[FOOD_INDEX].y);
    cout << " ";
    for (int i = 0; i < SIZE_SNAKE; i++) {
        gotoXY(snake[i].x, snake[i].y);
        cout << " ";
    }
}
void DrawSnake(int n, int m) {
    SetColor(228);
    for (int i = n; i < m; i++) {
        gotoXY(snake[i].x, snake[i].y);
        switch (i)
        {
        case 0:
            cout << "2";
            break;
        case 1:
            cout << "3";
            break;
        case 2:
            cout << "1";
            break;
        case 3:
            cout << "2";
            break;
        case 4:
            cout << "0";
            break;
        case 5:
            cout << "1";
            break;
        case 6:
            cout << "9";
            break;
        case 7:
            cout << "0";
            break;
        case 8:
            cout << "2";
            break;
        case 9:
            cout << "3";
            break;
        case 10:
            cout << "1";
            break;
        case 11:
            cout << "2";
            break;
        case 12:
            cout << "0";
            break;
        case 13:
            cout << "1";
            break;
        case 14:
            cout << "8";
            break;
        case 15:
            cout << "9";
            break;
        case 16:
            cout << "2";
            break;
        case 17:
            cout << "3";
            break;
        case 18:
            cout << "1";
            break;
        case 19:
            cout << "2";
            break;
        case 20:
            cout << "0";
            break;
        case 21:
            cout << "2";
            break;
        case 22:
            cout << "0";
            break;
        case 23:
            cout << "2";
            break;
        case 24:
            cout << "2";
            break;
        case 25:
            cout << "3";
            break;
        case 26:
            cout << "1";
            break;
        case 27:
            cout << "2";
            break;
        case 28:
            cout << "0";
            break;
        case 29:
            cout << "2";
            break;
        case 30:
            cout << "0";
            break;
        case 31:
            cout << "7";
            break;
        case 32:
            cout << "2";
            break;
        case 33:
            cout << "3";
            break;
        case 34:
            cout << "1";
            break;
        case 35:
            cout << "2";
            break;
        case 36:
            cout << "0";
            break;
        case 37:
            cout << "2";
            break;
        case 38:
            cout << "0";
            break;
        case 39:
            cout << "8";
            break;
        }
    }
}
void DrawFood()
{
    if (TIMEBIGFOOD == 3)
    {
        gotoXY(food[FOOD_INDEX].x, food[FOOD_INDEX].y);
        cout << "B";
    }
    else
    {
        gotoXY(food[FOOD_INDEX].x, food[FOOD_INDEX].y);
        cout << "F";
    }
}
void Gate_End()
{
    gotoXY(WIDTH_CONSOLE + 1, HEIGH_CONSOLE / 2);
    cout << " ";
    gotoXY(WIDTH_CONSOLE - 1, HEIGH_CONSOLE / 2 + 1);
    cout << "XX";
    gotoXY(WIDTH_CONSOLE - 1, HEIGH_CONSOLE / 2 - 1);
    cout << "XX";
}
void Delete_Gate_End()
{
    gotoXY(WIDTH_CONSOLE + 1, HEIGH_CONSOLE / 2);
    cout << char(186);
    gotoXY(WIDTH_CONSOLE - 1, HEIGH_CONSOLE / 2 + 1);
    cout << "  ";
    gotoXY(WIDTH_CONSOLE - 1, HEIGH_CONSOLE / 2 - 1);
    cout << "  ";
}
void Delete_Gate_Start()
{
    gotoXY(1, HEIGH_CONSOLE / 2);
    cout << char(186);
    gotoXY(2, HEIGH_CONSOLE / 2 + 1);
    cout << "  ";
    gotoXY(2, HEIGH_CONSOLE / 2 - 1);
    cout << "  ";
}
void Gate_Start()
{
    gotoXY(1, HEIGH_CONSOLE / 2);
    cout << " ";
    gotoXY(2, HEIGH_CONSOLE / 2 + 1);
    cout << "XX";
    gotoXY(2, HEIGH_CONSOLE / 2 - 1);
    cout << "XX";
}
bool IsValid(int x, int y) // Kiểm tra sự trùng nhau của rắn và đồ ăn 
{
    for (int i = 0; i < SIZE_SNAKE; ++i)
        if (snake[i].x == x && snake[i].y == y)
            return false;
    for (int i = 0; i < COUNT; ++i)
    {
        if (LEVEL == 1 && wall_2[i].x == x && wall_2[i].y == y)
            return false;
    }
    return true;
}
void GenerateFood() {
    int x, y;
    srand(static_cast<unsigned int>(time(NULL)));
    for (int i = 0; i < MAX_SIZE_FOOD; i++) {
        do {
            x = rand() % (WIDTH_CONSOLE - 1) + 2;
            y = rand() % (HEIGH_CONSOLE - 1) + 1;
        } while (IsValid(x, y) == false);
        food[i] = { x,y };
    }
}
void ResetData() {
    // Khởi tạo các biến cục bộ
    CHAR_LOCK = 'A', MOVING = 'D', SPEED = 1; FOOD_INDEX = 0, SIZE_SNAKE = 4, LEVEL = 1;
    HEIGH_CONSOLE = 20;
    WIDTH_CONSOLE = 70;
    snake[0] = { 1, HEIGH_CONSOLE / 2 };
}
void DrawBox(int x, int y, int w, int h) {
    system("cls");
    SetColor(9);
    for (int ix = x; ix <= x + w; ix++) {
        gotoXY(ix, y);
        cout << char(205);
        gotoXY(ix, y + h);
        cout << char(205);
    }
    for (int iy = y; iy <= y + h; iy++) {
        gotoXY(x, iy);
        cout << char(186);
        gotoXY(x + w, iy);
        cout << char(186);
    }
    gotoXY(x, y);			cout << char(201);
    gotoXY(x + w, y);		cout << char(187);
    gotoXY(x, y + h);		cout << char(200);
    gotoXY(x + w, y + h);	cout << char(188);
}
void Level_1(int x, int y, int width, int height) {
    COUNT = 0;
    system("cls");
    DrawBox(x, y, WIDTH_CONSOLE, HEIGH_CONSOLE);
    int n = (width * 2) / 7 + 1;
    int pos = 1;
    gotoXY((width * 2) / 7 + 1, height / 3);
    cout << "X";
    for (int i = 0; i < (width * 5) / 7 + 1; i += 2)
    {
        wall_2[i] = { n,height / 3 };
        wall_2[pos] = { n, (height * 2) / 3 };
        pos += 2;
        n++;
    }
    for (int i = 0; i < (width * 5) / 7 + 1; ++i)
    {
        COUNT++;
        gotoXY(wall_2[i].x, wall_2[i].y);
        cout << "X";
    }
}
void Level_2(int x, int y, int width, int height)
{
    system("cls");

}
void StartLevel()
{

    NEW_LEVEL = 0;
    Gate_Start();
    snake[0] = { 2, HEIGH_CONSOLE / 2 };
    int add_body = 2;
    while (add_body < SIZE_SNAKE)
    {
        DeleteSnakeAndFood();
        for (int i = add_body - 2; i >= 0; --i)
        {
            snake[i + 1].x = snake[i].x;
            snake[i + 1].y = snake[i].y;
        }
        add_body++;
        snake[0].x++;
        DrawSnake(0, add_body - 1);
        Sleep(250);
    }
    if (LEVEL == 1)
    {
        Level_1(1, 0, WIDTH_CONSOLE, HEIGH_CONSOLE);
    }
    else if (LEVEL == 2)
    {
        Level_2(1, 0, WIDTH_CONSOLE, HEIGH_CONSOLE);
    }
    GenerateFood();
    STOP_DRAW_FOOD = 0;
}
void StartGame() {
    system("cls");
    ResetData(); // Khởi tạo dữ liệu ban 
    Level_1(1, 0, WIDTH_CONSOLE, HEIGH_CONSOLE);
    StartLevel();
    STATE = 1;// Bắt đầu chạy luồng
}
// Hàm thoát game
void closeWindow() {
    HWND hwnd = GetForegroundWindow(); // Lấy cửa sổ hiện đang được focus
    if (hwnd != NULL) {
        PostMessage(hwnd, WM_CLOSE, 0, 0); // Gửi thông điệp WM_CLOSE để đóng cửa sổ
    }
}
// Hàm thoát game
void PauseGame(HANDLE t) {
    SuspendThread(t);
}
void EatBigFood()
{
    FOOD_INDEX += 3;
    SIZE_SNAKE += 3;
    if (MOVING == 'D')
    {
        int pos = 1;
        snake[SIZE_SNAKE - 2].x = food[FOOD_INDEX].x;
        snake[SIZE_SNAKE - 2].y = food[FOOD_INDEX].y;
        for (int i = 1; i >= 0; --i)
        {
            snake[SIZE_SNAKE - i].x = food[FOOD_INDEX].x + pos++;
            snake[SIZE_SNAKE - i].y = food[FOOD_INDEX].y;
        }
    }
    if (MOVING == 'A')
    {
        int pos = 1;
        snake[SIZE_SNAKE - 2].x = food[FOOD_INDEX].x;
        snake[SIZE_SNAKE - 2].y = food[FOOD_INDEX].y;
        for (int i = 1; i >= 0; --i)
        {
            snake[SIZE_SNAKE - i].x = food[FOOD_INDEX].x - pos++;
            snake[SIZE_SNAKE - i].y = food[FOOD_INDEX].y;
        }
    }
    if (MOVING == 'S')
    {
        int pos = 1;
        snake[SIZE_SNAKE - 2].x = food[FOOD_INDEX].x;
        snake[SIZE_SNAKE - 2].y = food[FOOD_INDEX].y;
        for (int i = 1; i >= 0; --i)
        {
            snake[SIZE_SNAKE - i].x = food[FOOD_INDEX].x;
            snake[SIZE_SNAKE - i].y = food[FOOD_INDEX].y + pos++;
        }
    }
    if (MOVING == 'W')
    {
        int pos = 1;
        snake[SIZE_SNAKE - 2].x = food[FOOD_INDEX].x;
        snake[SIZE_SNAKE - 2].y = food[FOOD_INDEX].y;
        for (int i = 1; i >= 0; --i)
        {
            snake[SIZE_SNAKE - i].x = food[FOOD_INDEX].x;
            snake[SIZE_SNAKE - i].y = food[FOOD_INDEX].y - pos++;
        }
    }
}
// Hàm cập nhật biến cục bộ
void Eat() {
    if (FOOD_INDEX == MAX_SIZE_FOOD - 1)
    {
        FOOD_INDEX = 0;
        SIZE_SNAKE = 6;
        if (SPEED == MAX_SPEED) SPEED = 1;
        else SPEED++;
        GenerateFood();
    }
    else {
        if (TIMEBIGFOOD == 3)
        {
            EatBigFood();
            TIMEBIGFOOD = 0;
            SCORE += 3;
        }
        else
        {
            SCORE++;
            FOOD_INDEX++;
            SIZE_SNAKE++;
            snake[SIZE_SNAKE].x = food[FOOD_INDEX].x;
            snake[SIZE_SNAKE].y = food[FOOD_INDEX].y;
            TIMEBIGFOOD++;
        }
    }
}
// Hàm kiểm tra rắn chết
void ProcessDead() {
    STATE = 0;
    gotoXY(0, HEIGH_CONSOLE + 2);
    printf("Dead, type y to continue or anykey to exit");
}
void ProcessDeadInLevel()
{
    if (LEVEL == 1)
        for (int i = 0; i < COUNT; ++i)
        {
            if (MOVING == 'D')
            {
                if (snake[0].x + 1 == wall_2[i].x && snake[0].y == wall_2[i].y)
                    ProcessDead();
            }
            if (MOVING == 'S')
            {
                if (snake[0].x == wall_2[i].x && snake[0].y + 1 == wall_2[i].y)
                    ProcessDead();
            }
            if (MOVING == 'W')
            {
                if (snake[0].x == wall_2[i].x && snake[0].y - 1 == wall_2[i].y)
                    ProcessDead();
            }
            if (MOVING == 'A')
            {
                if (snake[0].x - 1 == wall_2[i].x && snake[0].y == wall_2[i].y)
                    ProcessDead();
            }
        }
}
void ChangeLevel()
{
    LEVEL += 1;
    int i = 0;
    int delete_body = 0;
    int head = 0;
    while (delete_body < SIZE_SNAKE)
    {
        DeleteSnakeAndFood();
        for (i = SIZE_SNAKE - 2; i >= delete_body; --i) {
            snake[i + 1].x = snake[i].x;
            snake[i + 1].y = snake[i].y;
        }
        snake[head++] = { 2, HEIGH_CONSOLE / 2 };
        delete_body++;
        DrawSnake(delete_body, SIZE_SNAKE);
        Sleep(250);
    }
    if (LEVEL > 2)
        LEVEL = 1;
    Delete_Gate_End();
    if (LEVEL == 1)
    {
        Level_1(1, 0, WIDTH_CONSOLE, HEIGH_CONSOLE);
    }
    else if (LEVEL == 2)
    {
        Level_2(1, 0, WIDTH_CONSOLE, HEIGH_CONSOLE);
    }
}
// Hàm vẽ rắn và đồ ăn
void MoveRight() {
    if (FOOD_INDEX % 9 == 0 && NEW_LEVEL == 1)
    {
        STOP_DRAW_FOOD = 1;
        Gate_End();
    }
    bool check_body = true;
    for (int i = SIZE_SNAKE - 1; i >= 1; --i)
    {
        if (snake[i].x == snake[0].x + 1 && snake[i].y == snake[0].y)
            check_body = false;
    }
    if (check_body == false)
        ProcessDead();
    ProcessDeadInLevel();
    if (snake[0].x + 1 == WIDTH_CONSOLE + 1 && snake[0].y == HEIGH_CONSOLE / 2 && STOP_DRAW_FOOD)
    {
        ChangeLevel();
        StartLevel();
        STOP_DRAW_FOOD = 0;
        Delete_Gate_Start();
    }
    else if (snake[0].x + 1 == WIDTH_CONSOLE + 1)
        ProcessDead();
    else {
        if (snake[0].x + 1 == food[FOOD_INDEX].x && snake[0].y == food[FOOD_INDEX].y) {
            Eat();
        }
        for (int i = SIZE_SNAKE - 2; i >= 0; --i) {
            snake[i + 1].x = snake[i].x;
            snake[i + 1].y = snake[i].y;
        }
        snake[0].x++;
    }
}
void MoveLeft() {
    if (FOOD_INDEX % 9 == 0 && NEW_LEVEL == 1)
    {
        STOP_DRAW_FOOD = 1;
        Gate_End();
        NEW_LEVEL = 0;
    }
    if (FOOD_INDEX == 5)
    {
        NEW_LEVEL = 1;
    }
    bool check_body = true;
    for (int i = SIZE_SNAKE - 1; i >= 1; --i)
    {
        if (snake[i].x == snake[0].x - 1 && snake[i].y == snake[0].y)
            check_body = false;
    }
    if (check_body == false)
        ProcessDead();
    ProcessDeadInLevel();
    if (snake[0].x - 1 == 1) {
        ProcessDead();
    }
    else {
        if (snake[0].x - 1 == food[FOOD_INDEX].x && snake[0].y == food[FOOD_INDEX].y)
        {
            Eat();
            NEW_LEVEL = 1;
        }
        for (int i = SIZE_SNAKE - 2; i >= 0; --i) {
            snake[i + 1].x = snake[i].x;
            snake[i + 1].y = snake[i].y;
        }
        snake[0].x--;
    }
}
void MoveDown() {
    if (FOOD_INDEX % 9 == 0 && NEW_LEVEL == 1)
    {
        STOP_DRAW_FOOD = 1;
        Gate_End();
        NEW_LEVEL = 0;
    }
    if (FOOD_INDEX == 5)
    {
        NEW_LEVEL = 1;
    }
    bool check_body = true;
    for (int i = SIZE_SNAKE - 1; i >= 1; --i)
    {
        if (snake[i].x == snake[0].x && snake[i].y == snake[0].y + 1)
            check_body = false;
    }
    if (check_body == false)
        ProcessDead();
    ProcessDeadInLevel();
    if (snake[0].y + 1 == HEIGH_CONSOLE) {
        ProcessDead();
    }
    else {
        if (snake[0].x == food[FOOD_INDEX].x && snake[0].y + 1 == food[FOOD_INDEX].y)
        {
            Eat();
            NEW_LEVEL = 1;
        }
        for (int i = SIZE_SNAKE - 2; i >= 0; --i) {
            snake[i + 1].x = snake[i].x;
            snake[i + 1].y = snake[i].y;
        }
        snake[0].y++;
    }
}
void MoveUp() {
    if (FOOD_INDEX % 9 == 0 && NEW_LEVEL == 1)
    {
        STOP_DRAW_FOOD = 1;
        Gate_End();
        NEW_LEVEL = 0;
    }
    if (FOOD_INDEX == 5)
    {
        NEW_LEVEL = 1;
    }
    bool check_body = true;
    for (int i = SIZE_SNAKE - 1; i >= 1; --i)
    {
        if (snake[i].x == snake[0].x && snake[i].y == snake[0].y - 1)
            check_body = false;
    }
    if (check_body == false)
        ProcessDead();
    ProcessDeadInLevel();
    if (snake[0].y - 1 == 0) {
        ProcessDead();
    }
    else {
        if (snake[0].x == food[FOOD_INDEX].x && snake[0].y - 1 == food[FOOD_INDEX].y)
        {
            Eat();
            NEW_LEVEL = 1;
        }
        for (int i = SIZE_SNAKE - 2; i >= 0; --i) {
            snake[i + 1].x = snake[i].x;
            snake[i + 1].y = snake[i].y;
        }
        snake[0].y--;
    }
}
// Luồng chạy chính của game
void ThreadFunc() {
    while (1) {
        if (STATE == 1) { // Nếu rắn còn sống
            DeleteSnakeAndFood();
            switch (MOVING) {
            case 'A':
                MoveLeft();
                break;
            case 'D':
                MoveRight();
                break;
            case 'W':
                MoveUp();
                break;
            case 'S':
                MoveDown();
                break;
            }
            DrawSnake(0, SIZE_SNAKE);
            if (STOP_DRAW_FOOD == 0)
                DrawFood();
            if (MOVING == 'A' && MOVING == 'D')
                Sleep(50 / SPEED); // Hàm điều chỉnh tốc độ khung hình
            else
                Sleep(100 / SPEED);
        }
    }
}
void Play()
{
    int temp;

    Hide_Console_Cursor();
    StartGame();
    thread t1(ThreadFunc); // Tạo luồng chuyển động cho snake
    HANDLE handle_t1 = t1.native_handle(); //Take handle of thread
    while (1) {
        temp = toupper(_getch());
        if (STATE == 1) {
            if (temp == 'P') {
                PauseGame(handle_t1);
            }
            else if (temp == 27) {
                closeWindow();
            }
            else {
                ResumeThread(handle_t1);
                if ((temp != CHAR_LOCK) && (temp == 'D' || temp == 'A' || temp == 'W' || temp == 'S')) {
                    if (temp == 'D') CHAR_LOCK = 'A';
                    else if (temp == 'W') CHAR_LOCK = 'S';
                    else if (temp == 'S') CHAR_LOCK = 'W';
                    else CHAR_LOCK = 'D';
                    MOVING = temp;
                }
            }
        }
        else {
            if (temp == 'Y') StartGame();
            else {
                closeWindow();
            }
        }
    }
}
// load game
void boxphu(int x, int y, int w, int h, int t_color) {
    SetColor(t_color);
    for (int ix = x; ix <= x + w; ix++) {
        gotoXY(ix, y);
        cout << char(196);
        gotoXY(ix, y + h);
        cout << char(196);
    }
    for (int iy = y; iy <= y + h; iy++) {
        gotoXY(x, iy);
        cout << char(179);
        gotoXY(x + w, iy);
        cout << char(179);
    }
    gotoXY(x, y);			cout << char(218);
    gotoXY(x + w, y);		cout << char(191);
    gotoXY(x, y + h);		cout << char(192);
    gotoXY(x + w, y + h);	cout << char(217);
}
void thanhsangphu(int x, int y, int w, int h, int b_color) {
    textcolor(b_color);
    for (int iy = y + 1; iy <= y + h - 1; iy++) {
        for (int ix = x + 1; ix <= x + w - 1; ix++) {
            gotoXY(ix, iy); cout << " ";
        }
    }
}
void number3()
{
    boxphu(53, 11, 12, 11, 6);
    thanhsangphu(53, 11, 12, 11, 180);
    textcolor(118);
    gotoXY(55, 14);
    cout << " ";
    gotoXY(56, 13);
    for (int i = 0; i < 7; i++) {
        cout << " ";
    }
    gotoXY(63, 14);
    cout << " ";
    gotoXY(63, 15);
    cout << " ";
    gotoXY(62, 16);
    cout << " ";
    gotoXY(61, 16);
    cout << " ";
    int t = 5;
    int xl = 61;
    while (t >= 0)
    {
        gotoXY(xl, 16);
        cout << " ";
        xl--;
        t--;
    }
    gotoXY(63, 17);
    cout << " ";
    gotoXY(63, 18);
    cout << " ";
    xl = 62;
    t = 6;
    while (t >= 0)
    {
        gotoXY(xl, 19);
        cout << " ";
        xl--;
        t--;
    }
    gotoXY(55, 18);
    cout << " ";
}
void number2()
{
    boxphu(53, 11, 13, 11, 6);
    thanhsangphu(53, 11, 13, 11, 180);
    textcolor(118);
    gotoXY(56, 14);
    cout << " ";
    gotoXY(57, 13);
    for (int i = 0; i < 6; i++) {
        cout << " ";
    }
    int xl = 63, yl = 14;
    int t = 6;
    while (t >= 0)
    {
        gotoXY(xl, yl);
        cout << " ";
        xl--;
        yl++;
        t--;
    } //x =57
    xl = 58;
    for (int i = 0; i < 6; i++)
    {
        gotoXY(xl, 20);
        cout << " ";
        xl++;
    }
}
void number1()
{
    boxphu(53, 11, 12, 11, 6);
    thanhsangphu(53, 11, 12, 11, 180);
    textcolor(118);
    gotoXY(56, 16);
    cout << " ";
    int yl = 15, xl = 57;
    while (yl != 12)
    {
        gotoXY(xl, yl);
        cout << " ";
        yl--;
        xl++;
    }
    xl = 59;
    yl = 14;
    for (int i = 0; i < 6; i++) {
        gotoXY(xl, yl);
        cout << " ";
        yl++;
    }
    yl = 20;
    xl = 59;
    for (int i = 0; i < 4; i++) {
        gotoXY(xl, yl);
        cout << " ";
        xl--;
    }
    xl = 60;
    for (int i = 0; i < 3; i++) {
        gotoXY(xl, 20);
        cout << " ";
        xl++;
    }
}
void loadgame()
{
    ShowCur(0);
    number3();
    Sleep(500);
    system("cls");
    number2();
    Sleep(500);
    system("cls");
    number1();
    Sleep(500);
    system("cls");
}
// o tren la load game
void check(int y, string& nd) {
    if (y == 10) nd = "Play";
    else if (y == 14) nd = "About";
    else if (y == 18) nd = "Reload";
    else if (y == 22) nd = "Settings";
    else if (y == 26) nd = "Exit";
}
void box(int x, int y, int w, int h, int t_color, int b_color, string nd) {
    SetColor(t_color);
    for (int ix = x; ix <= x + w; ix++) {
        gotoXY(ix, y);
        cout << char(196);
        gotoXY(ix, y + h);
        cout << char(196);
    }
    for (int iy = y; iy <= y + h; iy++) {
        gotoXY(x, iy);
        cout << char(179);
        gotoXY(x + w, iy);
        cout << char(179);
    }
    gotoXY(x, y);			cout << char(218);
    gotoXY(x + w, y);		cout << char(191);
    gotoXY(x, y + h);		cout << char(192);
    gotoXY(x + w, y + h);	cout << char(217);
    SetColor(b_color);
    gotoXY(x + w / 2 - nd.size() / 2, y + h / 2); cout << nd;
}
void n_box(int x, int y, int w, int h, int t_color, int b_color, string& nd) {
    int iy;
    for (int i = 0; i < 5; i++) {
        iy = y + i * 4;
        check(iy, nd);
        box(x, iy, w, h, t_color, b_color, nd);
    }
}
void thanhsang(int x, int y, int w, int h, int b_color, int c_color, string& nd) {
    textcolor(b_color);
    for (int iy = y + 1; iy <= y + h - 1; iy++) {
        for (int ix = x + 1; ix <= x + w - 1; ix++) {
            gotoXY(ix, iy); cout << " ";
        }
    }
    check(y, nd);
    SetColor(c_color);

    gotoXY(x + w / 2 - nd.size() / 2, y + h / 2); cout << nd;
}
void boxname(int x, int y, int w, int h, int t_color, string s) {
    SetColor(t_color);
    for (int ix = x; ix <= x + w; ix++) {
        gotoXY(ix, y);
        cout << char(196);
        gotoXY(ix, y + h);
        cout << char(196);
    }
    for (int iy = y; iy <= y + h; iy++) {
        gotoXY(x, iy);
        cout << char(179);
        gotoXY(x + w, iy);
        cout << char(179);
    }
    gotoXY(x, y);			cout << char(218);
    gotoXY(x + w, y);		cout << char(191);
    gotoXY(x, y + h);		cout << char(192);
    gotoXY(x + w, y + h);	cout << char(217);
    gotoXY(x + 1, y + 1); cout << s;
}
struct users {
    string name;
    int score;
};
users user;
void clearInputBuffer() {
    cin.clear();
}
void removeSubstring(string& str, const string& subStr) {
    size_t pos = str.find(subStr);
    if (pos != string::npos) {
        str.erase(pos, subStr.length());
    }
}
// Hàm để nhập chuỗi có giới hạn kí tự
string inputLimitedString(int maxLength, users& user) {
    bool kt = true;
    while (kt == true) {
        getline(cin, user.name);
        // Kiểm tra xem chuỗi có vượt quá giới hạn không
        if (user.name.length() > maxLength || user.name.length() == 0) {
            ShowCur(0);
            string tmp = user.name;
            removeSubstring(user.name, tmp);
            gotoXY(37, 20);
            cout << "INVALID NAME! please enter to write your name again";
            clearInputBuffer(); // Xóa bộ nhớ đệm đến kí tự '\n'
            if (_getch()) {
                system("cls");
                ShowCur(1);
                boxname(20, 5, 80, 2, 5, "enter your name: ");
            }
        }
        else {
            kt = false;
        }
    }
    return user.name;
}
void nhapten()
{
    ShowCur(1);
    boxname(20, 5, 80, 2, 5, "enter your name: ");
    inputLimitedString(15, user);
    system("cls");
}
void printM(int x, int y, int h) {
    textcolor(20);
    for (int iy = y; iy <= y + h; iy++) {
        for (int ix = x; ix <= x + 3; ix++) {
            gotoXY(ix, iy); cout << " ";
        }
        for (int ix = x + 10; ix <= x + 13; ix++) {
            gotoXY(ix, iy); cout << " ";
        }
    }
    gotoXY(x + 4, y + 1); cout << "  ";
    gotoXY(x + 8, y + 1); cout << "  ";
    gotoXY(x + 4, y + 2); cout << "      ";
    gotoXY(x + 4, y + 3); cout << "      ";
    gotoXY(x + 6, y + 4); cout << "  ";
}
void printN(int x, int y, int h) {
    textcolor(20);
    for (int iy = y; iy <= y + h; iy++) {
        gotoXY(x, iy); cout << "   ";
        gotoXY(x + 9, iy); cout << "   ";
    }
    for (int iy = y + 1; iy <= y + 3; iy++) {
        for (int i = iy; i <= iy + 2; i++) {
            gotoXY(x + 3, i);
            cout << "  ";
        }
        x += 2;
    }
}
void box_nocontent(int x, int y, int w, int h) {
    SetColor(9);
    for (int ix = x; ix <= x + w; ix++) {
        gotoXY(ix, y);
        cout << char(205);
        gotoXY(ix, y + h);
        cout << char(205);
    }
    for (int iy = y; iy <= y + h; iy++) {
        gotoXY(x, iy);
        cout << char(186);
        gotoXY(x + w, iy);
        cout << char(186);
    }
    gotoXY(x, y);			cout << char(201);
    gotoXY(x + w, y);		cout << char(187);
    gotoXY(x, y + h);		cout << char(200);
    gotoXY(x + w, y + h);	cout << char(188);
}
void PrintMainMenu(int x, int y, int h) {
    textcolor(20);
    // IN CHU M
    for (int iy = y; iy <= y + h; iy++) {
        for (int ix = x; ix <= x + 3; ix++) {
            gotoXY(ix, iy); cout << " ";
        }
        for (int ix = x + 10; ix <= x + 13; ix++) {
            gotoXY(ix, iy); cout << " ";
        }
    }
    gotoXY(x + 4, y + 1); cout << "  ";
    gotoXY(x + 8, y + 1); cout << "  ";
    gotoXY(x + 4, y + 2); cout << "      ";
    gotoXY(x + 4, y + 3); cout << "      ";
    gotoXY(x + 6, y + 4); cout << "  ";

    // IN CHU A
    x += 15;
    for (int iy = y + h; iy >= y + 3; iy--) {
        for (int ix = x; ix <= x + 2; ix++) {
            gotoXY(ix, iy); cout << " ";
        }
        for (int ix = x + 9; ix <= x + 11; ix++) {
            gotoXY(ix, iy); cout << " ";
        }
    }
    gotoXY(x + 1, y + 2); cout << "    ";
    gotoXY(x + 7, y + 2); cout << "    ";
    gotoXY(x + 3, y + 1); cout << "      ";
    gotoXY(x + 5, y); cout << "  ";
    gotoXY(x + 3, y + 4); cout << "      ";

    // IN CHU I
    x += 13;
    gotoXY(x, y); cout << "         ";
    gotoXY(x, y + h); cout << "         ";
    for (int iy = y + 1; iy <= y + h - 1; iy++) {
        gotoXY(x + 2, iy); cout << "     ";
    }
    // IN CHU N
    x += 10;
    for (int iy = y; iy <= y + h; iy++) {
        gotoXY(x, iy); cout << "   ";
        gotoXY(x + 9, iy); cout << "   ";
    }
    for (int iy = y + 1; iy <= y + 3; iy++) {
        for (int i = iy; i <= iy + 2; i++) {
            gotoXY(x + 3, i);
            cout << "  ";
        }
        x += 2;
    }
    x += 11;
    printM(x, y, h);
    x += 15;
    // IN CHU E
    for (int iy = y; iy <= y + h; iy++) {
        gotoXY(x, iy);
        if (iy == y || iy == y + h) {
            cout << "         ";
        }
        else if (iy == (y + h) / 2 + 1) cout << "       ";
        else cout << "   ";
    }
    x += 10;
    printN(x, y, h);
    x += 13;
    // IN CHU U
    for (int iy = y; iy <= y + h - 1; iy++) {
        gotoXY(x, iy); cout << "   ";
        gotoXY(x + 9, iy); cout << "   ";
    }
    gotoXY(x + 1, y + h); cout << "          ";
    textcolor(120);
}
void menu() {
    ShowCur(0);

    box_nocontent(0, 0, 119, 9);
    PrintMainMenu(7, 1, 6);
    int x = 50, y = 10, t_color = 5;
    string nd;
    n_box(x, y, 20, 3, t_color, 68, nd);
    thanhsang(x, y, 20, 3, 233, 68, nd);
    int check = 1;
    int xp = x, yp = y;
    int xcu = xp, ycu = yp;
    bool kt = true;
    while (true) {
        if (kt == true) {
            gotoXY(xcu, ycu);
            thanhsang(xcu, ycu, 20, 3, 120, 68, nd);
            xcu = xp; ycu = yp;
            thanhsang(xp, yp, 20, 3, 233, 68, nd);
            kt = false;
        }
        while (!_kbhit()) {

        }
        char c = _getch();
        if (c == -32) {
            kt = true;
            c = _getch();
            if (c == 72 && yp == y) {
                yp = y + 16;
                check = 5;
            }
            else if (c == 72) {
                yp -= 4;
                if (yp == 22) check = 4;
                else if (yp == 18) check = 3;
                else if (yp == 14) check = 2;
                else if (yp == 10) check = 1;
            }
            else if (c == 80 && yp == y + 16)
            {
                yp = y;
                check = 1;
            }
            else if (c == 80) {
                yp += 4;
                if (yp == 14) check = 2;
                else if (yp == 18) check = 3;
                else if (yp == 22) check = 4;
                else if (yp == 26) check = 5;
            }
        }
        if (check != 0) {
            if (c == 13) {
                system("cls");
                system("color 70");
                break;
            }
        }

    }
    if (check == 1) {
        nhapten();
        loadgame();
        Play();
    }
    else if (check == 5)
    {
        closeWindow();
    }
}
//tên người dùng
void main()
{
    system("color 70");
    HWND console = GetConsoleWindow();
    ShowWindow(console, SW_MAXIMIZE);
    FixConsoleWindow();
    menu();
    (void)_getch();
}