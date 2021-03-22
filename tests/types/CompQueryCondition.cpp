//
// Created by fo on 21/03/2021.
//
// This file contains a use-case of CompQueryCondition
#include "../../types/CompQueryCondition.h"
#include <iostream>

using QueryCondition = DBQueryManager::Type::QueryCondition;
using CompQueryCondition = DBQueryManager::Type::CompQueryCondition;

int main() {
    //initialize two Query-conditions
    QueryCondition::Ptr condition(new QueryCondition(QueryCondition::AND, "clientId", QueryCondition::Operator::GE, true, "1"));
    QueryCondition::Ptr condition2(new QueryCondition(QueryCondition::OR, "cipherName", QueryCondition::Operator::E, false, "aes-256-cbc"));

    //initialize the CompQueryCondition
    QueryCondition::Ptr condition3(new CompQueryCondition({condition, condition2}));

    try {
        //output the composite QueryCondition with the first conjunction showing
        std::cout << "With conjunction: " << condition3->toString(true) << std::endl;
        std::cout << "Without conjunction: " << condition3->toString(false) << std::endl;
    } catch (std::logic_error &error) {
        std::cout << "\aError: " << error.what() << std::endl;
    }

    return 0;
}
