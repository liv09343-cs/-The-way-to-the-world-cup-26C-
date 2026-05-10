#include "teamselectwindow.h"
#include "ui_teamselectwindow.h"
#include <QPushButton>
#include <QGridLayout>
#include <QFont>
#include <QDir>
#include <QIcon>
#include <QPixmap>
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QCoreApplication>
#include <QDebug>
#include <QMap>

const QStringList TeamSelectWindow::GROUP_LETTERS = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L"};

const QList<QStringList> TeamSelectWindow::TEAMS_BY_GROUP = {
    {"墨西哥", "南非", "韩国", "捷克"},
    {"加拿大", "波黑", "卡塔尔", "瑞士"},
    {"巴西", "摩洛哥", "海地", "苏格兰"},
    {"美国", "巴拉圭", "澳大利亚", "土耳其"},
    {"德国", "库拉索", "科特迪瓦", "厄瓜多尔"},
    {"荷兰", "日本", "瑞典", "突尼斯"},
    {"比利时", "埃及", "伊朗", "新西兰"},
    {"西班牙", "佛得角", "沙特阿拉伯", "乌拉圭"},
    {"法国", "塞内加尔", "伊拉克", "挪威"},
    {"阿根廷", "阿尔及利亚", "奥地利", "约旦"},
    {"葡萄牙", "民主刚果", "乌兹别克斯坦", "哥伦比亚"},
    {"英格兰", "克罗地亚", "加纳", "巴拿马"}
};

const QMap<QString, QString> TeamSelectWindow::CHINESE_TO_ENGLISH = {
    {"墨西哥", "Mexico"}, {"南非", "SouthAfrica"}, {"韩国", "SouthKorea"}, {"捷克", "Czech"},
    {"加拿大", "Canada"}, {"波黑", "Bosnia"}, {"卡塔尔", "Qatar"}, {"瑞士", "Switzerland"},
    {"巴西", "Brazil"}, {"摩洛哥", "Morocco"}, {"海地", "Haiti"}, {"苏格兰", "Scotland"},
    {"美国", "USA"}, {"巴拉圭", "Paraguay"}, {"澳大利亚", "Australia"}, {"土耳其", "Turkey"},
    {"德国", "Germany"}, {"库拉索", "Curacao"}, {"科特迪瓦", "CoteDIvoire"}, {"厄瓜多尔", "Ecuador"},
    {"荷兰", "Netherlands"}, {"日本", "Japan"}, {"瑞典", "Sweden"}, {"突尼斯", "Tunisia"},
    {"比利时", "Belgium"}, {"埃及", "Egypt"}, {"伊朗", "Iran"}, {"新西兰", "NewZealand"},
    {"西班牙", "Spain"}, {"佛得角", "CapeVerde"}, {"沙特阿拉伯", "SaudiArabia"}, {"乌拉圭", "Uruguay"},
    {"法国", "France"}, {"塞内加尔", "Senegal"}, {"伊拉克", "Iraq"}, {"挪威", "Norway"},
    {"阿根廷", "Argentina"}, {"阿尔及利亚", "Algeria"}, {"奥地利", "Austria"}, {"约旦", "Jordan"},
    {"葡萄牙", "Portugal"}, {"民主刚果", "DRCongo"}, {"乌兹别克斯坦", "Uzbekistan"}, {"哥伦比亚", "Colombia"},
    {"英格兰", "England"}, {"克罗地亚", "Croatia"}, {"加纳", "Ghana"}, {"巴拿马", "Panama"}
};

