#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "participant_manager.h"
#include "qlistwidget.h"
#include "secret_santa_participant.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    ParticipantManager* manager = nullptr;

    SecretSantaParticipant* selectedParticipant;

    QListWidgetItem* selectedItem;

private slots:
    void on_add_participant_button_clicked();

    void on_clear_participant_button_clicked();

    void on_participant_list_itemClicked(QListWidgetItem *item);

    void on_name_edit_textChanged(const QString &arg1);

    void on_Adderss_edit_textChanged();

    void on_ideas_edit_textChanged();

    void on_add_exclude_button_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
