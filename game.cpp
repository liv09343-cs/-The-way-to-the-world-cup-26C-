#include "game.h"
#include <QRandomGenerator>
#include <cmath>

Game::Game(QObject *parent) : QObject(parent)
{
    state = Ready;
    ballPosition = QPointF(400, 400); // 点球点位置
    goalkeeperPosition = QPointF(400, 125); // 球门中间位置
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
    isPlayerTurn = true; // 确保玩家先射
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

    // 计算球的初速度
    ballVelocity.setX(angle * 0.5);
    ballVelocity.setY(-power * 0.5);

    ballTimer->start(30);

    // 守门员开始扑救
    saveDirection = QRandomGenerator::global()->bounded(3) - 1; // -1, 0, 1 分别代表左、中、右
    goalkeeperTimer->start(30);
}

void Game::computerShoot()
{
    if (state != Ready) return;
    
    // 设置为射门状态
    state = Shooting;
    emit stateChanged(state);
    
    // 确保球计时器停止后再重新启动
    ballTimer->stop();
    
    // 电脑随机踢球
    // 随机选择射门方向：左、中、右
    int direction = QRandomGenerator::global()->bounded(3); // 0, 1, 2 分别代表左、中、右
    
    // 根据方向计算球的目标位置
    int targetX;
    switch (direction) {
    case 0: // 左边
        targetX = 320 + QRandomGenerator::global()->bounded(30);
        break;
    case 1: // 中间
        targetX = 400 + QRandomGenerator::global()->bounded(21) - 10;
        break;
    case 2: // 右边
        targetX = 450 + QRandomGenerator::global()->bounded(30);
        break;
    default:
        targetX = 400;
    }
    
    // 计算球的速度向量
    // 球从(400, 400)飞向目标位置(targetX, 120)
    double dx = targetX - 400;
    double dy = 120 - 400;
    
    // 计算距离和速度
    double distance = sqrt(dx * dx + dy * dy);
    
    // 确保距离不为零，避免除零错误
    if (distance > 0.01) {
        double speed = 25.0;
        ballVelocity.setX(dx / distance * speed);
        ballVelocity.setY(dy / distance * speed);
    } else {
        // 默认向球门中间射门
        ballVelocity.setX(0);
        ballVelocity.setY(-25);
    }
    
    // 确保球从正确位置开始
    ballPosition = QPointF(400, 400);
    
    // 启动球的移动计时器
    ballTimer->start(8);
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
    case 0: // 左边
        targetX = 320 + QRandomGenerator::global()->bounded(40);
        break;
    case 1: // 中间
        targetX = 400 + QRandomGenerator::global()->bounded(21) - 10;
        break;
    case 2: // 右边
        targetX = 460 + QRandomGenerator::global()->bounded(40);
        break;
    default:
        targetX = 400;
    }
    
    double dx = targetX - 400;
    double dy = 100 - 400;
    double distance = sqrt(dx * dx + dy * dy);
    
    double speed = 30.0;
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

Game::GameState Game::getState() const
{
    return state;
}

QPointF Game::getBallPosition() const
{
    return ballPosition;
}

QPointF Game::getGoalkeeperPosition() const
{
    return goalkeeperPosition;
}

int Game::getPlayerScore() const
{
    return playerScore;
}

int Game::getComputerScore() const
{
    return computerScore;
}

QString Game::getPlayerTeam() const
{
    return playerTeam;
}

QString Game::getComputerTeam() const
{
    return computerTeam;
}

QString Game::getGroup() const
{
    return group;
}

int Game::getRound() const
{
    return round;
}

bool Game::isGameOver() const
{
    return state == GameOver;
}

QString Game::getWinner() const
{
    if (playerScore > computerScore) {
        return playerTeam;
    } else if (computerScore > playerScore) {
        return computerTeam;
    } else {
        return "";
    }
}

bool Game::getIsPlayerTurn() const
{
    return isPlayerTurn;
}

