#ifndef TEAMSELECTWINDOW_H
#define TEAMSELECTWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>

QT_BEGIN_NAMESPACE
namespace Ui {
class TeamSelectWindow;
}
QT_END_NAMESPACE

class TeamSelectWindow : public QMainWindow
{
    Q_OBJECT

public:
    struct Team {
        QString name;
        QString group;
    };

    explicit TeamSelectWindow(QWidget *parent = nullptr);
    ~TeamSelectWindow();

    QString getSelectedTeam() const;
    QString getSelectedTeamGroup() const;

signals:
    void teamSelected();
    void backClicked();

private slots:
    void onTeamButtonClicked();
    void on_backButton_clicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::TeamSelectWindow *ui;
    QMap<QString, Team> teams;
    QString selectedTeam;
    QString selectedTeamGroup;

    void createTeamButtons();
    void setupTeams();
};

#endif // TEAMSELECTWINDOW_H