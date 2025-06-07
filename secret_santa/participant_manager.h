#ifndef PARTICIPANT_MANAGER_H
#define PARTICIPANT_MANAGER_H

#include <vector>
#include <string>
#include "secret_santa_participant.h"

/**
 * @brief Manages the list of Secret Santa participants and their assignments.
 */
class ParticipantManager {
private:
    /// Collection of all participants
    std::vector<SecretSantaParticipant> participants;

public:
    /// Default constructor
    ParticipantManager() = default;

    /// Destructor
    ~ParticipantManager();

    /**
     * @brief Adds a new participant.
     * @param participant The participant to add.
     */
    void addParticipant(const SecretSantaParticipant& participant);

    /**
     * @brief Removes a participant by name.
     * @param name The name of the participant to remove.
     */
    void removeParticipant(const std::string& name);

    /**
     * @brief Removes all participants from the list.
     */
    void removeAll();

    /**
     * @brief Validates if exclusions make assignment impossible.
     * @param reason Output string explaining the reason if validation fails.
     * @return True if exclusions are valid; false otherwise.
     */
    bool validateExclusions(std::string& reason) const;

    /**
     * @brief Gets a pointer to a participant by name.
     * @param name The name of the participant.
     * @return Pointer to the participant, or nullptr if not found.
     */
    SecretSantaParticipant* getParticipant(const std::string& name);

    /**
     * @brief Gets a reference to the list of all participants.
     * @return Reference to the vector of participants.
     */
    std::vector<SecretSantaParticipant>& getAllParticipants();

    /**
     * @brief Prints details of all participants (for debugging/logging).
     */
    void displayAllParticipants() const;

    /**
     * @brief Randomly assigns Secret Santa partners, respecting exclusions.
     */
    void randomlyAssignParticipants();
};

#endif // PARTICIPANT_MANAGER_H
