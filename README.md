# This file contains a trivial example

#include "../../types/CompQueryCondition.h"
#include "../../builders/MYSQLBuilder.h"

#include <iostream>

//perform type-aliasing for convenience
using IQueryBuilder = DBQueryBuilder::Builder::IQueryBuilder;
using MYSQLBuilder = DBQueryBuilder::Builder::MYSQLBuilder;
using QCondition = DBQueryBuilder::Type::QueryCondition;
using CQCondition = DBQueryBuilder::Type::CompQueryCondition;

int main() {
//initialize a new MYSQLBuilder
IQueryBuilder::Ptr builder(new MYSQLBuilder());

    //start to build the query
    builder->setTableName("CipherPackage");
    builder->setAction(DBQueryBuilder::Action::SELECT);
    builder->setTargetColumns({"clientId", "cipherName", "key", "id"});
    builder->setOrder("clientId", DBQueryBuilder::Order::ASC);
    builder->addCondition(std::make_shared<QCondition>(QCondition::Conjunction::AND, "clientId", QCondition::Operator::E,
                                                       false, "0"));

    try {
        //get the query
        std::string query = builder->buildQuery();

        //output the query
        std::cout << "Query: " << query << std::endl;
    } catch (std::logic_error &error) {
        std::cout << "\aError: " << error.what() << std::endl;
    }

    return 0;
}

### output: select  `clientId`, `cipherName`, `key`, `id` from `CipherPackage` where  `clientId`='0' order by `clientId` asc 

### compilation for g++, assuming main.cpp is your source-file:
g++ main.coo src/builders/MYSQLBuilder.cpp src/types/CompQueryCondition.cpp src/types/QueryCondition.cpp -std=c++17 -o main
