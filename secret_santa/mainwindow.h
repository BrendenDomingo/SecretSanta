#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

#include "participant_manager.h"
#include "secret_santa_participant.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @brief The MainWindow class
 * Main application window handling the UI and interactions
 * for managing Secret Santa participants.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /// Manager for all participants and exclusions
    ParticipantManager* manager = nullptr;

    /// Currently selected participant
    SecretSantaParticipant* selectedParticipant = nullptr;

    /// Currently selected item in the participant list
    QListWidgetItem* selectedItem = nullptr;

private slots:
    /// Adds a new participant from the input fields
    void on_add_participant_button_clicked();

    /// Clears the currently selected participant
    void on_clear_participant_button_clicked();

    /// Triggered when a participant list item is clicked
    void on_participant_list_itemClicked(QListWidgetItem *item);

    /// Updates name when text is edited
    void on_name_edit_textChanged(const QString &arg1);

    /// Updates address when text is edited
    void on_Adderss_edit_textChanged();

    /// Updates gift ideas when text is edited
    void on_ideas_edit_textChanged();

    /// Adds an exclusion between participants
    void on_add_exclude_button_clicked();

    /// Generates Secret Santa assignments
    void on_secret_santa_button_clicked();

    /// Clears exclusions for the selected participant
    void on_clear_exclude_button_clicked();

    /// Creates a new session (clears current data)
    void on_actionNew_triggered();

    /// Saves the current session to disk
    void on_actionSave_triggered();

    /// Loads a session from disk
    void on_actionOpen_triggered();

    /// Serializes and saves the current application state
    void saveSession();

    /// Restores application state from a previously saved session
    void loadSession();

private:
    /// UI object generated from .ui file
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
