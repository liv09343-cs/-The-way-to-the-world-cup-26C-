#ifndef WORLDCUPMANAGER_H
#define WORLDCUPMANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>

struct Team {
    QString name;
    QString group;
    int points;
    int goalsScored;
    int goalsConceded;
    int played;
    bool isPlayerTeam;

    Team() : points(0), goalsScored(0), goalsConceded(0), played(0), isPlayerTeam(false) {}
    Team(const QString &n, const QString &g) : name(n), group(g), points(0), goalsScored(0), goalsConceded(0), played(0), isPlayerTeam(false) {}
};

struct MatchResult {
    QString homeTeam;
    QString awayTeam;
    int homeScore;
    int awayScore;
};

class WorldCupManager : public QObject
{
    Q_OBJECT

public:
    enum TournamentStage {
        GroupStage,
        RoundOf32,
        RoundOf16,
        Quarterfinals,
        Semifinals,
        ThirdPlaceMatch,
        Final,
        TournamentEnded
    };

    explicit WorldCupManager(QObject *parent = nullptr);

    void initializeTournament(const QString &playerTeamName);
    QString getPlayerTeam() const;
    TournamentStage getCurrentStage() const;
    QList<Team> getGroupStandings(const QString &group) const;
    QList<Team> getAllTeams() const;

    bool hasPlayerTeamAdvanced() const;
    QString getPlayerTeamResult() const;
    QString getChampion() const;

public slots:
    void simulateGroupStage();
    void advanceToKnockoutStage();
    void simulateNextRound();

signals:
    void groupStageComplete();
    void playerEliminated(const QString &reason);
    void stageAdvanced(TournamentStage stage);
    void matchResult(const MatchResult &result);
    void tournamentComplete(const QString &winner, const QString &playerResult);

private:
    void createGroups();
    void simulateMatch(Team &team1, Team &team2);
    void simulatePlayerMatch(Team &playerTeam, Team &opponent);
    void sortGroups();
    QList<Team> getQualifiedTeams();
    void knockoutDraw();
    void simulateKnockoutMatch(Team &team1, Team &team2);

    QMap<QString, QList<Team>> groups;
    QString playerTeamName;
    Team *playerTeam;
    TournamentStage currentStage;
    QList<Team> knockoutTeams;
    QString tournamentWinner;
    QString playerResult;
};

#endif // WORLDCUPMANAGER_H