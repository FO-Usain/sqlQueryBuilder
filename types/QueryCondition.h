//
// Created by fo on 21/03/2021.
//
// This file contains the definition of QueryCondition, the abstraction of a guard-condition that a SQL-query may embody
#include <string>
#include <memory>

#ifndef DBQUERYBUILDER_QUERYCONDITION_H
#define DBQUERYBUILDER_QUERYCONDITION_H

namespace DBQueryBuilder::Type {

    class QueryCondition {
    public:
        using Ptr = std::shared_ptr<QueryCondition>;

        /**
         * @brief: the enumeration of all conjunctions that bind QueryConditions to "where"-clauses
         */
        enum Conjunction {
            AND,
            OR
        };

        /**
         * @brief: the enumeration of all operators that can operate on operands of QueryCondition
         */
        enum Operator {
            E,      //equal
            L,      //lesser
            G,      //greater
            LE,     //lesser than or equal
            GE,     //greater than or equal
        };

        QueryCondition(const Conjunction &conjunction, const std::string &field, const Operator &_operator, const bool &negate, const std::string value);
        QueryCondition();

        /**
         * @brief: converts this QueryCondition to its string form
         * @param addConj; true, if the conjunction should be added to the string-form of this QueryCondition
         * @return std::string: the string form of this QueryCondition
         */
        virtual std::string toString(const bool &addConj = true);

        /**
         * @brief: returns the string form of the conjunction that binds this QueryCondition to a "where"-clause
         * @return
         */
        virtual std::string strConj();

        virtual ~QueryCondition();
    private:
        class Core;     //the core of QueryConditions

        Core *_core;
    };

}

#endif //DBQUERYBUILDER_QUERYCONDITION_H
