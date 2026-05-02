#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class StartWindow;
}
QT_END_NAMESPACE

class StartWindow : public QMainWindow
{
    Q_OBJECT

public:
    StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

signals:
    void startGameClicked();
    void exitClicked();

private slots:
    void on_startButton_clicked();
    void on_exitButton_clicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::StartWindow *ui;
};

#endif // STARTWINDOW_H