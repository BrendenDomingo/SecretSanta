// ParticipantManager.cpp

#include "participant_manager.h"
#include <algorithm>
#include <ctime>

// Function to add a participant
void ParticipantManager::addParticipant(const SecretSantaParticipant& participant) {
    participants.push_back(participant);
}

// Function to remove a participant by name
void ParticipantManager::removeParticipant(const std::string& name) {
    auto it = std::remove_if(participants.begin(), participants.end(),
                             [name](const SecretSantaParticipant& participant) {
                                 return participant.getName() == name;
                             });

    participants.erase(it, participants.end());
}

void ParticipantManager::removeAll() {
    participants.clear();
}

// Function to get a participant by name
SecretSantaParticipant* ParticipantManager::getParticipant(const std::string& name) {
    for(auto& participant : participants) {
        if(participant.getName() == name) {
            return &participant;
        }
    }
    return nullptr;
}

// Display information for all participants
void ParticipantManager::displayAllParticipants() const {
    for (const auto& participant : participants) {
        participant.displayInformation();
    }
}

// Randomly assign participants based on rules
void ParticipantManager::randomlyAssignParticipants() {
    // Seed the random number generator
    std::srand(static_cast<unsigned int>(time(nullptr)));

    // Shuffle the participants vector
    std::random_shuffle(participants.begin(), participants.end());

    // Iterate through the shuffled participants to assign them
    for(size_t i = 0; i < participants.size(); ++i) {
        // Get a participant
        SecretSantaParticipant& currentParticipant = participants[i];

        // Find a suitable assignment based on rules
        bool validAssignment = false;
        size_t attempts = 0;

        while(!validAssignment && attempts < participants.size()) {
            // Randomly select a participant from the remaining pool
            size_t randomIndex = std::rand() % participants.size();
            SecretSantaParticipant* potentialAssignment = &participants[randomIndex];

            // Check rule 1: They cannot be assigned a participant in the cannotBeAssignedTo list
            if(std::find(currentParticipant.getCannotBeAssignedTo().begin(), currentParticipant.getCannotBeAssignedTo().end(), potentialAssignment) == currentParticipant.getCannotBeAssignedTo().end()) {
                // Check rule 2: They cannot be assigned a participant that has them as their participant
                if(potentialAssignment != &currentParticipant && potentialAssignment->getAssignedParticipant() != &currentParticipant) {
                    // Assign the participant
                    currentParticipant.assignParticipant(potentialAssignment);
                    validAssignment = true;
                }
            }

            attempts++;
        }
    }
}
