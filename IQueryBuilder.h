//
// Created by fo on 08/01/2021.
//
// This file contains the definition of the interface IQueryBuilder, the interface realized by all QueryBuilders of the system.

#include <string>
#include <vector>
#include <system_error>
#include <memory>

#ifndef CONTENTGETTER_IQUERYBUILDER_H
#define CONTENTGETTER_IQUERYBUILDER_H

namespace QueryBuilder {

    /**
     * @brief: The enumeration of all the queries that are presently recognizable by  ideal IQueryBuilders.
     * If the concerned IQueryBuilder does not recognize it, it returns a std::error_code of category QueryBuilder::ErrorCategory, where std::error_code::value() = 1
     */
    enum QueryType {
        NIL,
        INSERT,
        SELECT,
        UPDATE,
        DELETE,
    };

    /**
     * @brief: The enumeration of all the conjunctions that can bind a new query-condition to the 'where' clause of the query.
     * If the concerned IQueryBuilder does not recognize it, it returns a std::error_code of category QueryBuilder::ErrorCategory, where std::error_code::value() = 2
     */
    enum Conjunction {
        AND,
        OR
    };

    /**
     * @brief: The enumeration of all the operations that can define the conditions of the 'where' clause of the query being built.
     * If the concerned IQueryBuilder does not recognize it, it returns a std::error_code of category QueryBuilder::ErrorCategory, where std::error_code::value() = 3
     */
    enum Operation {
        EQUAL_TO,
        GREATER_THAN,
        LESSER_THAN,
        GREATER_THAN_OR_EQUAL_TO,
        LESSER_THAN_OR_EQUAL_TO,
        REGEX
    };

    /**
     * @brief: The enumeration of all the supported direction for the 'order' clause of a query.
     * If the concerned IQueryBuilder does not recognize it, it returns a std::error_code of the category QueryBuilder::ErrorCategory, where std::error_code::value() = 4
     */
    enum Order {
        ASCEND,
        DESCEND
    };

    class IQueryBuilder {
    public:
        using Ptr = std::shared_ptr<IQueryBuilder>;

        /**
         * @brief: returns the built query. Throws a QueryBuilder::Error exception if any error occurs.
         * This should be the last member-function called(that is, after every component of the query has been added)
         * @return: std::string: The built query
         */
        virtual std::string getQuery() = 0;

        /**
         * @brief: Sets the target table-name for the query to tableName and returns a std::error-code of QueryBuilder::ErrorCategory category, indicating whether or not the name was set successfully.
         * If the std::error_code::value() is not 0, then operation was not successful, because the system detects that tableName is unsafe to use(contains malicious string)
         * @param tableName: The value to set for the target table-name.
         * @return: std::error_code: reports the status of the operation.
         */
        virtual std::error_code setTableName(const std::string &tableName) = 0;

        /**
         * @brief: adds to elements of fields(the argument) to the list of fields to be affected by the query(target-fields).
         * If this method has been called successfully before, the contents are added to the existing ones.
         * If the system detects malicious content in any of them, it aborts the operation, adding non of them, and returns with a std::error_code indicating the error.
         * @param fields: The vector of fields to add to the list of affected fields.
         * Note that the system is unable to detect whether or not the fields exists in the target-table and does not test for that. It is left to the client's hands to pass valid fields.
         * @return: std::error_code: reports the status of the operation.
         */
        virtual std::error_code addTargetFields(const std::vector <std::string> &fields) = 0;


        /**
         * @brief:
         * @param fieldName
         * @param value
         * @return
         */
        virtual std::error_code setValuedField(const std::string &fieldName, const std::string &value) = 0;

        /**
         * @brief: sets the kind of operation the query will query the database for. It should support all query types such as SELECT, INSERT, DELETE, e.t.c.
         * if the passed argument is not a recognized query-type, it returns a std::error_code of QueryBuilder::ErrorCategory category, where std::error_code::value() = 1, indicating that the query-type is unknown.
         * @param queryType: The query-type the query-type of the query is to be set to.
         * @return: std::error_code: reports the status of the operation.
         */
        virtual std::error_code setQueryType(const QueryType &queryType) = 0;

        /**
         * @brief: appends a new condition to the 'where' clause of the query.
         * If any error occurs, an appropriate std::error_code of QueryBuilder::ErrorCategory is returned.
         * @param conjunction: The conjunction between the new condition and the already existing 'where' clause.
         * If this function is has not been called before, this argument has no effect.
         * But no matter the number of times(even if never) the function has been called, an a std::error_code, reporting that the Conjunction is unknown, is passed if the system does not recognize the passed value
         * @param field: The field the of the target-table that this condition is affecting.
         * Note that the system is unable to detect whether or not the fields exists in the target-table and does not test for that. It is left to the client's hands to pass valid fields
         * @param operation: The operation that relates field to value-argument. If the passed operation is not recognized, the appropriate std::error_code is returned
         * @param negate: True if the operation should be negated and false, if not.
         * @param value: The value that field is being tested for
         * @return: std::error_code: reports the status of the operation.
         */
        virtual std::error_code addCondition(const Conjunction &conjunction, const std::string &field, const Operation &operation,
                                  const bool &negate, const std::string &value) = 0;

        /**
         * @brief: set the order of the affected rows. Returns a non-success error if direction is not recognized and if the system detects malicious contents within the field
         * @param field: The field that determines the order
         * Note that the system is unable to detect whether or not the fields exists in the target-table and does not test for that. It is left to the client's hands to pass valid fields
         * @param direction: The direction of the order
         * @return: std::error_code: reports the status of the operation
         */
        virtual std::error_code setOrder(const std::string &field, const Order &direction) = 0;

        /**
         * @brief: sets the maximum number of affected rows
         * @param limit: The number of rows to be affected by the operation. This function should be called when the query being built is a retrieval one.
         */
        virtual void setLimit(const int &limit) = 0;
    };

}       //QueryBuilder

#endif //CONTENTGETTER_IQUERYBUILDER_H
