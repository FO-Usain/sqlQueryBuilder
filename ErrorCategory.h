//
// Created by fo on 08/01/2021.
//
// This file contains the definition of ErrorCategory.
// Most of the std::error_codes returned by any IQueryBuilder belongs to this category
// Content inherits from the abstract class std::error_category, which makes it compatible with the std::error_code.
// This way, there is a uniform mechanism to address system_errors and errors related Content.


#include <system_error>
#include <map>
#include <string>
#include <algorithm>

#ifndef CONTENTGETTER_ERRORCATEGORY_H
#define CONTENTGETTER_ERRORCATEGORY_H

namespace QueryBuilder {

    class ErrorCategory : public std::error_category {
    public:
        /**
         * @brief: Attempts to get the error-message that maps to the passed error-code.
         * It attempts to find the message in the customErrors first and if found there, returns that value,
         * else, the error-message is searched for through the in-built messages, with the default being 'Unknown error'
         * If the code doesn't match to any message, the default message is returned.
         * @param code: The error-code that has its mapped error-message to be returned
         * @return: std::string: The error-message that maps to passed the error-code
         */
        inline std::string message(int code) const override {
            //search for it in the customErrors
            if (customErrors.find(code)->first) {     //the code has a mapped message in customError
                //return the custom error-message
                return customErrors[code];
            }

            std::string message;        //stores the error-message that is to be returned

            ///go through the default error-messages to find a match
            switch (code) {
                case 0:
                    message = "Ok";
                    break;
                case 1:
                    message = "Unknown query-type";
                    break;
                case 2:
                    message = "Unknown query-conjunction-type";
                    break;
                case 3:
                    message = "Unknown query-operation-type";
                    break;
                case 4:
                    message = "Unknown query-order-direction-type";
                    break;
                case 5:
                    message = "Cannot create query-main-clause";
                    break;
                case 6:
                    message = "Table-name not specified";
                    break;
                case 7:
                    message = "Query-type not specified";
                    break;
                default:
                    message = "Unknown error";
            }

            //return the error-message
            return message;
        }

        /**
         * @brief: gets the name of this error_category
         * @return: const char*: the name of this error_category
         */
        inline const char *name() const noexcept override {
            return "QueryBuilder";
        }

        /**
         * @brief: This function allows a client to map a custom error-code to a message.
         * If the error-code is already mapped to a message, its message is updated to the newly passed message.
         * @param code: The custom error-code
         * @param message: The custom error-message
         */
        static void addError(int code, std::string message) {
            //set the error
            customErrors[code] = std::move(message);
        }

    private:
        static std::map<int, std::string> customErrors;        //assigns an error-code to an errorMessage

    };

    std::map<int, std::string> ErrorCategory::customErrors;        //The custom Errors set by client


    /**
     * @brief: factory function for ErrorCategory
     * @return: ErrorCategory: The created ErrorCategory
     */
    const ErrorCategory &errorCategory() {
        static const ErrorCategory errorCategory;

        return errorCategory;
    }
}       //QueryBuilder

#endif //CONTENTGETTER_ERRORCATEGORY_H
