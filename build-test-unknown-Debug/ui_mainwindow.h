/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *lineEdit_versions;
    QLabel *label_2;
    QLineEdit *lineEdit_system_load;
    QLabel *label_3;
    QLineEdit *lineEdit_up_time;
    QLabel *label_4;
    QLineEdit *lineEdit_memory;
    QLabel *label_5;
    QLineEdit *lineEdit_ip;
    QLabel *label_6;
    QLineEdit *lineEdit_cpu;
    QLabel *label_7;
    QLineEdit *lineEdit_usage;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(600, 345);
        MainWindow->setMinimumSize(QSize(0, 0));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalSpacer_2 = new QSpacerItem(20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(130, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        lineEdit_versions = new QLineEdit(centralwidget);
        lineEdit_versions->setObjectName(QString::fromUtf8("lineEdit_versions"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit_versions);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        lineEdit_system_load = new QLineEdit(centralwidget);
        lineEdit_system_load->setObjectName(QString::fromUtf8("lineEdit_system_load"));

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEdit_system_load);

        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        lineEdit_up_time = new QLineEdit(centralwidget);
        lineEdit_up_time->setObjectName(QString::fromUtf8("lineEdit_up_time"));

        formLayout->setWidget(2, QFormLayout::FieldRole, lineEdit_up_time);

        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        lineEdit_memory = new QLineEdit(centralwidget);
        lineEdit_memory->setObjectName(QString::fromUtf8("lineEdit_memory"));

        formLayout->setWidget(3, QFormLayout::FieldRole, lineEdit_memory);

        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_5);

        lineEdit_ip = new QLineEdit(centralwidget);
        lineEdit_ip->setObjectName(QString::fromUtf8("lineEdit_ip"));

        formLayout->setWidget(4, QFormLayout::FieldRole, lineEdit_ip);

        label_6 = new QLabel(centralwidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label_6);

        lineEdit_cpu = new QLineEdit(centralwidget);
        lineEdit_cpu->setObjectName(QString::fromUtf8("lineEdit_cpu"));

        formLayout->setWidget(5, QFormLayout::FieldRole, lineEdit_cpu);

        label_7 = new QLabel(centralwidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        formLayout->setWidget(6, QFormLayout::LabelRole, label_7);

        lineEdit_usage = new QLineEdit(centralwidget);
        lineEdit_usage->setObjectName(QString::fromUtf8("lineEdit_usage"));

        formLayout->setWidget(6, QFormLayout::FieldRole, lineEdit_usage);


        horizontalLayout->addLayout(formLayout);


        gridLayout->addLayout(horizontalLayout, 1, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(129, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        verticalSpacer = new QSpacerItem(20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 2, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QApplication::translate("MainWindow", "     welcome to :", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "    system load :", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "           up time :", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "memory usage :", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "                     IP :", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "         cpu temp :", nullptr));
        label_7->setText(QApplication::translate("MainWindow", "         usage of / :", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
