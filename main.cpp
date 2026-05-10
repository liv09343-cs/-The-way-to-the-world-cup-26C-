#include <QApplication>
#include "startwindow.h"
#include "teamselectwindow.h"
#include "mainwindow.h"
#include "resultwindow.h"
#include <QRandomGenerator>
#include <algorithm>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    StartWindow *startWindow = new StartWindow();
    TeamSelectWindow *teamSelectWindow = new TeamSelectWindow();
    MainWindow *mainWindow = new MainWindow();
    ResultWindow *resultWindow = new ResultWindow();

    QString selectedTeam;
    QStringList groupOpponents;
    QStringList knockoutOpponents;
    int playerWins = 0;
    int playerLosses = 0;
    int knockoutRound = 0;
    bool inGroupStage = true;

    QString stageNames[] = {"32强", "16强", "8强", "半决赛", "决赛"};

    QList<QStringList> teamsByGroup = {
        {"墨西哥", "南非", "韩国", "捷克"},
        {"加拿大", "波黑", "卡塔尔", "瑞士"},
        {"巴西", "摩洛哥", "海地", "苏格兰"},
        {"美国", "巴拉圭", "澳大利亚", "土耳其"},
        {"德国", "库拉索", "科特迪瓦", "厄瓜多尔"},
        {"荷兰", "日本", "瑞典", "突尼斯"},
        {"比利时", "埃及", "伊朗", "新西兰"},
        {"西班牙", "佛得角", "沙特阿拉伯", "乌拉圭"},
        {"法国", "塞内加尔", "伊拉克", "挪威"},
        {"阿根廷", "阿尔及利亚", "奥地利", "约旦"},
        {"葡萄牙", "民主刚果", "乌兹别克斯坦", "哥伦比亚"},
        {"英格兰", "克罗地亚", "加纳", "巴拿马"}
    };

    auto startKnockoutMatch = [&]() {
        if (knockoutOpponents.isEmpty()) {
            resultWindow->setResult("冠军！", QString("恭喜%1 获得2026年美加墨世界杯冠军！").arg(selectedTeam), selectedTeam);
            resultWindow->exec();
            return;
        }
        QString opponent = knockoutOpponents.takeAt(QRandomGenerator::global()->bounded(knockoutOpponents.size()));
        mainWindow->setTeams(selectedTeam, opponent);
        mainWindow->show();
        mainWindow->startGame();
    };

    auto initKnockout = [&]() {
        knockoutOpponents.clear();
        for (const QStringList &group : teamsByGroup) {
            for (const QString &team : group) {
                if (team != selectedTeam) knockoutOpponents << team;
            }
        }
        std::random_shuffle(knockoutOpponents.begin(), knockoutOpponents.end());
        knockoutRound = 0;
        startKnockoutMatch();
    };

    auto startNextGroupMatch = [&]() {
        if (groupOpponents.isEmpty()) {
            inGroupStage = false;
            if (playerWins > 0) {
                resultWindow->setResult("恭喜晋级32强！", QString("%1 在小组赛中获得 %2 胜 %3 负，成功晋级32强淘汰赛！").arg(selectedTeam).arg(playerWins).arg(playerLosses), selectedTeam);
                resultWindow->exec();
                initKnockout();
            } else {
                resultWindow->setResult("小组出局", QString("很遗憾，%1 在小组赛中全败，未能晋级淘汰赛！").arg(selectedTeam), selectedTeam);
                resultWindow->exec();
            }
            return;
        }
        QString opponent = groupOpponents.takeFirst();
        mainWindow->setTeams(selectedTeam, opponent);
        mainWindow->show();
        mainWindow->startGame();
    };

    QObject::connect(startWindow, SIGNAL(startGame()), teamSelectWindow, SLOT(show()));
    QObject::connect(startWindow, SIGNAL(exitGame()), &a, SLOT(quit()));

    QObject::connect(teamSelectWindow, &TeamSelectWindow::teamSelected, [&](const QString &team, const QString &group) {
        selectedTeam = team;
        teamSelectWindow->hide();
        groupOpponents.clear();
        
        int groupIndex = group.at(0).toLatin1() - 'A';
        QStringList groupTeams = teamsByGroup[groupIndex];
        
        for (const QString &t : groupTeams) {
            if (t != team) groupOpponents << t;
        }

        playerWins = 0;
        playerLosses = 0;
        knockoutRound = 0;
        inGroupStage = true;
        startNextGroupMatch();
    });

    QObject::connect(mainWindow, &MainWindow::gameFinished, [&](const QString &winner) {
        mainWindow->hide();
        QString opponent = mainWindow->getGame()->getComputerTeam();

        if (inGroupStage) {
            if (winner == selectedTeam) {
                playerWins++;
                resultWindow->setResult("比赛胜利！", QString("恭喜您的球队 %1 击败了 %2！").arg(selectedTeam).arg(opponent), selectedTeam);
            } else {
                playerLosses++;
                resultWindow->setResult("比赛失利", QString("很遗憾，%1 输给了 %2").arg(selectedTeam).arg(opponent), selectedTeam);
            }
            resultWindow->exec();
            startNextGroupMatch();
        } else {
            knockoutRound++;
            if (winner == selectedTeam) {
                if (knockoutRound >= 5) {
                    resultWindow->setResult("冠军！", QString("恭喜%1 获得2026年美加墨世界杯冠军！").arg(selectedTeam), selectedTeam);
                    resultWindow->exec();
                } else {
                    resultWindow->setResult(QString("恭喜晋级%1！").arg(stageNames[knockoutRound]), QString("%1 在%2淘汰赛中击败 %3，成功晋级！").arg(selectedTeam).arg(stageNames[knockoutRound-1]).arg(opponent), selectedTeam);
                    resultWindow->exec();
                    startKnockoutMatch();
                }
            } else {
                if (knockoutRound == 4) {
                    bool thirdPlace = QRandomGenerator::global()->bounded(2) == 0;
                    if (thirdPlace) {
                        resultWindow->setResult("季军", QString("%1 获得2026年美加墨世界杯季军！").arg(selectedTeam), selectedTeam);
                    } else {
                        resultWindow->setResult("第四名", QString("%1 获得2026年美加墨世界杯第四名").arg(selectedTeam), selectedTeam);
                    }
                    resultWindow->exec();
                } else {
                    resultWindow->setResult(QString("止步%1").arg(stageNames[knockoutRound-1]), QString("%1 在%2淘汰赛中被 %3 淘汰").arg(selectedTeam).arg(stageNames[knockoutRound-1]).arg(opponent), selectedTeam);
                    resultWindow->exec();
                }
            }
        }
    });

    startWindow->show();
    return a.exec();
}