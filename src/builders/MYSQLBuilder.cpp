//
// Created by fo on 21/03/2021.
//
// This file contains the implementation of all the member-functions oc MYSQLBuilder
#include "../../builders/MYSQLBuilder.h"
#include "../../types/CompQueryCondition.h"
#include <map>
#include <vector>
#include <string>

using MYSQLBuilder = DBQueryBuilder::Builder::MYSQLBuilder;


class MYSQLBuilder::QueryDescriptor {
private:
    DBQueryBuilder::Action _queryType;
    std::map<std::string, std::map<DBQueryBuilder::Action, bool>> _qDes;

public:
    QueryDescriptor(DBQueryBuilder::Action queryType) : _queryType(queryType) {}

    /**
     * @brief: confirms whether or not the concerned query may expect a "where" clause
     * @return
     */
    bool setsWhere(const DBQueryBuilder::Action &action) {
        //confirm whether or not the "where"-description for the concerned query-type has been cached
        if (_qDes["where"].find(_queryType) == _qDes["where"].end()) {        //description has not been cached
            //get and cache description
            switch (_queryType) {
                case DBQueryBuilder::Action::INSERT:     //for INSERT
                    _qDes["where"][_queryType] = false;
                    break;
                case DBQueryBuilder::Action::SELECT:     //for SELECT
                case DBQueryBuilder::Action::UPDATE:      //for UPDATE
                case DBQueryBuilder::Action::DELETE:      //for DElETE
                    _qDes["where"][_queryType] = true;
                    break;
                default:        //The query-type is not recognized
                    throw std::logic_error("In Builder::MYSQLBuilder::QueryDescriptor::setsWhere: query-type unknown");
            }
        }

        return _qDes["where"][_queryType];
    }

    /**
     * @brief: confirms whether or not the query being built may expect an "order by" clause
     * @param action
     * @return
     */
    bool setsOrder(const DBQueryBuilder::Action &action) {
        //confirm whether or not the "order"-description for the concerned query-type has been cached
        if (_qDes["order"].find(_queryType) == _qDes["order"].end()) {        //description has not been cached
            //get and cache description
            switch (_queryType) {
                case DBQueryBuilder::Action::SELECT:     //for SELECT
                case DBQueryBuilder::Action::DELETE:      //for DElETE
                case DBQueryBuilder::Action::UPDATE:      //for UPDATE
                    _qDes["order"][_queryType] = true;
                    break;
                case DBQueryBuilder::Action::INSERT:     //for INSERT
                    _qDes["order"][_queryType] = false;
                    break;
                default:        //The query-type is not recognized
                    throw std::logic_error("In Builder::MYSQLBuilder::QueryDescriptor::setsOrder: query-type unknown");
            }
        }

        return _qDes["order"][_queryType];
    }


    /**
     * @brief: confirms whether or not the query being built may expect a "limit" phrase
     * @param action
     * @return
     */
    bool setsLimit(const DBQueryBuilder::Action &action) {
        //confirm whether or not the "limit"-description for the concerned query-type has been cached
        if (_qDes["limit"].find(_queryType) == _qDes["limit"].end()) {        //description has not been cached
            //get and cache description
            switch (_queryType) {
                case DBQueryBuilder::Action::INSERT:     //for INSERT
                    _qDes["limit"][_queryType] = false;
                    break;
                case DBQueryBuilder::Action::SELECT:     //for SELECT
                case DBQueryBuilder::Action::UPDATE:      //for UPDATE
                case DBQueryBuilder::Action::DELETE:      //for DElETE
                    _qDes["limit"][_queryType] = true;
                    break;
                default:        //The query-type is not recognized
                    throw std::logic_error("In Builder::MYSQLBuilder::QueryDescriptor::setsLimit: query-type unknown");
            }
        }

        return _qDes["limit"][_queryType];
    }
};

class MYSQLBuilder::MainClauseCreator {
public:
    /**
     * @@brief: returns the main-clause for a "SELECT" query, using the passed parameters
     * @param columns: the vector of the names of the concerned Columns
     * @param tableName: the name of the database-table concerned
     * @return std::string: the main clause
     */
    inline std::string buildSelect(const std::vector<std::string> &columns, const std::string &tableName) {
        if (tableName.empty()) {        //the passed table-name is empty
            //report the error
            throw std::logic_error("In Builder::MYSQLBuilder::Core::buildSelect: empty table-name");
        }

        //begin the construction of the main-clause
        std::string clause = "select ";

        //confirm that target-columns were passed
        if (columns.empty()) {      //no column was passed
            //construct query to select all available columns
            clause += "*";
        } else {        //columns were passed
            for (const std::string &column : columns) {
                //add the present column to the collection of columns to be select
                clause += (columns[0] == column) ? " `" : ", `";
                clause += column;
                clause += "`";
            }
        }

        //add the table-name
        clause += " from `";
        clause += tableName;
        clause += "`";

        return clause;
    }

