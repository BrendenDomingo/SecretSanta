// ParticipantManager.h

#ifndef PARTICIPANT_MANAGER_H
#define PARTICIPANT_MANAGER_H

#include <vector>
#include "secret_santa_participant.h"

class ParticipantManager {
private:
    std::vector<SecretSantaParticipant> participants;

public:
    // Constructors
    ParticipantManager() = default;
    ~ParticipantManager() = default;

    // Function to add a participant
    void addParticipant(const SecretSantaParticipant& participant);

    // Function to remove a participant by name
    void removeParticipant(const std::string& name);

    // Function to remove all participants
    void removeAll();

    // Function to get a participant by name
    SecretSantaParticipant* getParticipant(const std::string& name);

    // Display information for all participants
    void displayAllParticipants() const;

    // Randomly assign participants
    void randomlyAssignParticipants();
};

#endif // PARTICIPANT_MANAGER_H
