/********************************************************************************
** Form generated from reading UI file 'startwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STARTWINDOW_H
#define UI_STARTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StartWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_2;
    QLabel *titleLabel;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_2;
    QPushButton *startButton;
    QPushButton *storyButton;
    QPushButton *quitButton;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_3;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *StartWindow)
    {
        if (StartWindow->objectName().isEmpty())
            StartWindow->setObjectName("StartWindow");
        StartWindow->resize(800, 600);
        centralwidget = new QWidget(StartWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setStyleSheet(QString::fromUtf8("background-color: #228B22;"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalSpacer_2 = new QSpacerItem(20, 100, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        titleLabel = new QLabel(centralwidget);
        titleLabel->setObjectName("titleLabel");
        QFont font;
        font.setPointSize(32);
        font.setBold(true);
        titleLabel->setFont(font);
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet(QString::fromUtf8("color: #FFD700;"));

        verticalLayout->addWidget(titleLabel);

        verticalSpacer = new QSpacerItem(20, 150, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        startButton = new QPushButton(centralwidget);
        startButton->setObjectName("startButton");
        QFont font1;
        font1.setPointSize(20);
        startButton->setFont(font1);
        startButton->setMinimumSize(QSize(200, 50));
        startButton->setMaximumSize(QSize(200, 50));
        startButton->setStyleSheet(QString::fromUtf8("background-color: #FFFFFF; color: #000000;"));

        verticalLayout_2->addWidget(startButton);

        storyButton = new QPushButton(centralwidget);
        storyButton->setObjectName("storyButton");
        storyButton->setFont(font1);
        storyButton->setMinimumSize(QSize(200, 50));
        storyButton->setMaximumSize(QSize(200, 50));
        storyButton->setStyleSheet(QString::fromUtf8("background-color: #FFD700; color: #000000;"));

        verticalLayout_2->addWidget(storyButton);

        quitButton = new QPushButton(centralwidget);
        quitButton->setObjectName("quitButton");
        quitButton->setFont(font1);
        quitButton->setMinimumSize(QSize(200, 50));
        quitButton->setMaximumSize(QSize(200, 50));
        quitButton->setStyleSheet(QString::fromUtf8("background-color: #FFFFFF; color: #000000;"));

        verticalLayout_2->addWidget(quitButton);


        horizontalLayout->addLayout(verticalLayout_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer_3 = new QSpacerItem(20, 100, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        StartWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(StartWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 26));
        StartWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(StartWindow);
        statusbar->setObjectName("statusbar");
        StartWindow->setStatusBar(statusbar);

        retranslateUi(StartWindow);

        QMetaObject::connectSlotsByName(StartWindow);
    } // setupUi

    void retranslateUi(QMainWindow *StartWindow)
    {
        StartWindow->setWindowTitle(QCoreApplication::translate("StartWindow", "2026\345\271\264\347\276\216\345\212\240\345\242\250\344\270\226\347\225\214\346\235\257\344\271\213\350\267\257", nullptr));
        titleLabel->setText(QCoreApplication::translate("StartWindow", "2026\345\271\264\347\276\216\345\212\240\345\242\250\344\270\226\347\225\214\346\235\257\344\271\213\350\267\257", nullptr));
        startButton->setText(QCoreApplication::translate("StartWindow", "\345\274\200\345\247\213\346\270\270\346\210\217", nullptr));
        storyButton->setText(QCoreApplication::translate("StartWindow", "剧情模式", nullptr));
        quitButton->setText(QCoreApplication::translate("StartWindow", "\351\200\200\345\207\272\346\270\270\346\210\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StartWindow: public Ui_StartWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARTWINDOW_H