TeamSelectWindow::TeamSelectWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TeamSelectWindow)
{
    ui->setupUi(this);
    setFixedSize(800, 600);
    setStyleSheet("background-color: #006400;");

    QGridLayout *gridLayout = qobject_cast<QGridLayout*>(ui->scrollAreaWidgetContents->layout());
    
    if (!gridLayout) {
        gridLayout = new QGridLayout(ui->scrollAreaWidgetContents);
        ui->scrollAreaWidgetContents->setLayout(gridLayout);
    }
    ui->scrollAreaWidgetContents->setStyleSheet("background-color: #006400;");

    QString flagsPath = "c:/Users/spong/Documents/trae_projects/world-cup/flags/";
    qDebug() << "Flags path:" << flagsPath;
    
    QDir dir(flagsPath);
    qDebug() << "Dir exists:" << dir.exists();
    QStringList files = dir.entryList(QStringList() << "*.png", QDir::Files);
    qDebug() << "PNG files in dir:" << files.count();

    int row = 0;
    for (int i = 0; i < GROUP_LETTERS.size(); i++) {
        QLabel *groupLabel = new QLabel(QString("Group %1").arg(GROUP_LETTERS[i]));
        groupLabel->setFont(QFont("Arial", 14, QFont::Bold));
        groupLabel->setStyleSheet("color: #FFD700;");
        gridLayout->addWidget(groupLabel, row, 0, 1, 4);
        row++;

        for (int j = 0; j < TEAMS_BY_GROUP[i].size(); j++) {
            QString teamName = TEAMS_BY_GROUP[i][j];
            
            QPushButton *teamButton = new QPushButton();
            teamButton->setObjectName(QString("%1_%2").arg(GROUP_LETTERS[i]).arg(teamName));
            
            QString englishName = CHINESE_TO_ENGLISH.value(teamName, teamName);
            QString flagPath = flagsPath + QString("flag_%1.png").arg(englishName);
            
            QFile file(flagPath);
            bool fileExists = file.exists();
            qDebug() << "File exists:" << fileExists << "-" << flagPath;
            
            QPixmap pixmap;
            if (fileExists) {
                pixmap.load(flagPath, "PNG", Qt::AutoColor);
            }
            qDebug() << "Pixmap is null:" << pixmap.isNull();
            
            QHBoxLayout *buttonLayout = new QHBoxLayout();
            buttonLayout->setContentsMargins(6, 4, 12, 4);
            buttonLayout->setSpacing(8);
            
            QLabel *flagLabel = new QLabel();
            flagLabel->setFixedSize(36, 24);
            if (!pixmap.isNull()) {
                flagLabel->setPixmap(pixmap.scaled(36, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                qDebug() << "Flag loaded successfully for:" << teamName;
            } else {
                flagLabel->setStyleSheet("background-color: #333333;");
                qDebug() << "Failed to load flag for:" << teamName;
            }
            buttonLayout->addWidget(flagLabel);
            
            QLabel *textLabel = new QLabel(teamName);
            textLabel->setFont(QFont("SimHei", 11, QFont::Bold));
            textLabel->setStyleSheet("color: white; text-shadow: 1px 1px 2px black;");
            buttonLayout->addWidget(textLabel);
            buttonLayout->addStretch();
            
            QWidget *buttonContent = new QWidget();
            buttonContent->setLayout(buttonLayout);
            teamButton->setLayout(buttonLayout);
            
            teamButton->setStyleSheet(
                "QPushButton {"
                "    border-radius: 5px;"
                "    border: 2px solid #FFD700;"
                "    background-color: #008000;"
                "}"
                "QPushButton:hover {"
                "    background-color: #00A000;"
                "}"
            );
            teamButton->setMinimumSize(140, 45);
            teamButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            connect(teamButton, &QPushButton::clicked, this, &TeamSelectWindow::onTeamButtonClicked);
            gridLayout->addWidget(teamButton, row, j);
        }
        row++;
    }
}

TeamSelectWindow::~TeamSelectWindow()
{
    delete ui;
}

void TeamSelectWindow::onTeamButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString objName = button->objectName();
        QStringList parts = objName.split("_");
        QString group = parts[0];
        QString team = parts.mid(1).join("_");
        emit teamSelected(team, group);
    }
}
