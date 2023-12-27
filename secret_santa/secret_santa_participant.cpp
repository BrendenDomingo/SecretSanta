// SecretSantaParticipant.cpp

#include "secret_santa_participant.h"
#include <iostream>
#include <ostream>

// Constructors
SecretSantaParticipant::SecretSantaParticipant(const std::string& name, const std::string& address, const std::string& interests)
    : name(name), address(address), interests(interests), assignedParticipant(nullptr) {}

SecretSantaParticipant::SecretSantaParticipant(const std::string& name)
    : name(name), assignedParticipant(nullptr) {}

SecretSantaParticipant::SecretSantaParticipant()
    : assignedParticipant(nullptr) {}

// Member functions to access private members
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

std::vector<SecretSantaParticipant *> SecretSantaParticipant::getCannotBeAssignedTo() {
    return cannotBeAssignedTo;
}

void SecretSantaParticipant::setName(std::string nameString) {
    name = nameString;
}

void SecretSantaParticipant::setAddress(std::string addressString) {
    address = addressString;
}

void SecretSantaParticipant::setInterests(std::string interestsString) {
    interests = interestsString;
}

// Assign a participant to the current participant
void SecretSantaParticipant::assignParticipant(SecretSantaParticipant* participant) {
    assignedParticipant = participant;
}

// Add a participant to the list of those they cannot be assigned to
void SecretSantaParticipant::addCannotBeAssignedTo(SecretSantaParticipant* participant) {
    cannotBeAssignedTo.push_back(participant);
}

// Display participant information
void SecretSantaParticipant::displayInformation() const {
    std::cout << "Name: " << name << std::endl;
    std::cout << "Address: " << address << std::endl;
    std::cout << "Interests: " << interests << std::endl;
    if(assignedParticipant != nullptr) {
        std::cout << "Assigned Participant: " << assignedParticipant->getName() << std::endl;
    } else {
        std::cout << "Assigned Participant: Not assigned yet" << std::endl;
    }
    std::cout << "-----------------------" << std::endl;
}
