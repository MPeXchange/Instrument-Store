#include <iostream>

#include <InstrumentStore/AWSDynamoDBClient.h>

int main () {
  AWSDynamoDBClient client;

  client.PutItem ("Equity", "Ticker", "SPY");
  std::cout << (int) client.DoesItemExist ("Equity", "Ticker", "SPY")
            << std::endl;
  std::cout << (int) client.DeleteItem ("Equity", "Ticker", "SPY") << std::endl;
  std::cout << (int) client.DoesItemExist ("Equity", "Ticker", "SPY")
            << std::endl;
}
