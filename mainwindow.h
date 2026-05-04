#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "game.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setTeams(const QString &playerTeam, const QString &computerTeam);
    void setGroup(const QString &group);
    void startGame();

private slots:
    void on_startButton_clicked();
    void on_quitButton_clicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::MainWindow *ui;
    Game *game;
    QTimer *gameTimer;
};

#endif // MAINWINDOW_H