#include "repositories/InstructorRepository.h"

InstructorRepository::InstructorRepository(const std::string& filePath, IDataStorage<InstructorPtr>& storage)
    : AbstractFileRepository<InstructorPtr>(filePath, storage) {}

std::string InstructorRepository::reportInstructors() const {
    std::string info;
    for (const auto& instructor : repo) {
        if (instructor != nullptr) {
            info += instructor->getInfo();
        }
    }
    return info;
}