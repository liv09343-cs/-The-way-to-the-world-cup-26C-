#include "startwindow.h"
#include "ui_startwindow.h"
#include <QPainter>

StartWindow::StartWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    setFixedSize(800, 600);
    
    // 手动连接信号和槽
    connect(ui->startButton, &QPushButton::clicked, this, &StartWindow::on_startButton_clicked);
    connect(ui->exitButton, &QPushButton::clicked, this, &StartWindow::on_exitButton_clicked);
}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    // 绘制世界杯赛场背景
    painter.setBrush(QColor(34, 139, 34));
    painter.drawRect(0, 0, width(), height());

    // 绘制世界杯标志
    painter.setPen(QColor(255, 215, 0));
    painter.setFont(QFont("Arial", 24, QFont::Bold));
    painter.drawText(30, 50, "FIFA WORLD CUP 2026");
}

void StartWindow::on_startButton_clicked()
{
    emit startGameClicked();
}

void StartWindow::on_exitButton_clicked()
{
    emit exitClicked();
}