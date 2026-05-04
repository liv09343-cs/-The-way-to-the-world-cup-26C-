#include "startwindow.h"
#include "ui_startwindow.h"

StartWindow::StartWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    setFixedSize(800, 600);
}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::on_startButton_clicked()
{
    emit startGame();
}

void StartWindow::on_quitButton_clicked()
{
    emit exitGame();
}