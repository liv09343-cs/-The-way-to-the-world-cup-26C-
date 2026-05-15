#include "game.h"
#include <QRandomGenerator>
#include <cmath>

const double Game::GOAL_X_MIN = 150;
const double Game::GOAL_X_MAX = 650;
const double Game::GOAL_Y_MIN = 0;
const double Game::GOAL_Y_MAX = 230;
const double Game::GOAL_Z_MIN = 0;
const double Game::GOAL_Z_MAX = 60;
const double Game::BALL_RADIUS = 10;

Game::Game(QObject *parent) : QObject(parent)
{
    state = Ready;
    ballX = 400;
    ballY = 500;
    ballZ = 0;
    lastBallX = 400;
    lastBallY = 500;
    lastBallZ = 0;
    ballVX = 0;
    ballVY = 0;
    ballVZ = 0;
    goalkeeperPosition = QPointF(400, 145);
    playerScore = 0;
    computerScore = 0;
    isPlayerTurn = true;
    round = 1;
    isOvertime = false;
    isDiving = false;
    diveDirection = 0;
    kickFrame = 0;
    kickAngle = 0;
    kickPower = 0;
    selectedAngle = 0;
    selectedPower = 0;
    currentAimAngle = 0;
    useTargetMode = false;

    ballTimer = new QTimer(this);
    goalkeeperTimer = new QTimer(this);
    kickTimer = new QTimer(this);

    connect(ballTimer, &QTimer::timeout, this, &Game::updateBallPosition);
    connect(goalkeeperTimer, &QTimer::timeout, this, &Game::updateGoalkeeperPosition);
    connect(kickTimer, &QTimer::timeout, this, &Game::handleKickAnimation);
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
    ballX = 400;
    ballY = 500;
    ballZ = 0;
    ballVX = 0;
    ballVY = 0;
    ballVZ = 0;
    goalkeeperPosition = QPointF(400, 145);
    playerScore = 0;
    computerScore = 0;
    round = 1;
    isOvertime = false;
    isPlayerTurn = true;
    isDiving = false;
    diveDirection = 0;
    
    playerShootHistory.clear();
    computerShootHistory.clear();
    
    emit stateChanged(state);
    emit ballMoved(QPointF(ballX, ballY));
    emit goalkeeperMoved(goalkeeperPosition);
    emit scoreUpdated(playerScore, computerScore);
    emit roundUpdated(round);
}

void Game::handleKickAnimation()
{
    if (state == Kicking) {
        kickAnimation();
    } else if (state == ComputerKicking) {
        computerKickAnimation();
    }
}

void Game::shoot(int angle, int power)
{
    if (state != Ready) return;

    kickAngle = angle;
    kickPower = power;
    kickFrame = 0;
    playerRunX = 550;
    
    state = Kicking;
    emit stateChanged(state);
    
    kickTimer->start(30);
}

void Game::kickAnimation()
{
    kickFrame++;
    
    if (kickFrame <= 20) {
        playerRunX -= 4;
        if (playerRunX < 380) playerRunX = 380;
    }
    
    emit stateChanged(state);
    
    if (kickFrame >= 25) {
        kickTimer->stop();
        
        state = Shooting;
        emit stateChanged(state);

        double angleRad = kickAngle * 3.14159 / 180.0;
        
        double dx = -tan(angleRad) * (GOAL_Z_MAX - ballY);
        double dy = GOAL_Z_MAX - ballY;
        double dz = kickPower * 0.45;
        if (dz > GOAL_Y_MAX * 0.7) dz = GOAL_Y_MAX * 0.7;
        
        double distance = sqrt(dx * dx + dy * dy + dz * dz);
        
        double speed = 26.0;
        ballVX = dx / distance * speed;
        ballVY = dy / distance * speed;
        ballVZ = dz / distance * speed;

        ballTimer->start(8);

        saveDirection = QRandomGenerator::global()->bounded(3) - 1;
        goalkeeperTimer->start(20);
    }
}

