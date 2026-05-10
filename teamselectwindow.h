#ifndef TEAMSELECTWINDOW_H
#define TEAMSELECTWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QMap>

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
    static const QMap<QString, QString> COUNTRY_STYLES;
    static const QMap<QString, QString> COUNTRY_SECOND_COLORS;
    static const QMap<QString, QString> CHINESE_TO_ENGLISH;
};

#endif // TEAMSELECTWINDOW_H