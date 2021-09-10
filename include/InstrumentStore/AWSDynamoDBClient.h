#pragma once

#include <iostream>
#include <utility>

#include <aws/core/Aws.h>
#include <aws/dynamodb/DynamoDBClient.h>

#include <aws/dynamodb/model/CreateTableRequest.h>
#include <aws/dynamodb/model/ListTablesRequest.h>

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
  ~AWSDynamoDBClient ();

  /**
   * @brief This function lists all of the tables in the DynamoDB database.
   * @param dynamoClient a connected DynamoDB client.
   * @return a list of tables in the DynamoDB database.
   */
  [[nodiscard]] auto ListTables () -> std::vector<std::string>;
};