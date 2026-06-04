#include "managers/InstructorManager.h"

#include "model/Instructor.h"

InstructorManager::InstructorManager(IRepository<InstructorPtr>& repo, IRepository<InstructorPtr>& archiveRepo) :
    instructorRepo(repo), archiveInstructorRepo(archiveRepo) {}

std::vector<InstructorPtr> InstructorManager::findInstructors(const InstructorPredicate& predicate) const {
    return instructorRepo.findBy(predicate);
}

std::vector<InstructorPtr> InstructorManager::findArchiveInstructors(const InstructorPredicate& predicate) const {
    return archiveInstructorRepo.findBy(predicate);
}

std::vector<InstructorPtr> InstructorManager::findAllInstructors() const {
    return instructorRepo.findAll();
}

std::vector<InstructorPtr> InstructorManager::findAllArchiveInstructors() const {
    return archiveInstructorRepo.findAll();
}

void InstructorManager::registerInstructor(InstructorPtr instructor) {
    if (instructor == nullptr) return;

    instructorRepo.add(instructor);
}

void InstructorManager::unregisterInstructor(int instructorID) {
    InstructorPtr instructor = getInstructorById(instructorID);

    instructorRepo.remove(instructorID);
    archiveInstructorRepo.add(instructor);
}

void InstructorManager::assignToCategory(int instructorID, Category category) {
    InstructorPtr instructor = getInstructorById(instructorID);

    instructor->addQualification(category);
}
std::string InstructorManager::getInstructorReport() const {
    std::string report;
    const auto& instructors = instructorRepo.findAll();

    if (instructors.empty()) {
        report += "No instructors found.\n";
        return report;
    }

    for (const auto& ins : instructors) {
        if (ins != nullptr) {
            report += ins->getInfo() + "\n";
        }
    }
    return report;
}
std::string InstructorManager::getArchiveInstructorReport() const {
    std::string report;
    const auto& instructors = archiveInstructorRepo.findAll();

    if (instructors.empty()) {
        report += "No instructors found.\n";
        return report;
    }

    for (const auto& ins : instructors) {
        if (ins != nullptr) {
            report += ins->getInfo() + "\n";
        }
    }
    return report;
}

InstructorPtr InstructorManager::getInstructorById(int instructorId) const {
    return instructorRepo.getById(instructorId);
}

InstructorPtr InstructorManager::getArchiveInstructorById(int instructorId) const {
    return archiveInstructorRepo.getById(instructorId);
}