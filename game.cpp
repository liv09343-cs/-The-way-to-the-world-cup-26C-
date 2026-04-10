#include "game.h"
#include <QRandomGenerator>

Game::Game(QObject *parent) : QObject(parent)
{
    state = Ready;
    ballPosition = QPointF(400, 400); // 点球点位置
    goalkeeperPosition = QPointF(400, 125); // 球门中间位置
    playerScore = 0;
    computerScore = 0;
    isPlayerTurn = true;

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
    goalkeeperPosition = QPointF(400, 125);
    playerScore = 0;
    computerScore = 0;
    emit stateChanged(state);
    emit ballMoved(ballPosition);
    emit goalkeeperMoved(goalkeeperPosition);
    emit scoreUpdated(playerScore, computerScore);
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

void Game::save(int direction)
{
    if (state != Ready) return;

    state = Saving;
    emit stateChanged(state);

    saveDirection = direction;
    goalkeeperTimer->start(30);
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

void Game::updateBallPosition()
{
    // 更新球的位置
    ballPosition += ballVelocity;
    
    // 模拟重力
    ballVelocity.setY(ballVelocity.y() + 0.5);

    emit ballMoved(ballPosition);

    // 检查球是否进入球门
    if (ballPosition.y() < 200 && ballPosition.x() > 300 && ballPosition.x() < 500) {
        // 检查是否被守门员扑出
        if (qAbs(ballPosition.x() - goalkeeperPosition.x()) < 30) {
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
            state = Ready;
            ballPosition = QPointF(400, 400);
            goalkeeperPosition = QPointF(400, 125);
            emit stateChanged(state);
            emit ballMoved(ballPosition);
            emit goalkeeperMoved(goalkeeperPosition);
            isPlayerTurn = !isPlayerTurn;
        });
    }
    // 检查球是否出界
    else if (ballPosition.y() > 600 || ballPosition.x() < 0 || ballPosition.x() > 800) {
        state = Miss;
        emit stateChanged(state);
        ballTimer->stop();
        goalkeeperTimer->stop();
        
        // 重置游戏状态
        QTimer::singleShot(2000, this, [=]() {
            state = Ready;
            ballPosition = QPointF(400, 400);
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
