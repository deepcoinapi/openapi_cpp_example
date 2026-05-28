#include "deepcoin/examples.h"

#include "deepcoin/constants.h"

#include <iostream>
#include <utility>

namespace deepcoin {
namespace c = constants;

ExampleRegistry::ExampleRegistry(HttpClient client) : client_(std::move(client)) {
  add("market:orderbook", [this] {
    return print_response(client_.get(c::kMarketBooks, {{"instId", "BTC-USDT-SWAP"}, {"sz", "5"}}, false));
  });
  add("market:kline", [this] {
    return print_response(client_.get(c::kMarketCandles, {{"instId", "BTC-USDT"}, {"bar", "1m"}, {"limit", "10"}}, false));
  });
  add("market:index-kline", [this] {
    return print_response(client_.get(c::kMarketIndexCandles, {{"instId", "BTC-USDT-SWAP"}, {"bar", "1m"}, {"limit", "10"}}, false));
  });
  add("market:mark-kline", [this] {
    return print_response(client_.get(c::kMarketMarkCandles, {{"instId", "BTC-USDT-SWAP"}, {"bar", "1m"}, {"limit", "10"}}, false));
  });
  add("market:instruments", [this] {
    return print_response(client_.get(c::kMarketInstruments, {{"instType", c::kSwap}, {"uly", "BTC-USDT"}}, false));
  });
  add("market:tickers", [this] {
    return print_response(client_.get(c::kMarketTickers, {{"instType", c::kSpot}, {"uly", "BTC-USDT"}}, false));
  });
  add("market:trades", [this] {
    return print_response(client_.get(c::kMarketTrades, {{"instId", "BTC-USDT-SWAP"}, {"limit", "10"}}, false));
  });
  add("market:step-margin", [this] {
    return print_response(client_.get(c::kMarketStepMargin, {{"instId", "BTC-USDT-SWAP"}}, false));
  });
  add("market:book-spread", [this] {
    return print_response(client_.get(c::kMarketBookSpread, {{"instId", "BTC-USDT-SWAP"}}, false));
  });
  add("market:time", [this] {
    return print_response(client_.get(c::kMarketTime, {}, false));
  });
  add("market:ping", [this] {
    return print_response(client_.get(c::kMarketPing, {}, false));
  });
  add("market:handicap-kline", [this] {
    return print_response(client_.get(c::kMarketHandicapKline1m,
      {{"symbol", "BTC-USDT-SWAP"}, {"stime", "1700000000"}, {"limit", "10"}}, false));
  });
  add("market:handicap-orderbook", [this] {
    return print_response(client_.get(c::kMarketHandicapOrderbook,
      {{"symbol", "BTC-USDT-SWAP"}, {"stime", "1700000000"}, {"limit", "10"}}, false));
  });
  add("market:handicap-trade", [this] {
    return print_response(client_.get(c::kMarketHandicapTrade,
      {{"symbol", "BTC-USDT-SWAP"}, {"stime", "1700000000"}, {"limit", "10"}}, false));
  });

  add("account:balance", [this] {
    return print_response(client_.get(c::kAccountBalance, {{"instType", c::kSwap}, {"ccy", "USDT"}}));
  });
  add("account:bills", [this] {
    return print_response(client_.get(c::kAccountBills, {{"instType", c::kSpot}}));
  });
  add("account:positions", [this] {
    return print_response(client_.get(c::kPositions, {{"instType", c::kSwap}}));
  });
  add("account:positions-history", [this] {
    return print_response(client_.get(c::kPositionsHistory,
      {{"instType", c::kSwap}, {"limit", "10"}}));
  });
  add("account:leverage-info", [this] {
    return print_response(client_.get(c::kAccountLeverageInfo,
      {{"instId", "BTC-USDT-SWAP"}, {"mgnMode", c::kCross}, {"mrgPosition", c::kMerge}}));
  });
  add("account:set-leverage", [this] {
    const std::string body =
        R"({"instId":"BTC-USDT-SWAP","lever":"17","mgnMode":"cross","mrgPosition":"merge"})";
    return print_response(client_.post(c::kSetLeverage, body));
  });

  add("trade:order", [this] {
    const std::string body =
        R"({"instId":"BTC-USDT-SWAP","tdMode":"cross","ccy":"USDT","side":"buy","posSide":"long","mrgPosition":"split","ordType":"market","sz":"1","px":"1"})";
    return print_response(client_.post(c::kTradeOrder, body));
  });
  add("trade:batch-orders", [this] {
    const std::string body =
        R"({"orders":[{"instId":"BTC-USDT-SWAP","tdMode":"cross","side":"buy","posSide":"long","mrgPosition":"merge","ordType":"market","sz":"1"},{"instId":"ETH-USDT-SWAP","tdMode":"cross","side":"buy","posSide":"long","mrgPosition":"merge","ordType":"market","sz":"1"}]})";
    return print_response(client_.post(c::kTradeBatchOrders, body));
  });
  add("trade:batch-order-query", [this] {
    const std::string body =
        R"({"orders":[{"instId":"BTC-USDT-SWAP","ordId":"1000000000000000"},{"instId":"ETH-USDT-SWAP","ordId":"1000000000000001"}]})";
    return print_response(client_.post(c::kTradeBatchOrderQuery, body));
  });
  add("trade:cancel-order", [this] {
    return print_response(client_.post(c::kTradeCancelOrder, R"({"instId":"BTC-USDT-SWAP","ordId":"1000000000000000"})"));
  });
  add("trade:replace-order", [this] {
    return print_response(client_.post(c::kTradeReplaceOrder, R"({"orderSysID":"1000000000000000","price":70000,"volume":1})"));
  });
  add("trade:batch-cancel-order", [this] {
    return print_response(client_.post(c::kTradeBatchCancelOrder, R"({"orderSysIDs":["1000000000000000","1000000000000001"]})"));
  });
  add("trade:cancel-all", [this] {
    return print_response(client_.post(c::kTradeCancelAll, R"({"instrumentID":"BTCUSDT","productGroup":"SwapU","IsCrossMargin":1,"IsMergeMode":0})"));
  });
  add("trade:fills", [this] {
    return print_response(client_.get(c::kTradeFills, {{"instType", c::kSwap}, {"instId", "BTC-USDT-SWAP"}}));
  });
  add("trade:order-by-id", [this] {
    return print_response(client_.get(c::kTradeOrderById, {{"instId", "BTC-USDT-SWAP"}, {"ordId", "1000000000000000"}}));
  });
  add("trade:finish-order-by-id", [this] {
    return print_response(client_.get(c::kTradeFinishOrderById, {{"instId", "BTC-USDT-SWAP"}, {"ordId", "1000000000000000"}}));
  });
  add("trade:orders-history", [this] {
    return print_response(client_.get(c::kTradeHistoryOrder, {{"instType", c::kSwap}, {"ordType", "market"}}));
  });
  add("trade:orders-pending", [this] {
    return print_response(client_.get(c::kTradePendingOrder, {{"instId", "BTC-USDT-SWAP"}, {"index", "1"}, {"limit", "100"}}));
  });
  add("trade:funding-rate", [this] {
    return print_response(client_.get(c::kTradeFundingRate, {{"instType", "SwapU"}}));
  });
  add("trade:current-funding-rate", [this] {
    return print_response(client_.get(c::kTradeCurrentFundingRate, {{"instId", "BTC-USDT-SWAP"}}));
  });
  add("trade:history-funding-rate", [this] {
    return print_response(client_.get(c::kTradeHistoryFundingRate, {{"instId", "BTC-USDT-SWAP"}, {"limit", "10"}}));
  });
  add("trade:trigger-order", [this] {
    return print_response(client_.post(c::kTradeTriggerOrder, R"({"instId":"BTC-USDT-SWAP","tdMode":"cross","side":"buy","posSide":"long","ordType":"market","sz":"1","triggerPx":"50000","triggerPxType":"last"})"));
  });
  add("trade:cancel-trigger-order", [this] {
    return print_response(client_.post(c::kTradeCancelTriggerOrder,
      R"({"instId":"BTC-USDT-SWAP","ordId":"1000000000000000"})"));
  });
  add("trade:cancel-trigger-all", [this] {
    return print_response(client_.post(c::kTradeCancelTriggerAll,
      R"({"ProductGroup":"SwapU","InstrumentID":"BTCUSDT","IsCrossMargin":-1,"IsMergeMode":-1})"));
  });
  add("trade:batch-close-position", [this] {
    return print_response(client_.post(c::kTradeBatchClosePosition, R"({"instId":"BTC-USDT-SWAP","posSide":"long","mgnMode":"cross"})"));
  });
  add("trade:replace-order-sltp", [this] {
    return print_response(client_.post(c::kTradeReplaceOrderSlTp, R"({"orderSysID":"1000000000000000","tpTriggerPx":110000,"slTriggerPx":70000})"));
  });
  add("trade:close-position-by-ids", [this] {
    return print_response(client_.post(c::kTradeClosePositionByIds, R"({"posIds":["1000000000000000"]})"));
  });
  add("trade:set-position-sltp", [this] {
    return print_response(client_.post(c::kTradeSetPositionSlTp, R"({"posId":"1000000000000000","tpTriggerPx":"110000","slTriggerPx":"70000"})"));
  });
  add("trade:modify-position-sltp", [this] {
    return print_response(client_.post(c::kTradeModifyPositionSlTp, R"({"posId":"1000000000000000","tpTriggerPx":"110000","slTriggerPx":"70000"})"));
  });
  add("trade:cancel-position-sltp", [this] {
    return print_response(client_.post(c::kTradeCancelPositionSlTp, R"({"posId":"1000000000000000"})"));
  });
  add("trade:trigger-orders-pending", [this] {
    return print_response(client_.get(c::kTradeTriggerOrdersPending, {{"instId", "BTC-USDT-SWAP"}, {"limit", "10"}}));
  });
  add("trade:trigger-orders-history", [this] {
    return print_response(client_.get(c::kTradeTriggerOrdersHistory, {{"instId", "BTC-USDT-SWAP"}, {"limit", "10"}}));
  });
  add("trade:trace-order", [this] {
    return print_response(client_.post(c::kTradeTraceOrder, R"({"instId":"BTC-USDT-SWAP","posSide":"long","callbackRatio":"0.01","sz":"1"})"));
  });
  add("trade:trace-order-list", [this] {
    return print_response(client_.get(c::kTradeTraceOrderList, {{"instId", "BTC-USDT-SWAP"}, {"limit", "10"}}));
  });
  add("trade:merge-positions", [this] {
    return print_response(client_.post(c::kTradeMergePositions,
      R"({"instId":"BTC-USDT-SWAP","posIds":["1000000000000000","1000000000000001"]})"));
  });
  add("trade:increase-position", [this] {
    return print_response(client_.post(c::kTradeIncreasePosition,
      R"({"instId":"BTC-USDT-SWAP","posId":"1000000000000000","ordType":"market","sz":"1"})"));
  });

  add("listenkey:acquire", [this] {
    return print_response(client_.get(c::kListenKeyAcquire));
  });
  add("listenkey:extend", [this] {
    return print_response(client_.get(c::kListenKeyExtend, {{"listenKey", "replace-with-listen-key"}}));
  });

  add("copytrading:leader-settings", [this] {
    return print_response(client_.post(c::kCopyLeaderSettings, R"({"status":0,"homeMode":1,"isClosedCopyCode":true,"copyCode":""})"));
  });
  add("copytrading:support-contracts", [this] {
    return print_response(client_.get(c::kCopySupportContracts));
  });
  add("copytrading:set-contracts", [this] {
    return print_response(client_.post(c::kCopySetContracts, R"({"constracts":["BTCUSDT","ETHUSDT"]})"));
  });
  add("copytrading:leader-position", [this] {
    return print_response(client_.get(c::kCopyLeaderPosition, {{"pageNum", "1"}, {"pageSize", "1"}}));
  });
  add("copytrading:estimate-profit", [this] {
    return print_response(client_.get(c::kCopyEstimateProfit, {{"pageNum", "1"}, {"pageSize", "20"}}));
  });
  add("copytrading:history-profit", [this] {
    return print_response(client_.get(c::kCopyHistoryProfit));
  });
  add("copytrading:follower-rank", [this] {
    return print_response(client_.get(c::kCopyFollowerRank, {{"status", "2"}}));
  });
  add("copytrading:get-account-ids", [this] {
    return print_response(client_.get(c::kCopyGetAccountIds));
  });
  add("copytrading:position-type", [this] {
    return print_response(client_.get(c::kCopyPositionType));
  });
  add("copytrading:update-position-type", [this] {
    return print_response(client_.post(c::kCopyPositionType, R"({"positionType":1})"));
  });

  add("asset:internal-transfer-support", [this] {
    return print_response(client_.get(c::kInternalTransferSupport));
  });
  add("asset:internal-transfer", [this] {
    return print_response(client_.post(c::kInternalTransfer, R"({"amount":"10","coin":"USDT","receiverUID":"36007196"})"));
  });
  add("asset:internal-transfer-history", [this] {
    return print_response(client_.get(c::kInternalTransferHistory));
  });
  add("asset:deposit-list", [this] {
    return print_response(client_.get(c::kAssetDepositList, {{"size", "50"}}));
  });
  add("asset:withdraw-list", [this] {
    return print_response(client_.get(c::kAssetWithdrawList, {{"size", "10"}}));
  });
  add("asset:recharge-chain-list", [this] {
    return print_response(client_.get(c::kAssetRechargeChainList, {{"coin", "USDT"}}));
  });
  add("asset:transfer", [this] {
    return print_response(client_.post(c::kAssetTransfer, R"({"coin":"USDT","amount":"10","from":"fund","to":"swap"})"));
  });
  add("asset:withdrawal", [this] {
    const std::string body =
        R"({"ccy":"USDT","chain":"USDT-TRC20","amt":"30","addressId":"replace-with-address-id"})";
    return print_response(client_.post(c::kAssetWithdrawal, body));
  });
  add("asset:cancel-withdrawal", [this] {
    return print_response(client_.post(c::kAssetCancelWithdrawal,
      R"({"wdId":"replace-with-wd-id","ccy":"USDT"})"));
  });
  add("asset:withdraw-assets", [this] {
    return print_response(client_.get(c::kAssetWithdrawAssets, {{"ccy", "USDT"}}));
  });
  add("asset:withdraw-chains", [this] {
    return print_response(client_.get(c::kAssetWithdrawChains, {{"ccy", "USDT"}}));
  });
  add("asset:withdraw-addresses", [this] {
    return print_response(client_.get(c::kAssetWithdrawAddresses, {{"ccy", "USDT"}}));
  });
  add("asset:withdraw-config", [this] {
    return print_response(client_.get(c::kAssetWithdrawConfig, {{"ccy", "USDT"}}));
  });

  add("subaccount:list", [this] {
    return print_response(client_.get(c::kSubAccountList));
  });
  add("subaccount:transfer", [this] {
    return print_response(client_.post(c::kSubAccountTransfer, R"({"coin":"USDT","amount":"10","from":"main","to":"sub","subUid":"replace-with-sub-uid"})"));
  });
  add("subaccount:transfer-record", [this] {
    return print_response(client_.get(c::kSubAccountTransferRecord, {{"limit", "10"}}));
  });

  add("rebate:rate", [this] {
    return print_response(client_.get(c::kRebateConfig, {{"uid", "replace-with-uid"}}));
  });
  add("rebate:config", [this] {
    return print_response(client_.post(c::kRebateConfig, R"({"uid":"replace-with-uid","rate":"0.1"})"));
  });
  add("rebate:agents-users", [this] {
    return print_response(client_.get(c::kAgentsUsers, {{"limit", "10"}}));
  });
  add("rebate:list", [this] {
    return print_response(client_.get(c::kRebateList, {{"limit", "10"}}));
  });
  add("rebate:total", [this] {
    return print_response(client_.get(c::kRebateTotal, {{"limit", "10"}}));
  });

}

int ExampleRegistry::run(const std::string& command) const {
  const auto it = commands_.find(command);
  if (it == commands_.end()) {
    std::cerr << "unknown command: " << command << "\n\n";
    print_commands();
    return 2;
  }
  return it->second();
}

void ExampleRegistry::print_commands() const {
  std::cerr << "Available commands:\n";
  for (const auto& name : names()) {
    std::cerr << "  " << name << '\n';
  }
}

std::vector<std::string> ExampleRegistry::names() const {
  std::vector<std::string> result;
  for (const auto& entry : commands_) {
    result.push_back(entry.first);
  }
  return result;
}

void ExampleRegistry::add(const std::string& name, Command command) {
  commands_.emplace(name, std::move(command));
}

int ExampleRegistry::print_response(const HttpResponse& response) const {
  std::cout << "HTTP " << response.status << '\n' << response.body << '\n';
  return response.status >= 200 && response.status < 300 ? 0 : 1;
}

}  // namespace deepcoin
