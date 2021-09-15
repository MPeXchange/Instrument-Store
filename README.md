# Instrument Store

Instrument Store is a component of MPeXchange (MPX).

This component interfaces with AWS DynamoDB. DynamoDB is used as a persistent store for all securities traded on the
exchange and all instruments created by users of the exchange.

### v0.1.0 Targeted Features

- [X] Equity Securities.
  - [X] Create
    - Example: `./InstrumentStoreClient -c -e AAPL` creates a new security with the symbol **AAPL**.
  - [X] List
    - Example: `./InstrumentStoreClient -l -e` returns a list of all securities defined in the database.
    - Example: `./InstrumentStoreClient -l -e AAPL` returns whether a security with the symbol **AAPL** exists.
  - [X] Delete (for administrative purposes)
    - Example: `./InstrumentStoreClient -d -e AAPL` deletes the security with the symbol **AAPL**.

### v0.2.0 Targeted Features

- [ ] Options
  - [ ] Create
    - Example: `./InstrumentStoreClient -c -o AAPL -p 155 -s 20220101` creates an option on **AAPL** with a strike price
      of \$155 and a strike date of January 1, 2022 .
  - [ ] List
    - [ ] By Strike Price
      - Example: `./InstrumentStoreClient -l -o AAPL -p 155` returns a list of all **AAPL** options with a strike price
        of \$155 .
    - [ ] By Strike Date
      - Example: `./InstrumentStoreClient -l -o AAPL -s 20220101` returns a list of all **AAPL** options with a strike
        date of January 1, 2022 .
  - [ ] Delete (for administrative purposes)
    - Example: `./InstrumentStoreClient -d -o AAPL -p 155 -s 20220101` deletes the **AAPL** option with a strike price
      of \$155 and a strike date of January 1, 2022 .

### v0.3.0 Targeted Features

- [ ] User Defined Instruments (UDIs)

The specific implementation details for UDIs has not been planned.