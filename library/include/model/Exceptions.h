#ifndef CENTRUMSZKOLENIOWE_EXCEPTIONS_H
#define CENTRUMSZKOLENIOWE_EXCEPTIONS_H

#pragma once
#include <stdexcept>
#include <string>

class OSKException : public std::runtime_error {
public:
    explicit OSKException(const std::string& message);
};

class NotFoundException : public OSKException {
public:
    explicit NotFoundException(const std::string& message);
};

class ValidationException : public OSKException {
public:
    explicit ValidationException(const std::string& message);
};

class NullPointerException : public OSKException {
public:
    explicit NullPointerException(const std::string& message);
};

class InvalidInputException : public OSKException {
public:
    explicit InvalidInputException(const std::string& message);
};

#endif