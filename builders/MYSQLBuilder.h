//
// Created by fo on 21/03/2021.
//
// This file contains the definition of MYSQLBuilder, an implementer of IQueryBuilder.
// MSQBuilder is responsible for building queries form MYSQL
#include "IQueryBuilder.h"

#ifndef DBQUERYBUILDER_MYSQLBUILDER_H
#define DBQUERYBUILDER_MYSQLBUILDER_H

namespace DBQueryBuilder::Builder {

    class MYSQLBuilder : public IQueryBuilder {
    public:
        MYSQLBuilder();

        /**
         *
         * @param tableName
         * @return
         */
        int setTableName(const std::string &tableName) override;

        /**
         *
         * @param action
         * @return
         */
        int setAction(const Action &action) override;

        /**
         *
         * @param columns
         * @return
         */
        int setTargetColumns(const std::vector<std::string> &columns) override;

        /**
         *
         * @param qCondition
         * @return
         */
        int addCondition(const std::shared_ptr<Type::QueryCondition> &qCondition) override;

        /**
         *
         * @param qConditions
         * @return
         */
        int addCompositeCondition(const std::vector<std::shared_ptr<Type::QueryCondition>> qConditions) override;

        /**
         *
         * @param resourceRep
         * @return
         */
        int addResourceRep(const std::map<std::string, std::string> &resourceRep) override;

        /**
         *
         * @param columnName
         * @param order
         * @return
         */
        int setOrder(const std::string &columnName, const Order &order) override;

        /**
         *
         * @param limit
         * @return
         */
        int setLimit(const int &limit) override;

        /**
         *
         * @return
         */
        std::string buildQuery() override;

        ~MYSQLBuilder() override;

    private:
        /**
         * @brief: describes the type of the query being built
         */
        class QueryDescriptor;

        /**
         * @brief: creates the main-clauses for MYSQL-queries
         */
        class MainClauseCreator;

        class Core;     //The implementer of MYSQLBuilder

        Core *_core;        //pimpl
};

}

#endif //DBQUERYBUILDER_MYSQLQUERYBUILDER_H
