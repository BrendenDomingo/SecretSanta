// SecretSantaParticipant.cpp

#include "secret_santa_participant.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>

// ---------- Constructors ----------

// Full constructor
SecretSantaParticipant::SecretSantaParticipant(const std::string& name,
                                               const std::string& address,
                                               const std::string& interests)
    : name(name), address(address), interests(interests), assignedParticipant(nullptr) {}

// Name-only constructor
SecretSantaParticipant::SecretSantaParticipant(const std::string& name)
    : name(name), assignedParticipant(nullptr) {}

// Default constructor
SecretSantaParticipant::SecretSantaParticipant()
    : assignedParticipant(nullptr) {}

// ---------- Getters ----------

std::string SecretSantaParticipant::getName() const {
    return name;
}

std::string SecretSantaParticipant::getAddress() const {
    return address;
}

std::string SecretSantaParticipant::getInterests() const {
    return interests;
}

SecretSantaParticipant* SecretSantaParticipant::getAssignedParticipant() const {
    return assignedParticipant;
}

std::vector<SecretSantaParticipant*> SecretSantaParticipant::getCannotBeAssignedTo() const {
    return cannotBeAssignedTo;
}

// ---------- Setters ----------

void SecretSantaParticipant::setName(std::string nameString) {
    name = std::move(nameString);
}

void SecretSantaParticipant::setAddress(std::string addressString) {
    address = std::move(addressString);
}

void SecretSantaParticipant::setInterests(std::string interestsString) {
    interests = std::move(interestsString);
}

// ---------- Assignment Handling ----------

// Assign another participant to this one (as their gift recipient)
void SecretSantaParticipant::assignParticipant(SecretSantaParticipant* participant) {
    assignedParticipant = participant;
}

// Add a participant to the exclusion list
void SecretSantaParticipant::addCannotBeAssignedTo(SecretSantaParticipant* participant) {
    if (participant &&
        std::find(cannotBeAssignedTo.begin(), cannotBeAssignedTo.end(), participant) == cannotBeAssignedTo.end()) {
        cannotBeAssignedTo.push_back(participant);
    }
}

// Clear the exclusion list
void SecretSantaParticipant::clearCannotBeAssignedTo() {
    cannotBeAssignedTo.clear();
}

// ---------- Output & Debugging ----------

// Write assignment details to a file in the given directory
void SecretSantaParticipant::writeAssignmentToFile(const std::string& directoryPath) {
    if (!assignedParticipant) {
        std::cerr << "No assigned participant for " << name << std::endl;
        return;
    }

    // Create a safe filename
    std::string sanitizedName = name;
    std::replace(sanitizedName.begin(), sanitizedName.end(), ' ', '_');
    std::string filename = sanitizedName + "_SecretSanta.txt";

    // Construct full file path
    std::filesystem::path fullPath = std::filesystem::path(directoryPath) / filename;

    // Open and write the file
    std::ofstream outFile(fullPath);
    if (!outFile) {
        std::cerr << "Failed to open file: " << fullPath << std::endl;
        return;
    }

    outFile << "🎁 Secret Santa Assignment 🎁\n\n";
    outFile << "Dear " << name << ",\n\n";
    outFile << "You are the Secret Santa for:\n\n";
    outFile << "Name: " << assignedParticipant->getName() << "\n";
    outFile << "Address: " << assignedParticipant->getAddress() << "\n";
    outFile << "Gift Ideas / Interests: " << assignedParticipant->getInterests() << "\n";

    outFile.close();
}

// Print participant info to console
void SecretSantaParticipant::displayInformation() const {
    std::cout << "Name: " << name << std::endl;
    std::cout << "Address: " << address << std::endl;
    std::cout << "Interests: " << interests << std::endl;

    if (assignedParticipant != nullptr) {
        std::cout << "Assigned Participant: " << assignedParticipant->getName() << std::endl;
    } else {
        std::cout << "Assigned Participant: Not assigned yet" << std::endl;
    }

    std::cout << "-----------------------" << std::endl;
}
