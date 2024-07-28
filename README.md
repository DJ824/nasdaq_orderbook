# nasdaq_orderbook
nasdaq itch orderbook, data available at https://emi.nasdaq.com/ITCH/Nasdaq%20ITCH/

handles multiple symbols by having a central_orderbook, which is just std::unordered_map<symbol, orderbook*>
handles the following itch data messages: 
Add Order, Execute Order, Replace Order(modify), Cancel Order, Delete Order
market buy/sells are ignored as the execute order message reflects the same result of the trade in the book

currently parses 6.3 million messages in ~4 seconds
going to try multithreading, one thread per symbol
