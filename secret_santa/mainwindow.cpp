#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QDir>

// Constructor: Initializes UI and attempts to load previous session
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    manager = new ParticipantManager();
    loadSession();
}

// Destructor: Saves session and cleans up resources
MainWindow::~MainWindow() {
    saveSession();
    delete manager;
    delete ui;
}

// Add a new participant with a default name
void MainWindow::on_add_participant_button_clicked() {
    QString baseName = "New Participant";
    int participantCount = ui->participant_list->count();
    QString newName = (participantCount > 0) ? baseName + QString::number(participantCount) : baseName;

    SecretSantaParticipant participant;
    participant.setName(newName.toStdString());

    manager->addParticipant(participant);
    ui->participant_list->addItem(newName);
}

// Clear all participants and UI
void MainWindow::on_clear_participant_button_clicked() {
    manager->removeAll();

    ui->name_edit->clear();
    ui->Adderss_edit->clear();
    ui->ideas_edit->clear();
    ui->exclude_list->clear();
    ui->participant_list->clear();
    ui->exclude_comboBox->clear();

    selectedParticipant = nullptr;
    selectedItem = nullptr;
}

// Handle participant selection in UI
void MainWindow::on_participant_list_itemClicked(QListWidgetItem *item) {
    QString name = item->text();
    selectedParticipant = manager->getParticipant(name.toStdString());
    selectedItem = item;

    if (!selectedParticipant) return;

    ui->name_edit->setText(QString::fromStdString(selectedParticipant->getName()));
    ui->Adderss_edit->setText(QString::fromStdString(selectedParticipant->getAddress()));
    ui->ideas_edit->setText(QString::fromStdString(selectedParticipant->getInterests()));

    ui->exclude_list->clear();
    for (auto &excluded : selectedParticipant->getCannotBeAssignedTo()) {
        ui->exclude_list->addItem(QString::fromStdString(excluded->getName()));
    }

    ui->exclude_comboBox->clear();
    for (int i = 0; i < ui->participant_list->count(); i++) {
        ui->exclude_comboBox->addItem(ui->participant_list->item(i)->text());
    }
}

// Update participant name
void MainWindow::on_name_edit_textChanged(const QString &arg1) {
    if (selectedParticipant && selectedItem) {
        selectedParticipant->setName(arg1.toStdString());
        selectedItem->setText(arg1);
    }
}

// Update participant address
void MainWindow::on_Adderss_edit_textChanged() {
    if (selectedParticipant) {
        selectedParticipant->setAddress(ui->Adderss_edit->toPlainText().toStdString());
    }
}

// Update participant interests
void MainWindow::on_ideas_edit_textChanged() {
    if (selectedParticipant) {
        selectedParticipant->setInterests(ui->ideas_edit->toPlainText().toStdString());
    }
}

// Add an exclusion for the selected participant
void MainWindow::on_add_exclude_button_clicked() {
    SecretSantaParticipant* excludedParticipant = manager->getParticipant(ui->exclude_comboBox->currentText().toStdString());

    if (!excludedParticipant || excludedParticipant == selectedParticipant) return;

    for (auto& existing : selectedParticipant->getCannotBeAssignedTo()) {
        if (existing == excludedParticipant) return;
    }

    selectedParticipant->addCannotBeAssignedTo(excludedParticipant);

    ui->exclude_list->clear();
    for (auto& excluded : selectedParticipant->getCannotBeAssignedTo()) {
        ui->exclude_list->addItem(QString::fromStdString(excluded->getName()));
    }
}

// Execute Secret Santa logic and write results
void MainWindow::on_secret_santa_button_clicked() {
    std::ostringstream warningStream;
    bool missingInfo = false;

    for (const auto& participant : manager->getAllParticipants()) {
        if (participant.getAddress().empty()) {
            warningStream << "- " << participant.getName() << " is missing an address.\n";
            missingInfo = true;
        }
        if (participant.getInterests().empty()) {
            warningStream << "- " << participant.getName() << " has no gift ideas/interests.\n";
            missingInfo = true;
        }
    }

    if (missingInfo) {
        int ret = QMessageBox::warning(
            this, "Missing Participant Info",
            QString("Some participants are missing important information:\n\n%1\n\nDo you want to continue?").arg(QString::fromStdString(warningStream.str())),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (ret == QMessageBox::No) return;
    }

    std::string reason;
    if (!manager->validateExclusions(reason)) {
        QMessageBox::critical(this, "Invalid Exclusion Configuration", QString::fromStdString(reason));
        return;
    }

    try {
        manager->randomlyAssignParticipants();
        QString selectedDir = QFileDialog::getExistingDirectory(this, "Select Directory to Save Assignments");
        if (selectedDir.isEmpty()) {
            QMessageBox::information(this, "Cancelled", "No directory selected. Operation cancelled.");
            return;
        }

        for (SecretSantaParticipant& participant : manager->getAllParticipants()) {
            participant.writeAssignmentToFile(selectedDir.toStdString());
        }

        QMessageBox::information(this, "Success", "Secret Santa assignments saved successfully.");
    } catch (const std::runtime_error& e) {
        QMessageBox::critical(this, "Assignment Error", e.what());
    } catch (...) {
        QMessageBox::critical(this, "Assignment Error", "An unknown error occurred during assignment.");
    }
}

// Clear exclusions for selected participant
void MainWindow::on_clear_exclude_button_clicked() {
    if (!selectedParticipant) return;
    selectedParticipant->clearCannotBeAssignedTo();
    ui->exclude_list->clear();
}

// File -> New menu action
void MainWindow::on_actionNew_triggered() {
    on_clear_participant_button_clicked();
}

// File -> Save menu action
void MainWindow::on_actionSave_triggered() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save Participant Data", "", "Secret Santa Files (*.json);;All Files (*)");
    if (fileName.isEmpty()) return;

    QJsonArray participantsArray;
    for (const auto& participant : manager->getAllParticipants()) {
        QJsonObject obj;
        obj["name"] = QString::fromStdString(participant.getName());
        obj["address"] = QString::fromStdString(participant.getAddress());
        obj["interests"] = QString::fromStdString(participant.getInterests());

        QJsonArray excludedArray;
        for (auto* excluded : participant.getCannotBeAssignedTo()) {
            excludedArray.append(QString::fromStdString(excluded->getName()));
        }
        obj["excluded"] = excludedArray;

        if (participant.getAssignedParticipant()) {
            obj["assigned"] = QString::fromStdString(participant.getAssignedParticipant()->getName());
        }

        participantsArray.append(obj);
    }

    QJsonDocument doc;
    QJsonObject root;
    root["participants"] = participantsArray;
    doc.setObject(root);

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Error", "Unable to open file for writing.");
        return;
    }
    file.write(doc.toJson());
    file.close();

    QMessageBox::information(this, "Saved", "Participant data saved successfully.");
}

