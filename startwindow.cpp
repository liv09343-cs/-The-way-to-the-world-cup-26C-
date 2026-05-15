#include "startwindow.h"
#include "ui_startwindow.h"
#include <QPainter>
#include <QCoreApplication>

StartWindow::StartWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    setFixedSize(800, 600);
    
    ui->centralwidget->setStyleSheet("");
    
    QStringList backgroundPaths = {
        QCoreApplication::applicationDirPath() + "/background_start.png",
        "c:/Users/spong/Documents/trae_projects/world-cup/background_start.png",
        "c:/Users/spong/Desktop/background_start.png"
    };
    
    for (const QString& path : backgroundPaths) {
        if (backgroundPixmap.load(path)) {
            break;
        }
    }
}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    if (!backgroundPixmap.isNull()) {
        painter.drawPixmap(0, 0, width(), height(), backgroundPixmap);
    } else {
        painter.setBrush(QColor(34, 139, 34));
        painter.drawRect(0, 0, width(), height());
    }

    painter.setPen(QColor(255, 215, 0));
    painter.setFont(QFont("Arial", 24, QFont::Bold));
    painter.drawText(30, 50, "FIFA WORLD CUP 2026");
}

void StartWindow::on_startButton_clicked()
{
    emit startGame();
}

void StartWindow::on_quitButton_clicked()
{
    emit exitGame();
}

void StartWindow::on_storyButton_clicked()
{
    emit startStory();
}