    /**
     * @brief: returns the main-clause for an "INSERT" query, using the passed parameters
     * @param tableName: the name of the concerned table
     * @param resourceRep: the new representation that should be pushed to the database
     * @return std::string : the main clause
     */
    inline std::string buildInsert(const std::string &tableName, std::map<std::string, std::string> resourceRep) {

        //confirm that the table-name is not empty
        if (tableName.empty()) {        //the passed table-name is empty
            throw std::logic_error("In Builder::MYSQLBuilder::Core::buildInsert: empty table-name");
        } else if (resourceRep.empty()) {       //the passed Resource-representation is empty
            throw std::logic_error("In Builder::MYSQLBuilder::Core::buildInsert: empty Resource-representation");
        }

        //begin the construction of the main-clause
        std::string clause = "insert into `";
        clause += tableName;
        clause += "`";
        int counter = 0;        //counts the number of iteration through a loop

        std::string columns = "(", values = "(";
        for (std::pair attr: resourceRep) {
            columns += (++counter == 1) ? "`" : ", `", values += (counter == 1) ? "'" : ", '";
            columns += attr.first, values += attr.second;
            columns += "`", values += "'";

        }
        columns += ")", values += ")";

        //append the enumeration of the concerned columns to the query
        clause += columns;
        clause += " values";
        //append the enumeration of values set for the columns
        clause += values;

        return clause;
    }

    /**
     * @brief: returns the main-clause for an "UPDATE" query, using the passed parameters
     * @param tableName: the name of the table being affected
     * @param resourceRep: the new representation that should be pushed to the database
     * @return std::string : the main clause
     */
    inline std::string
    buildUpdate(const std::string &tableName, const std::map<std::string, std::string> &resourceRep) {
        //confirm that the table-name is not empty
        if (tableName.empty()) {        //the passed table-name is empty
            throw std::logic_error("In Builder::MYSQLBuilder::Core::buildUpdate: empty table-name");
        } else if (resourceRep.empty()) {       //the passed Resource-representation is empty
            throw std::logic_error("In Builder::MYSQLBuilder::Core::buildUpdate: empty Resource-representation");
        }

        //begin the construction of the main-clause
        std::string clause = "update `";
        clause += tableName;
        clause += "` set";

        int counter = 0;        //counts the number of iteration through a loop
        for (std::pair<std::string, std::string> _attr : resourceRep) {
            clause += (++counter == 1) ? "`" : ", `";
            clause += _attr.first;
            clause += "`='";
            clause += _attr.second;
            clause += "'";
        }

        return clause;
    }

    /**
     * @brief: returns the main-clause for a "DELETE" query, using the passed parameters
     * @param tableName: the name of the table that is to be affected
     * @return std::string : the main clause
     */
    inline std::string buildDelete(const std::string &tableName) {

        //confirm that the table-name is not empty
        if (tableName.empty()) {        //the passed table-name is empty
            throw std::logic_error("In Builder::MYSQLBuilder::Core::buildDelete: empty table-name");
        }

        //begin the main clause
        std::string clause = "delete from `";
        clause += tableName;
        clause += "`";

        return clause;
    }
};

class MYSQLBuilder::Core {
public:
    QueryDescriptor *_qDescriptor;        //describes type of query being built
    MainClauseCreator *_mainClauseCreator;      //creates main-clause of query being built

    DBQueryBuilder::Action _action{Action::NIL};     //the action of the query
    std::string _tableName;     //the name of the concerned table
    std::vector<std::string> _columns;      //the vector of column-names the query being built is concerned with
    std::map<std::string, std::string> _resourceRep;        //the representation of the resource being pushed to the database

    std::string _whereClause{""};        //the "where"-clause of the query built so far
    std::string _orderClause{""};       //the "order"-clause of the query built so far
    std::string _limitPhrase{""};        //the "limit"-phrase of the query built so far

public:
    Core() {
        //allocate memory for the main-clause creator
        _mainClauseCreator = new MainClauseCreator();
    }

    inline int setTableName(const std::string &tableName) {
        //cache the table Name
        _tableName = tableName;
        return 0;
    }

    inline int setAction(const Action &action) {
        //cache the action
        _action = action;

        //allocate memory for the QueryDescriptor
        _qDescriptor = new QueryDescriptor(_action);

        return 0;
    }

    inline int setTargetColumns(const std::vector<std::string> &columns) {
        //cache the target-columns
        _columns = columns;

        return 0;
    }


    inline int addCondition(const DBQueryBuilder::Type::QueryCondition::Ptr &qCondition) {
        //check is the "where"-clause has been initialized
        if (_whereClause.empty()) {     //the "where"-clause has not been initialized
            //initialize the "where"-clause
            _whereClause = "where ";

            //append the QueryConjunction to the where-clause
            _whereClause += qCondition->toString(false);
        } else {        //the "where"-clause has been initialized
            //append the QueryConjunction ot the "where"-clause
            _whereClause += " ";
            _whereClause += qCondition->toString(true);
        }

        return 0;
    }

