//
// Created by fo on 21/03/2021.
//
// This file contains the definition of  all the member-functions of QueryCondition and its nested class, Core
#include "../../types/QueryCondition.h"
#include <system_error>

using QueryCondition = DBQueryBuilder::Type::QueryCondition;

class QueryCondition::Core {
    QueryCondition::Conjunction _conjunction;
    std::string _field;
    QueryCondition::Operator _operator;
    bool _negate;
    std::string _value;


public:
    std::string strConj;        //the string form of the _conjunction

    inline Core(const Conjunction &conjunction, const std::string &field, const Operator &_operator, const bool &negate,
         const std::string value) : _conjunction(conjunction), _field(field), _operator(_operator), _negate(negate),
                                    _value(value) {

        //set the string-form of the passed conjunction
        switch (_conjunction) {
            case QueryCondition::Conjunction::AND:
                strConj = "and";
                break;
            case QueryCondition::Conjunction::OR:
                strConj = "or";
                break;
            default:        //the passed conjunction is not recognized
                throw std::logic_error("In Type::QueryCondition::toString: the conjunction-type is unknown");
        }
    }

    inline std::string toString(bool addConj = true) {
        //construct the string-form of this QueryCondition
        std::string queryCondition;

        if (addConj) {      //the conjunction should be added
            queryCondition += strConj;
        }
        queryCondition += " ";

        if (_negate) {      //the operation is negated should be negated
            queryCondition += "not ";
        }

        //append the concerned field
        queryCondition += "`";
        queryCondition += _field;
        queryCondition += "`";

        //determine the operator to be added
        switch (_operator) {
            case QueryCondition::Operator::E:
                queryCondition += "='";
                break;
            case QueryCondition::Operator::L:
                queryCondition += "<'";
                break;
            case QueryCondition::Operator::G:
                queryCondition += ">'";
                break;
            case QueryCondition::Operator::LE:
                queryCondition += "<='";
                break;
            case QueryCondition::Operator::GE:
                queryCondition += ">='";
                break;
            default:
                throw std::logic_error("In Type::QueryCondition::toString: the operator-type is unknown");
        }

        //append the value of the field
        queryCondition += _value;
        queryCondition += "'";

        return  queryCondition;
    }

};

QueryCondition::QueryCondition(const Conjunction &conjunction, const std::string &field, const Operator &_operator,
                               const bool &negate, const std::string value) {
    _core = new Core(conjunction, field, _operator, negate, value);
}
QueryCondition::QueryCondition() {}

std::string QueryCondition::toString(const bool &addConj) {
    return _core != nullptr ? _core->toString(addConj) : std::string("");
}

std::string QueryCondition::strConj() {
    return _core != nullptr ? _core->strConj : "";
}

QueryCondition::~QueryCondition() {
    delete _core;
}