//
// Created by fo on 08/01/2021.
//

#include "../../mysqlQueryBuilder/MYSQLQueryBuilder.h"
#include "../../Error.h"

using MYSQLQueryBuilder = QueryBuilder::MYSQLQueryBuilder::MYSQLQueryBuilder;

std::string MYSQLQueryBuilder::getQuery() {

    //confirm that the type of query is known
    if (_queryType == QueryBuilder::QueryType::NIL) {       //the query-type was not specifiedt
        throw QueryBuilder::Error(std::error_code(7, QueryBuilder::errorCategory()));
    }

    try {
        //get the MainClause-creator
        mainClauseCreator = createMainClauseCreator(_queryType);
    } catch (QueryBuilder::Error &error) {
        //re-throw the exception
        throw QueryBuilder::Error(error.code());
    }

    if (!mainClauseCreator) {       //mainClauseCreator was not created
        //throw an exception, reporting that the mainClauseCreator could not be created
        throw QueryBuilder::Error(std::error_code(5, QueryBuilder::errorCategory()));
    }

    //add the main clause of the query to _query
    _query = mainClauseCreator->createMainClause();

    if (!_valuedFields.empty() && (_queryType == QueryBuilder::QueryType::UPDATE)) {       //there are valuedFields
            //append the 'set' clause to the query being built
            appendSetClause();
    }

    if (!_queryConditions.empty()) {        //there are conditions for the query
        //append the where-clause to the query
        appendWhereClause();
    }

    if (!_order.empty()) {       //the order is specified
        //append the oder-clause to the query
        _query.append(" ");
        _query.append(_order);
    }

    if (_limit) {       //the limit of the affected rows is specified
        //append the limit
        _query.append(" limit ");
        _query.append(std::to_string(_limit));
    }

    return _query;
}


std::error_code MYSQLQueryBuilder::setTableName(const std::string &tableName) {
    //confirm that tableName is safe to use(For protection against SQL injection attacks)

    std::string tableName_ = tableName;

    //escape tableName

    //reserve enough space in _tableName to contain the formatted tableName
    _tableName.reserve(tableName_.length() + 2);

    _tableName += "`";
    _tableName += tableName_;
    _tableName += "`";

    //return with a 'success' error_code
    return std::error_code(0, QueryBuilder::errorCategory());
}

std::error_code MYSQLQueryBuilder::addTargetFields(const std::vector <std::string> &fields) {
    //confirm that all the fields are safe to use(For protection against SQL injection attack)

    std::vector <std::string> _fields = fields;

    //reserve space in the buffer for targetFields
    _targetFields.reserve(_targetFields.size() + _fields.size());

    //format the contents of fields in to _targetFields(make them MYSQL Worthy
    for (std::string &field : _fields) {

        //escape field

        //add a new element into _targetFields to contain the formatted field
        _targetFields.emplace_back("");
        //reserve enough space in the current element of _targetFields, to contain formatted _targetField
        _targetFields[_targetFields.size() - 1].reserve(field.length() + 2);

        //format the new field into the _targetFields
        _targetFields[_targetFields.size() - 1] += "`";
        _targetFields[_targetFields.size() - 1] += field;
        _targetFields[_targetFields.size() - 1] += "`";
    }

    //return with a success error_code
    return std::error_code(0, QueryBuilder::errorCategory());
}

std::error_code MYSQLQueryBuilder::setValuedField(const std::string &fieldName, const std::string &value) {

    //confirm that all field is safe to use(For protection against SQL injection attack)

    //confirm that value is safe to use(For protection against SQL injection attack)

    std::string _fieldName = fieldName;
    std::string _value = value;

    //escape _fieldName;

    //escape _value

    std::string formattedName = "`";
    formattedName += _fieldName + "`";

    //store the new(formatted version) fieldName-value pair in _valuedFields
    _valuedFields[formattedName] = "'";
    _valuedFields[formattedName] += _value + "'";

    //return with a success std::error_code
    return std::error_code(0, QueryBuilder::errorCategory());
}

std::error_code MYSQLQueryBuilder::setQueryType(const QueryBuilder::QueryType &queryType) {

    //initialize _queryType with the corresponding string-value of queryType
    switch (queryType) {
        case QueryBuilder::QueryType::SELECT:
            _queryType = QueryBuilder::QueryType::SELECT;
            break;
        case QueryBuilder::QueryType::UPDATE:
            _queryType = QueryBuilder::QueryType::UPDATE;
            break;
        case QueryBuilder::QueryType::DELETE:
            _queryType = QueryBuilder::QueryType::DELETE;
            break;
        case QueryBuilder::QueryType::INSERT:
            _queryType = QueryBuilder::QueryType::INSERT;
            break;

        default:
            //return with error_code indicating that the queryType is unknown
            return std::error_code(1, QueryBuilder::errorCategory());
    }

    //return with success error_code
    return std::error_code(0, QueryBuilder::errorCategory());
}

