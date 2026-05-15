#include <QApplication>
#include "startwindow.h"
#include "teamselectwindow.h"
#include "mainwindow.h"
#include "resultwindow.h"
#include "storyselectwindow.h"
#include "storyscenewindow.h"
#include "matchresultwindow.h"
#include <QRandomGenerator>
#include <algorithm>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    StartWindow *startWindow = new StartWindow();
    TeamSelectWindow *teamSelectWindow = new TeamSelectWindow();
    MainWindow *mainWindow = new MainWindow();
    ResultWindow *resultWindow = new ResultWindow();
    
    StorySelectWindow *storySelectWindow = new StorySelectWindow();
    StorySceneWindow *storySceneWindow = new StorySceneWindow();
    MatchResultWindow *matchResultWindow = new MatchResultWindow();

    QString selectedTeam;
    QStringList groupOpponents;
    QStringList knockoutOpponents;
    int playerWins = 0;
    int playerLosses = 0;
    int knockoutRound = 0;
    bool inGroupStage = true;
    bool inStoryMode = false;
    int currentStoryScene = 0;
    
    QStringList storyGroupOpponents = {"比利时", "埃及", "新西兰"};
    QStringList storyKnockoutOpponents = {"西班牙", "阿根廷", "葡萄牙", "日本", "法国"};
    int storyMatchIndex = 0;
    bool storyQualified = false;

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

    auto startNextStoryMatch = [&]() {
        if (!storyQualified) {
            if (storyMatchIndex < storyGroupOpponents.size()) {
                QString opponent = storyGroupOpponents[storyMatchIndex];
                mainWindow->setTeams("中国队", opponent);
                mainWindow->show();
                mainWindow->startGame();
            } else {
                if (playerWins > 0) {
                    storyQualified = true;
                    matchResultWindow->setResult(true, QString("恭喜！中国队以 %1 胜 %2 负晋级淘汰赛！").arg(playerWins).arg(playerLosses), 10);
                    matchResultWindow->show();
                } else {
                    matchResultWindow->setResult(false, "很遗憾！中国队小组赛全败，未能出线...", 11);
                    matchResultWindow->show();
                }
            }
        } else {
            if (knockoutRound < storyKnockoutOpponents.size()) {
                QString opponent = storyKnockoutOpponents[knockoutRound];
                mainWindow->setTeams("中国队", opponent);
                mainWindow->show();
                mainWindow->startGame();
            }
        }
    };

    QObject::connect(startWindow, SIGNAL(startGame()), teamSelectWindow, SLOT(show()));
    QObject::connect(startWindow, SIGNAL(exitGame()), &a, SLOT(quit()));
    
    QObject::connect(startWindow, &StartWindow::startStory, [&]() {
        startWindow->hide();
        storySelectWindow->show();
    });

    QObject::connect(storySelectWindow, &StorySelectWindow::backClicked, [&]() {
        storySelectWindow->hide();
        startWindow->show();
    });

    QObject::connect(storySelectWindow, &StorySelectWindow::storySelected, [&](int storyId) {
        storySelectWindow->hide();
        inStoryMode = true;
        currentStoryScene = 1;
        storySceneWindow->setScene(1);
        storySceneWindow->show();
    });

    QObject::connect(storySceneWindow, &StorySceneWindow::nextScene, [&]() {
        currentStoryScene++;
        storySceneWindow->setScene(currentStoryScene);
    });

    QObject::connect(storySceneWindow, &StorySceneWindow::startMatch, [&](const QString &playerTeam, const QString &computerTeam) {
        storySceneWindow->hide();
        playerWins = 0;
        playerLosses = 0;
        knockoutRound = 0;
        storyMatchIndex = 0;
        storyQualified = false;
        mainWindow->setTeams(playerTeam, computerTeam);
        mainWindow->show();
        mainWindow->startGame();
    });

    QObject::connect(matchResultWindow, &MatchResultWindow::continueGame, [&]() {
        matchResultWindow->hide();
        if (!storyQualified) {
            if (storyMatchIndex >= storyGroupOpponents.size()) {
                if (playerWins > 0) {
                    matchResultWindow->setResult(true, QString("恭喜！中国队以 %1 胜 %2 负晋级淘汰赛！").arg(playerWins).arg(playerLosses), 10);
                    storyQualified = true;
                    knockoutRound = 0;
                } else {
                    matchResultWindow->setResult(false, "很遗憾！中国队小组赛全败，未能出线...", 11);
                }
                matchResultWindow->show();
            } else {
                QString opponent = storyGroupOpponents[storyMatchIndex];
                mainWindow->setTeams("中国队", opponent);
                mainWindow->show();
                mainWindow->startGame();
            }
        } else {
            startNextStoryMatch();
        }
    });

    QObject::connect(matchResultWindow, &MatchResultWindow::exitStory, [&]() {
        matchResultWindow->hide();
        startWindow->show();
    });

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
        inStoryMode = false;
        startNextGroupMatch();
    });

    QObject::connect(mainWindow, &MainWindow::gameFinished, [&](const QString &winner) {
        mainWindow->hide();
        QString opponent = mainWindow->getGame()->getComputerTeam();

        if (inStoryMode) {
            int currentMatchIndex = 0;
            if (!storyQualified) {
                currentMatchIndex = storyMatchIndex + 1;
                if (winner == "中国队") {
                    playerWins++;
                    matchResultWindow->setResult(true, QString("胜利！中国队击败 %1！").arg(opponent), currentMatchIndex);
                } else {
                    playerLosses++;
                    matchResultWindow->setResult(false, QString("失利！中国队输给了 %1").arg(opponent), currentMatchIndex);
                }
                storyMatchIndex++;
            } else {
                currentMatchIndex = storyGroupOpponents.size() + knockoutRound + 1;
                if (winner == "中国队") {
                    knockoutRound++;
                    if (knockoutRound >= storyKnockoutOpponents.size()) {
                        matchResultWindow->setResult(true, "恭喜！中国队获得2026年美加墨世界杯冠军！", currentMatchIndex);
                    } else {
                        matchResultWindow->setResult(true, QString("胜利！中国队晋级%1！").arg(stageNames[knockoutRound]), currentMatchIndex);
                    }
                } else {
                    knockoutRound++;
                    if (knockoutRound == 4) {
                        matchResultWindow->setResult(false, "虽败犹荣！中国队获得世界杯亚军！", currentMatchIndex);
                    } else {
                        matchResultWindow->setResult(false, QString("遗憾出局！中国队止步%1").arg(stageNames[knockoutRound-1]), currentMatchIndex);
                    }
                }
            }
            matchResultWindow->show();
        } else {
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
        }
    });

    startWindow->show();
    return a.exec();
}