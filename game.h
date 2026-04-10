#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QPointF>
#include <QTimer>

class Game : public QObject
{
    Q_OBJECT

public:
    enum GameState {
        Ready,      // 准备状态
        Shooting,   // 射门中
        Saving,     // 扑救中
        Goal,       // 进球
        Miss,       // 未进
        Saved       // 被扑出
    };

    explicit Game(QObject *parent = nullptr);
    ~Game();

    void startGame();
    void shoot(int angle, int power);
    void save(int direction);

    GameState getState() const;
    QPointF getBallPosition() const;
    QPointF getGoalkeeperPosition() const;
    int getPlayerScore() const;
    int getComputerScore() const;

signals:
    void stateChanged(GameState newState);
    void ballMoved(QPointF position);
    void goalkeeperMoved(QPointF position);
    void scoreUpdated(int player, int computer);

private slots:
    void updateBallPosition();
    void updateGoalkeeperPosition();

private:
    GameState state;
    QPointF ballPosition;
    QPointF goalkeeperPosition;
    QPointF ballVelocity;
    QTimer *ballTimer;
    QTimer *goalkeeperTimer;
    int playerScore;
    int computerScore;
    int saveDirection;
    bool isPlayerTurn;
};

#endif // GAME_H