#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "game.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setTeams(const QString &playerTeam, const QString &computerTeam);
    void setGroup(const QString &group);
    void startGame();

signals:
    void backToStart();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void on_startButton_clicked();
    void on_quitButton_clicked();
    void updateGame();

private:
    Ui::MainWindow *ui;
    Game *game;
    QTimer *gameTimer;
};

#endif // MAINWINDOW_H