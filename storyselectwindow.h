#ifndef STORYSELECTWINDOW_H
#define STORYSELECTWINDOW_H

#include <QMainWindow>
#include <QPixmap>

namespace Ui {
class StorySelectWindow;
}

class StorySelectWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StorySelectWindow(QWidget *parent = nullptr);
    ~StorySelectWindow();

signals:
    void backClicked();
    void storySelected(int storyId);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void on_backButton_clicked();
    void on_chinaStoryButton_clicked();

private:
    Ui::StorySelectWindow *ui;
    QPixmap backgroundPixmap;
};

#endif // STORYSELECTWINDOW_H