#ifndef SECRET_SANTA_PARTICIPANT_H
#define SECRET_SANTA_PARTICIPANT_H

#include <string>
#include <vector>

/**
 * @brief Represents a participant in a Secret Santa exchange.
 * Stores personal info, exclusions, and assigned recipient.
 */
class SecretSantaParticipant {
private:
    std::string name;                                 ///< Participant's name
    std::string address;                              ///< Mailing address
    std::string interests;                            ///< Gift ideas or interests
    SecretSantaParticipant* assignedParticipant;      ///< Assigned gift recipient
    std::vector<SecretSantaParticipant*> cannotBeAssignedTo; ///< Exclusion list

public:
    /// Constructors
    SecretSantaParticipant(const std::string& name, const std::string& address, const std::string& interests);
    SecretSantaParticipant(const std::string& name);
    SecretSantaParticipant();

    /// Default destructor
    ~SecretSantaParticipant() = default;

    /// Getters
    std::string getName() const;
    std::string getAddress() const;
    std::string getInterests() const;
    SecretSantaParticipant* getAssignedParticipant() const;
    std::vector<SecretSantaParticipant*> getCannotBeAssignedTo() const;

    /// Setters
    void setName(std::string nameString);
    void setAddress(std::string addressString);
    void setInterests(std::string interestsString);

    /**
     * @brief Assigns another participant as the gift recipient.
     * @param participant Pointer to the participant to assign.
     */
    void assignParticipant(SecretSantaParticipant* participant);

    /**
     * @brief Adds a participant to the exclusion list.
     * @param participant Pointer to the participant to exclude.
     */
    void addCannotBeAssignedTo(SecretSantaParticipant* participant);

    /**
     * @brief Clears all exclusions from the participant.
     */
    void clearCannotBeAssignedTo();

    /**
     * @brief Writes this participant's assignment to a file.
     * @param directoryPath Directory where the file will be saved.
     */
    void writeAssignmentToFile(const std::string& directoryPath);

    /**
     * @brief Displays participant info in the console (for debugging/logging).
     */
    void displayInformation() const;
};

#endif // SECRET_SANTA_PARTICIPANT_H
