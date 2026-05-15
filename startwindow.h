#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include <QPixmap>

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
    void startStory();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void on_startButton_clicked();
    void on_quitButton_clicked();
    void on_storyButton_clicked();

private:
    Ui::StartWindow *ui;
    QPixmap backgroundPixmap;
};

#endif // STARTWINDOW_H