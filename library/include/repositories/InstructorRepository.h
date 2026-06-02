#pragma once
#include "AbstractFileRepository.h"
#include "model/Instructor.h"

#include "typedefs.h"

class InstructorRepository : public AbstractFileRepository<InstructorPtr> {
public:
    InstructorRepository(const std::string& filePath, IDataStorage<InstructorPtr>& storage);
    std::string reportInstructors() const;
};