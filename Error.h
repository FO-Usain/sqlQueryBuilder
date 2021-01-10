//
// Created by fo on 09/01/2021.
//
// This file contains the definition of Error, the exception, which QueryBuilder::ErrorCategory belongs to.

#include "ErrorCategory.h"

#include <exception>
#include <string>

#ifndef QUERYBUILDER_ERROR_H
#define QUERYBUILDER_ERROR_H

namespace QueryBuilder {

    class Error : std::exception {
    public:
        inline Error(unsigned int code, std::string message): _code(code), _message(message) {}
        inline Error(const std::error_code &errorCode) {
            _code = errorCode.value();
            _message = errorCode.message();
        }

        /**
         * @brief: Returns the error-message, using the mechanism of std::exception
         * @return: The error-message associated with this DB exception
         */
        inline const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
            return _message.c_str();
        }

        /**
         * @brief: returns the std::error_code that is associated with this DB exception
         * @return: std::error_code: the error_code associated with this DB exception
         */
        inline std::error_code code() const {
            return std::error_code(_code, ErrorCategory());
        }


    private:
        unsigned int _code;
        std::string _message;
    };

}

#endif //QUERYBUILDER_ERROR_H
