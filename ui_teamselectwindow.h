/********************************************************************************
** Form generated from reading UI file 'teamselectwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEAMSELECTWINDOW_H
#define UI_TEAMSELECTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TeamSelectWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *TeamSelectWindow)
    {
        if (TeamSelectWindow->objectName().isEmpty())
            TeamSelectWindow->setObjectName("TeamSelectWindow");
        TeamSelectWindow->resize(800, 600);
        centralwidget = new QWidget(TeamSelectWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setStyleSheet(QString::fromUtf8("background-color: #228B22;"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        titleLabel = new QLabel(centralwidget);
        titleLabel->setObjectName("titleLabel");
        QFont font;
        font.setPointSize(24);
        font.setBold(true);
        titleLabel->setFont(font);
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet(QString::fromUtf8("color: #FFD700;"));

        verticalLayout->addWidget(titleLabel);

        scrollArea = new QScrollArea(centralwidget);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 780, 500));
        scrollAreaWidgetContents->setStyleSheet(QString::fromUtf8("background-color: #228B22;"));
        gridLayout = new QGridLayout(scrollAreaWidgetContents);
        gridLayout->setObjectName("gridLayout");
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

        TeamSelectWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(TeamSelectWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 26));
        TeamSelectWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(TeamSelectWindow);
        statusbar->setObjectName("statusbar");
        TeamSelectWindow->setStatusBar(statusbar);

        retranslateUi(TeamSelectWindow);

        QMetaObject::connectSlotsByName(TeamSelectWindow);
    } // setupUi

    void retranslateUi(QMainWindow *TeamSelectWindow)
    {
        TeamSelectWindow->setWindowTitle(QCoreApplication::translate("TeamSelectWindow", "\351\200\211\346\213\251\347\220\203\351\230\237", nullptr));
        titleLabel->setText(QCoreApplication::translate("TeamSelectWindow", "\351\200\211\346\213\251\346\202\250\347\232\204\347\220\203\351\230\237", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TeamSelectWindow: public Ui_TeamSelectWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEAMSELECTWINDOW_H
