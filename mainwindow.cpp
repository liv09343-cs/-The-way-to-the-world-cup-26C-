#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QPixmap>
#include <QDir>
#include <QMap>
#include <QCoreApplication>

const QMap<QString, QString> MainWindow::CHINESE_TO_ENGLISH = {
    {"中国", "China"}, {"中国队", "China"}, {"墨西哥", "Mexico"}, {"南非", "SouthAfrica"}, {"韩国", "SouthKorea"}, {"捷克", "Czech"},
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

    QStringList goalkeeperPaths = {
        QCoreApplication::applicationDirPath() + "/goalkeeper.png",
        "c:/Users/spong/Documents/trae_projects/world-cup/goalkeeper.png",
        "c:/Users/spong/Desktop/新建文件夹（2）/goalkeeper.png",
        "c:/Users/spong/Desktop/goalkeeper.png"
    };
    
    for (const QString& path : goalkeeperPaths) {
        if (goalkeeperPixmap.load(path)) {
            break;
        }
    }
    
    QStringList backgroundPaths = {
        QCoreApplication::applicationDirPath() + "/background_game.png",
        "c:/Users/spong/Documents/trae_projects/world-cup/background_game.png",
        "c:/Users/spong/Desktop/background_game.png"
    };
    
    for (const QString& path : backgroundPaths) {
        if (backgroundPixmap.load(path)) {
            break;
        }
    }

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
    
    QStringList paths = {
        ":/flags/flag_" + englishName + ".png",
        QCoreApplication::applicationDirPath() + "/flags/flag_" + englishName + ".png",
        QCoreApplication::applicationDirPath() + "/../flags/flag_" + englishName + ".png",
        "c:/Users/spong/Documents/trae_projects/world-cup/flags/flag_" + englishName + ".png",
        QDir::currentPath() + "/flags/flag_" + englishName + ".png",
        "flags/flag_" + englishName + ".png"
    };
    
    QPixmap pixmap;
    for (const QString& path : paths) {
        if (pixmap.load(path)) {
            break;
        }
    }
    
    if (pixmap.isNull()) {
        pixmap = QPixmap(32, 20);
        pixmap.fill(QColor(128, 128, 128));
    }
    
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

    if (!backgroundPixmap.isNull()) {
        painter.drawPixmap(0, 0, width(), height(), backgroundPixmap);
    } else {
        painter.setBrush(QColor(34, 139, 34));
        painter.drawRect(0, 0, width(), height());
    }

    const double goalWidth = 500;
    const double goalHeight = 200;
    const double goalX = width()/2 - goalWidth/2;
    const double goalY = 30;
    const double goalLineY = goalY + goalHeight;
    
    painter.setPen(QPen(QColor(255, 0, 0), 4));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(goalX, goalY, goalWidth, goalHeight);

    painter.setPen(QPen(QColor(255, 0, 0), 4));
    painter.drawLine(goalX, goalLineY, goalX + goalWidth, goalLineY);
    
    painter.setPen(QPen(QColor(139, 69, 19), 5));
    painter.drawLine(goalX, goalY, goalX, goalLineY);
    painter.drawLine(goalX + goalWidth, goalY, goalX + goalWidth, goalLineY);
    painter.drawLine(goalX, goalY, goalX + goalWidth, goalY);

    painter.setBrush(QColor(255, 255, 255));
    painter.drawEllipse(width()/2 - 8, height() - 100, 16, 16);

    painter.setFont(QFont("Arial", 18, QFont::Bold));
    painter.setPen(QColor(255, 215, 0));
    painter.drawText(30, 30, "2026年美加墨世界杯");

    painter.setFont(QFont("Arial", 16));
    painter.setPen(QColor(255, 255, 255));
    painter.drawText(width()/2 - 50, 70, QString("Round %1").arg(game->getRound()));

    QPointF ballPos = game->getBallPosition();
    painter.setBrush(QColor(255, 255, 255));
    painter.drawEllipse(ballPos, 10, 10);

    QPointF keeperPos = game->getGoalkeeperPosition();
    if (!goalkeeperPixmap.isNull()) {
        painter.drawPixmap(keeperPos.x() - 60, goalLineY - 140, 120, 140, goalkeeperPixmap);
    } else {
        painter.setBrush(QColor(0, 150, 0));
        painter.drawRect(keeperPos.x() - 50, goalLineY - 120, 100, 120);
        painter.setBrush(QColor(255, 255, 255));
        painter.drawEllipse(keeperPos.x() - 18, goalLineY - 130, 36, 36);
    }

    int playerY = height() - 160;
    int playerX;
    bool isComputerKick = (game->getState() == Game::ComputerKicking);
    bool isKicking = (game->getState() == Game::Kicking || isComputerKick);
    
    if (isKicking) {
        playerX = static_cast<int>(game->getPlayerRunX());
    } else {
        playerX = width() - 85;
    }
    
    QColor bodyColor = isComputerKick ? QColor(100, 100, 255) : QColor(255, 100, 100);
    QColor shirtColor = isComputerKick ? QColor(255, 0, 0) : QColor(0, 0, 255);
    
    painter.setBrush(bodyColor);
    painter.drawRect(playerX - 18, playerY - 70, 36, 90);
    
    painter.setBrush(QColor(255, 215, 0));
    painter.drawEllipse(playerX - 10, playerY - 82, 20, 20);
    
    painter.setBrush(shirtColor);
    painter.drawRect(playerX - 15, playerY - 50, 30, 18);
    
    painter.setBrush(bodyColor);
    painter.drawRect(playerX - 18, playerY - 30, 14, 55);
    
    if (isKicking) {
        double kickProgress = game->getKickFrame() / 25.0;
        
        if (kickProgress < 0.75) {
            int legStep = (game->getKickFrame() % 4) - 2;
            painter.setBrush(bodyColor);
            painter.drawRect(playerX + 4, playerY - 30 + legStep * 6, 14, 55);
        } else {
            double legSwing = sin((kickProgress - 0.75) * 6 * 3.14159) * 50;
            
            painter.save();
            painter.translate(playerX, playerY - 30);
            painter.rotate(-legSwing);
            painter.setBrush(bodyColor);
            painter.drawRect(-5, 0, 30, 12);
            painter.restore();
        }
    } else {
        painter.setBrush(bodyColor);
        painter.drawRect(playerX + 4, playerY - 30, 14, 55);
    }

    painter.setPen(QColor(255, 255, 255));
    painter.setFont(QFont("Arial", 12));
    
    int panelX = 10;
    int panelY = 30;
    int panelWidth = 120;
    int panelHeight = 70;
    
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRect(panelX, panelY, panelWidth, panelHeight);
    
    if (!playerFlag.isNull()) {
        painter.drawPixmap(panelX + 6, panelY + 8, playerFlag.scaled(24, 15));
    }
    if (!computerFlag.isNull()) {
        painter.drawPixmap(panelX + 6, panelY + 35, computerFlag.scaled(24, 15));
    }
    
    painter.setPen(QColor(255, 255, 255));
    painter.setFont(QFont("Arial", 9));
    painter.drawText(panelX + 38, panelY + 20, QString("%1").arg(game->getPlayerTeam()));
    painter.drawText(panelX + 38, panelY + 47, QString("%1").arg(game->getComputerTeam()));
    
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.drawText(panelX + panelWidth - 20, panelY + 18, QString("%1").arg(game->getPlayerScore()));
    painter.drawText(panelX + panelWidth - 20, panelY + 45, QString("%1").arg(game->getComputerScore()));
    
    painter.setPen(QPen(QColor(128, 128, 128), 1));
    painter.drawLine(panelX + 5, panelY + 30, panelX + panelWidth - 5, panelY + 30);
    
    const QList<bool>& playerHistory = game->getPlayerShootHistory();
    const QList<bool>& computerHistory = game->getComputerShootHistory();
    
    int dotSize = 8;
    int dotSpacing = 10;
    int playerDotsStartX = panelX + 6;
    int playerDotsY = panelY + 28;
    int computerDotsStartX = panelX + 6;
    int computerDotsY = panelY + 55;
    
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
    case Game::Aiming:
        painter.drawText(width()/2 - 80, height()/2 - 50, "选择射门方向");
        {
            painter.setPen(QPen(QColor(255, 215, 0), 3));
            int arrowLength = 100;
            double angleRad = game->getCurrentAimAngle() * 3.14159 / 180.0;
            int startX = width()/2;
            int startY = height() - 100;
            int endX = startX + sin(angleRad) * arrowLength;
            int endY = startY - cos(angleRad) * arrowLength;
            
            painter.drawLine(startX, startY, endX, endY);
            
            double arrowAngle = atan2(endY - startY, endX - startX);
            painter.drawLine(endX, endY, 
                            endX - 15 * cos(arrowAngle - 3.14159/6), 
                            endY - 15 * sin(arrowAngle - 3.14159/6));
            painter.drawLine(endX, endY, 
                            endX - 15 * cos(arrowAngle + 3.14159/6), 
                            endY - 15 * sin(arrowAngle + 3.14159/6));
        }
        painter.drawText(width()/2 - 100, height()/2 + 30, QString("当前角度: %1°").arg(game->getCurrentAimAngle()));
        break;
    case Game::PowerSelecting:
        painter.drawText(width()/2 - 80, height()/2 - 50, "选择射门力度");
        {
            int powerBarWidth = 200;
            int powerBarHeight = 20;
            int powerBarX = width()/2 - powerBarWidth/2;
            int powerBarY = height()/2;
            
            painter.setBrush(QColor(60, 60, 60));
            painter.drawRect(powerBarX, powerBarY, powerBarWidth, powerBarHeight);
            
            int powerWidth = game->getSelectedPower() * powerBarWidth / 100;
            QColor powerColor;
            if (game->getSelectedPower() < 50) {
                powerColor = QColor(0, 255, 0);
            } else if (game->getSelectedPower() < 80) {
                powerColor = QColor(255, 255, 0);
            } else {
                powerColor = QColor(255, 0, 0);
            }
            painter.setBrush(powerColor);
            painter.drawRect(powerBarX, powerBarY, powerWidth, powerBarHeight);
            
            painter.setPen(QColor(255, 255, 255));
            painter.drawText(width()/2 - 30, height()/2 + 40, QString("%1%").arg(game->getSelectedPower()));
        }
        break;
    case Game::Kicking:
        painter.drawText(width()/2 - 50, height()/2, "踢球中...");
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
    case Game::ComputerKicking:
        painter.drawText(width()/2 - 50, height()/2, "电脑踢球中...");
        break;
    }

    painter.setFont(QFont("Arial", 14));
    if (game->getState() == Game::Ready) {
        if (game->getIsPlayerTurn()) {
            painter.drawText(30, height() - 30, "点击屏幕开始射门");
        } else {
            painter.drawText(30, height() - 30, "点击屏幕扑救，选择扑救方向");
        }
    } else if (game->getState() == Game::Aiming) {
        painter.drawText(30, height() - 30, "左右移动鼠标选择角度，点击确认");
    } else if (game->getState() == Game::PowerSelecting) {
        painter.drawText(30, height() - 30, "上下移动鼠标选择力度，点击确认");
    } else if (game->getState() == Game::GameOver) {
        painter.drawText(30, height() - 30, "游戏结束");
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (game->getState() == Game::Ready) {
        bool isPlayerTurn = game->getIsPlayerTurn();

        if (isPlayerTurn) {
            game->startAiming();
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
    } else if (game->getState() == Game::Aiming) {
        game->confirmAngle();
    } else if (game->getState() == Game::PowerSelecting) {
        game->confirmPower();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (game->getState() == Game::Aiming) {
        int mouseX = event->position().x();
        int angle = (mouseX - width()/2) / 4;
        game->setAimAngle(angle);
    } else if (game->getState() == Game::PowerSelecting) {
        int mouseY = event->position().y();
        int power = (height() - mouseY) * 100 / height();
        game->setPower(power);
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