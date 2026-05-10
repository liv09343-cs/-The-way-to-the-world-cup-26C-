#include "worldcupmanager.h"
#include <QRandomGenerator>
#include <algorithm>

WorldCupManager::WorldCupManager(QObject *parent) : QObject(parent), playerTeam(nullptr), currentStage(GroupStage)
{
}

void WorldCupManager::initializeTournament(const QString &playerTeamName)
{
    this->playerTeamName = playerTeamName;
    createGroups();
    currentStage = GroupStage;
}

void WorldCupManager::createGroups()
{
    QStringList teamNames;
    for (int i = 1; i <= 48; i++) {
        teamNames << QString("Team%1").arg(i);
    }

    QStringList groupsList = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L"};

    int teamIndex = 0;
    for (const QString &groupName : groupsList) {
        QList<Team> groupTeams;
        for (int i = 0; i < 4; i++) {
            Team team(teamNames[teamIndex], groupName);
            if (team.name == playerTeamName) {
                team.isPlayerTeam = true;
                playerTeam = &team;
            }
            groupTeams << team;
            teamIndex++;
        }
        groups[groupName] = groupTeams;
    }
}

void WorldCupManager::simulateGroupStage()
{
    QString playerGroup;
    for (auto it = groups.begin(); it != groups.end(); ++it) {
        QList<Team> &groupTeams = it.value();
        for (int i = 0; i < groupTeams.size(); i++) {
            for (int j = i + 1; j < groupTeams.size(); j++) {
                Team &team1 = groupTeams[i];
                Team &team2 = groupTeams[j];

                if (team1.isPlayerTeam || team2.isPlayerTeam) {
                    playerGroup = team1.group;
                    continue;
                }

                simulateMatch(team1, team2);
            }
        }
    }

    sortGroups();
    emit groupStageComplete();
}

void WorldCupManager::simulateMatch(Team &team1, Team &team2)
{
    int goals1 = QRandomGenerator::global()->bounded(5);
    int goals2 = QRandomGenerator::global()->bounded(5);

    team1.played++;
    team2.played++;
    team1.goalsScored += goals1;
    team1.goalsConceded += goals2;
    team2.goalsScored += goals2;
    team2.goalsConceded += goals1;

    if (goals1 > goals2) {
        team1.points += 3;
    } else if (goals2 > goals1) {
        team2.points += 3;
    } else {
        team1.points += 1;
        team2.points += 1;
    }

    emit matchResult({team1.name, team2.name, goals1, goals2});
}

void WorldCupManager::simulatePlayerMatch(Team &player, Team &opponent)
{
    int goalsPlayer = QRandomGenerator::global()->bounded(5);
    int goalsOpponent = QRandomGenerator::global()->bounded(5);

    player.played++;
    opponent.played++;
    player.goalsScored += goalsPlayer;
    player.goalsConceded += goalsOpponent;
    opponent.goalsScored += goalsOpponent;
    opponent.goalsConceded += goalsPlayer;

    if (goalsPlayer > goalsOpponent) {
        player.points += 3;
    } else if (goalsOpponent > goalsPlayer) {
        opponent.points += 3;
    } else {
        player.points += 1;
        opponent.points += 1;
    }

    emit matchResult({player.name, opponent.name, goalsPlayer, goalsOpponent});
}

void WorldCupManager::sortGroups()
{
    for (auto it = groups.begin(); it != groups.end(); ++it) {
        QList<Team> &groupTeams = it.value();
        std::sort(groupTeams.begin(), groupTeams.end(), [](const Team &a, const Team &b) {
            if (a.points != b.points) return a.points > b.points;
            int goalDiffA = a.goalsScored - a.goalsConceded;
            int goalDiffB = b.goalsScored - b.goalsConceded;
            if (goalDiffA != goalDiffB) return goalDiffA > goalDiffB;
            return a.goalsScored > b.goalsScored;
        });
    }
}

QList<Team> WorldCupManager::getQualifiedTeams()
{
    QList<Team> qualified;

    for (auto it = groups.begin(); it != groups.end(); ++it) {
        QList<Team> groupTeams = it.value();
        qualified << groupTeams[0];
        qualified << groupTeams[1];
    }

    QList<Team> thirdPlaceTeams;
    for (auto it = groups.begin(); it != groups.end(); ++it) {
        QList<Team> groupTeams = it.value();
        if (groupTeams.size() > 2) {
            thirdPlaceTeams << groupTeams[2];
        }
    }

    std::sort(thirdPlaceTeams.begin(), thirdPlaceTeams.end(), [](const Team &a, const Team &b) {
        if (a.points != b.points) return a.points > b.points;
        int goalDiffA = a.goalsScored - a.goalsConceded;
        int goalDiffB = b.goalsScored - b.goalsConceded;
        if (goalDiffA != goalDiffB) return goalDiffA > goalDiffB;
        return a.goalsScored > b.goalsScored;
    });

    for (int i = 0; i < 8 && i < thirdPlaceTeams.size(); i++) {
        qualified << thirdPlaceTeams[i];
    }

    return qualified;
}

