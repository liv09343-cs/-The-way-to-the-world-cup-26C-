/********************************************************************************
** Form generated from reading UI file 'resultwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESULTWINDOW_H
#define UI_RESULTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ResultWindow
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QLabel *teamLabel;
    QLabel *messageLabel;
    QPushButton *okButton;

    void setupUi(QDialog *ResultWindow)
    {
        if (ResultWindow->objectName().isEmpty())
            ResultWindow->setObjectName("ResultWindow");
        ResultWindow->resize(400, 300);
        verticalLayout = new QVBoxLayout(ResultWindow);
        verticalLayout->setObjectName("verticalLayout");
        titleLabel = new QLabel(ResultWindow);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet(QString::fromUtf8("font-size: 24px; font-weight: bold; color: #DAA520;"));

        verticalLayout->addWidget(titleLabel);

        teamLabel = new QLabel(ResultWindow);
        teamLabel->setObjectName("teamLabel");
        teamLabel->setAlignment(Qt::AlignCenter);
        teamLabel->setStyleSheet(QString::fromUtf8("font-size: 18px;"));

        verticalLayout->addWidget(teamLabel);

        messageLabel = new QLabel(ResultWindow);
        messageLabel->setObjectName("messageLabel");
        messageLabel->setAlignment(Qt::AlignCenter);
        messageLabel->setWordWrap(true);
        messageLabel->setStyleSheet(QString::fromUtf8("font-size: 16px; color: #333;"));

        verticalLayout->addWidget(messageLabel);

        okButton = new QPushButton(ResultWindow);
        okButton->setObjectName("okButton");
        okButton->setStyleSheet(QString::fromUtf8("font-size: 16px; padding: 10px 40px;"));

        verticalLayout->addWidget(okButton);


        retranslateUi(ResultWindow);

        QMetaObject::connectSlotsByName(ResultWindow);
    } // setupUi

    void retranslateUi(QDialog *ResultWindow)
    {
        ResultWindow->setWindowTitle(QCoreApplication::translate("ResultWindow", "\346\257\224\350\265\233\347\273\223\346\236\234", nullptr));
        titleLabel->setText(QCoreApplication::translate("ResultWindow", "\346\257\224\350\265\233\347\273\223\346\236\234", nullptr));
        teamLabel->setText(QCoreApplication::translate("ResultWindow", "\347\220\203\351\230\237:", nullptr));
        messageLabel->setText(QCoreApplication::translate("ResultWindow", "\346\266\210\346\201\257", nullptr));
        okButton->setText(QCoreApplication::translate("ResultWindow", "\347\241\256\345\256\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ResultWindow: public Ui_ResultWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESULTWINDOW_H
