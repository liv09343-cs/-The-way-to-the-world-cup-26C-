#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QPixmap>
#include <QDir>
#include <QMap>

const QMap<QString, QString> MainWindow::CHINESE_TO_ENGLISH = {
    {"墨西哥", "Mexico"}, {"南非", "SouthAfrica"}, {"韩国", "SouthKorea"}, {"捷克", "Czech"},
    {"加拿大", "Canada"}, {"波黑", "Bosnia"}, {"卡塔尔", "Qatar"}, {"瑞士", "Switzerland"},
    {"巴西", "Brazil"}, {"摩洛哥", "Morocco"}, {"海地", "Haiti"}, {"苏格兰", "Scotland"},
    {"美国", "USA"}, {"巴拉圭", "Paraguay"}, {"澳大利亚", "Australia"}, {"土耳其", "Turkey"},
    {"德国", "Germany"}, {"库拉索", "Curacao"}, {"科特迪瓦", "CoteDIvoire"}, {"厄瓜多尔", "Ecuador"},
    {"荷兰", "Netherlands"}, {"日本", "Japan"}, {"瑞典", "Sweden"}, {"突尼斯", "Tunisia"},
    {"比利时", "Belgium"}, {"埃及", "Egypt"}, {"伊朗", "Iran"}, {"新西兰", "NewZealand"},
    {"西班牙", "Spain"}, {"佛得角", "CapeVerde"}, {"沙特阿拉伯", "SaudiArabia"}, {"乌拉圭", "Uruguay"},
    {"法国", "France"}, {"塞内加尔", "Senegal"}, {"伊拉克", "Iraq"}, {"挪威", "Norway"},
    {"阿根廷", "Argentina"}, {"阿尔及利亚", "Algeria"}, {"奥地利", "Austria"}, {"约旦", "Jordan"},
    {"葡萄牙", "Portugal"}, {"民主刚果", "DRCongo"}, {"乌兹别克斯坦", "Uzbekistan"}, {"哥伦比亚", "Colombia"},
    {"英格兰", "England"}, {"克罗地亚", "Croatia"}, {"加纳", "Ghana"}, {"巴拿马", "Panama"}
};

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
    connect(game, &Game::gameOver, this, &MainWindow::onGameOver);

    gameTimer->start(30);
}

QPixmap MainWindow::loadFlag(const QString &teamName)
{
    QString englishName = CHINESE_TO_ENGLISH.value(teamName, teamName);
    QString appDir = QCoreApplication::applicationDirPath();
    QString flagPath = appDir + "/flags/flag_" + englishName + ".png";
    qDebug() << "App dir:" << appDir;
    qDebug() << "Loading flag for:" << teamName << "->" << englishName << "path:" << flagPath;
    QPixmap pixmap(flagPath);
    qDebug() << "Flag loaded successfully:" << !pixmap.isNull();
    return pixmap.scaled(32, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation);
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
    painter.setFont(QFont("Arial", 20));
    
    int panelX = 30;
    int panelY = 100;
    int panelWidth = 200;
    int panelHeight = 120;
    
    painter.setBrush(QColor(46, 46, 46));
    painter.drawRect(panelX, panelY, panelWidth, panelHeight);
    
    if (!playerFlag.isNull()) {
        painter.drawPixmap(panelX + 10, panelY + 15, playerFlag);
    }
    if (!computerFlag.isNull()) {
        painter.drawPixmap(panelX + 10, panelY + 60, computerFlag);
    }
    
    painter.setPen(QColor(255, 255, 255));
    painter.setFont(QFont("Arial", 12));
    painter.drawText(panelX + 50, panelY + 28, QString("%1").arg(game->getPlayerTeam()));
    painter.drawText(panelX + 50, panelY + 73, QString("%1").arg(game->getComputerTeam()));
    
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(panelX + panelWidth - 30, panelY + 25, QString("%1").arg(game->getPlayerScore()));
    painter.drawText(panelX + panelWidth - 30, panelY + 70, QString("%1").arg(game->getComputerScore()));
    
    const QList<bool>& playerHistory = game->getPlayerShootHistory();
    const QList<bool>& computerHistory = game->getComputerShootHistory();
    
    int dotSize = 12;
    int dotSpacing = 15;
    int playerDotsStartX = panelX + 10;
    int playerDotsY = panelY + 45;
    int computerDotsStartX = panelX + 10;
    int computerDotsY = panelY + 90;
    
    for (int i = 0; i < playerHistory.size(); i++) {
        int x = playerDotsStartX + i * dotSpacing;
        if (playerHistory[i]) {
            painter.setBrush(QColor(0, 255, 0));
        } else {
            painter.setBrush(QColor(255, 0, 0));
        }
        painter.drawEllipse(x, playerDotsY, dotSize, dotSize);
    }
    
    for (int i = 0; i < computerHistory.size(); i++) {
        int x = computerDotsStartX + i * dotSpacing;
        if (computerHistory[i]) {
            painter.setBrush(QColor(0, 255, 0));
        } else {
            painter.setBrush(QColor(255, 0, 0));
        }
        painter.drawEllipse(x, computerDotsY, dotSize, dotSize);
    }

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
    playerFlag = loadFlag(playerTeam);
    computerFlag = loadFlag(computerTeam);
}

void MainWindow::setGroup(const QString &group)
{
    game->setGroup(group);
}

void MainWindow::startGame()
{
    game->startGame();
}

void MainWindow::onGameOver(const QString &winner)
{
    emit gameFinished(winner);
}