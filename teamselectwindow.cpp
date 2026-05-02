#include "teamselectwindow.h"
#include "ui_teamselectwindow.h"
#include <QPainter>

TeamSelectWindow::TeamSelectWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::TeamSelectWindow)
{
    ui->setupUi(this);
    setFixedSize(800, 600);
    setupTeams();
    createTeamButtons();
    
    // 手动连接信号和槽
    connect(ui->backButton, &QPushButton::clicked, this, &TeamSelectWindow::on_backButton_clicked);
}

TeamSelectWindow::~TeamSelectWindow()
{
    delete ui;
}

void TeamSelectWindow::setupTeams()
{
    // 分组设置：A组到L组，每组4支队伍
    QString groups[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L"};
    
    // 队伍列表
    QString teamNames[] = {
        "Team1", "Team2", "Team3", "Team4",  // A组
        "Team5", "Team6", "Team7", "Team8",  // B组
        "Team9", "Team10", "Team11", "Team12", // C组
        "Team13", "Team14", "Team15", "Team16", // D组
        "Team17", "Team18", "Team19", "Team20", // E组
        "Team21", "Team22", "Team23", "Team24", // F组
        "Team25", "Team26", "Team27", "Team28", // G组
        "Team29", "Team30", "Team31", "Team32", // H组
        "Team33", "Team34", "Team35", "Team36", // I组
        "Team37", "Team38", "Team39", "Team40", // J组
        "Team41", "Team42", "Team43", "Team44", // K组
        "Team45", "Team46", "Team47", "Team48"  // L组
    };

    // 填充队伍数据
    int teamIndex = 0;
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 4; j++) {
            Team team;
            team.name = teamNames[teamIndex];
            team.group = groups[i];
            teams[team.name] = team;
            teamIndex++;
        }
    }
}

void TeamSelectWindow::createTeamButtons()
{
    // 创建主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(ui->teamsWidget);

    // 分组显示队伍
    QString groups[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L"};

    for (int i = 0; i < 12; i++) {
        // 创建分组标题
        QLabel *groupLabel = new QLabel(QString("Group %1").arg(groups[i]));
        groupLabel->setFont(QFont("Arial", 16, QFont::Bold));
        groupLabel->setStyleSheet("color: #ffffff;");
        mainLayout->addWidget(groupLabel);

        // 创建分组队伍按钮
        QHBoxLayout *groupLayout = new QHBoxLayout();
        
        // 查找该组的队伍
        QList<Team> groupTeams;
        for (auto it = teams.begin(); it != teams.end(); ++it) {
            if (it.value().group == groups[i]) {
                groupTeams.append(it.value());
            }
        }

        // 创建队伍按钮
        for (const Team &team : groupTeams) {
            QPushButton *teamButton = new QPushButton(team.name);
            teamButton->setFixedSize(150, 50);
            teamButton->setFont(QFont("Arial", 12));
            teamButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; border-radius: 5px; } QPushButton:hover { background-color: #45a049; }");
            groupLayout->addWidget(teamButton);

            // 连接按钮点击事件
            connect(teamButton, &QPushButton::clicked, this, &TeamSelectWindow::onTeamButtonClicked);
        }

        mainLayout->addLayout(groupLayout);
        mainLayout->addSpacing(20);
    }

    ui->teamsWidget->setLayout(mainLayout);
}

void TeamSelectWindow::onTeamButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        selectedTeam = button->text();
        selectedTeamGroup = teams[selectedTeam].group;
        emit teamSelected();
    }
}

void TeamSelectWindow::on_backButton_clicked()
{
    emit backClicked();
}

QString TeamSelectWindow::getSelectedTeam() const
{
    return selectedTeam;
}

QString TeamSelectWindow::getSelectedTeamGroup() const
{
    return selectedTeamGroup;
}

void TeamSelectWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    // 绘制背景
    painter.setBrush(QColor(34, 139, 34));
    painter.drawRect(0, 0, width(), height());
}
