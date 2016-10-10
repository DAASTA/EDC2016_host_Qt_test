/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QLabel *labelCapture;
    QLabel *labelStatus;
    QVBoxLayout *verticalLayout;
    QLabel *labelSubCapture;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(988, 525);
        horizontalLayout = new QHBoxLayout(Widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        labelCapture = new QLabel(Widget);
        labelCapture->setObjectName(QStringLiteral("labelCapture"));
        labelCapture->setMinimumSize(QSize(640, 480));
        labelCapture->setMaximumSize(QSize(640, 480));

        verticalLayout_2->addWidget(labelCapture);

        labelStatus = new QLabel(Widget);
        labelStatus->setObjectName(QStringLiteral("labelStatus"));

        verticalLayout_2->addWidget(labelStatus);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        labelSubCapture = new QLabel(Widget);
        labelSubCapture->setObjectName(QStringLiteral("labelSubCapture"));
        labelSubCapture->setMinimumSize(QSize(320, 240));
        labelSubCapture->setMaximumSize(QSize(320, 240));

        verticalLayout->addWidget(labelSubCapture);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", 0));
        labelCapture->setText(QApplication::translate("Widget", "labelCapture", 0));
        labelStatus->setText(QApplication::translate("Widget", "Status", 0));
        labelSubCapture->setText(QApplication::translate("Widget", "labelSubCapture", 0));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
