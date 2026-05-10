#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "game.h"
#include <QPixmap>
#include <QMap>

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
    Game* getGame() const { return game; }

signals:
    void gameFinished(const QString &winner);

private slots:
    void on_startButton_clicked();
    void on_quitButton_clicked();
    void onGameOver(const QString &winner);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::MainWindow *ui;
    Game *game;
    QTimer *gameTimer;
    
    QPixmap playerFlag;
    QPixmap computerFlag;
    
    static const QMap<QString, QString> CHINESE_TO_ENGLISH;
    
    QPixmap loadFlag(const QString &teamName);
};

#endif // MAINWINDOW_H