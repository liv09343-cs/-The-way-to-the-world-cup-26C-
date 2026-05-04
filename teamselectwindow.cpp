#include "teamselectwindow.h"
#include "ui_teamselectwindow.h"
#include <QPushButton>
#include <QGridLayout>
#include <QFont>

const QStringList TeamSelectWindow::GROUP_LETTERS = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L"};

const QList<QStringList> TeamSelectWindow::TEAMS_BY_GROUP = {
    {"Team1", "Team2", "Team3", "Team4"},
    {"Team5", "Team6", "Team7", "Team8"},
    {"Team9", "Team10", "Team11", "Team12"},
    {"Team13", "Team14", "Team15", "Team16"},
    {"Team17", "Team18", "Team19", "Team20"},
    {"Team21", "Team22", "Team23", "Team24"},
    {"Team25", "Team26", "Team27", "Team28"},
    {"Team29", "Team30", "Team31", "Team32"},
    {"Team33", "Team34", "Team35", "Team36"},
    {"Team37", "Team38", "Team39", "Team40"},
    {"Team41", "Team42", "Team43", "Team44"},
    {"Team45", "Team46", "Team47", "Team48"}
};

TeamSelectWindow::TeamSelectWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TeamSelectWindow)
{
    ui->setupUi(this);
    setFixedSize(800, 600);

    QGridLayout *gridLayout = qobject_cast<QGridLayout*>(ui->scrollAreaWidgetContents->layout());
    
    if (!gridLayout) {
        gridLayout = new QGridLayout(ui->scrollAreaWidgetContents);
        ui->scrollAreaWidgetContents->setLayout(gridLayout);
    }

    int row = 0;
    for (int i = 0; i < GROUP_LETTERS.size(); i++) {
        QLabel *groupLabel = new QLabel(QString("Group %1").arg(GROUP_LETTERS[i]));
        groupLabel->setFont(QFont("Arial", 14, QFont::Bold));
        groupLabel->setStyleSheet("color: #FFD700;");
        gridLayout->addWidget(groupLabel, row, 0, 1, 4);
        row++;

        for (int j = 0; j < TEAMS_BY_GROUP[i].size(); j++) {
            QPushButton *teamButton = new QPushButton(TEAMS_BY_GROUP[i][j]);
            teamButton->setObjectName(QString("%1_%2").arg(GROUP_LETTERS[i]).arg(TEAMS_BY_GROUP[i][j]));
            teamButton->setFont(QFont("Arial", 12));
            teamButton->setStyleSheet("background-color: #FFFFFF; color: #000000; padding: 5px 15px;");
            teamButton->setMinimumSize(80, 35);
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
        QString team = parts[1];
        emit teamSelected(team, group);
    }
}