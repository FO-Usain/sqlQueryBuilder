//
// Created by fo on 08/01/2021.
//
// This file contains the definition of MYSQLQueryBuilder
//
// As the name implies, MYSQLQueryBuilder, builds MYSQl queries.s
// MYSQLQueryBuilder realizes IQueryBuilder interface and also implements the Builder Design pattern.
// Every MYSQL query has a main clause( the part of the query that can stand alone and still make sense).
// MYSQLQueryBuilder delegates the creation of such the main clauses to the IMainClauseCreator interface.
// MYSQLQueryBuilder has a factory member-function that returns a an instance of an implementer of IMainClauseCreator, based on the argument passed to it.
// When the getQuery() member-function is called on MYSQLQueryBuilder, depending on the query-type(instance of QueryBuilder::QueryType),
// MYSQLQuery creates an IMainClauseCreator and invokes the identifying member-function and then appends the remaining sub-ordinate clause(e.g. 'where' clause) if any exists.s

#include "../ErrorCategory.h"
#include "../IQueryBuilder.h"

#include "mainClauseCreator/MainClauseCreators.h"

#include <map>
#include <sstream>

#ifndef CONTENTGETTER_MYSQLQUERYBUILDER_H
#define CONTENTGETTER_MYSQLQUERYBUILDER_H

namespace QueryBuilder {
    namespace MYSQLQueryBuilder {

        class MYSQLQueryBuilder : public IQueryBuilder {
        public:
            using Ptr = std::shared_ptr<MYSQLQueryBuilder>;

//            inline MYSQLQueryBuilder(MYSQL *mysql) : _mysql(mysql) {}
            MYSQLQueryBuilder() {}

            /**
             * @brief: check queryBuilder/IQueryBuilder.h
             * @return
             */
            std::string getQuery() override;

            /**
             * @brief: check queryBuilder/IQueryBuilder.h
             * @return
             */
            std::error_code setTableName(const std::string &tableName) override;

            /**
             * @brief: check queryBuilder/IQueryBuilder.h
             * @return
             */
            std::error_code addTargetFields(const std::vector <std::string> &fields) override;

            /**
             * @brief: check queryBuilder/IQueryBuilder.h
             * @return
             */
            std::error_code setQueryType(const QueryType &queryType) override;

            /**
             * @brief: check queryBuilder/IQueryBuilder.h
             * @return
             */
            std::error_code
            addCondition(const Conjunction &conjunction, const std::string &field, const Operation &operation,
                         const bool &negate, const std::string &value) override;

            /**
             * @brief: check queryBuilder/IQueryBuilder.h
             * @return
             */
            std::error_code setOrder(const std::string &field, const Order &direction) override;

            /**
             * @brief: check queryBuilder/IQueryBuilder.h
             * @return
             */
            void setLimit(const int &limit = 0) override;


        private:
            using IMainClauseCreator = QueryBuilder::MYSQLQueryBuilder::MainClauseCreator::IMainClauseCreator;
            using SELECTClauseCreator = QueryBuilder::MYSQLQueryBuilder::MainClauseCreator::SELECTClauseCreator;


            std::string formRegex(const std::string &pattern);

            /**
             * @brief: creates and returns an instance of an implementer of IMainClauseCreator, depending on the argument passed to it
             * @param queryType: The query-type the IMainClauseCreator is to handle
             * @return: IMainClauseCreator : The IMainClauseCreator
             */
            inline IMainClauseCreator::Ptr createMainClauseCreator(QueryType &queryType) {
                switch (queryType) {
                    case QueryType::SELECT:
                        return std::shared_ptr<IMainClauseCreator>(new SELECTClauseCreator(_tableName, _targetFields));
                        break;
                    default:
                        //return an empty ptr
                        return std::shared_ptr<IMainClauseCreator>(nullptr);
                }
            }

            /**
             * @brief: appends the 'where' clause to the query being built
             */
            inline void appendWhereClause() {
                std::stringstream whereClause;

                whereClause << " where";

                //add all the conditions to the clause
                for (const std::string &condition : _queryConditions) {
                    //append this condition to the where-clause
                    whereClause << condition << ((*_queryConditions.rbegin() == condition) ? " " : "");
                }

                //append the where clause to the clause
                _query += " " + whereClause.str();
            }


            /**
             * @brief: creates a new custom-error, from the code and message passed, and returns a std::error_code of category QueryBuilder::ErrorCategory that points to that error
             * @param code: the number-identifier of the error
             * @param message: the message of the error
             * @return: std::error_code: contains the newly formed error
             */
            inline std::error_code getCustomError(int code, std::string message) {
                //add the error to the CustomErrors
                ErrorCategory::addError(code, message);

                return std::error_code(code, errorCategory());
            }

            std::string _tableName;     //The formatted name of the targeted-table
            std::vector <std::string> _targetFields;        // The vector of formatted target-fields
            QueryType _queryType;       //The type of the query
            std::vector <std::string> _queryConditions;     // The vector of all formatted conditions of the query
            std::string _order;     //The order of the affected rows of the query
            int _limit{0};      //The limit of the affected rows of the query

            IMainClauseCreator::Ptr mainClauseCreator{nullptr};     //The creator of the main-clause of the query
            std::string _query;     //The query.

//            MYSQL *_mysql;
        };
    }       //MYSQLQueryCreator
}       //QueryBuilder

#endif //CONTENTGETTER_MYSQLQUERYBUILDER_H