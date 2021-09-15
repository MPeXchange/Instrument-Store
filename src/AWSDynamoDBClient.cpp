#include <InstrumentStore/AWSDynamoDBClient.h>

auto AWSDynamoDBClient::ListTables () const -> std::vector<std::string> {

  using Aws::DynamoDB::Model::ListTablesOutcome;
  using Aws::DynamoDB::Model::ListTablesRequest;

  ListTablesRequest req;
  req.SetLimit (50);

  if (const ListTablesOutcome &outcome = _client.ListTables (req);
      outcome.IsSuccess ()) {
    return outcome.GetResult ().GetTableNames ();
  } else {
    std::cout << "Error: " << outcome.GetError ().GetMessage () << std::endl;
    return {};
  }
}

auto AWSDynamoDBClient::CheckTable (const std::string &tableName) const
    -> bool {
  const auto tables = ListTables ();
  return std::find (begin (tables), end (tables), tableName) != end (tables);
}

auto AWSDynamoDBClient::CreateTableSimplePrimaryKey (
    const std::string &tableName, const std::string &primaryKey) -> void {

  std::cout << "Creating table " << tableName << " with simple primary key "
            << primaryKey << std::endl;

  Aws::DynamoDB::Model::CreateTableRequest req;

  Aws::DynamoDB::Model::AttributeDefinition hashKey;
  hashKey.SetAttributeName (primaryKey);
  hashKey.SetAttributeType (Aws::DynamoDB::Model::ScalarAttributeType::S);
  req.AddAttributeDefinitions (hashKey);

  Aws::DynamoDB::Model::KeySchemaElement kse;
  kse.WithAttributeName (primaryKey)
      .WithKeyType (Aws::DynamoDB::Model::KeyType::HASH);
  req.AddKeySchema (kse);

  Aws::DynamoDB::Model::ProvisionedThroughput thruput;
  thruput.WithReadCapacityUnits (5).WithWriteCapacityUnits (5);
  req.SetProvisionedThroughput (thruput);

  req.SetTableName (tableName);

  const Aws::DynamoDB::Model::CreateTableOutcome &outcome =
      _client.CreateTable (req);
  if (outcome.IsSuccess ()) {
    std::cout << "Table "
              << outcome.GetResult ().GetTableDescription ().GetTableName ()
              << " created!" << std::endl;
  } else {
    std::cout << "FAILED: " << outcome.GetError ().GetMessage () << std::endl;
  }
}

auto AWSDynamoDBClient::DeleteTable (const std::string &tableName) -> void {
  Aws::DynamoDB::Model::DeleteTableRequest req;
  req.SetTableName (tableName);

  const auto &outcome = _client.DeleteTable (req);
  if (outcome.IsSuccess ()) {
    std::cout << "Table "
              << outcome.GetResult ().GetTableDescription ().GetTableName ()
              << " deleted!" << std::endl;
  } else {
    std::cout << "FAILED: " << outcome.GetError ().GetMessage () << std::endl;
  }
}

auto AWSDynamoDBClient::PutItem (const std::string &tableName,
                                 const std::string &attributeKey,
                                 const std::string &attributeValue)
    -> PutItemResponse {
  Aws::DynamoDB::Model::PutItemRequest req;
  req.SetTableName (tableName);

  if (DoesItemExist (tableName, attributeKey, attributeValue)
      == DoesItemExistResponse::EXISTS) {
    return PutItemResponse::EXISTS;
  }

  Aws::DynamoDB::Model::AttributeValue attrVal;
  attrVal.SetS (attributeValue);
  req.AddItem (attributeKey, attrVal);

  const Aws::DynamoDB::Model::PutItemOutcome outcome = _client.PutItem (req);
  if (!outcome.IsSuccess ()) {
    std::cout << "Error: " << outcome.GetError ().GetMessage () << std::endl;
    return PutItemResponse::ERROR;
  }
  return PutItemResponse::SUCCESS;
}

auto AWSDynamoDBClient::DoesItemExist (const std::string &tableName,
                                       const std::string &attributeKey,
                                       const std::string &attributeValue)
    -> DoesItemExistResponse {
  Aws::DynamoDB::Model::GetItemRequest req;
  req.SetTableName (tableName);
  Aws::DynamoDB::Model::AttributeValue hashKey;
  hashKey.SetS (attributeValue);

  req.AddKey (attributeKey, hashKey);

  const Aws::DynamoDB::Model::GetItemOutcome &outcome = _client.GetItem (req);
  if (outcome.IsSuccess ()) {
    const Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> &item =
        outcome.GetResult ().GetItem ();
    if (!item.empty ()) return DoesItemExistResponse::EXISTS;
    else
      return DoesItemExistResponse::DOESNT_EXIST;
  } else {
    std::cout << "Error: " << outcome.GetError ().GetMessage () << std::endl;
    return DoesItemExistResponse::ERROR;
  }
}

// TODO: Add existence checking and std::optional.
auto AWSDynamoDBClient::GetItem (const std::string &tableName,
                                 const std::string &attributeKey,
                                 const std::string &attributeValue)
    -> std::vector<std::pair<std::string, std::string>> {
  std::vector<std::pair<std::string, std::string>> ret;

  Aws::DynamoDB::Model::GetItemRequest req;
  req.SetTableName (tableName);
  Aws::DynamoDB::Model::AttributeValue hashKey;
  hashKey.SetS (attributeValue);
  req.AddKey (attributeKey, hashKey);

  const Aws::DynamoDB::Model::GetItemOutcome &outcome = _client.GetItem (req);
  if (outcome.IsSuccess ()) {
    const Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> &item =
        outcome.GetResult ().GetItem ();
    if (!item.empty ()) {
      for (const auto &i : item) {
        ret.emplace_back (i.first, i.second.GetS ());
      }
    }
  }
  return ret;
}

auto AWSDynamoDBClient::DeleteItem (const std::string &tableName,
                                    const std::string &attributeKey,
                                    const std::string &attributeValue)
    -> AWSDynamoDBClient::DeleteItemResponse {

  Aws::DynamoDB::Model::DeleteItemRequest req;
  req.SetTableName (tableName);
  Aws::DynamoDB::Model::AttributeValue hashKey;
  hashKey.SetS (attributeValue);

  req.AddKey (attributeKey, hashKey);

  if (DoesItemExist (tableName, attributeKey, attributeValue)
      == DoesItemExistResponse::DOESNT_EXIST) {
    return DeleteItemResponse::DOES_NOT_EXIST;
  }

  const Aws::DynamoDB::Model::DeleteItemOutcome &outcome =
      _client.DeleteItem (req);
  if (outcome.IsSuccess ()) {
    return DeleteItemResponse::SUCCESS;
  } else {
    std::cout << outcome.GetError ().GetMessage () << std::endl;
    return DeleteItemResponse::ERROR;
  }
}
auto AWSDynamoDBClient::ScanTable (const std::string &tableName,
                                   const std::string &projection = "")
    -> std::vector<std::string> {
  Aws::DynamoDB::Model::ScanRequest req;
  req.SetTableName (tableName);

  if (!projection.empty ()) { req.SetProjectionExpression (projection); }

  const Aws::DynamoDB::Model::ScanOutcome &outcome = _client.Scan (req);
  std::vector<std::string> ret {};
  if (outcome.IsSuccess ()) {
    const Aws::Vector<
        Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>> &items =
        outcome.GetResult ().GetItems ();
    for (const auto &item : items) {
      for (const auto &i : item) { ret.push_back (i.second.GetS ()); }
    }
  } else {
    std::cout << outcome.GetError ().GetMessage () << std::endl;
  }
  return ret;
}
