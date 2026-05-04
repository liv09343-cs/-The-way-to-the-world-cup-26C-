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
        Ready,
        Shooting,
        Saving,
        Goal,
        Miss,
        Saved,
        GameOver
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
    bool canGameEndEarly();

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
};

#endif // GAME_H