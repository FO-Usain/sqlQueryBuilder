//
// Created by fo on 21/03/2021.
//
// This file contains a use-case of QueryCondition
#include "../../types/QueryCondition.h"
#include <iostream>

using QueryCondition = DBQueryManager::Type::QueryCondition;

int main() {
    //create a new QueryCondition
    QueryCondition::Ptr condition(new QueryCondition(QueryCondition::AND, "clientId", QueryCondition::Operator::GE, true, "1"));

    try {
        //output the string-form of the query
        std::cout << condition->toString(true) << std::endl;
    } catch (std::logic_error &error) {
        std::cout << "\aError: " << error.what() << std::endl;
    }

    return 0;
}
