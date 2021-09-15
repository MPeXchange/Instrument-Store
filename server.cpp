#include <iostream>
#include <utility>

#include <InstrumentStore/AWSDynamoDBClient.h>

#include <grpcpp/grpcpp.h>

#include "instrument_store.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;

using mpx::instrumentstore::EquityService;

using mpx::instrumentstore::CreateEquityRequest;
using mpx::instrumentstore::CreateEquityResponse;

using mpx::instrumentstore::ListEquityRequest;
using mpx::instrumentstore::ListEquityResponse;

using mpx::instrumentstore::DeleteEquityRequest;
using mpx::instrumentstore::DeleteEquityResponse;

class EquityServiceImpl final : public EquityService::Service {
 private:
  AWSDynamoDBClient &_dynamo_client;

 public:
  explicit EquityServiceImpl (AWSDynamoDBClient &dynamo_client)
      : _dynamo_client (dynamo_client) {};

  Status CreateEquity (ServerContext *context,
                       const CreateEquityRequest *request,
                       CreateEquityResponse *response) override {
    for (const auto &ticker : request->tickers ()) {
      std::cout << "Creating: " << ticker << std::endl;
      auto AWSResponse = _dynamo_client.PutItem ("Equity", "Ticker", ticker);
      response->set_success (AWSResponse
                             != AWSDynamoDBClient::PutItemResponse::ERROR);
    }
    return Status::OK;
  }

  Status DeleteEquity (ServerContext *context,
                       const DeleteEquityRequest *request,
                       DeleteEquityResponse *response) override {
    for (const auto &ticker : request->tickers ()) {
      std::cout << "Deleting: " << ticker << std::endl;
      auto AWSResponse = _dynamo_client.DeleteItem ("Equity", "Ticker", ticker);
      response->set_success (AWSResponse
                             != AWSDynamoDBClient::DeleteItemResponse::ERROR);
    }
    return Status::OK;
  }

  Status ListEquity (ServerContext *context, const ListEquityRequest *request,
                     ListEquityResponse *response) override {
    if (request->tickers ().empty ()) {
      auto AWSResponse = _dynamo_client.ScanTable ("Equity", "");
      for (const auto &ticker : AWSResponse) { response->add_tickers (ticker); }
      return Status::OK;
    } else {
      for (const auto &ticker : request->tickers ()) {
        if (_dynamo_client.DoesItemExist ("Equity", "Ticker", ticker)
            == AWSDynamoDBClient::DoesItemExistResponse::EXISTS) {
          response->add_tickers (ticker);
        }
      }
      return Status::OK;
    }
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