# Instrument Store

Instrument Store is a component of MPeXchange (MPX).

This component interfaces with AWS DynamoDB. DynamoDB is used as a persistent store for all securities traded on the
exchange and all instruments created by users of the exchange.

### v0.1.0 Targeted Features

- [ ] Equity Securities.
  - [ ] Create
    - Example: `CREATE EQUITY AAPL` creates a new security with the symbol **AAPL**.
  - [ ] List
    - Example: `LIST EQUITY` returns a list of all securities defined in the database.
  - [ ] Delete (for administrative purposes)
    - Example: `DELETE EQUITY AAPL` deletes the security with the symbol **AAPL**.

### v0.2.0 Targeted Features

- [ ] Options
  - [ ] Create
    - Example: `CREATE OPTION AAPL PRICE 155 DATE 20220101` creates an option on **AAPL** with a strike price of \$155
      and a strike date of January 1, 2022 .
  - [ ] List
    - [ ] By Strike Price
      - Example: `LIST OPTION AAPL PRICE 155` returns a list of all **AAPL** options with a strike price of \$155 .
    - [ ] By Strike Date
      - Example: `LIST OPTION AAPL DATE 20220101` returns a list of all **AAPL** options with a strike date of January
        1, 2022 .
  - [ ] Delete (for administrative purposes)
    - Example: `DELETE OPTION AAPL PRICE 155 DATE 20220101` deletes the **AAPL** option with a strike price of \$155 and
      a strike date of January 1, 2022 .

### v0.3.0 Targeted Features

- [ ] User Defined Instruments (UDIs)

The specific implementation details for UDIs has not been planned.