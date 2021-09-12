#include <iostream>
#include <utility>

#include <InstrumentStore/AWSDynamoDBClient.h>

#include <grpcpp/grpcpp.h>

#include "instrument_store.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using mpx::instrumentstore::CreateEquityRequest;
using mpx::instrumentstore::CreateEquityResponse;
using mpx::instrumentstore::EquityService;

class EquityServiceImpl final : public EquityService::Service {
 private:
  AWSDynamoDBClient &_dynamo_client;

 public:
  explicit EquityServiceImpl (AWSDynamoDBClient &dynamo_client)
      : _dynamo_client (dynamo_client) {};

  Status CreateEquity (ServerContext *context,
                       const CreateEquityRequest *request,
                       CreateEquityResponse *response) override {
    std::cout << request->ticker () << std::endl;

    // Insert AWS Dynamo DB code here.
    _dynamo_client.PutItem ("Equity", "Ticker", request->ticker ());

    response->set_ticker (request->ticker ());
    return Status::OK;
  }
};

void RunServer () {
  AWSDynamoDBClient dynamo_client;

  std::string server_address ("0.0.0.0:50051");

  EquityServiceImpl service (dynamo_client);

  ServerBuilder builder;
  builder.AddListeningPort (server_address, grpc::InsecureServerCredentials ());
  builder.RegisterService (&service);

  std::unique_ptr<Server> server (builder.BuildAndStart ());
  std::cout << "Server listening on port: " << server_address << std::endl;
  server->Wait ();
}

int main () {
  RunServer ();
  return 0;
}