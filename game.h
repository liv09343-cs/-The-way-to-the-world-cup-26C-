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
        Aiming,
        PowerSelecting,
        Kicking,
        Shooting,
        ComputerKicking,
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
    void shootToTarget(int targetX, int targetY);
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
    void handleKickAnimation();
    void kickAnimation();
    void computerKickAnimation();

private:
    bool canGameEndEarly();

    GameState state;
    double ballX;
    double ballY;
    double ballZ;
    double lastBallX;
    double lastBallY;
    double lastBallZ;
    double ballVX;
    double ballVY;
    double ballVZ;
    QPointF goalkeeperPosition;
    
    static const double GOAL_X_MIN;
    static const double GOAL_X_MAX;
    static const double GOAL_Y_MIN;
    static const double GOAL_Y_MAX;
    static const double GOAL_Z_MIN;
    static const double GOAL_Z_MAX;
    static const double BALL_RADIUS;
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
    
    QList<bool> playerShootHistory;
    QList<bool> computerShootHistory;
    
    bool isDiving;
    int diveDirection;
    
    QTimer *kickTimer;
    int kickAngle;
    int kickPower;
    int kickFrame;
    double playerRunX;
    
    int selectedAngle;
    int selectedPower;
    int currentAimAngle;
    
    QPointF targetPosition;
    bool useTargetMode;
    
public:
    const QList<bool>& getPlayerShootHistory() const;
    const QList<bool>& getComputerShootHistory() const;
    bool getIsDiving() const { return isDiving; }
    int getDiveDirection() const { return diveDirection; }
    int getKickFrame() const { return kickFrame; }
    double getPlayerRunX() const { return playerRunX; }
    int getSelectedAngle() const { return selectedAngle; }
    double getBallZ() const { return ballZ; }
    int getSelectedPower() const { return selectedPower; }
    int getCurrentAimAngle() const { return currentAimAngle; }
    
    void startAiming();
    void setAimAngle(int angle);
    void confirmAngle();
    void startPowerSelecting();
    void setPower(int power);
    void confirmPower();
};

#endif // GAME_H