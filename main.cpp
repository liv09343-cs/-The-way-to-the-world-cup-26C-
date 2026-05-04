#include <QApplication>
#include "startwindow.h"
#include "teamselectwindow.h"
#include "mainwindow.h"
#include <QRandomGenerator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    StartWindow *startWindow = new StartWindow();
    TeamSelectWindow *teamSelectWindow = new TeamSelectWindow();
    MainWindow *mainWindow = new MainWindow();

    QString selectedTeam;
    QString selectedGroup;
    QString computerTeam;

    QObject::connect(startWindow, &StartWindow::startGame, [&]() {
        startWindow->hide();
        teamSelectWindow->show();
    });

    QObject::connect(startWindow, &StartWindow::exitGame, [&]() {
        a.quit();
    });

    QObject::connect(teamSelectWindow, &TeamSelectWindow::teamSelected, [&](const QString &team, const QString &group) {
        selectedTeam = team;
        selectedGroup = group;

        QStringList groupTeams = {"Team1", "Team2", "Team3", "Team4"};
        int groupIndex = group.at(0).toLatin1() - 'A';
        int teamIndex = team.mid(4).toInt() - 1;
        int baseTeam = groupIndex * 4;

        QList<int> otherTeamIndices;
        for (int i = 0; i < 4; i++) {
            if (i != (teamIndex % 4)) {
                otherTeamIndices.append(baseTeam + i);
            }
        }

        int randIndex = QRandomGenerator::global()->bounded(otherTeamIndices.size());
        int computerIndex = otherTeamIndices[randIndex];
        computerTeam = QString("Team%1").arg(computerIndex + 1);

        teamSelectWindow->hide();
        mainWindow->setTeams(selectedTeam, computerTeam);
        mainWindow->setGroup(selectedGroup);
        mainWindow->show();
        mainWindow->startGame();
    });

    startWindow->show();

    return a.exec();
}