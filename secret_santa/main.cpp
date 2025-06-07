#include "mainwindow.h"
#include <QApplication>

/**
 * @brief Entry point of the Secret Santa application.
 * Initializes the QApplication and displays the main window.
 */
int main(int argc, char *argv[])
{
    // Create the Qt application
    QApplication a(argc, argv);

    // Create and show the main window
    MainWindow w;
    w.show();

    // Enter the main event loop
    return a.exec();
}
