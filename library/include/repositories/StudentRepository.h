#pragma once
#include "AbstractFileRepository.h"
#include "model/Student.h"

#include "typedefs.h"

class StudentRepository : public AbstractFileRepository<StudentPtr> {
public:
    StudentRepository(const std::string& filePath, IDataStorage<StudentPtr>& storage);
    std::string reportStudents() const;
};