    inline int addCompositeCondition(const std::vector<DBQueryBuilder::Type::QueryCondition::Ptr> qConditions) {
        DBQueryBuilder::Type::QueryCondition *qCondition =  new DBQueryBuilder::Type::CompQueryCondition(qConditions);

        //check is the "where"-clause has been initialized
        if (_whereClause.empty()) {     //the "where"-clause has not been initialized
            //initialize the "where"-clause
            _whereClause = "where ";

            //append the QueryConjunction to the where-clause
            _whereClause += qCondition->toString(false);
        } else {        //the "where"-clause has been initialized
            //append the QueryConjunction ot the "where"-clause
            _whereClause += " ";
            _whereClause += qCondition->toString(true);
        }


        //free the temporarily allocated-memory
        delete qCondition;
        return  0;
    }

    inline int addResourceRep(const std::map<std::string, std::string> &resourceRep) {
        //cache the Resource-representation
        _resourceRep = resourceRep;

        return 0;
    }

    inline int setLimit(const int &limit) {
        //create the "limit"-phrase of the query being built
        _limitPhrase = "limit ";
        _limitPhrase += std::to_string(limit);

        return 0;
    }

    inline int setOrder(const std::string &columnName, const Order &order) {
        //create the "order by" clause of the query being built
        _orderClause = "order by `";
        _orderClause += columnName;
        _orderClause += "` ";

        //set the proper ordering for the clause
        switch (order) {
            case Order::ASC:
                _orderClause += "asc";
                break;
            case Order::DEC:
                _orderClause += "desc";
                break;
            default:        //the passed order is not known
                break;
        }

        return 0;
    }

    inline std::string buildQuery() {
        std::string query;      //the query to be returned

        //confirm that the action has been set
        if (_action == Action::NIL) {       //The action has not been set
            //report the error
            throw std::logic_error("In Builder::MYSQLBuilder::Core::buildQuery: call to Builder::MYSQLBuilder::Core::setAction(const Action &) must be made before query can be built");
        }

        //build the main-clause and append to the query
        switch (_action) {
            case Action::INSERT:
                query = _mainClauseCreator->buildInsert(_tableName, _resourceRep);
                break;
            case Action::SELECT:
                query = _mainClauseCreator->buildSelect(_columns, _tableName);
                break;
            case Action::UPDATE:
                query = _mainClauseCreator->buildUpdate(_tableName, _resourceRep);
                break;
            case Action::DELETE:
                query = _mainClauseCreator->buildDelete(_tableName);
                break;
            default:        //_action is not accounted for
                //report the error
                throw std::logic_error("In Builder::MYSQLBuilder::Core::buildQuery: the action of the query is not accounted for");
        }

        //confirm whether or not the query may expect a "where"-clause
        if (_qDescriptor->setsWhere(_action)) {     //a "where"-cause may be set
            query += " ";       //space for next subordinate
            query += _whereClause;
        }

        //confirm whether or not the query may expect an "order"-clause
        if (_qDescriptor->setsOrder(_action)) {        //an "order"-clause may be set
            query += " ";       //space for next subordinate
            query += _orderClause;
        }

        //confirm whether or not the query may expect a "limit"-phrase
        if (_qDescriptor->setsLimit(_action)) {     //a "limit-phrase may be set"
            query += " ";       //space for the next subordinate
            query += _limitPhrase;
        }

        return query;
    }

    ~Core() {
        //free allocated memory
        delete _qDescriptor, delete _mainClauseCreator;
    }
};

MYSQLBuilder::MYSQLBuilder() {
    _core = new Core();
}

int MYSQLBuilder::setTableName(const std::string &tableName) {
    return _core->setTableName(tableName);
}

int MYSQLBuilder::setAction(const Action &action) {
    return _core->setAction(action);
}

int MYSQLBuilder::setTargetColumns(const std::vector<std::string> &columns) {
    return _core->setTargetColumns(columns);
}

int MYSQLBuilder::addCondition(const std::shared_ptr<Type::QueryCondition> &qCondition) {
    return _core->addCondition(qCondition);
}

int MYSQLBuilder::addCompositeCondition(const std::vector<std::shared_ptr<Type::QueryCondition>> qConditions) {
    return _core->addCompositeCondition(qConditions);
}

int MYSQLBuilder::addResourceRep(const std::map<std::string, std::string> &resourceRep) {
    return _core->addResourceRep(resourceRep);
}

int MYSQLBuilder::setOrder(const std::string &columnName, const Order &order) {
    return _core->setOrder(columnName, order);
}

int MYSQLBuilder::setLimit(const int &limit) {
    return _core->setLimit(limit);
}

std::string MYSQLBuilder::buildQuery() {
    return _core->buildQuery();
}

MYSQLBuilder::~MYSQLBuilder() {
    delete _core;
}