void WorldCupManager::advanceToKnockoutStage()
{
    knockoutTeams = getQualifiedTeams();

    bool playerQualified = false;
    for (const Team &team : knockoutTeams) {
        if (team.name == playerTeamName) {
            playerQualified = true;
            break;
        }
    }

    if (!playerQualified) {
        playerResult = "小组出局";
        emit playerEliminated("很遗憾，您的球队未能晋级淘汰赛！");
        currentStage = TournamentEnded;
        return;
    }

    currentStage = RoundOf32;
    knockoutDraw();
    emit stageAdvanced(currentStage);
}

void WorldCupManager::knockoutDraw()
{
    std::random_shuffle(knockoutTeams.begin(), knockoutTeams.end());
}

void WorldCupManager::simulateNextRound()
{
    QList<Team> winners;

    for (int i = 0; i < knockoutTeams.size(); i += 2) {
        if (i + 1 < knockoutTeams.size()) {
            Team team1 = knockoutTeams[i];
            Team team2 = knockoutTeams[i + 1];

            int goals1 = QRandomGenerator::global()->bounded(4);
            int goals2 = QRandomGenerator::global()->bounded(4);

            while (goals1 == goals2) {
                goals1 = QRandomGenerator::global()->bounded(4);
                goals2 = QRandomGenerator::global()->bounded(4);
            }

            emit matchResult({team1.name, team2.name, goals1, goals2});

            if (goals1 > goals2) {
                winners << team1;
            } else {
                winners << team2;
            }
        }
    }

    knockoutTeams = winners;

    bool playerStillIn = false;
    for (const Team &team : knockoutTeams) {
        if (team.name == playerTeamName) {
            playerStillIn = true;
            break;
        }
    }

    if (!playerStillIn && currentStage != ThirdPlaceMatch) {
        if (currentStage == Semifinals) {
            playerResult = "第四名";
            emit playerEliminated("您在半决赛中失利，获得第四名！");
        } else {
            QString stageName;
            switch (currentStage) {
            case RoundOf32: stageName = "32强"; break;
            case RoundOf16: stageName = "16强"; break;
            case Quarterfinals: stageName = "8强"; break;
            default: stageName = "淘汰赛";
            }
            playerResult = QString("止步%1").arg(stageName);
            emit playerEliminated(QString("很遗憾，您的球队止步%1！").arg(stageName));
        }
        currentStage = TournamentEnded;
        return;
    }

    switch (currentStage) {
    case RoundOf32:
        currentStage = RoundOf16;
        playerResult = "16强";
        break;
    case RoundOf16:
        currentStage = Quarterfinals;
        playerResult = "8强";
        break;
    case Quarterfinals:
        currentStage = Semifinals;
        playerResult = "半决赛";
        break;
    case Semifinals:
        if (knockoutTeams.size() == 2) {
            currentStage = Final;
        } else {
            currentStage = ThirdPlaceMatch;
        }
        break;
    case ThirdPlaceMatch:
        currentStage = Final;
        break;
    case Final:
        tournamentWinner = knockoutTeams[0].name;
        if (knockoutTeams[0].name == playerTeamName) {
            playerResult = "冠军";
        } else {
            playerResult = "亚军";
        }
        currentStage = TournamentEnded;
        emit tournamentComplete(tournamentWinner, playerResult);
        return;
    default:
        break;
    }

    emit stageAdvanced(currentStage);
}

QString WorldCupManager::getPlayerTeam() const
{
    return playerTeamName;
}

WorldCupManager::TournamentStage WorldCupManager::getCurrentStage() const
{
    return currentStage;
}

QList<Team> WorldCupManager::getGroupStandings(const QString &group) const
{
    return groups.value(group);
}

QList<Team> WorldCupManager::getAllTeams() const
{
    QList<Team> allTeams;
    for (auto it = groups.begin(); it != groups.end(); ++it) {
        allTeams << it.value();
    }
    return allTeams;
}

bool WorldCupManager::hasPlayerTeamAdvanced() const
{
    if (!playerTeam) return false;
    QString groupName = playerTeam->group;
    QList<Team> standings = groups.value(groupName);
    if (standings.isEmpty()) return false;

    for (int i = 0; i < qMin(2, standings.size()); i++) {
        if (standings[i].name == playerTeamName) return true;
    }

    QList<Team> thirdPlaceTeams;
    for (auto it = groups.begin(); it != groups.end(); ++it) {
        QList<Team> groupTeams = it.value();
        if (groupTeams.size() > 2) {
            thirdPlaceTeams << groupTeams[2];
        }
    }

    std::sort(thirdPlaceTeams.begin(), thirdPlaceTeams.end(), [](const Team &a, const Team &b) {
        if (a.points != b.points) return a.points > b.points;
        int goalDiffA = a.goalsScored - a.goalsConceded;
        int goalDiffB = b.goalsScored - b.goalsConceded;
        if (goalDiffA != goalDiffB) return goalDiffA > goalDiffB;
        return a.goalsScored > b.goalsScored;
    });

    for (int i = 0; i < 8 && i < thirdPlaceTeams.size(); i++) {
        if (thirdPlaceTeams[i].name == playerTeamName) return true;
    }

    return false;
}

QString WorldCupManager::getPlayerTeamResult() const
{
    return playerResult;
}

QString WorldCupManager::getChampion() const
{
    return tournamentWinner;
}