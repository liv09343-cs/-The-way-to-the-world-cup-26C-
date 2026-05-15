#ifndef TEAMSELECTWINDOW_H
#define TEAMSELECTWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QMap>
#include <QPixmap>

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
    void backClicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onTeamButtonClicked();
    void on_backButton_clicked();

private:
    Ui::TeamSelectWindow *ui;
    QPixmap backgroundPixmap;

    static const QStringList GROUP_LETTERS;
    static const QList<QStringList> TEAMS_BY_GROUP;
    static const QMap<QString, QString> COUNTRY_STYLES;
    static const QMap<QString, QString> COUNTRY_SECOND_COLORS;
    static const QMap<QString, QString> CHINESE_TO_ENGLISH;
};

#endif // TEAMSELECTWINDOW_H