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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    manager = new ParticipantManager();

    loadSession();
}

MainWindow::~MainWindow() {
    saveSession();

    delete manager;
    delete ui;
}

void MainWindow::on_add_participant_button_clicked() {
    QString baseName = "New Participant";
    int participantCount = ui->participant_list->count();
    QString newName = baseName;

    if (participantCount > 0) {
        newName = QString("New Participant%1").arg(participantCount);
    }

    SecretSantaParticipant participant;
    participant.setName(newName.toStdString());

    // Add to manager and UI
    manager->addParticipant(participant);
    ui->participant_list->addItem(newName);
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

    selectedParticipant = nullptr;
    selectedItem = nullptr;
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

    ui->exclude_list->clear();
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
    if (selectedParticipant && selectedItem) {
        selectedParticipant->setName(arg1.toStdString());
        selectedItem->setText(QString::fromStdString(selectedParticipant->getName()));
    }
}


void MainWindow::on_Adderss_edit_textChanged() { 
    if (selectedParticipant && selectedItem) {
        selectedParticipant->setAddress(ui->Adderss_edit->toPlainText().toStdString());
        selectedItem->setText(QString::fromStdString(selectedParticipant->getName()));
    }
}


void MainWindow::on_ideas_edit_textChanged() {
    if (selectedParticipant && selectedItem) {
        selectedParticipant->setInterests(ui->ideas_edit->toPlainText().toStdString());
        selectedItem->setText(QString::fromStdString(selectedParticipant->getName()));
    }
}


void MainWindow::on_add_exclude_button_clicked() {
    // Get pointer to the excluded participant
    SecretSantaParticipant* excludedParticipant = manager->getParticipant(ui->exclude_comboBox->currentText().toStdString());

    // Protect against null pointers and self-exclusion
    if (!excludedParticipant || excludedParticipant == selectedParticipant) return;

    // Prevent duplicates
    for (auto& existing : selectedParticipant->getCannotBeAssignedTo()) {
        if (existing == excludedParticipant)
            return; // Already excluded
    }

    selectedParticipant->addCannotBeAssignedTo(excludedParticipant);

    // Refresh the exclude list in UI
    ui->exclude_list->clear();
    for(auto& excluded : selectedParticipant->getCannotBeAssignedTo()) {
        ui->exclude_list->addItem(QString::fromStdString(excluded->getName()));
    }
}


void MainWindow::on_secret_santa_button_clicked()
{
    // Check participants for missing info
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
            this,
            "Missing Participant Info",
            QString("Some participants are missing important information:\n\n%1\n\nDo you want to continue?")
                .arg(QString::fromStdString(warningStream.str())),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No
            );

        if (ret == QMessageBox::No) {
            // User chose not to continue
            return;
        }
    }

    // Validate exclusions
    std::string reason;
    if (!manager->validateExclusions(reason)) {
        QMessageBox::critical(this, "Invalid Exclusion Configuration",
                              QString::fromStdString(reason));
        return;
    }

    try {
        manager->randomlyAssignParticipants();

        // Prompt user to select a directory
        QString selectedDir = QFileDialog::getExistingDirectory(this, "Select Directory to Save Assignments");

        // If user cancels the dialog, don't proceed
        if (selectedDir.isEmpty()) {
            QMessageBox::information(this, "Cancelled", "No directory selected. Operation cancelled.");
            return;
        }

        for (SecretSantaParticipant& participant : manager->getAllParticipants()) {
            participant.writeAssignmentToFile(selectedDir.toStdString());
        }

        QMessageBox::information(this, "Success", "Secret Santa assignments saved successfully.");
    }
    catch (const std::runtime_error& e) {
        QMessageBox::critical(this, "Assignment Error", e.what());
    }
    catch (...) {
        QMessageBox::critical(this, "Assignment Error", "An unknown error occurred during assignment.");
    }
}


void MainWindow::on_clear_exclude_button_clicked()
{
    if (!selectedParticipant) return;

    // Manually clear the exclusion list
    selectedParticipant->clearCannotBeAssignedTo();

    // Update the UI
    ui->exclude_list->clear();
}


void MainWindow::on_actionNew_triggered()
{
    // Clear all participants from the manager
    manager->removeAll();

    // Clear UI elements
    ui->name_edit->setText("");
    ui->Adderss_edit->setText("");
    ui->ideas_edit->setText("");
    ui->exclude_list->clear();

    ui->participant_list->clear();
    ui->exclude_comboBox->clear();

    selectedParticipant = nullptr;
    selectedItem = nullptr;
}


void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Save Participant Data",
        "",
        "Secret Santa Files (*.json);;All Files (*)"
        );

    if (fileName.isEmpty()) return;

    QJsonArray participantsArray;

    for (const auto& participant : manager->getAllParticipants()) {
        QJsonObject participantObject;
        participantObject["name"] = QString::fromStdString(participant.getName());
        participantObject["address"] = QString::fromStdString(participant.getAddress());
        participantObject["interests"] = QString::fromStdString(participant.getInterests());

        QJsonArray excludedArray;
        for (const auto* excluded : participant.getCannotBeAssignedTo()) {
            excludedArray.append(QString::fromStdString(excluded->getName()));
        }
        participantObject["excluded"] = excludedArray;

        // Optional: assigned participant (if exists)
        if (participant.getAssignedParticipant()) {
            participantObject["assigned"] = QString::fromStdString(participant.getAssignedParticipant()->getName());
        }

        participantsArray.append(participantObject);
    }

    QJsonObject root;
    root["participants"] = participantsArray;

    QJsonDocument saveDoc(root);

    QFile saveFile(fileName);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Error", "Unable to open file for writing.");
        return;
    }

    saveFile.write(saveDoc.toJson());
    saveFile.close();

    QMessageBox::information(this, "Saved", "Participant data saved successfully.");
}


