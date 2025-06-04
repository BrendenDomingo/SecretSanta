#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    manager = new ParticipantManager();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_add_participant_button_clicked() {
    SecretSantaParticipant participant;

    int participantCount = ui->participant_list->count();

    // If this is the first participant added
    if(participantCount != 0){

        // Dont append count to name
        participant.setName(QString("New Participant%1").arg(participantCount).toStdString());
    } else {

        // Do append count to name
        participant.setName(QString("New Participant").toStdString());
    }

    // Add the new participant to the participant manager
    manager->addParticipant(participant);

    // Add the new participant to the participant list UI element
    ui->participant_list->addItem(QString::fromStdString(participant.getName()));
}

void MainWindow::on_clear_participant_button_clicked() {
    // Remove all participants from the participant manager
    manager->removeAll();

    // Clear UI elements
    ui->name_edit->setText("");
    ui->Adderss_edit->setText("");
    ui->ideas_edit->setText("");
    ui->exclude_list->clear();

    ui->participant_list->clear();
    ui->exclude_comboBox->clear();
}


void MainWindow::on_participant_list_itemClicked(QListWidgetItem *item) {
    int participantCount = ui->participant_list->count();

    // Get the selected participant
    QString name = item->text();
    selectedParticipant = manager->getParticipant(name.toStdString());
    selectedItem = item;

    // Set the UI elements for the selected participant
    ui->name_edit->setText(QString::fromStdString(selectedParticipant->getName()));
    ui->Adderss_edit->setText(QString::fromStdString(selectedParticipant->getAddress()));
    ui->ideas_edit->setText(QString::fromStdString(selectedParticipant->getInterests()));

    for(auto &excluded : selectedParticipant->getCannotBeAssignedTo()) {
        ui->exclude_list->addItem(QString::fromStdString(excluded->getName()));
    }

    // Populate the exclude combo box with valid participants to exclude
    ui->exclude_comboBox->clear();
    for(int i = 0; i < participantCount; i++) {
        ui->exclude_comboBox->addItem(ui->participant_list->item(i)->text());
    }
}


void MainWindow::on_name_edit_textChanged(const QString &arg1) {
    // Set the new name for the selected participant
    selectedParticipant->setName(arg1.toStdString());

    // Update the UI to reflect name change
    selectedItem->setText(QString::fromStdString(selectedParticipant->getName()));
}


void MainWindow::on_Adderss_edit_textChanged() {
    // Set the new address for the selected participant
    selectedParticipant->setAddress(ui->Adderss_edit->toPlainText().toStdString());
}


void MainWindow::on_ideas_edit_textChanged() {
    // Set the new ideas for the selected participant
    selectedParticipant->setInterests(ui->ideas_edit->toPlainText().toStdString());
}


void MainWindow::on_add_exclude_button_clicked() {

}


void MainWindow::on_secret_santa_button_clicked()
{

}

