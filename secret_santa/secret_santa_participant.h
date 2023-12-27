#ifndef SECRET_SANTA_PARTICIPANT_H
#define SECRET_SANTA_PARTICIPANT_H

#include <string>
#include <vector>

class SecretSantaParticipant {
private:
    std::string name;
    std::string address;
    std::string interests;
    SecretSantaParticipant* assignedParticipant; // Pointer to the assigned participant
    std::vector<SecretSantaParticipant*> cannotBeAssignedTo; // List of participants they cannot be assigned to

public:
    // Constructors
    SecretSantaParticipant(const std::string& name, const std::string& address, const std::string& interests);
    SecretSantaParticipant(const std::string& name);
    SecretSantaParticipant();

    ~SecretSantaParticipant() = default;

    // Member functions to access private members
    std::string getName() const;
    std::string getAddress() const;
    std::string getInterests() const;
    SecretSantaParticipant* getAssignedParticipant() const;
    std::vector<SecretSantaParticipant*> getCannotBeAssignedTo();

    void setName(std::string nameString);
    void setAddress(std::string addressString);
    void setInterests(std::string interestsString);

    // Assign a participant to the current participant
    void assignParticipant(SecretSantaParticipant* participant);

    // Add a participant to the list of those they cannot be assigned to
    void addCannotBeAssignedTo(SecretSantaParticipant* participant);

    // Display participant information
    void displayInformation() const;
};
#endif // SECRET_SANTA_PARTICIPANT_H
