//
// Created by fo on 09/01/2021.
//
// This file contains the definition of the interface IMainClauseCreator.
// IMainClauseCreator has it purpose to be to create and return the main-clause of a MYSQL query

#include "../../Error.h"

#include <string>
#include <memory>

#ifndef QUERYBUILDER_IMAINCLAUSECREATOR_H
#define QUERYBUILDER_IMAINCLAUSECREATOR_H

namespace QueryBuilder {
    namespace MYSQLQueryBuilder {
        namespace MainClauseCreator {

            class IMainClauseCreator {
            public:
                using Ptr = std::shared_ptr<IMainClauseCreator>;

                /**
                 * @brief: creates and returns a MYSQL query's main-clause
                 * @return: std::string: the MYSQL query's main-clause
                 */
                virtual std::string createMainClause() = 0;
            };

        }       //MainClauseCreator
    }       //MYSQLQueryBuilder
}       //QueryBuilder

#endif //QUERYBUILDER_IMAINCLAUSECREATOR_H