void Game::shootToTarget(int targetX, int targetY)
{
    if (state != Ready) return;

    state = Shooting;
    emit stateChanged(state);

    double dx = targetX - ballX;
    double dy = targetY - ballY;
    double distance = sqrt(dx * dx + dy * dy);

    double baseSpeed = 20.0;
    double powerMultiplier = 1.0 + selectedPower / 100.0;
    double speed = baseSpeed * powerMultiplier;
    
    ballVX = dx / distance * speed;
    ballVY = dy / distance * speed;
    ballVZ = selectedPower * 0.3;

    ballTimer->start(8);

    saveDirection = QRandomGenerator::global()->bounded(3) - 1;
    goalkeeperTimer->start(30);
}

void Game::startAiming()
{
    if (state != Ready) return;
    state = Aiming;
    currentAimAngle = 0;
    emit stateChanged(state);
}

void Game::setAimAngle(int angle)
{
    if (state != Aiming) return;
    currentAimAngle = qBound(-45, angle, 45);
    emit stateChanged(state);
}

void Game::confirmAngle()
{
    if (state != Aiming) return;
    selectedAngle = currentAimAngle;
    state = PowerSelecting;
    selectedPower = 0;
    emit stateChanged(state);
}

void Game::startPowerSelecting()
{
    if (state != Ready) return;
    state = PowerSelecting;
    selectedPower = 0;
    emit stateChanged(state);
}

void Game::setPower(int power)
{
    if (state != PowerSelecting) return;
    selectedPower = qBound(0, power, 100);
    emit stateChanged(state);
}

void Game::confirmPower()
{
    if (state != PowerSelecting) return;
    
    kickAngle = selectedAngle;
    kickPower = selectedPower;
    kickFrame = 0;
    playerRunX = 550;
    
    const int SCREEN_WIDTH = 800;
    double targetY = 190;
    targetPosition = QPointF(SCREEN_WIDTH/2 + kickAngle * 5, targetY);
    useTargetMode = true;
    
    state = Kicking;
    emit stateChanged(state);
    
    kickTimer->start(30);
}

void Game::save(int direction)
{
    if (state != Ready) return;

    saveDirection = direction;
    
    kickFrame = 0;
    playerRunX = 550;
    state = ComputerKicking;
    emit stateChanged(state);
    
    kickTimer->start(30);
}

void Game::computerKickAnimation()
{
    kickFrame++;
    
    if (kickFrame <= 20) {
        playerRunX -= 4;
        if (playerRunX < 380) playerRunX = 380;
    }
    
    emit stateChanged(state);
    
    if (kickFrame >= 25) {
        kickTimer->stop();
        
        state = Saving;
        emit stateChanged(state);

        goalkeeperTimer->start(15);

        int randDir = QRandomGenerator::global()->bounded(3);
        double targetX;

        switch (randDir) {
        case 0:
            targetX = 300 + QRandomGenerator::global()->bounded(60);
            break;
        case 1:
            targetX = 380 + QRandomGenerator::global()->bounded(41) - 20;
            break;
        case 2:
            targetX = 440 + QRandomGenerator::global()->bounded(60);
            break;
        default:
            targetX = 400;
        }

        double dx = targetX - 400;
        double dy = GOAL_Z_MAX - 500;
        double dz = 50;
        double distance = sqrt(dx * dx + dy * dy + dz * dz);

        double speed = 22.0;
        ballVX = dx / distance * speed;
        ballVY = dy / distance * speed;
        ballVZ = dz / distance * speed;

        ballX = 400;
        ballY = 500;
        ballZ = 0;
        emit ballMoved(QPointF(ballX, ballY));

        ballTimer->start(8);
    }
}

void Game::setTeams(const QString &playerTeam, const QString &computerTeam)
{
    this->playerTeam = playerTeam;
    this->computerTeam = computerTeam;
    playerShootHistory.clear();
    computerShootHistory.clear();
}

void Game::setGroup(const QString &group)
{
    this->group = group;
}

