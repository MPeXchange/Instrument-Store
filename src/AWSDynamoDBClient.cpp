#include <InstrumentStore/AWSDynamoDBClient.h>

AWSDynamoDBClient::~AWSDynamoDBClient () { Aws::ShutdownAPI (_options); }

auto AWSDynamoDBClient::ListTables () -> std::vector<std::string> {

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
