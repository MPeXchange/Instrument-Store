#include <iostream>

#include <grpcpp/grpcpp.h>

#include "instrument_store.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using mpx::instrumentstore::CreateEquityRequest;
using mpx::instrumentstore::CreateEquityResponse;
using mpx::instrumentstore::EquityService;

class EquityServiceClient {
 private:
  std::unique_ptr<EquityService::Stub> _stub;

 public:
  explicit EquityServiceClient (const std::shared_ptr<Channel> &channel)
      : _stub (EquityService::NewStub (channel)) {};

  void CreateEquity (const std::string &ticker) {
    ClientContext context;

    CreateEquityRequest request;
    request.set_ticker (ticker);

    CreateEquityResponse response;

    Status status = _stub->CreateEquity (&context, request, &response);

    if (status.ok ()) {
      std::cout << "OK: " << request.ticker () << " created." << std::endl;
    } else {
      std::cout << status.error_code () << ": " << status.error_message ()
                << std::endl;
    }
  }
};

int main () {

  EquityServiceClient equity_service_client (grpc::CreateChannel (
      "0.0.0.0:50051", grpc::InsecureChannelCredentials ()));

  std::cout << "--------------- CreateEquity ---------------" << std::endl;
  equity_service_client.CreateEquity ("GOOG");

  return 0;
}
