#include "managers/InstructorManager.h"
#include "repositories/InstructorRepository.h"
#include "model/Instructor.h"
#include "model/Exceptions.h"

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

void InstructorManager::registerInstructor(const InstructorPtr& instructor) {
    if (instructor == nullptr) {
        throw NullPointerException("Cannot register instructor: Provided pointer is null.");
    }

    instructorRepo.add(instructor);
}

void InstructorManager::unregisterInstructor(int instructorID) {
    if (instructorID < 0) {
        throw InvalidInputException("Instructor ID cannot be negative.");
    }

    InstructorPtr instructor = getInstructorById(instructorID);

    if (instructor == nullptr) {
        throw NotFoundException(
            "Cannot unregister instructor: Instructor with ID " + std::to_string(instructorID) + " not found.");
    }

    instructorRepo.remove(instructorID);
    archiveInstructorRepo.add(instructor);
}

void InstructorManager::assignToCategory(int instructorID, Category category) const {
    if (instructorID < 0) {
        throw InvalidInputException("Instructor ID cannot be negative.");
    }

    InstructorPtr instructor = getInstructorById(instructorID);
    if (instructor == nullptr) {
        throw NotFoundException(
            "Cannot assign category: Instructor with ID " + std::to_string(instructorID) + " not found.");
    }

    instructor->addQualification(category);
}

std::string InstructorManager::getInstructorReport() const {
    const auto& concreteRepo = dynamic_cast<const InstructorRepository&>(instructorRepo);

    return concreteRepo.reportInstructors();
}

std::string InstructorManager::getArchiveInstructorReport() const {
    const auto& concreteRepo = dynamic_cast<const InstructorRepository&>(archiveInstructorRepo);

    return concreteRepo.reportInstructors();
}

InstructorPtr InstructorManager::getInstructorById(int instructorId) const {
    return instructorRepo.getById(instructorId);
}

InstructorPtr InstructorManager::getArchiveInstructorById(int instructorId) const {
    return archiveInstructorRepo.getById(instructorId);
}