// File -> Open menu action
void MainWindow::on_actionOpen_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Participant Data", "", "Secret Santa Files (*.json);;All Files (*)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "Unable to open file for reading.");
        return;
    }
    QByteArray data = file.readAll();
    file.close();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError) {
        QMessageBox::critical(this, "Parse Error", err.errorString());
        return;
    }

    on_clear_participant_button_clicked();

    QJsonObject root = doc.object();
    QJsonArray arr = root["participants"].toArray();

    std::unordered_map<std::string, SecretSantaParticipant*> nameMap;
    for (const auto& val : arr) {
        QJsonObject obj = val.toObject();
        SecretSantaParticipant p(
            obj["name"].toString().toStdString(),
            obj["address"].toString().toStdString(),
            obj["interests"].toString().toStdString()
            );
        manager->addParticipant(p);
    }

    for (auto& p : manager->getAllParticipants()) {
        nameMap[p.getName()] = &p;
        ui->participant_list->addItem(QString::fromStdString(p.getName()));
    }

    for (const auto& val : arr) {
        QJsonObject obj = val.toObject();
        std::string name = obj["name"].toString().toStdString();
        auto* p = nameMap[name];

        QJsonArray exclusions = obj["excluded"].toArray();
        for (const auto& ex : exclusions) {
            std::string exName = ex.toString().toStdString();
            if (nameMap.count(exName)) p->addCannotBeAssignedTo(nameMap[exName]);
        }

        if (obj.contains("assigned")) {
            std::string assignName = obj["assigned"].toString().toStdString();
            if (nameMap.count(assignName)) p->assignParticipant(nameMap[assignName]);
        }
    }

    QMessageBox::information(this, "Loaded", "Participant data loaded successfully.");
}

// Save temporary session to disk
void MainWindow::saveSession() {
    QJsonArray array;

    for (const auto& p : manager->getAllParticipants()) {
        QJsonObject obj;
        obj["name"] = QString::fromStdString(p.getName());
        obj["address"] = QString::fromStdString(p.getAddress());
        obj["interests"] = QString::fromStdString(p.getInterests());

        QJsonArray ex;
        for (auto* exP : p.getCannotBeAssignedTo()) {
            ex.append(QString::fromStdString(exP->getName()));
        }
        obj["exclusions"] = ex;

        array.append(obj);
    }

    QJsonDocument doc(array);

    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(path);
    QFile file(path + "/session.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

// Load temporary session from disk
void MainWindow::loadSession() {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/session.json";
    QFile file(path);
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) return;

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) return;

    QJsonArray array = doc.array();
    manager->removeAll();
    ui->participant_list->clear();

    for (const auto& val : array) {
        QJsonObject obj = val.toObject();
        SecretSantaParticipant p;
        p.setName(obj["name"].toString().toStdString());
        p.setAddress(obj["address"].toString().toStdString());
        p.setInterests(obj["interests"].toString().toStdString());
        manager->addParticipant(p);
        ui->participant_list->addItem(QString::fromStdString(p.getName()));
    }

    std::unordered_map<std::string, SecretSantaParticipant*> map;
    for (auto& p : manager->getAllParticipants()) {
        map[p.getName()] = &p;
    }

    for (const auto& val : array) {
        QJsonObject obj = val.toObject();
        auto* p = map[obj["name"].toString().toStdString()];
        for (const auto& ex : obj["exclusions"].toArray()) {
            std::string exName = ex.toString().toStdString();
            if (map.count(exName)) {
                p->addCannotBeAssignedTo(map[exName]);
            }
        }
    }
}
