//
// Created by fo on 21/03/2021.
//
// This file contains the definition of CompQueryCondition, a descendant of QueryCondition
// CompQueryCondition stands for CompositeQueryCondition, a composite QueryCondition formed from the combination of two or more QueryConditions
#include "QueryCondition.h"
#include <vector>

#ifndef DBQUERYBUILDER_COMPQUERYCONDITION_H
#define DBQUERYBUILDER_COMPQUERYCONDITION_H

namespace DBQueryBuilder::Type {

    class CompQueryCondition : public QueryCondition {
    public:
        using Ptr = std::shared_ptr<CompQueryCondition>;

        CompQueryCondition(const std::vector<QueryCondition::Ptr> &conditions);

        /**
         * @brief:  converts this QueryCondition to its string form
         * @param addFirstConj: true, if the conjunction of the first QueryCondition should be added to the string-form of this QueryCondition
         * @return std::string: the string form of this QueryCondition
         */
        std::string toString(const bool &addFirstConj = true) override;

        /**
         *
         * @return
         */
        std::string strConj() override;

        ~CompQueryCondition() override;

    private:
        /**
         * @brief: the core of CompQueryConditions
         */
        class Core;

        Core *_core;        //pimpl
    };

}

#endif //DBQUERYBUILDER_COMPOSITEQUERYCONDITION_H
