#include <iostream>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <InstrumentStore/EquityServiceClient.h>

enum class InstrumentStoreVerb { CREATE = 1, LIST = 0, DELETE = -1 };

int main (int argc, char **argv) {
  po::variables_map vm;
  po::options_description desc ("Allowed Options");
  po::positional_options_description pdesc;

  // VERB FLAG
  desc.add_options () ("create,c", po::bool_switch ()->default_value (false),
                       "Create an instrument.");

  desc.add_options () ("list,l", po::bool_switch ()->default_value (false),
                       "List all instruments.");

  desc.add_options () ("delete,d", po::bool_switch ()->default_value (false),
                       "Delete an instrument.");

  // INSTRUMENT CLASS FLAG
  desc.add_options () ("equity,e", po::bool_switch ()->default_value (false),
                       "Equity.");

  // TICKERS
  desc.add_options () ("tickers", po::value<std::vector<std::string>> (),
                       "Tickers.");

  pdesc.add ("tickers", -1);

  po::store (po::command_line_parser (argc, argv)
                 .options (desc)
                 .positional (pdesc)
                 .run (),
             vm);
  po::notify (vm);

  // Ensure exactly one verb.
  if ((int) vm["create"].as<bool> () + (int) vm["list"].as<bool> ()
          + (int) vm["delete"].as<bool> ()
      != 1) {
    std::cerr << "Please select one action." << std::endl;
    return -1;
  }

  // Ensure exactly one class.
  if ((int) vm["equity"].as<bool> () != 1) {
    std::cerr << "Please select one asset class." << std::endl;
    return -1;
  }

  InstrumentStoreVerb verb;
  if (vm["create"].as<bool> ()) verb = InstrumentStoreVerb::CREATE;
  if (vm["list"].as<bool> ()) verb = InstrumentStoreVerb::LIST;
  if (vm["delete"].as<bool> ()) verb = InstrumentStoreVerb::DELETE;

  if (vm["equity"].as<bool> ()) {
    EquityServiceClient equity_service_client (grpc::CreateChannel (
        "0.0.0.0:50051", grpc::InsecureChannelCredentials ()));

    switch (verb) {
      case InstrumentStoreVerb::LIST:
        if (vm["tickers"].empty ()) {
          auto AWSResponse = equity_service_client.ListEquities ({});
          std::cout << "The following AWSResponse exist:\n\t";
          for (const auto &ticker : AWSResponse) { std::cout << ticker << " "; }
          std::cout << std::endl;
          return 0;
        } else {
          auto tickers = vm["tickers"].as<std::vector<std::string>> ();
          auto AWSResponse = equity_service_client.ListEquities (tickers);
          std::cout << "The following AWSResponse exist:\n\t";
          for (const auto &ticker : AWSResponse) { std::cout << ticker << " "; }
          std::cout << std::endl;
          return 0;
        }

      case InstrumentStoreVerb::CREATE:
        if (vm["tickers"].empty ()) {
          std::cout << "Please enter at least one equity to create."
                    << std::endl;
          return -1;
        } else {
          auto tickers = vm["tickers"].as<std::vector<std::string>> ();
          bool success = equity_service_client.CreateEquities (tickers);
          if (success) {
            std::cout << "Equities successfully created!" << std::endl;
            return 1;
          } else {
            std::cout << "Error in creating equities!" << std::endl;
            return -1;
          }
        }

      case InstrumentStoreVerb::DELETE:
        if (vm["tickers"].empty ()) {
          std::cout << "Please enter at least one equity to delete."
                    << std::endl;
          return -1;
        } else {
          auto tickers = vm["tickers"].as<std::vector<std::string>> ();
          bool success = equity_service_client.DeleteEquities (tickers);
          if (success) {
            std::cout << "Equities successfully deleted!" << std::endl;
            return 1;
          } else {
            std::cout << "Error in deleting equities!" << std::endl;
            return -1;
          }
        }
    }
  }
}
