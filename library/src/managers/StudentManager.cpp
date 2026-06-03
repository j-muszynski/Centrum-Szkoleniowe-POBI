#include "managers/StudentManager.h"

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
    std::vector<StudentPtr> v = findStudents([studentID](StudentPtr std) {
        return std->getId() == studentID;
    });

    if (v.empty()) return;

    StudentPtr student = v[0];

    studentRepo.remove(studentID);
    archiveStudentRepo.add(student);
}

void StudentManager::updateExamStatus(int studentID) {
    std::vector<StudentPtr> v = findStudents([studentID](StudentPtr std) {
        return std->getId() == studentID;
    });

    if (v.empty()) return;

    StudentPtr student = v[0];

    student->setReadyForExam(true);
    unregisterStudent(studentID); // student that finished his course is supposed to be in archive repo
}

std::string StudentManager::getStudentReport() const {
    std::string report;
    const auto& students = studentRepo.findAll();

    if (students.empty()) {
        report += "No students found.\n";
        return report;
    }

    for (const auto& std : students) {
        if (std != nullptr) {
            report += std->getInfo() + "\n";
        }
    }
    return report;
}
std::string StudentManager::getArchiveStudentReport() const {
    std::string report;
    const auto& students = archiveStudentRepo.findAll();

    if (students.empty()) {
        report += "No students found.\n";
        return report;
    }

    for (const auto& std : students) {
        if (std != nullptr) {
            report += std->getInfo() + "\n";
        }
    }
    return report;
}
