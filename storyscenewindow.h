#ifndef STORYSCENEWINDOW_H
#define STORYSCENEWINDOW_H

#include <QMainWindow>
#include <QPixmap>

namespace Ui {
class StorySceneWindow;
}

class StorySceneWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StorySceneWindow(QWidget *parent = nullptr);
    ~StorySceneWindow();
    
    void setScene(int sceneNum);

signals:
    void nextScene();
    void startMatch(const QString &playerTeam, const QString &computerTeam);
    void exitStory();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void on_nextButton_clicked();

private:
    Ui::StorySceneWindow *ui;
    QPixmap scenePixmap;
    int currentScene;
};

#endif // STORYSCENEWINDOW_H