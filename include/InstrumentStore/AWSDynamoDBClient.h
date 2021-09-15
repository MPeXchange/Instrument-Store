#pragma once

#include <algorithm>
#include <iostream>
#include <utility>

#include <aws/core/Aws.h>
#include <aws/dynamodb/DynamoDBClient.h>

#include <aws/dynamodb/model/CreateTableRequest.h>
#include <aws/dynamodb/model/DeleteTableRequest.h>
#include <aws/dynamodb/model/ListTablesRequest.h>
#include <aws/dynamodb/model/ScanRequest.h>

#include <aws/dynamodb/model/DeleteItemRequest.h>
#include <aws/dynamodb/model/GetItemRequest.h>
#include <aws/dynamodb/model/PutItemRequest.h>

// TODO: Refactor Responses into std::optional.

/**
 * @brief AWS DynamoDB Interface.
 */
class AWSDynamoDBClient {
 private:
  Aws::SDKOptions _options;
  Aws::Client::ClientConfiguration _clientConfig;
  Aws::DynamoDB::DynamoDBClient _client;

 public:
  AWSDynamoDBClient ()
      : _options {Aws::SDKOptions {}},
        _clientConfig {
            (Aws::InitAPI (_options), Aws::Client::ClientConfiguration {})},
        _client {Aws::DynamoDB::DynamoDBClient {_clientConfig}} {};
  ~AWSDynamoDBClient () { Aws::ShutdownAPI (_options); };

  /**
   * @brief This function lists all of the tables in the DynamoDB database.
   * @return a list of tables in the DynamoDB database.
   */
  [[nodiscard]] auto ListTables () const -> std::vector<std::string>;

  /**
   * @brief This function checks whether or not a table with a given name
   * exists.
   * @param tableName is the table to check existence of.
   * @return whether or not the input table exists in the DynamoDB database.
   */
  [[nodiscard]] auto CheckTable (const std::string &tableName) const -> bool;

  /**
   * @brief This function creates a table with a given name and a simple primary
   * key.
   * @param tableName is the desired name of the table.
   * @param primaryKey is the desired simple primary field.
   */
  auto CreateTableSimplePrimaryKey (const std::string &tableName,
                                    const std::string &primaryKey) -> void;

  /**
   * @brief This function deletes a given table in the DynamoDB database.
   * @param tableName is the name of the table to be deleted.
   */
  auto DeleteTable (const std::string &tableName) -> void;

  enum class PutItemResponse { SUCCESS = 1, EXISTS = 0, ERROR = -1 };

  /**
   * @brief This function inserts an item into a table.
   * @param tableName is the name of the table to put the item in.
   * @param attributeKey is the name of the attribute key of item to insert.
   * @param attributeValue is the value of the attribute of the item to insert.
   * @return a response code corresponding to success or failure.
   */
  auto PutItem (const std::string &tableName, const std::string &attributeKey,
                const std::string &attributeValue) -> PutItemResponse;

  enum class DoesItemExistResponse { DOESNT_EXIST = 0, EXISTS = 1, ERROR = -1 };

  /**
   * @brief This function checks whether a given item exists in a table.
   * @param tableName is the name of the table to check.
   * @param attributeKey is the name of the attribute key of the item to search
   * for.
   * @param attributeValue is the value of the attribute of the item to search
   * for.
   * @return a response code corresponding to the items existence, or error.
   */
  [[nodiscard]] auto DoesItemExist (const std::string &tableName,
                                    const std::string &attributeKey,
                                    const std::string &attributeValue)
      -> DoesItemExistResponse;

  /**
   * @brief This function searches a table for an item.
   * @param tableName is the name of the table to check.
   * @param attributeKey is the name of the attribute key of the item to search
   * for.
   * @param attributeValue is the value of the attribute of the item to search
   * for.
   * @return the item if it exists.
   */
  [[nodiscard]] auto GetItem (const std::string &tableName,
                              const std::string &attributeKey,
                              const std::string &attributeValue)
      -> std::vector<std::pair<std::string, std::string>>;

  enum class DeleteItemResponse { DOES_NOT_EXIST = 0, SUCCESS = 1, ERROR = -1 };

  /**
   * @brief This function deletes an item from a table if it exists.
   * @param tableName is the name of the table to delete from.
   * @param attributeKey is the name of the attribute key of the item to delete.
   * @param attributeValue is the value of the attribute of the item to delete.
   * @return an error code corresponding to success or failure.
   */
  auto DeleteItem (const std::string &tableName,
                   const std::string &attributeKey,
                   const std::string &attributeValue) -> DeleteItemResponse;

  /**
   * @brief This function scans a table for a projection.
   * @param tableName is the name of the table to scan.
   * @param projection is a projection to scan the table with.
   * @return the items in a table matching the projection.
   */
  auto ScanTable (const std::string &tableName, const std::string &projection)
      -> std::vector<std::string>;

  // TODO: UpdateItemInTable
};