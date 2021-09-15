#pragma once

#include <grpcpp/grpcpp.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

#include "instrument_store.grpc.pb.h"

using mpx::instrumentstore::EquityService;

using mpx::instrumentstore::CreateEquityRequest;
using mpx::instrumentstore::CreateEquityResponse;

using mpx::instrumentstore::DeleteEquityRequest;
using mpx::instrumentstore::DeleteEquityResponse;

using mpx::instrumentstore::ListEquityRequest;
using mpx::instrumentstore::ListEquityResponse;

class EquityServiceClient {
 private:
  std::unique_ptr<EquityService::Stub> _stub;

 public:
  explicit EquityServiceClient (const std::shared_ptr<Channel> &channel)
      : _stub (EquityService::NewStub (channel)) {};

  auto CreateEquities (const std::vector<std::string> &tickers) -> bool {
    ClientContext context;

    CreateEquityRequest request;
    for (const auto &ticker : tickers) { request.add_tickers (ticker); }

    CreateEquityResponse response;

    Status status = _stub->CreateEquity (&context, request, &response);

    if (!status.ok ()) {
      std::cout << status.error_code () << ": " << status.error_message ()
                << std::endl;
    }

    return response.success ();
  }

  auto DeleteEquities (const std::vector<std::string> &tickers) -> bool {
    ClientContext context;

    DeleteEquityRequest request;
    for (const auto &ticker : tickers) { request.add_tickers (ticker); }

    DeleteEquityResponse response;

    Status status = _stub->DeleteEquity (&context, request, &response);

    if (!status.ok ()) {
      std::cout << status.error_code () << ": " << status.error_message ()
                << std::endl;
    }

    return response.success ();
  }

  auto ListEquities (const std::vector<std::string> &tickers)
      -> std::vector<std::string> {
    ClientContext context;

    ListEquityRequest request;
    for (const auto &ticker : tickers) { request.add_tickers (ticker); };

    ListEquityResponse response;

    Status status = _stub->ListEquity (&context, request, &response);

    if (!status.ok ()) {
      std::cout << status.error_code () << ": " << status.error_message ()
                << std::endl;
    }

    std::vector<std::string> result {};
    for (const auto &ticker : response.tickers ()) {
      result.push_back (ticker);
    }
    return result;
  }
};
