#include "mainwindow.h"
#include "startwindow.h"
#include "teamselectwindow.h"
#include <QApplication>
#include <QWidget>
#include <QRandomGenerator>

class AppManager : public QObject
{
    Q_OBJECT

public:
    AppManager(QApplication *app)
        : app(app), startWindow(nullptr), teamSelectWindow(nullptr), gameWindow(nullptr)
    {
        // 创建开始界面
        startWindow = new StartWindow();
        connect(startWindow, &StartWindow::startGameClicked, this, &AppManager::showTeamSelectWindow);
        connect(startWindow, &StartWindow::exitClicked, app, &QApplication::quit);
        startWindow->show();
    }

public slots:
    void showTeamSelectWindow()
    {
        // 隐藏开始界面
        startWindow->hide();

        // 创建队伍选择界面
        teamSelectWindow = new TeamSelectWindow();
        connect(teamSelectWindow, &TeamSelectWindow::teamSelected, this, &AppManager::showGameWindow);
        connect(teamSelectWindow, &TeamSelectWindow::backClicked, this, &AppManager::showStartWindow);
        teamSelectWindow->show();
    }

    void showGameWindow()
    {
        // 隐藏队伍选择界面
        teamSelectWindow->hide();

        // 创建游戏界面
        gameWindow = new MainWindow();
        
        // 设置队伍和分组信息
        QString playerTeam = teamSelectWindow->getSelectedTeam();
        QString group = teamSelectWindow->getSelectedTeamGroup();
        
        // 随机选择同组的一个电脑队伍
        QString computerTeam;
        
        // 计算同组队伍的范围
        int groupIndex = group.at(0).toLatin1() - 'A';
        int startTeam = groupIndex * 4 + 1;
        int endTeam = startTeam + 3;
        
        // 随机选择一个同组队伍
        int teamNumber;
        do {
            teamNumber = QRandomGenerator::global()->bounded(startTeam, endTeam + 1);
        } while (QString("Team%1").arg(teamNumber) == playerTeam);
        
        computerTeam = QString("Team%1").arg(teamNumber);
        
        // 设置游戏的队伍和分组
        gameWindow->setTeams(playerTeam, computerTeam);
        gameWindow->setGroup(group);
        
        // 开始游戏
        gameWindow->startGame();
        
        // 连接游戏结束信号
        connect(gameWindow, &MainWindow::backToStart, this, &AppManager::showStartWindow);
        
        gameWindow->show();
    }

    void showStartWindow()
    {
        // 隐藏所有其他界面
        if (teamSelectWindow) {
            teamSelectWindow->hide();
            delete teamSelectWindow;
            teamSelectWindow = nullptr;
        }
        if (gameWindow) {
            gameWindow->hide();
            delete gameWindow;
            gameWindow = nullptr;
        }

        // 显示开始界面
        startWindow->show();
    }

private:
    QApplication *app;
    StartWindow *startWindow;
    TeamSelectWindow *teamSelectWindow;
    MainWindow *gameWindow;
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AppManager manager(&a);

    return a.exec();
}

#include "main.moc"