std::error_code MYSQLQueryBuilder::addCondition(const QueryBuilder::Conjunction &conjunction, const std::string &field,
                                                const QueryBuilder::Operation &operation, const bool &negate,
                                                const std::string &value) {
    //confirm that field is safe to use(For protection against SQL injection)

    //confirm that value is safe to use (For protection against SQL injection)

    std::string _field = field;
    std::string _value = value;

    //escape field

    //escape value

    //create a new element in _queryConditions to contain the new query-condition
    _queryConditions.emplace_back("");

    //reserve enough space in the newly allocated element of _queryCondition that will be enough to contain the maximum size of the query-condition to be formed
    // conjunction-max = 2(for and , which will be &&), operation-max = 6 (for regex, which will be regexp), negate = 1 (!), 4 for nesting field and value and all the 'space' characters(5)
    _queryConditions[_queryConditions.size() - 1].reserve((_field.length()) + 2 + 6 + 1 + 4 + 5);

    //append the appropriate conjunction to the element of _queryConditions to contain this query-condition
    switch (conjunction) {
        case QueryBuilder::Conjunction::AND:
            (_queryConditions.size() == 1) ? (_queryConditions[_queryConditions.size() - 1].append("")) :
            (_queryConditions[_queryConditions.size() - 1].append(" &&"));
            break;
        case QueryBuilder::Conjunction::OR:
            (_queryConditions.size() == 1) ? (_queryConditions[_queryConditions.size() - 1].append("")) :
            (_queryConditions[_queryConditions.size() - 1].append(" ||"));
            break;

        default:
            //pop the newly allocated element
            _queryConditions.pop_back();
            //return with error_code indicating that the conjunction is unknown
            return std::error_code(2, QueryBuilder::errorCategory());
    }

    if (negate) {
        //negate the statement
        _queryConditions[_queryConditions.size() - 1].append(" !");
    }

    //append field
    _queryConditions[_queryConditions.size() - 1].append(" `");
    _queryConditions[_queryConditions.size() - 1].append(_field);
    _queryConditions[_queryConditions.size() - 1].append("`");

    //append the operation
    switch (operation) {
        case QueryBuilder::Operation::EQUAL_TO:
            _queryConditions[_queryConditions.size() - 1].append(" = '");
            break;
        case QueryBuilder::Operation::GREATER_THAN:
            _queryConditions[_queryConditions.size() - 1].append(" > '");
            break;
        case QueryBuilder::Operation::LESSER_THAN:
            _queryConditions[_queryConditions.size() - 1].append(" < '");
            break;
        case QueryBuilder::Operation::GREATER_THAN_OR_EQUAL_TO:
            _queryConditions[_queryConditions.size() - 1].append(" >= '");
            break;
        case QueryBuilder::Operation::LESSER_THAN_OR_EQUAL_TO:
            _queryConditions[_queryConditions.size() - 1].append(" <= '");
            break;
        case QueryBuilder::Operation::REGEX:
            _queryConditions[_queryConditions.size() - 1].append(" regexp '");
            break;

        default:
            //pop the newly allocated element
            _queryConditions.pop_back();
            //return with error_code indicating that the operation is unknown
            return std::error_code(3, QueryBuilder::errorCategory());
    }

    //append value
    _queryConditions[_queryConditions.size() - 1].append(
            (operation == QueryBuilder::Operation::REGEX) ? formRegex(_value) : _value);
    _queryConditions[_queryConditions.size() - 1].append("'");

    //return with success error_code
    return std::error_code(0, QueryBuilder::errorCategory());
}

std::error_code MYSQLQueryBuilder::setOrder(const std::string &field, const QueryBuilder::Order &direction) {
    //confirm that field is safe to use(for protection against SQL injection

    std::string _field = field;

    //escape field

    //clear _order
    _order.clear();

    //reserve enough space in _order to contain the maximum size of Order clause
    //order-max = 7 (order by), direction-max = 4(desc), 'space' characters = 3 "'" = 2(for nesting field)
    _order.reserve(_field.length() + 7 + 4 + 3 + 2);

    _order.append("order by `");
    _order.append(_field);
    _order.append("`");

    switch (direction) {
        case QueryBuilder::Order::ASCEND:
            _order.append(" asc");
            break;
        case QueryBuilder::Order::DESCEND:
            _order.append(" desc");
            break;

        default:
            //return an error_code indicating that the direction is unknown
            return std::error_code(3, QueryBuilder::errorCategory());
    }

    //return with success error_code
    return std::error_code(0, QueryBuilder::errorCategory());
}

void MYSQLQueryBuilder::setLimit(const int &limit) {
    //set the limit
    _limit = limit;
}

std::string MYSQLQueryBuilder::formRegex(const std::string &pattern) {
    return pattern;
}
