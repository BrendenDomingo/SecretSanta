/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QListWidget *participant_list;
    QPushButton *add_participant_button;
    QPushButton *clear_participant_button;
    QPushButton *secret_santa_button;
    QGroupBox *groupBox;
    QLabel *name_label;
    QLineEdit *name_edit;
    QLabel *address_label;
    QTextEdit *Adderss_edit;
    QLabel *ideas_label;
    QTextEdit *ideas_edit;
    QListWidget *exclude_list;
    QPushButton *add_exclude_button;
    QPushButton *clear_exclude_button;
    QLabel *exclude_label;
    QComboBox *exclude_comboBox;
    QPushButton *browse_exclude_Button;
    QLabel *participant_list_label;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuHelp;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(756, 561);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        participant_list = new QListWidget(centralwidget);
        participant_list->setObjectName("participant_list");
        participant_list->setGeometry(QRect(10, 30, 261, 301));
        add_participant_button = new QPushButton(centralwidget);
        add_participant_button->setObjectName("add_participant_button");
        add_participant_button->setGeometry(QRect(10, 340, 80, 24));
        clear_participant_button = new QPushButton(centralwidget);
        clear_participant_button->setObjectName("clear_participant_button");
        clear_participant_button->setGeometry(QRect(180, 340, 91, 24));
        secret_santa_button = new QPushButton(centralwidget);
        secret_santa_button->setObjectName("secret_santa_button");
        secret_santa_button->setGeometry(QRect(50, 430, 171, 81));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(280, 10, 451, 501));
        name_label = new QLabel(groupBox);
        name_label->setObjectName("name_label");
        name_label->setGeometry(QRect(10, 40, 49, 20));
        name_edit = new QLineEdit(groupBox);
        name_edit->setObjectName("name_edit");
        name_edit->setGeometry(QRect(60, 40, 151, 24));
        address_label = new QLabel(groupBox);
        address_label->setObjectName("address_label");
        address_label->setGeometry(QRect(10, 80, 49, 16));
        Adderss_edit = new QTextEdit(groupBox);
        Adderss_edit->setObjectName("Adderss_edit");
        Adderss_edit->setGeometry(QRect(60, 80, 191, 70));
        ideas_label = new QLabel(groupBox);
        ideas_label->setObjectName("ideas_label");
        ideas_label->setGeometry(QRect(10, 160, 31, 16));
        ideas_edit = new QTextEdit(groupBox);
        ideas_edit->setObjectName("ideas_edit");
        ideas_edit->setGeometry(QRect(60, 159, 261, 71));
        exclude_list = new QListWidget(groupBox);
        exclude_list->setObjectName("exclude_list");
        exclude_list->setGeometry(QRect(10, 260, 421, 191));
        add_exclude_button = new QPushButton(groupBox);
        add_exclude_button->setObjectName("add_exclude_button");
        add_exclude_button->setGeometry(QRect(10, 460, 80, 24));
        clear_exclude_button = new QPushButton(groupBox);
        clear_exclude_button->setObjectName("clear_exclude_button");
        clear_exclude_button->setGeometry(QRect(350, 460, 81, 24));
        exclude_label = new QLabel(groupBox);
        exclude_label->setObjectName("exclude_label");
        exclude_label->setGeometry(QRect(10, 240, 51, 16));
        exclude_comboBox = new QComboBox(groupBox);
        exclude_comboBox->setObjectName("exclude_comboBox");
        exclude_comboBox->setGeometry(QRect(100, 460, 211, 24));
        browse_exclude_Button = new QPushButton(groupBox);
        browse_exclude_Button->setObjectName("browse_exclude_Button");
        browse_exclude_Button->setGeometry(QRect(320, 460, 21, 24));
        participant_list_label = new QLabel(centralwidget);
        participant_list_label->setObjectName("participant_list_label");
        participant_list_label->setGeometry(QRect(10, 10, 81, 16));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 756, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName("menuEdit");
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName("menuHelp");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuHelp->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        add_participant_button->setText(QCoreApplication::translate("MainWindow", "Add", nullptr));
        clear_participant_button->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        secret_santa_button->setText(QCoreApplication::translate("MainWindow", "SECRET SANTA! ", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Participant Info", nullptr));
        name_label->setText(QCoreApplication::translate("MainWindow", "Name", nullptr));
        address_label->setText(QCoreApplication::translate("MainWindow", "Address", nullptr));
        ideas_label->setText(QCoreApplication::translate("MainWindow", "Ideas", nullptr));
        add_exclude_button->setText(QCoreApplication::translate("MainWindow", "Add", nullptr));
        clear_exclude_button->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        exclude_label->setText(QCoreApplication::translate("MainWindow", "Exclude", nullptr));
        browse_exclude_Button->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        participant_list_label->setText(QCoreApplication::translate("MainWindow", "Participant List", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("MainWindow", "Edit", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
