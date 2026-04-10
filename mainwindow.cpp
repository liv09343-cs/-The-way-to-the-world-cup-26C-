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
        // 处理游戏状态变化
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
    painter.drawText(100, 50, QString("玩家: %1").arg(game->getPlayerScore()));
    painter.drawText(width() - 200, 50, QString("电脑: %1").arg(game->getComputerScore()));

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
    }

    // 绘制操作提示
    painter.setFont(QFont("Arial", 14));
    painter.drawText(30, height() - 30, "点击屏幕射门，根据点击位置控制角度和力度");
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (game->getState() == Game::Ready) {
        // 计算射门角度和力度
        int angle = (event->position().x() - width()/2) / 4;
        int power = (height() - event->position().y()) / 10;
        game->shoot(angle, power);
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