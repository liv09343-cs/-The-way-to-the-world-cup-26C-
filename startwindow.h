#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>

namespace Ui {
class StartWindow;
}

class StartWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

signals:
    void startGame();
    void exitGame();

private slots:
    void on_startButton_clicked();
    void on_quitButton_clicked();

private:
    Ui::StartWindow *ui;
};

#endif // STARTWINDOW_H