void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Open Participant Data",
        "",
        "Secret Santa Files (*.json);;All Files (*)"
        );

    if (fileName.isEmpty()) return;

    QFile loadFile(fileName);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "Unable to open file for reading.");
        return;
    }

    QByteArray fileData = loadFile.readAll();
    loadFile.close();

    QJsonParseError parseError;
    QJsonDocument loadDoc = QJsonDocument::fromJson(fileData, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        QMessageBox::critical(this, "Parse Error", "Failed to parse the file:\n" + parseError.errorString());
        return;
    }

    // Clear current data
    manager->removeAll();
    ui->participant_list->clear();
    ui->name_edit->clear();
    ui->Adderss_edit->clear();
    ui->ideas_edit->clear();
    ui->exclude_list->clear();
    ui->exclude_comboBox->clear();
    selectedParticipant = nullptr;
    selectedItem = nullptr;

    QJsonObject root = loadDoc.object();
    QJsonArray participantsArray = root["participants"].toArray();

    // Temporary map to resolve references after all are created
    std::unordered_map<std::string, SecretSantaParticipant*> nameToParticipant;

    // 1. Create participants without exclusions/assignments first
    for (int i = 0; i < participantsArray.size(); ++i) {
        QJsonObject obj = participantsArray[i].toObject();
        SecretSantaParticipant p(
            obj["name"].toString().toStdString(),
            obj["address"].toString().toStdString(),
            obj["interests"].toString().toStdString()
            );
        manager->addParticipant(p);
    }

    // Build name-to-pointer map
    for (auto& p : manager->getAllParticipants()) {
        nameToParticipant[p.getName()] = &p;
    }

    // 2. Now resolve exclusions and assignments
    int index = 0;
    for (int i = 0; i < participantsArray.size(); ++i) {
        QJsonObject obj = participantsArray[i].toObject();
        std::string name = obj["name"].toString().toStdString();
        SecretSantaParticipant* p = nameToParticipant[name];

        // Excluded participants
        QJsonArray excludedArray = obj["excluded"].toArray();
        for (int j = 0; j < excludedArray.size(); ++j) {
            std::string exName = excludedArray[j].toString().toStdString();
            if (nameToParticipant.count(exName)) {
                p->addCannotBeAssignedTo(nameToParticipant[exName]);
            }
        }

        // Assigned participant (optional)
        if (obj.contains("assigned")) {
            std::string assignedName = obj["assigned"].toString().toStdString();
            if (nameToParticipant.count(assignedName)) {
                p->assignParticipant(nameToParticipant[assignedName]);
            }
        }

        // Add to UI
        ui->participant_list->addItem(QString::fromStdString(p->getName()));
        ++index;
    }

    QMessageBox::information(this, "Loaded", "Participant data loaded successfully.");
}

void MainWindow::saveSession()
{
    QJsonArray participantArray;

    for (const auto& participant : manager->getAllParticipants()) {
        QJsonObject pObj;
        pObj["name"] = QString::fromStdString(participant.getName());
        pObj["address"] = QString::fromStdString(participant.getAddress());
        pObj["interests"] = QString::fromStdString(participant.getInterests());

        QJsonArray excludeArray;
        for (auto* excluded : participant.getCannotBeAssignedTo()) {
            excludeArray.append(QString::fromStdString(excluded->getName()));
        }
        pObj["exclusions"] = excludeArray;

        participantArray.append(pObj);
    }

    QJsonDocument doc(participantArray);

    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(path);
    QFile file(path + "/session.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

void MainWindow::loadSession()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/session.json";
    QFile file(path);

    if (!file.exists() || !file.open(QIODevice::ReadOnly)) return;

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    file.close();

    if (!doc.isArray()) return;

    QJsonArray array = doc.array();
    manager->removeAll();
    ui->participant_list->clear();

    // First pass: Add all participants
    for (const QJsonValue& val : array) {
        QJsonObject obj = val.toObject();
        SecretSantaParticipant p;
        p.setName(obj["name"].toString().toStdString());
        p.setAddress(obj["address"].toString().toStdString());
        p.setInterests(obj["interests"].toString().toStdString());
        manager->addParticipant(p);
        ui->participant_list->addItem(QString::fromStdString(p.getName()));
    }

    // Build a quick lookup table
    std::unordered_map<std::string, SecretSantaParticipant*> nameMap;
    for (const auto& p : manager->getAllParticipants()) {
        nameMap[p.getName()] = manager->getParticipant(p.getName());
    }

    // Second pass: Add exclusions
    for (const QJsonValue& val : array) {
        QJsonObject obj = val.toObject();
        std::string pName = obj["name"].toString().toStdString();
        auto* p = manager->getParticipant(pName);
        if (!p) continue;

        QJsonArray exclusions = obj["exclusions"].toArray();
        for (const QJsonValue& exVal : exclusions) {
            std::string exName = exVal.toString().toStdString();
            if (exName != pName && nameMap.count(exName)) {
                p->addCannotBeAssignedTo(nameMap[exName]);
            }
        }
    }
}

