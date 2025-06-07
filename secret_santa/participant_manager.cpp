// ParticipantManager.cpp

#include "participant_manager.h"

#include <algorithm>
#include <ctime>
#include <random>
#include <stdexcept>

// Destructor
ParticipantManager::~ParticipantManager() {
    // Nothing specific to clean up
}

// Add a participant to the list
void ParticipantManager::addParticipant(const SecretSantaParticipant& participant) {
    participants.push_back(participant);
}

// Remove a participant by name
void ParticipantManager::removeParticipant(const std::string& name) {
    auto it = std::remove_if(participants.begin(), participants.end(),
                             [name](const SecretSantaParticipant& participant) {
                                 return participant.getName() == name;
                             });

    participants.erase(it, participants.end());
}

// Remove all participants
void ParticipantManager::removeAll() {
    participants.clear();
}

// Retrieve a participant by name (non-const)
SecretSantaParticipant* ParticipantManager::getParticipant(const std::string& name) {
    for (auto& participant : participants) {
        if (participant.getName() == name) {
            return &participant;
        }
    }
    return nullptr;
}

// Get all participants by reference
std::vector<SecretSantaParticipant>& ParticipantManager::getAllParticipants() {
    return participants;
}

// Display info for all participants (to console or stdout)
void ParticipantManager::displayAllParticipants() const {
    for (const auto& participant : participants) {
        participant.displayInformation();
    }
}

// Validate that no participant excludes everyone else
bool ParticipantManager::validateExclusions(std::string& reason) const {
    for (const auto& participant : participants) {
        int totalOthers = static_cast<int>(participants.size()) - 1;
        int excludedCount = 0;

        for (const auto& other : participants) {
            if (&participant == &other) continue;

            const auto& exclusions = participant.getCannotBeAssignedTo();
            if (std::find(exclusions.begin(), exclusions.end(), &other) != exclusions.end()) {
                ++excludedCount;
            }
        }

        if (excludedCount >= totalOthers) {
            reason = "Participant \"" + participant.getName() + "\" excludes all others.";
            return false;
        }
    }
    return true;
}

// Assign each participant to another participant, ensuring rules are followed
void ParticipantManager::randomlyAssignParticipants() {
    constexpr size_t maxAttempts = 1000;  // Prevent infinite loops

    // Pointer list used for shuffling
    std::vector<SecretSantaParticipant*> shuffled;

    // Try up to maxAttempts to find a valid assignment
    for (size_t attempt = 0; attempt < maxAttempts; ++attempt) {
        shuffled.clear();
        for (auto& p : participants) {
            shuffled.push_back(&p);
        }

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(shuffled.begin(), shuffled.end(), g);

        bool valid = true;

        // Check each assignment for validity
        for (size_t i = 0; i < participants.size(); ++i) {
            SecretSantaParticipant* giver = &participants[i];
            SecretSantaParticipant* receiver = shuffled[i];

            // Rule 1: Cannot assign to oneself
            if (giver == receiver) {
                valid = false;
                break;
            }

            // Rule 2: Cannot assign to someone in the exclusion list
            const auto& exclusions = giver->getCannotBeAssignedTo();
            if (std::find(exclusions.begin(), exclusions.end(), receiver) != exclusions.end()) {
                valid = false;
                break;
            }
        }

        // If valid assignment found, apply it
        if (valid) {
            for (size_t i = 0; i < participants.size(); ++i) {
                participants[i].assignParticipant(shuffled[i]);
            }
            return;
        }
    }

    // If no valid assignment was found after maxAttempts
    throw std::runtime_error("Failed to generate valid Secret Santa assignments.");
}
