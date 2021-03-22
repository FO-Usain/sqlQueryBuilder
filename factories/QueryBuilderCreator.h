//
// Created by fo on 22/03/2021.
//
//This file contains the implementation of a factory-function for QueryBuilder
#include "../builders/MYSQLBuilder.h"

#ifndef DBQUERYBUILDER_QUERYBUILDERCREATOR_H
#define DBQUERYBUILDER_QUERYBUILDERCREATOR_H

namespace DBQueryBuilder::Factory {

    /**
     * @brief: factory-function that creates a new QueryBuilder, according to type passed
     * @throws std::logic_error: if the passed type is not known
     * @param type: the type of
     * @return
     */
    DBQueryBuilder::Builder::IQueryBuilder::Ptr makeQueryBuilder(const std::string &type) {
        if (type == "mysql" || type == "MYSQL") {
            //create a new MYSQLBuilder
            return std::make_shared<DBQueryBuilder::Builder::MYSQLBuilder>();
        } else {        //The passed type is unknown
            //build the message to be reported
            std::string msg = "In Factory::makeQueryBuilder: QueryBuilder type \"";
            msg += msg;
            msg += "\" is unknown";

            //report the error
            throw std::logic_error(msg);
        }
    }

}

#endif //DBQUERYBUILDER_QUERYBUILDERCREATOR_H
