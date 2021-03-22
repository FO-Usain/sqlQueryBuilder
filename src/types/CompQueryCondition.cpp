//
// Created by fo on 21/03/2021.
//
// this file contains the definition of all the member-functions and nested class of CompQueryCondition
#include "../../types/CompQueryCondition.h"

using CompQueryCondition = DBQueryBuilder::Type::CompQueryCondition;

class CompQueryCondition::Core {
    std::vector<QueryCondition::Ptr> _queryConditions;      //The collection of QueryConditions to aggregate
    std::string firstConj;      //the string form of the conjunction of the first QueryCondition within this CompQueryCondition

public:
    inline Core(std::vector<QueryCondition::Ptr> queryConditions) : _queryConditions(queryConditions) {
        //initialize the firstConj
        firstConj = queryConditions[0]->strConj();
    }

    inline std::string toString(const bool &addFirstConj) {
        //begin the construction of the string-form of this QueryCondition
        std::string condition;
        int counter = 0;        //the number of iterations through the concerned loop, so far

        if (addFirstConj) {     //the conjunction of the first component-query should be added
            for (QueryCondition::Ptr qCondition : _queryConditions)  {
                condition += (++counter == 1) ? firstConj : " ";
                condition += (counter == 1) ? " (" : "";
                condition += (counter == 1) ? qCondition->toString(false) : qCondition->toString(true);
            }
        } else {        //the conjunction of the first component-query should not be added
            condition += "(";
            for (QueryCondition::Ptr qCondition : _queryConditions)  {
                condition += (++counter == 1) ? "" : " ";
                condition += (counter == 1) ? qCondition->toString(false) : qCondition->toString(true);
            }
        }

        //close the composite query-condition
        condition += ")";

        return condition;
    }

    inline std::string strConj() {
        return firstConj;
    }
};

CompQueryCondition::CompQueryCondition(const std::vector<QueryCondition::Ptr> &conditions) {
    _core = new Core(conditions);
}

std::string CompQueryCondition::toString(const bool &addFirstConj) {
    return _core->toString(addFirstConj);
}

std::string CompQueryCondition::strConj() {
    return _core->strConj();
}

CompQueryCondition::~CompQueryCondition() {
    delete _core;
}