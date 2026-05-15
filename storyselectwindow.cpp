#include "storyselectwindow.h"
#include "ui_storyselectwindow.h"
#include <QPainter>
#include <QCoreApplication>

StorySelectWindow::StorySelectWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StorySelectWindow)
{
    ui->setupUi(this);
    setFixedSize(800, 600);
    
    ui->centralwidget->setStyleSheet("");
    
    QStringList backgroundPaths = {
        QCoreApplication::applicationDirPath() + "/background_teamselect.png",
        "c:/Users/spong/Documents/trae_projects/world-cup/background_teamselect.png",
        "c:/Users/spong/Desktop/background_teamselect.png"
    };
    
    for (const QString& path : backgroundPaths) {
        if (backgroundPixmap.load(path)) {
            break;
        }
    }
}

StorySelectWindow::~StorySelectWindow()
{
    delete ui;
}

void StorySelectWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    
    if (!backgroundPixmap.isNull()) {
        painter.drawPixmap(0, 0, width(), height(), backgroundPixmap);
    } else {
        painter.setBrush(QColor(34, 139, 34));
        painter.drawRect(0, 0, width(), height());
    }
}

void StorySelectWindow::on_backButton_clicked()
{
    emit backClicked();
}

void StorySelectWindow::on_chinaStoryButton_clicked()
{
    emit storySelected(1);
}