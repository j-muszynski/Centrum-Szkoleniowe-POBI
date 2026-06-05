#pragma once

#include "typedefs.h"
#include "repositories/IRepository.h"
#include <vector>
#include <string>

/**
 * @class StudentManager
 * @brief Handles business logic for student management, status, and reporting.
 */
class StudentManager {
private:
    /** @brief Reference to the main student repository */
    IRepository<StudentPtr>& studentRepo;

    /** @brief Reference to the archive student repository */
    IRepository<StudentPtr>& archiveStudentRepo;

public:
    /**
     * @brief Constructs the StudentManager with the required repositories.
     * @param repo Reference to an implementation of IRepository managing StudentPtr.
     * @param archiveRepo Reference to an implementation of IRepository managing StudentPtr.
     */
    StudentManager(IRepository<StudentPtr>& repo, IRepository<StudentPtr>& archiveRepo);

    ~StudentManager() = default;

    /**
     * @brief Filters students based on a custom predicate function.
     * @param predicate A callable boolean filter evaluation function.
     * @return std::vector<StudentPtr> List of students matching the predicate.
     */
    std::vector<StudentPtr> findStudents(const StudentPredicate& predicate) const;

    /**
     * @brief Filters archive students based on a custom predicate function.
     * @param predicate A callable boolean filter evaluation function.
     * @return std::vector<StudentPtr> List of archive students matching the predicate.
     */
    std::vector<StudentPtr> findArchiveStudents(const StudentPredicate& predicate) const;


    /**
     * @brief Retrieves all students currently registered in the system.
     * @return std::vector<StudentPtr> List containing all students.
     */
    std::vector<StudentPtr> findAllStudents() const;

    /**
     * @brief Retrieves all students unregistered in the system.
     * @return std::vector<StudentPtr> List containing all students.
     */
    std::vector<StudentPtr> findAllArchiveStudents() const;

    /**
     * @brief Registers a new student in the repository using a generated unique ID.
     * @param student Student shared_pointer, which will be registered into system.
     */
    void registerStudent(const StudentPtr& student);

    /**
     * @brief Unregisters student which is already in the repository.
     * @param studentID id of student which is currently in studentRepo.
     */
    void unregisterStudent(int studentID);

    /**
    * @brief Sets students status isReadyForExam as true and moves the student to archive repo.
    * @param studentID id of student which is currently in studentRepo.
    */
    void updateExamStatus(int studentID);

    /**
     * @brief Generates a text report of all students.
     * @return std::string Formatted report string.
     */
    std::string getStudentReport() const;

    /**
     * @brief Generates a text report of all archive students.
     * @return std::string Formatted report string.
     */
    std::string getArchiveStudentReport() const;

    /**
     * @brief Retrieves a student entity by its unique identifier.
     * @param studentId The unique ID of the target student.
     * @return StudentPtr A shared pointer to the Student object, or nullptr if not found.
     */
    StudentPtr getStudentById(int studentId) const;

    /**
     * @brief Retrieves an archive student entity by its unique identifier.
     * @param studentId The unique ID of the target student.
     * @return StudentPtr A shared pointer to the Student object, or nullptr if not found.
     */
    StudentPtr getArchiveStudentById(int studentId) const;
};