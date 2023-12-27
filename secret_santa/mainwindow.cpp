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
    participant.setName(QString("New Participant%1").arg(ui->participant_list->count()).toStdString());
    manager->addParticipant(participant);

    ui->participant_list->addItem(QString::fromStdString(participant.getName()));
}

void MainWindow::on_clear_participant_button_clicked() {
    manager->removeAll();

    ui->participant_list->clear();
}


void MainWindow::on_participant_list_itemClicked(QListWidgetItem *item) {
    QString name = item->text();
    selectedParticipant = manager->getParticipant(name.toStdString());
    selectedItem = item;

    ui->name_edit->setText(QString::fromStdString(selectedParticipant->getName()));
    ui->Adderss_edit->setText(QString::fromStdString(selectedParticipant->getAddress()));
    ui->ideas_edit->setText(QString::fromStdString(selectedParticipant->getInterests()));

    for(auto &excluded : selectedParticipant->getCannotBeAssignedTo()) {
        ui->exclude_list->addItem(QString::fromStdString(excluded->getName()));
    }
}


void MainWindow::on_name_edit_textChanged(const QString &arg1) {
    selectedParticipant->setName(arg1.toStdString());

    selectedItem->setText(QString::fromStdString(selectedParticipant->getName()));
}


void MainWindow::on_Adderss_edit_textChanged() {
    selectedParticipant->setAddress(ui->Adderss_edit->toPlainText().toStdString());
}


void MainWindow::on_ideas_edit_textChanged() {
    selectedParticipant->setInterests(ui->ideas_edit->toPlainText().toStdString());
}


void MainWindow::on_add_exclude_button_clicked() {

}

