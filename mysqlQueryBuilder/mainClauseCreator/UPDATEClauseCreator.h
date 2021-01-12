//
// Created by fo on 12/01/2021.
//

#include "IMainClauseCreator.h"
#include <sstream>

#ifndef QUERYBUILDER_UPDATECLAUSECREATOR_H
#define QUERYBUILDER_UPDATECLAUSECREATOR_H

namespace QueryBuilder {
    namespace MYSQLQueryBuilder {
        namespace MainClauseCreator {

            class UPDATEClauseCreator : public IMainClauseCreator {
            public:
                using Ptr = std::shared_ptr<UPDATEClauseCreator>;

                inline UPDATEClauseCreator(const std::string &tableName) {
                    _mainClause << "UPDATE";

                    //append the tableName
                    _mainClause << " " << tableName;
                }

                inline std::string createMainClause() override {
                    return _mainClause.str();
                }

            private:
                std::stringstream _mainClause;
            };

        }       //MainClauseCreator
    }       //MYSQLQueryBuilder
}       //QueryBuilder

#endif //QUERYBUILDER_UPDATECLAUSECREATOR_H
