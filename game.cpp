#include "game.h"
#include <QRandomGenerator>
#include <cmath>

Game::Game(QObject *parent) : QObject(parent)
{
    state = Ready;
    ballPosition = QPointF(400, 400);
    goalkeeperPosition = QPointF(400, 125);
    playerScore = 0;
    computerScore = 0;
    isPlayerTurn = true;
    round = 1;
    isOvertime = false;

    ballTimer = new QTimer(this);
    goalkeeperTimer = new QTimer(this);

    connect(ballTimer, &QTimer::timeout, this, &Game::updateBallPosition);
    connect(goalkeeperTimer, &QTimer::timeout, this, &Game::updateGoalkeeperPosition);
}

const QList<bool>& Game::getPlayerShootHistory() const
{
    return playerShootHistory;
}

const QList<bool>& Game::getComputerShootHistory() const
{
    return computerShootHistory;
}

Game::~Game()
{
    delete ballTimer;
    delete goalkeeperTimer;
}

void Game::startGame()
{
    state = Ready;
    ballPosition = QPointF(400, 400);
    ballVelocity = QPointF(0, 0);
    goalkeeperPosition = QPointF(400, 125);
    playerScore = 0;
    computerScore = 0;
    round = 1;
    isOvertime = false;
    isPlayerTurn = true;
    
    playerShootHistory.clear();
    computerShootHistory.clear();
    
    emit stateChanged(state);
    emit ballMoved(ballPosition);
    emit goalkeeperMoved(goalkeeperPosition);
    emit scoreUpdated(playerScore, computerScore);
    emit roundUpdated(round);
}

void Game::shoot(int angle, int power)
{
    if (state != Ready) return;

    state = Shooting;
    emit stateChanged(state);

    double speed = 20.0;
    double angleRad = angle * 3.14159 / 180.0;
    ballVelocity.setX(speed * sin(angleRad));
    ballVelocity.setY(-speed * cos(angleRad));

    ballTimer->start(10);

    saveDirection = QRandomGenerator::global()->bounded(3) - 1;
    goalkeeperTimer->start(20);
}

void Game::save(int direction)
{
    if (state != Ready) return;

    state = Saving;
    emit stateChanged(state);

    saveDirection = direction;
    goalkeeperTimer->start(20);

    ballTimer->stop();

    int randDir = QRandomGenerator::global()->bounded(3);
    double targetX;

    switch (randDir) {
    case 0:
        targetX = 320 + QRandomGenerator::global()->bounded(40);
        break;
    case 1:
        targetX = 400 + QRandomGenerator::global()->bounded(21) - 10;
        break;
    case 2:
        targetX = 460 + QRandomGenerator::global()->bounded(40);
        break;
    default:
        targetX = 400;
    }

    double dx = targetX - 400;
    double dy = 100 - 400;
    double distance = sqrt(dx * dx + dy * dy);

    double speed = 20.0;
    ballVelocity.setX(dx / distance * speed);
    ballVelocity.setY(dy / distance * speed);

    ballPosition = QPointF(400, 400);
    ballPosition += ballVelocity;
    emit ballMoved(ballPosition);

    ballTimer->start(8);
}

void Game::setTeams(const QString &playerTeam, const QString &computerTeam)
{
    this->playerTeam = playerTeam;
    this->computerTeam = computerTeam;
}

void Game::setGroup(const QString &group)
{
    this->group = group;
}

Game::GameState Game::getState() const { return state; }
QPointF Game::getBallPosition() const { return ballPosition; }
QPointF Game::getGoalkeeperPosition() const { return goalkeeperPosition; }
int Game::getPlayerScore() const { return playerScore; }
int Game::getComputerScore() const { return computerScore; }
QString Game::getPlayerTeam() const { return playerTeam; }
QString Game::getComputerTeam() const { return computerTeam; }
QString Game::getGroup() const { return group; }
int Game::getRound() const { return round; }

QString Game::getWinner() const
{
    if (playerScore > computerScore) return playerTeam;
    else if (computerScore > playerScore) return computerTeam;
    return "";
}

bool Game::getIsPlayerTurn() const { return isPlayerTurn; }

