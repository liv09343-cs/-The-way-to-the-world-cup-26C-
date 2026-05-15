#include "storyscenewindow.h"
#include "ui_storyscenewindow.h"
#include <QPainter>
#include <QCoreApplication>
#include <QDebug>

StorySceneWindow::StorySceneWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StorySceneWindow),
    currentScene(0)
{
    ui->setupUi(this);
    setFixedSize(800, 600);
}

StorySceneWindow::~StorySceneWindow()
{
    delete ui;
}

void StorySceneWindow::setScene(int sceneNum)
{
    currentScene = sceneNum;
    
    QStringList paths = {
        QCoreApplication::applicationDirPath() + "\\jvqing\\" + QString("story%1.png").arg(sceneNum),
        "c:\\Users\\spong\\Documents\\trae_projects\\world-cup\\jvqing\\" + QString("story%1.png").arg(sceneNum),
        QCoreApplication::applicationDirPath() + "\\" + QString("story%1.png").arg(sceneNum),
        "c:\\Users\\spong\\Documents\\trae_projects\\world-cup\\" + QString("story%1.png").arg(sceneNum),
        "c:\\Users\\spong\\Desktop\\" + QString("story%1.png").arg(sceneNum)
    };
    
    qDebug() << "=== 尝试加载剧情图片 ===";
    qDebug() << "当前场景:" << sceneNum;
    qDebug() << "应用程序目录:" << QCoreApplication::applicationDirPath();
    
    scenePixmap = QPixmap();
    for (const QString& path : paths) {
        qDebug() << "尝试路径:" << path;
        if (scenePixmap.load(path)) {
            qDebug() << "成功加载!";
            break;
        } else {
            qDebug() << "加载失败";
        }
    }
    
    qDebug() << "图片是否为空:" << scenePixmap.isNull();
    update();
}

void StorySceneWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    
    if (!scenePixmap.isNull()) {
        painter.drawPixmap(0, 0, width(), height(), scenePixmap);
    } else {
        painter.setBrush(QColor(0, 0, 0));
        painter.drawRect(0, 0, width(), height());
        
        painter.setPen(QColor(255, 255, 255));
        painter.setFont(QFont("Arial", 18));
        
        QString storyText;
        switch(currentScene) {
        case 1:
            storyText = "2026年，中国国家队历史性地晋级世界杯！\n\n全国人民为之沸腾，期待着这支年轻的队伍\n能创造奇迹...";
            break;
        case 2:
            storyText = "小组赛首场，中国队对阵比利时！\n\n这是一场艰难的比赛，但队员们斗志昂扬，\n决心要拿下开门红！";
            break;
        case 3:
            storyText = "中场休息，主教练鼓励队员们坚持战术，\n相信自己的实力。\n\n当前比分依然胶着，下半场将决定命运！";
            break;
        case 4:
            storyText = "最终，中国队凭借顽强的意志\n赢得了这场比赛！\n\n全国上下一片欢腾，出线形势一片大好！";
            break;
        default:
            storyText = "剧情画面";
        }
        
        painter.drawText(50, 100, 700, 400, Qt::AlignCenter | Qt::TextWordWrap, storyText);
    }
}

void StorySceneWindow::on_nextButton_clicked()
{
    if (currentScene >= 4) {
        emit startMatch("中国队", "比利时");
    } else {
        emit nextScene();
    }
}