#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("2026年美加墨世界杯 - 点球大战");
    setFixedSize(800, 600);

    game = new Game(this);
    gameTimer = new QTimer(this);

    connect(gameTimer, &QTimer::timeout, this, [this]() { this->update(); });
    connect(game, &Game::stateChanged, this, [this](Game::GameState) { this->update(); });
    connect(game, &Game::ballMoved, this, [this](QPointF) { this->update(); });
    connect(game, &Game::goalkeeperMoved, this, [this](QPointF) { this->update(); });

    gameTimer->start(30);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    painter.setBrush(QColor(34, 139, 34));
    painter.drawRect(0, 0, width(), height());

    painter.setBrush(QColor(255, 255, 255));
    painter.drawRect(width()/2 - 100, 50, 200, 150);

    painter.setBrush(QColor(255, 255, 255));
    painter.drawEllipse(width()/2 - 5, height() - 200, 10, 10);

    painter.setFont(QFont("Arial", 18, QFont::Bold));
    painter.setPen(QColor(255, 215, 0));
    painter.drawText(30, 30, "2026年美加墨世界杯");

    painter.setFont(QFont("Arial", 16));
    painter.setPen(QColor(255, 255, 255));
    painter.drawText(30, 70, QString("Group %1").arg(game->getGroup()));
    painter.drawText(150, 70, QString("%1 vs %2").arg(game->getPlayerTeam()).arg(game->getComputerTeam()));

    painter.drawText(width()/2 - 50, 70, QString("Round %1").arg(game->getRound()));

    QPointF ballPos = game->getBallPosition();
    painter.setBrush(QColor(255, 255, 255));
    painter.drawEllipse(ballPos, 10, 10);

    QPointF keeperPos = game->getGoalkeeperPosition();
    painter.setBrush(QColor(255, 0, 0));
    painter.drawRect(keeperPos.x() - 20, keeperPos.y() - 30, 40, 60);

    painter.setPen(QColor(255, 255, 255));
    painter.setFont(QFont("Arial", 24));
    painter.drawText(100, 120, QString("%1: %2").arg(game->getPlayerTeam()).arg(game->getPlayerScore()));
    painter.drawText(width() - 200, 120, QString("%1: %2").arg(game->getComputerTeam()).arg(game->getComputerScore()));

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
        painter.drawText(width()/2 - 80, height()/2, QString("游戏结束！%1 获胜！").arg(game->getWinner()));
        break;
    }

    painter.setFont(QFont("Arial", 14));
    if (game->getState() == Game::Ready) {
        if (game->getIsPlayerTurn()) {
            painter.drawText(30, height() - 30, "点击屏幕射门，根据点击位置控制角度和力度");
        } else {
            painter.drawText(30, height() - 30, "点击屏幕扑救，选择扑救方向");
        }
    } else if (game->getState() == Game::GameOver) {
        painter.drawText(30, height() - 30, "游戏结束");
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (game->getState() == Game::Ready) {
        bool isPlayerTurn = game->getIsPlayerTurn();

        if (isPlayerTurn) {
            int mouseX = event->position().x();
            int mouseY = event->position().y();

            int angle = (mouseX - width()/2) / 4;
            int power = (height() - mouseY) / 10;
            game->shoot(angle, power);
        } else {
            int mouseX = event->position().x();
            int direction = 0;

            if (mouseX < width()/2 - 50) {
                direction = -1;
            } else if (mouseX > width()/2 + 50) {
                direction = 1;
            }

            game->save(direction);
        }
    }
}

void MainWindow::on_startButton_clicked()
{
    game->startGame();
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