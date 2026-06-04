#include "managers/StudentManager.h"
#include "repositories/StudentRepository.h"

#include "model/Student.h"

StudentManager::StudentManager(IRepository<StudentPtr>& repo, IRepository<StudentPtr>& archiveRepo) :
    studentRepo(repo), archiveStudentRepo(archiveRepo) {}

std::vector<StudentPtr> StudentManager::findStudents(const StudentPredicate& predicate) const {
    return studentRepo.findBy(predicate);
}

std::vector<StudentPtr> StudentManager::findArchiveStudents(const StudentPredicate& predicate) const {
    return archiveStudentRepo.findBy(predicate);
}

std::vector<StudentPtr> StudentManager::findAllStudents() const {
    return studentRepo.findAll();
}

std::vector<StudentPtr> StudentManager::findAllArchiveStudents() const {
    return archiveStudentRepo.findAll();
}

void StudentManager::registerStudent(StudentPtr student) {
    if (student == nullptr) return;

    studentRepo.add(student);
}

void StudentManager::unregisterStudent(int studentID) {
    StudentPtr student = getStudentById(studentID);

    studentRepo.remove(studentID);
    archiveStudentRepo.add(student);
}

void StudentManager::updateExamStatus(int studentID) {
    StudentPtr student = getStudentById(studentID);

    student->setReadyForExam(true);
    unregisterStudent(studentID); // student that finished his course is supposed to be in archive repo
}

std::string StudentManager::getStudentReport() const {
    const auto& concreteRepo = dynamic_cast<const StudentRepository&>(studentRepo);

    return concreteRepo.reportStudents();
}
std::string StudentManager::getArchiveStudentReport() const {
    const auto& concreteRepo = dynamic_cast<const StudentRepository&>(archiveStudentRepo);

    return concreteRepo.reportStudents();
}

StudentPtr StudentManager::getStudentById(int studentId) const {
    return studentRepo.getById(studentId);
}

StudentPtr StudentManager::getArchiveStudentById(int studentId) const {
    return archiveStudentRepo.getById(studentId);
}