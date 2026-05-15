#ifndef MATCHRESULTWINDOW_H
#define MATCHRESULTWINDOW_H

#include <QMainWindow>
#include <QPixmap>

namespace Ui {
class MatchResultWindow;
}

class MatchResultWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MatchResultWindow(QWidget *parent = nullptr);
    ~MatchResultWindow();
    
    void setResult(bool isWin, const QString &matchInfo, int matchIndex = 0);

signals:
    void continueGame();
    void exitStory();

private slots:
    void on_confirmButton_clicked();
    void on_exitButton_clicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::MatchResultWindow *ui;
    QPixmap resultPixmap;
    bool isWin;
    QString matchInfo;
    int matchIndex;
};

#endif // MATCHRESULTWINDOW_H