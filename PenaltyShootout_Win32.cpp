#include <Windows.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

// 游戏状态枚举
enum GameState {
    Ready,      // 准备状态
    Shooting,   // 射门中
    Saving,     // 扑救中
    Goal,       // 进球
    Miss,       // 未进
    Saved       // 被扑出
};

// 点结构体
struct Point {
    float x, y;
    Point(float x = 0, float y = 0) : x(x), y(y) {}
    Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }
    Point& operator+=(const Point& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
};

// 游戏类
class Game {
private:
    GameState state;
    Point ballPosition;
    Point goalkeeperPosition;
    Point ballVelocity;
    int playerScore;
    int computerScore;
    int saveDirection;
    bool isPlayerTurn;
    clock_t lastUpdateTime;

public:
    Game() {
        state = Ready;
        ballPosition = Point(400, 400); // 点球点位置
        goalkeeperPosition = Point(400, 125); // 球门中间位置
        playerScore = 0;
        computerScore = 0;
        isPlayerTurn = true;
        lastUpdateTime = clock();
        srand(time(NULL));
    }

    void startGame() {
        state = Ready;
        ballPosition = Point(400, 400);
        goalkeeperPosition = Point(400, 125);
        playerScore = 0;
        computerScore = 0;
        isPlayerTurn = true;
    }

    void shoot(int angle, int power) {
        if (state != Ready) return;

        state = Shooting;

        // 计算球的初速度
        ballVelocity.x = angle * 0.5;
        ballVelocity.y = -power * 0.5;

        // 守门员开始扑救
        saveDirection = (rand() % 3) - 1; // -1, 0, 1 分别代表左、中、右
    }

    void update() {
        clock_t currentTime = clock();
        float deltaTime = (currentTime - lastUpdateTime) / (float)CLOCKS_PER_SEC;
        lastUpdateTime = currentTime;

        if (state == Shooting) {
            // 更新球的位置
            ballPosition += ballVelocity