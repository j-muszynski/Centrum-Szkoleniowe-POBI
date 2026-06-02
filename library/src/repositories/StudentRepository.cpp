#include "repositories/StudentRepository.h"

StudentRepository::StudentRepository(const std::string& filePath, IDataStorage<StudentPtr>& storage)
    : AbstractFileRepository<StudentPtr>(filePath, storage) {}

std::string StudentRepository::reportStudents() const {
    std::string info;
    for (const auto& student : repo) {
        if (student != nullptr) {
            info += student->getInfo();
        }
    }
    return info;
}