bool Game::canGameEndEarly()
{
    if (!isOvertime && round < 5) {
        int remainingRounds = 5 - round;
        int playerMaxPossible = playerScore + remainingRounds;
        int computerMaxPossible = computerScore + remainingRounds;
        return playerMaxPossible < computerScore || computerMaxPossible < playerScore;
    }
    return false;
}

void Game::updateBallPosition()
{
    ballPosition += ballVelocity;
    emit ballMoved(ballPosition);

    if (ballPosition.y() < 150 && ballPosition.x() > 300 && ballPosition.x() < 500) {
        if (qAbs(ballPosition.x() - goalkeeperPosition.x()) < 35) {
            state = Saved;
            if (isPlayerTurn) {
                playerShootHistory.append(false);
            } else {
                computerShootHistory.append(false);
            }
        } else {
            state = Goal;
            if (isPlayerTurn) {
                playerScore++;
                playerShootHistory.append(true);
            } else {
                computerScore++;
                computerShootHistory.append(true);
            }
            emit scoreUpdated(playerScore, computerScore);
        }
        ballTimer->stop();
        goalkeeperTimer->stop();

        QTimer::singleShot(2000, this, [=]() {
            if (!isPlayerTurn) {
                if (canGameEndEarly()) {
                    state = GameOver;
                    emit stateChanged(state);
                    emit gameOver(getWinner());
                    return;
                }
                if (!isOvertime && round >= 5) {
                    if (playerScore != computerScore) {
                        state = GameOver;
                        emit stateChanged(state);
                        emit gameOver(getWinner());
                        return;
                    }
                    isOvertime = true;
                    round = 1;
                    emit roundUpdated(round);
                } else if (isOvertime) {
                    if (playerScore != computerScore) {
                        state = GameOver;
                        emit stateChanged(state);
                        emit gameOver(getWinner());
                        return;
                    }
                    round++;
                    emit roundUpdated(round);
                } else {
                    round++;
                    emit roundUpdated(round);
                }
            }

            state = Ready;
            ballPosition = QPointF(400, 400);
            ballVelocity = QPointF(0, 0);
            goalkeeperPosition = QPointF(400, 125);
            emit stateChanged(state);
            emit ballMoved(ballPosition);
            emit goalkeeperMoved(goalkeeperPosition);
            isPlayerTurn = !isPlayerTurn;
        });
    }
    else if (ballPosition.y() < 0 || ballPosition.y() > 600 || ballPosition.x() < 0 || ballPosition.x() > 800) {
        state = Miss;
        ballTimer->stop();
        goalkeeperTimer->stop();

        QTimer::singleShot(2000, this, [=]() {
            if (!isPlayerTurn) {
                if (canGameEndEarly()) {
                    state = GameOver;
                    emit stateChanged(state);
                    emit gameOver(getWinner());
                    return;
                }
                if (!isOvertime && round >= 5) {
                    if (playerScore != computerScore) {
                        state = GameOver;
                        emit stateChanged(state);
                        emit gameOver(getWinner());
                        return;
                    }
                    isOvertime = true;
                    round = 1;
                    emit roundUpdated(round);
                } else if (isOvertime) {
                    if (playerScore != computerScore) {
                        state = GameOver;
                        emit stateChanged(state);
                        emit gameOver(getWinner());
                        return;
                    }
                    round++;
                    emit roundUpdated(round);
                } else {
                    round++;
                    emit roundUpdated(round);
                }
            }

            state = Ready;
            ballPosition = QPointF(400, 400);
            ballVelocity = QPointF(0, 0);
            goalkeeperPosition = QPointF(400, 125);
            emit stateChanged(state);
            emit ballMoved(ballPosition);
            emit goalkeeperMoved(goalkeeperPosition);
            isPlayerTurn = !isPlayerTurn;
        });
    }
}

void Game::updateGoalkeeperPosition()
{
    goalkeeperPosition.setX(goalkeeperPosition.x() + saveDirection * 5);
    if (goalkeeperPosition.x() < 300) goalkeeperPosition.setX(300);
    if (goalkeeperPosition.x() > 500) goalkeeperPosition.setX(500);
    emit goalkeeperMoved(goalkeeperPosition);
}