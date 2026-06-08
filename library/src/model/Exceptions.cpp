#include "model/Exceptions.h"

OSKException::OSKException(const std::string& message) : std::runtime_error(message) {}
NotFoundException::NotFoundException(const std::string& message) : OSKException(message) {}
ValidationException::ValidationException(const std::string& message) : OSKException(message) {}
NullPointerException::NullPointerException(const std::string& message) : OSKException(message) {}
InvalidInputException::InvalidInputException(const std::string& message) : OSKException(message) {}