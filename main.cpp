#include <iostream>

#include <InstrumentStore/AWSDynamoDBClient.h>

int main () {
  AWSDynamoDBClient client {};

  for (const auto &table : client.ListTables ()) {
    std::cout << table << std::endl;
  }

  return 0;
}
