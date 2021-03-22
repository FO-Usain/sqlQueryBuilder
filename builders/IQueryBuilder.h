//
// Created by fo on 21/03/2021.
//
// This file contains the definition of the interface IQueryBuilder, which is responsible for building SQL queries
#include <string>
#include <vector>
#include <map>
#include <memory>

#ifndef DBQUERYBUILDER_IQUERYBUIKDER_H
#define DBQUERYBUILDER_IQUERYBUIKDER_H

namespace DBQueryBuilder {

    /**
     * @brief: the enumeration of all actions, relating to CRUD, that can be performed via a SQL query
     */
    enum Action {
        INSERT,
        SELECT,
        UPDATE,
        DELETE,
        NIL     //no action
    };

    /**
     * @brief: the enumeration of the order by which a SQL-query can affect database-entities
     */
    enum Order {
        ASC,     //ascend
        DEC     //Descend
    };

    namespace Type {
        /**
         * @brief: check types/QueryCondition.h
         */
        class QueryCondition;
    }       //Type

    namespace Builder {

        class IQueryBuilder {
        public:
            using Ptr = std::shared_ptr<IQueryBuilder>;

            /**
             * @brief: sets the name of the database-table that the query being built is targeted at
             * @param tableName
             * @return int: custom error-number
             */
            virtual int setTableName(const std::string &tableName) = 0;

            /**
             * @brief: sets the action that the query being built is intended to perform
             * @param action
             * @return int: custom error-number
             */
            virtual int setAction(const Action &action) = 0;

            /**
             * @brief: sets the columns that the query being built is targeted at
             * @param columns
             * @return int: custom error-number
             */
            virtual int setTargetColumns(const std::vector<std::string> &columns) = 0;

            /**
             * @brief: adds a guard-condition to the query being build
             * @param qCondition: the guard-condition being added to the query being built
             * @return int: custom error-number
             */
            virtual int addCondition(const std::shared_ptr<Type::QueryCondition> &qCondition) = 0;

            /**
             * @brief: adds a parenthesis-delimited composite QueryCondition, from the passed QueryConditions, to the query being built
             * @param qConditions: the collection of QueryConditions that the composite QueryCondition should be formed from
             * @return int: custom error-number
             */
            virtual int addCompositeCondition(const std::vector<std::shared_ptr<Type::QueryCondition>> qConditions) = 0;

            /**
             * @brief: adds the passed Resource-representation into the query being built
             * @param resourceRep: the Resource-representation that is to be added to the query being built
             * @return  int: custom error-number
             */
            virtual int addResourceRep(const std::map<std::string, std::string> &resourceRep) = 0;

            /**
             * @brief: sets the maximum number of database-entities that the query being build should affect
             * @param limit
             * @return
             */
            virtual int setLimit(const int &limit) = 0;

            /**
             * @brief: sets the Column by which the influence of the query being built should be ordered, in addition to the direction of influence, in respect to that column
             * @param columnName: the Column by which the influence of the query should be ordered
             * @param order: the direction of influence of the query, in respect the passed Column-name
             * @return int: custom error-number
             */
            virtual int setOrder(const std::string &columnName, const Order &order) = 0;

            /**
             * @brief: returns the query that has been built thus far
             * @return: the query that has been built thus far
             */
            virtual std::string buildQuery() = 0;

            virtual ~IQueryBuilder() = default;
        };

    }
}

#endif //DBQUERYBUILDER_IQUERYBUIKDER_H
