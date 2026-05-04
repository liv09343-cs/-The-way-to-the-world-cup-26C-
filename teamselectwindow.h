#ifndef TEAMSELECTWINDOW_H
#define TEAMSELECTWINDOW_H

#include <QMainWindow>
#include <QGridLayout>

namespace Ui {
class TeamSelectWindow;
}

class TeamSelectWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TeamSelectWindow(QWidget *parent = nullptr);
    ~TeamSelectWindow();

signals:
    void teamSelected(const QString &team, const QString &group);

private slots:
    void onTeamButtonClicked();

private:
    Ui::TeamSelectWindow *ui;

    static const QStringList GROUP_LETTERS;
    static const QList<QStringList> TEAMS_BY_GROUP;
};

#endif // TEAMSELECTWINDOW_H