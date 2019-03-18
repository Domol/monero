#include <iostream>

#include <cpr/cpr.h>
#include <json/json.hpp>
#include <string>
#include <unistd.h>


cpr::Response makeXmrtoPostRequest(nlohmann::json data, std::string url) {
    return cpr::Post(
        cpr::Url{url},
        cpr::Body(data.dump()),
        cpr::Header{{"Content-Type", "application/json"}},
        cpr::VerifySsl{false} // find a solution..
    );
}

std::string createOrder(std::string btcAddress, double btcAmount) {
    nlohmann::json data = {
        {"btc_dest_address", btcAddress},
        {"btc_amount", btcAmount},
    };
    auto response = makeXmrtoPostRequest(data, "https://test.xmr.to/api/v2/xmr2btc/order_create/");
    return json::parse(response.text)["uuid"];
}

json getOrderInfo(std::string orderUuid) {
    nlohmann::json data = {{"uuid", orderUuid}};
    auto response = makeXmrtoPostRequest(data, "https://test.xmr.to/api/v2/xmr2btc/order_status_query/");
    return json::parse(response.text);
}

void waitForOrderState(std::string orderUuid, std::string state) {
    while(getOrderInfo(orderUuid)["state"] != state) sleep(1.5);
}

void getTransactionData(std::string orderUuid) {
    nlohmann::json orderData = getOrderInfo(orderUuid);
    double amountToPay = orderData["xmr_amount_remaining"];
    std::string integratedAddress = orderData["xmr_receiving_integrated_address"];
    std::cout<<"Address to send XMR: "<<integratedAddress<<"\n";
    std::cout<<"Amount to pay: "<<amountToPay<<"\n";
}

void createXmrtoTransaction(std::string btcAddress, double btcAmount) {
    //createOrder("3P3QsMVK89JBNqZQv5zMAKG8FK3kJM4rjt", 0.001);
    // bitcoinAddress = "2N3zAThXJTzVrLE43t3EzauZqrcAQQeqBSW";
    // btcAmount = 0.001;
    std::string orderUuid = createOrder(btcAddress, btcAmount);
    std::cout<<"Waiting for xmr.to to create an order..."<<"\n";
    waitForOrderState(orderUuid, "UNPAID");
    getTransactionData(orderUuid);
}

