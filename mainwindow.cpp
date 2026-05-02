#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QImage>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("2026年美加墨世界杯 - 点球大战");
    setFixedSize(800, 600);

    game = new Game(this);
    gameTimer = new QTimer(this);

    connect(gameTimer, &QTimer::timeout, this, &MainWindow::updateGame);
    connect(game, &Game::stateChanged, this, [=](Game::GameState state) {
        update(); // 确保界面更新
    });
    connect(game, &Game::ballMoved, this, [=](QPointF pos) {
        update(); // 球移动时更新界面
    });
    connect(game, &Game::goalkeeperMoved, this, [=](QPointF pos) {
        update(); // 守门员移动时更新界面
    });

    // 手动连接信号和槽
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::on_startButton_clicked);
    connect(ui->quitButton, &QPushButton::clicked, this, &MainWindow::on_quitButton_clicked);

    gameTimer->start(30);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::updateGame()
{
    update();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    // 绘制足球场
    painter.setBrush(QColor(34, 139, 34));
    painter.drawRect(0, 0, width(), height());

    // 绘制球门
    painter.setBrush(QColor(255, 255, 255));
    painter.drawRect(width()/2 - 100, 50, 200, 150);

    // 绘制点球点
    painter.setBrush(QColor(255, 255, 255));
    painter.drawEllipse(width()/2 - 5, height() - 200, 10, 10);

    // 绘制2026年美加墨世界杯标志
    painter.setFont(QFont("Arial", 18, QFont::Bold));
    painter.setPen(QColor(255, 215, 0));
    painter.drawText(30, 30, "2026年美加墨世界杯");

    // 绘制队伍信息
    painter.setFont(QFont("Arial", 16));
    painter.setPen(QColor(255, 255, 255));
    painter.drawText(30, 70, QString("Group %1").arg(game->getGroup()));
    painter.drawText(150, 70, QString("%1 vs %2").arg(game->getPlayerTeam()).arg(game->getComputerTeam()));

    // 绘制回合信息
    painter.drawText(width()/2 - 50, 70, QString("Round %1").arg(game->getRound()));

    // 绘制球
    QPointF ballPos = game->getBallPosition();
    painter.setBrush(QColor(255, 255, 255));
    painter.drawEllipse(ballPos, 10, 10);

    // 绘制守门员
    QPointF keeperPos = game->getGoalkeeperPosition();
    painter.setBrush(QColor(255, 0, 0));
    painter.drawRect(keeperPos.x() - 20, keeperPos.y() - 30, 40, 60);

    // 绘制比分
    painter.setPen(QColor(255, 255, 255));
    painter.setFont(QFont("Arial", 24));
    painter.drawText(100, 120, QString("%1: %2").arg(game->getPlayerTeam()).arg(game->getPlayerScore()));
    painter.drawText(width() - 200, 120, QString("%1: %2").arg(game->getComputerTeam()).arg(game->getComputerScore()));

    // 绘制游戏状态
    painter.setFont(QFont("Arial", 16));
    switch (game->getState()) {
    case Game::Ready:
        painter.drawText(width()/2 - 50, height()/2, "准备就绪");
        break;
    case Game::Shooting:
        painter.drawText(width()/2 - 50, height()/2, "射门中...");
        break;
    case Game::Saving:
        painter.drawText(width()/2 - 50, height()/2, "扑救中...");
        break;
    case Game::Goal:
        painter.drawText(width()/2 - 50, height()/2, "进球！");
        break;
    case Game::Miss:
        painter.drawText(width()/2 - 50, height()/2, "未进！");
        break;
    case Game::Saved:
        painter.drawText(width()/2 - 50, height()/2, "被扑出！");
        break;
    case Game::GameOver:
        painter.drawText(width()/2 - 50, height()/2, QString("游戏结束！%1 获胜！").arg(game->getWinner()));
        break;
    }

    // 绘制操作提示
    painter.setFont(QFont("Arial", 14));
    if (game->getState() == Game::Ready) {
        painter.drawText(30, height() - 30, "点击屏幕射门，根据点击位置控制角度和力度");
    } else if (game->getState() == Game::GameOver) {
        painter.drawText(30, height() - 30, "游戏结束，点击开始按钮重新开始");
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (game->getState() == Game::Ready) {
        // 获取当前回合的控制方
        bool isPlayerTurn = game->getIsPlayerTurn();
        
        if (isPlayerTurn) {
            // 玩家回合，玩家控制射门
            int mouseX = event->position().x();
            int mouseY = event->position().y();
            
            int angle = (mouseX - width()/2) / 4;
            int power = (height() - mouseY) / 10;
            game->shoot(angle, power);
        } else {
            // 电脑回合，玩家控制守门员扑救
            int mouseX = event->position().x();
            int direction = 0;
            
            if (mouseX < width()/2 - 50) {
                direction = -1; // 向左扑救
            } else if (mouseX > width()/2 + 50) {
                direction = 1; // 向右扑救
            } else {
                direction = 0; // 中间扑救
            }
            
            game->save(direction);
        }
    }
}

void MainWindow::on_startButton_clicked()
{
    game->startGame();
    update();
}

void MainWindow::on_quitButton_clicked()
{
    close();
}

void MainWindow::setTeams(const QString &playerTeam, const QString &computerTeam)
{
    game->setTeams(playerTeam, computerTeam);
}

void MainWindow::setGroup(const QString &group)
{
    game->setGroup(group);
}

void MainWindow::startGame()
{
    game->startGame();
}