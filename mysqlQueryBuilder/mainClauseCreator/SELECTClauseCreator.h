//
// Created by fo on 09/01/2021.
//
// This file contains the definition of SELECTClauseCreator.
// SELECTClauseCreator is responsible for creating the main clause for mysql queries of query-type QueryBuilder::QueryType::SELECT

#include "IMainClauseCreator.h"
#include <vector>
#include <sstream>


#ifndef QUERYBUILDER_SELECTCLAUSECREATOR_H
#define QUERYBUILDER_SELECTCLAUSECREATOR_H

namespace QueryBuilder {
    namespace MYSQLQueryBuilder {
        namespace MainClauseCreator {

            class SELECTClauseCreator : public IMainClauseCreator {
            public:
                using Ptr = std::shared_ptr<SELECTClauseCreator>;

                /**
                 * @brief: constructor: creates the main-clause, using its passed arguments. If tableName is empty, it throws a QueryBuilder::Error exception indicating that
                 * @param tableName: the name of the target-table.
                 * @param targetFields: a vector of tall the target-fields
                 */
                inline SELECTClauseCreator(const std::string &tableName, const std::vector<std::string> &targetFields = {}) {
                    if (tableName.empty()) {        //there is no table name
                        //Throw an exception specifying that no tableName was passed
                        throw QueryBuilder::Error(std::error_code(6, QueryBuilder::errorCategory()));
                    }

                    _mainClause << "SELECT";

                    //append the fields to be affected on the target-Table
                    if (targetFields.empty()) {     //there are no particular target-fields
                        _mainClause << " *";
                    } else {
                        //make a comma-separated appendage of the fields to the mainClause
                        for (const std::string &targetField : targetFields) {
                            _mainClause << ((targetFields[0] == targetField) ? "" : ",") << " " << targetField;
                        }
                    }

                    //append the tableName phrase
                    _mainClause << " from " << tableName;
                }

                /**
                 * @brief: returns the created main-clause
                 * @return: std::string: the created main-clause
                 */
                inline std::string createMainClause() override {
                    return _mainClause.str();
                }

            private:
                std::stringstream _mainClause;
            };

        }       //MainClauseCreator
    }       //MYSQLQueryBuilder
}       //QueryBuilder

#endif //QUERYBUILDER_SELECTCLAUSECREATOR_H
