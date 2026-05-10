#include "resultwindow.h"
#include "ui_resultwindow.h"

ResultWindow::ResultWindow(QWidget *parent) : QDialog(parent), ui(new Ui::ResultWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    connect(ui->okButton, &QPushButton::clicked, this, &ResultWindow::on_okButton_clicked);
}

ResultWindow::~ResultWindow()
{
    delete ui;
}

void ResultWindow::setResult(const QString &title, const QString &message, const QString &teamName)
{
    ui->titleLabel->setText(title);
    ui->messageLabel->setText(message);
    ui->teamLabel->setText(QString("球队: %1").arg(teamName));
}

void ResultWindow::on_okButton_clicked()
{
    emit okClicked();
    close();
}