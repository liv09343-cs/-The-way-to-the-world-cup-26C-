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
        Saved,      // 被扑出
        GameOver    // 游戏结束
    };

    explicit Game(QObject *parent = nullptr);
    ~Game();

    void startGame();
    void shoot(int angle, int power);
    void save(int direction);
    void setTeams(const QString &playerTeam, const QString &computerTeam);
    void setGroup(const QString &group);

    GameState getState() const;
    QPointF getBallPosition() const;
    QPointF getGoalkeeperPosition() const;
    int getPlayerScore() const;
    int getComputerScore() const;
    QString getPlayerTeam() const;
    QString getComputerTeam() const;
    QString getGroup() const;
    int getRound() const;
    bool isGameOver() const;
    QString getWinner() const;
    bool getIsPlayerTurn() const;

signals:
    void stateChanged(GameState newState);
    void ballMoved(QPointF position);
    void goalkeeperMoved(QPointF position);
    void scoreUpdated(int player, int computer);
    void roundUpdated(int round);
    void gameOver(QString winner);

private slots:
    void updateBallPosition();
    void updateGoalkeeperPosition();

private:
    void computerShoot();

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
    int round;
    bool isOvertime;
    QString playerTeam;
    QString computerTeam;
    QString group;

    void checkGameOver();
    void startNextRound();
};

#endif // GAME_H