// ParticipantManager.cpp

#include "participant_manager.h"
#include <algorithm>
#include <ctime>
#include <random>
#include <stdexcept>

ParticipantManager::~ParticipantManager() {
    return;
}

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

bool ParticipantManager::validateExclusions(std::string &reason) const
{
    for (const auto& participant : participants) {
        int totalOthers = participants.size() - 1;
        int excludedCount = 0;

        for (const auto& other : participants) {
            if (&participant == &other) continue; // skip self
            std::vector<SecretSantaParticipant*> exclusions = participant.getCannotBeAssignedTo();

            if (std::find(exclusions.begin(), exclusions.end(), &other) != exclusions.end()) {
                excludedCount++;
            }
        }

        if (excludedCount >= totalOthers) {
            reason = "Participant \"" + participant.getName() + "\" excludes all others.";
            return false;
        }
    }

    return true;
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

std::vector<SecretSantaParticipant> &ParticipantManager::getAllParticipants()
{
    return participants;
}

// Display information for all participants
void ParticipantManager::displayAllParticipants() const {
    for (const auto& participant : participants) {
        participant.displayInformation();
    }
}

// Randomly assign participants based on rules
void ParticipantManager::randomlyAssignParticipants() {
    std::srand(static_cast<unsigned int>(time(nullptr)));

    const size_t maxAttempts = 1000;
    std::vector<SecretSantaParticipant*> shuffled;

    for (size_t attempt = 0; attempt < maxAttempts; ++attempt) {
        // Prepare shuffled list of pointers
        shuffled.clear();
        for (auto& p : participants) {
            shuffled.push_back(&p);
        }

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(shuffled.begin(), shuffled.end(), g);

        bool allValid = true;

        // Check for valid assignments
        for (size_t i = 0; i < participants.size(); ++i) {
            SecretSantaParticipant* giver = &participants[i];
            SecretSantaParticipant* receiver = shuffled[i];

            // Rule 1: Cannot assign to self
            if (giver == receiver) {
                allValid = false;
                break;
            }

            // Rule 2: Cannot assign to excluded
            const auto& excluded = giver->getCannotBeAssignedTo();
            if (std::find(excluded.begin(), excluded.end(), receiver) != excluded.end()) {
                allValid = false;
                break;
            }
        }

        if (allValid) {
            // Apply assignments
            for (size_t i = 0; i < participants.size(); ++i) {
                participants[i].assignParticipant(shuffled[i]);
            }
            return;
        }
    }

    // If we get here, we failed to find a valid assignment
    throw std::runtime_error("Failed to generate valid Secret Santa assignments.");
}
