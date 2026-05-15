#include "matchresultwindow.h"
#include "ui_matchresultwindow.h"
#include <QPainter>
#include <QCoreApplication>

MatchResultWindow::MatchResultWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MatchResultWindow),
    isWin(false),
    matchIndex(0)
{
    ui->setupUi(this);
    setFixedSize(800, 600);
}

MatchResultWindow::~MatchResultWindow()
{
    delete ui;
}

void MatchResultWindow::setResult(bool win, const QString &info, int index)
{
    isWin = win;
    matchInfo = info;
    matchIndex = index;
    
    QString specificFileName = isWin ? QString("win%1.png").arg(index) : QString("lose%1.png").arg(index);
    QString generalFileName = isWin ? "win.png" : "lose.png";
    
    QStringList paths = {
        QCoreApplication::applicationDirPath() + "\\jvqing\\" + specificFileName,
        "c:\\Users\\spong\\Documents\\trae_projects\\world-cup\\jvqing\\" + specificFileName,
        QCoreApplication::applicationDirPath() + "\\jvqing\\" + generalFileName,
        "c:\\Users\\spong\\Documents\\trae_projects\\world-cup\\jvqing\\" + generalFileName,
        QCoreApplication::applicationDirPath() + "\\" + specificFileName,
        "c:\\Users\\spong\\Documents\\trae_projects\\world-cup\\" + specificFileName,
        "c:\\Users\\spong\\Desktop\\" + specificFileName,
        QCoreApplication::applicationDirPath() + "\\" + generalFileName,
        "c:\\Users\\spong\\Documents\\trae_projects\\world-cup\\" + generalFileName,
        "c:\\Users\\spong\\Desktop\\" + generalFileName
    };
    
    resultPixmap = QPixmap();
    for (const QString& path : paths) {
        if (resultPixmap.load(path)) {
            break;
        }
    }
    
    update();
}

void MatchResultWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    
    if (!resultPixmap.isNull()) {
        painter.drawPixmap(0, 0, width(), height(), resultPixmap);
    } else {
        if (isWin) {
            painter.setBrush(QColor(0, 100, 0));
        } else {
            painter.setBrush(QColor(100, 0, 0));
        }
        painter.drawRect(0, 0, width(), height());
        
        painter.setPen(QColor(255, 255, 255));
        painter.setFont(QFont("Arial", 32, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, isWin ? "胜利！" : "失败...");
        
        painter.setFont(QFont("Arial", 18));
        painter.drawText(50, 100, 700, 100, Qt::AlignCenter, matchInfo);
    }
}

void MatchResultWindow::on_confirmButton_clicked()
{
    emit continueGame();
}

void MatchResultWindow::on_exitButton_clicked()
{
    emit exitStory();
}