void Game::updateBallPosition()
{
    // 更新球的位置 - 足球在水平面上运动，不需要考虑重力
    ballPosition += ballVelocity;

    emit ballMoved(ballPosition);

    // 检查球是否进入球门
    if (ballPosition.y() < 150 && ballPosition.x() > 300 && ballPosition.x() < 500) {
        // 检查是否被守门员扑出
        if (qAbs(ballPosition.x() - goalkeeperPosition.x()) < 35) {
            state = Saved;
            emit stateChanged(state);
        } else {
            state = Goal;
            emit stateChanged(state);
            if (isPlayerTurn) {
                playerScore++;
            } else {
                computerScore++;
            }
            emit scoreUpdated(playerScore, computerScore);
        }
        ballTimer->stop();
        goalkeeperTimer->stop();
        
        // 重置游戏状态
        QTimer::singleShot(2000, this, [=]() {
            // 检查是否完成了一个回合（双方各射一次）
            if (!isPlayerTurn) {
                // 完成了一个回合
                startNextRound();
            } else {
                // 玩家射门结束，切换到电脑回合，等待玩家扑救
                state = Ready;
                ballPosition = QPointF(400, 400);
                ballVelocity = QPointF(0, 0);
                goalkeeperPosition = QPointF(400, 125);
                emit stateChanged(state);
                emit ballMoved(ballPosition);
                emit goalkeeperMoved(goalkeeperPosition);
                isPlayerTurn = false;
            }
        });
    }
    // 检查球是否出界
    else if (ballPosition.y() < 0 || ballPosition.y() > 600 || ballPosition.x() < 0 || ballPosition.x() > 800) {
        state = Miss;
        emit stateChanged(state);
        ballTimer->stop();
        goalkeeperTimer->stop();
        
        // 重置游戏状态
        QTimer::singleShot(2000, this, [=]() {
            // 检查是否完成了一个回合（双方各射一次）
            if (!isPlayerTurn) {
                // 完成了一个回合
                startNextRound();
            } else {
                // 玩家射门结束，切换到电脑回合，等待玩家扑救
                state = Ready;
                ballPosition = QPointF(400, 400);
                ballVelocity = QPointF(0, 0);
                goalkeeperPosition = QPointF(400, 125);
                emit stateChanged(state);
                emit ballMoved(ballPosition);
                emit goalkeeperMoved(goalkeeperPosition);
                isPlayerTurn = false;
            }
        });
    }
}

void Game::updateGoalkeeperPosition()
{
    // 守门员移动
    goalkeeperPosition.setX(goalkeeperPosition.x() + saveDirection * 5);
    
    // 限制守门员在球门范围内
    if (goalkeeperPosition.x() < 300) {
        goalkeeperPosition.setX(300);
    } else if (goalkeeperPosition.x() > 500) {
        goalkeeperPosition.setX(500);
    }

    emit goalkeeperMoved(goalkeeperPosition);
}

void Game::checkGameOver()
{
    if (isOvertime) {
        // 加时赛，只要有一方得分，游戏结束
        if (playerScore != computerScore) {
            state = GameOver;
            emit stateChanged(state);
            emit gameOver(getWinner());
        }
    } else if (round > 5) {
        // 5回合结束，进入加时赛
        isOvertime = true;
        emit roundUpdated(round);
    }
}

void Game::startNextRound()
{
    round++;
    emit roundUpdated(round);
    
    // 检查游戏是否结束
    checkGameOver();
    
    if (state != GameOver) {
        state = Ready;
        ballPosition = QPointF(400, 400);
        ballVelocity = QPointF(0, 0); // 重置球的速度
        goalkeeperPosition = QPointF(400, 125);
        emit stateChanged(state);
        emit ballMoved(ballPosition);
        emit goalkeeperMoved(goalkeeperPosition);
        isPlayerTurn = true; // 每回合开始时玩家先射
    }
}