Game::GameState Game::getState() const { return state; }
QPointF Game::getBallPosition() const { return QPointF(ballX, ballY); }
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
    lastBallX = ballX;
    lastBallY = ballY;
    lastBallZ = ballZ;
    
    ballX += ballVX;
    ballY += ballVY;
    ballZ += ballVZ;
    
    ballVZ -= 0.25;
    if (ballZ < 0) {
        ballZ = 0;
        ballVZ = 0;
    }
    
    emit ballMoved(QPointF(ballX, ballY));

    const double keeperWidth = 80;
    const double keeperReach = 35;
    
    bool ballCenterWasOutside = (lastBallY > GOAL_Z_MAX);
    bool ballCenterIsInside = (ballY <= GOAL_Z_MAX);
    
    if (ballCenterWasOutside && ballCenterIsInside) {
        double t = (GOAL_Z_MAX - lastBallY) / (ballY - lastBallY);
        double crossX = lastBallX + t * (ballX - lastBallX);
        double crossZ = lastBallZ + t * (ballZ - lastBallZ);
        
        bool ballFullyCrossed = (ballY - BALL_RADIUS <= GOAL_Z_MAX);
        
        bool inXRange = (crossX >= GOAL_X_MIN + BALL_RADIUS && crossX <= GOAL_X_MAX - BALL_RADIUS);
        bool inYRange = (crossZ >= GOAL_Y_MIN && crossZ + BALL_RADIUS <= GOAL_Y_MAX);
        
        if (ballFullyCrossed && inXRange && inYRange) {
            double keeperLeft = goalkeeperPosition.x() - keeperWidth / 2 - keeperReach;
            double keeperRight = goalkeeperPosition.x() + keeperWidth / 2 + keeperReach;
            bool isSaved = (crossX >= keeperLeft && crossX <= keeperRight && crossZ <= GOAL_Y_MAX * 0.7);
            
            if (isSaved) {
                state = Saved;
                if (isPlayerTurn) {
                    playerShootHistory.append(false);
                } else {
                    computerShootHistory.append(false);
                }
                
                ballX = goalkeeperPosition.x();
                ballY = goalkeeperPosition.y() + 30;
                ballZ = crossZ;
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
                
                ballX = crossX;
                ballY = GOAL_Z_MAX - BALL_RADIUS;
                ballZ = crossZ;
            }
            ballVX = 0;
            ballVY = 0;
            ballVZ = 0;
            emit ballMoved(QPointF(ballX, ballY));
            
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
                ballX = 400;
                ballY = 500;
                ballZ = 0;
                ballVX = 0;
                ballVY = 0;
                ballVZ = 0;
                goalkeeperPosition = QPointF(400, 145);
                emit stateChanged(state);
                emit ballMoved(QPointF(ballX, ballY));
                emit goalkeeperMoved(goalkeeperPosition);
                isPlayerTurn = !isPlayerTurn;
            });
            return;
        }
    }

    if (ballY < -50 || ballY > 650 || ballX < -50 || ballX > 850) {
        state = Miss;
        if (isPlayerTurn) {
            playerShootHistory.append(false);
        } else {
            computerShootHistory.append(false);
        }
        
        if (ballX < -50) ballX = -50;
        if (ballX > 850) ballX = 850;
        if (ballY < -50) ballY = -50;
        if (ballY > 650) ballY = 650;
        ballVX = 0;
        ballVY = 0;
        ballVZ = 0;
        emit ballMoved(QPointF(ballX, ballY));
        
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
            ballX = 400;
            ballY = 500;
            ballZ = 0;
            ballVX = 0;
            ballVY = 0;
            ballVZ = 0;
            goalkeeperPosition = QPointF(400, 145);
            emit stateChanged(state);
            emit ballMoved(QPointF(ballX, ballY));
            emit goalkeeperMoved(goalkeeperPosition);
            isPlayerTurn = !isPlayerTurn;
        });
    }
}

void Game::updateGoalkeeperPosition()
{
    goalkeeperPosition.setX(goalkeeperPosition.x() + saveDirection * 8);
    if (goalkeeperPosition.x() < GOAL_X_MIN) goalkeeperPosition.setX(GOAL_X_MIN);
    if (goalkeeperPosition.x() > GOAL_X_MAX) goalkeeperPosition.setX(GOAL_X_MAX);
    emit goalkeeperMoved(goalkeeperPosition);
}