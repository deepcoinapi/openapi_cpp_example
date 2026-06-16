#pragma once

namespace deepcoin::constants {

inline constexpr char kGet[] = "GET";
inline constexpr char kPost[] = "POST";

inline constexpr char kSpot[] = "SPOT";
inline constexpr char kSwap[] = "SWAP";

inline constexpr char kCross[] = "cross";
inline constexpr char kMerge[] = "merge";
inline constexpr char kSplit[] = "split";
inline constexpr char kBuy[] = "buy";
inline constexpr char kMarket[] = "market";
inline constexpr char kLong[] = "long";

inline constexpr char kAccountBalance[] = "/deepcoin/account/balances";
inline constexpr char kAccountAllBalances[] = "/deepcoin/account/all-balances";
inline constexpr char kAccountUid[] = "/deepcoin/account/uid";
inline constexpr char kAccountTradeFee[] = "/deepcoin/account/trade-fee";
inline constexpr char kAccountBills[] = "/deepcoin/account/bills";
inline constexpr char kAccountLeverageInfo[] = "/deepcoin/account/leverage-info";
inline constexpr char kSetLeverage[] = "/deepcoin/account/set-leverage";
inline constexpr char kPositions[] = "/deepcoin/account/positions";
inline constexpr char kPositionsHistory[] = "/deepcoin/account/positions-history";

inline constexpr char kMarketBooks[] = "/deepcoin/market/books";
inline constexpr char kMarketCandles[] = "/deepcoin/market/candles";
inline constexpr char kMarketIndexCandles[] = "/deepcoin/market/index-candles";
inline constexpr char kMarketMarkPrice[] = "/deepcoin/market/mark-price";
inline constexpr char kMarketMarkCandles[] = "/deepcoin/market/mark-price-candles";
inline constexpr char kMarketOpenInterestVolume[] = "/deepcoin/market/open-interest-volume";
inline constexpr char kMarketLongShortRatio[] = "/deepcoin/market/long-short-ratio";
inline constexpr char kMarketTakerVolume[] = "/deepcoin/market/taker-volume";
inline constexpr char kMarketInstruments[] = "/deepcoin/market/instruments";
inline constexpr char kMarketTickers[] = "/deepcoin/market/tickers";
inline constexpr char kMarketTrades[] = "/deepcoin/market/trades";
inline constexpr char kMarketStepMargin[] = "/deepcoin/market/step-margin";
inline constexpr char kMarketBookSpread[] = "/deepcoin/market/book-spread";
inline constexpr char kMarketTime[] = "/deepcoin/market/time";
inline constexpr char kMarketPing[] = "/deepcoin/market/ping";
inline constexpr char kMarketHandicapKline1m[] = "/deepcoin/market/handicap-kline1m";
inline constexpr char kMarketHandicapOrderbook[] = "/deepcoin/market/handicap-orderbook";
inline constexpr char kMarketHandicapTrade[] = "/deepcoin/market/handicap-trade";

inline constexpr char kTradeOrder[] = "/deepcoin/trade/order";
inline constexpr char kTradeCancelOrder[] = "/deepcoin/trade/cancel-order";
inline constexpr char kTradeReplaceOrder[] = "/deepcoin/trade/replace-order";
inline constexpr char kTradeBatchCancelOrder[] = "/deepcoin/trade/batch-cancel-order";
inline constexpr char kTradeCancelAll[] = "/deepcoin/trade/swap/cancel-all";
inline constexpr char kTradeFills[] = "/deepcoin/trade/fills";
inline constexpr char kTradeOrderById[] = "/deepcoin/trade/orderByID";
inline constexpr char kTradeFinishOrderById[] = "/deepcoin/trade/finishOrderByID";
inline constexpr char kTradeHistoryOrder[] = "/deepcoin/trade/orders-history";
inline constexpr char kTradePendingOrder[] = "/deepcoin/trade/v2/orders-pending";
inline constexpr char kTradeFundingRate[] = "/deepcoin/trade/funding-rate";
inline constexpr char kTradeCurrentFundingRate[] = "/deepcoin/trade/fund-rate/current-funding-rate";
inline constexpr char kTradeHistoryFundingRate[] = "/deepcoin/trade/fund-rate/history";
inline constexpr char kTradeTriggerOrder[] = "/deepcoin/trade/trigger-order";
inline constexpr char kTradeBatchClosePosition[] = "/deepcoin/trade/batch-close-position";
inline constexpr char kTradeReplaceOrderSlTp[] = "/deepcoin/trade/replace-order-sltp";
inline constexpr char kTradeClosePositionByIds[] = "/deepcoin/trade/close-position-by-ids";
inline constexpr char kTradeSetPositionSlTp[] = "/deepcoin/trade/set-position-sltp";
inline constexpr char kTradeModifyPositionSlTp[] = "/deepcoin/trade/modify-position-sltp";
inline constexpr char kTradeCancelPositionSlTp[] = "/deepcoin/trade/cancel-position-sltp";
inline constexpr char kTradeTriggerOrdersPending[] = "/deepcoin/trade/trigger-orders-pending";
inline constexpr char kTradeTriggerOrdersHistory[] = "/deepcoin/trade/trigger-orders-history";
inline constexpr char kTradeTraceOrder[] = "/deepcoin/trade/trace-order";
inline constexpr char kTradeTraceOrderList[] = "/deepcoin/trade/trace-order-list";
inline constexpr char kTradeMergePositions[] = "/deepcoin/trade/merge-positions";
inline constexpr char kTradeIncreasePosition[] = "/deepcoin/trade/increase-position";
inline constexpr char kTradeBatchOrders[] = "/deepcoin/trade/batch-orders";
inline constexpr char kTradeBatchOrderQuery[] = "/deepcoin/trade/batch-order-query";
inline constexpr char kTradeCancelTriggerOrder[] = "/deepcoin/trade/cancel-trigger-order";
inline constexpr char kTradeCancelTriggerAll[] = "/deepcoin/trade/swap/cancel-trigger-all";

inline constexpr char kListenKeyAcquire[] = "/deepcoin/listenkey/acquire";
inline constexpr char kListenKeyExtend[] = "/deepcoin/listenkey/extend";

inline constexpr char kCopyLeaderSettings[] = "/deepcoin/copytrading/leader-settings";
inline constexpr char kCopySupportContracts[] = "/deepcoin/copytrading/support-contracts";
inline constexpr char kCopySetContracts[] = "/deepcoin/copytrading/set-contracts";
inline constexpr char kCopyLeaderPosition[] = "/deepcoin/copytrading/leader-position";
inline constexpr char kCopyEstimateProfit[] = "/deepcoin/copytrading/estimate-profit";
inline constexpr char kCopyHistoryProfit[] = "/deepcoin/copytrading/history-profit";
inline constexpr char kCopyFollowerRank[] = "/deepcoin/copytrading/follower-rank";
inline constexpr char kCopyGetAccountIds[] = "/deepcoin/copytrading/get-accountIDs";
inline constexpr char kCopyPositionType[] = "/deepcoin/copytrading/position-type";

inline constexpr char kInternalTransferSupport[] = "/deepcoin/internal-transfer/support";
inline constexpr char kInternalTransfer[] = "/deepcoin/internal-transfer";
inline constexpr char kInternalTransferHistory[] = "/deepcoin/internal-transfer/history-order";
inline constexpr char kAssetDepositList[] = "/deepcoin/asset/deposit-list";
inline constexpr char kAssetWithdrawList[] = "/deepcoin/asset/withdraw-list";
inline constexpr char kAssetRechargeChainList[] = "/deepcoin/asset/recharge-chain-list";
inline constexpr char kAssetTransfer[] = "/deepcoin/asset/transfer";
inline constexpr char kAssetWithdrawal[] = "/deepcoin/asset/withdrawal";
inline constexpr char kAssetCancelWithdrawal[] = "/deepcoin/asset/cancel-withdrawal";
inline constexpr char kAssetWithdrawAssets[] = "/deepcoin/asset/withdraw-assets";
inline constexpr char kAssetWithdrawChains[] = "/deepcoin/asset/withdraw-chains";
inline constexpr char kAssetWithdrawAddresses[] = "/deepcoin/asset/withdraw-addresses";
inline constexpr char kAssetWithdrawConfig[] = "/deepcoin/asset/withdraw-config";

inline constexpr char kSubAccountTransfer[] = "/deepcoin/sub-account/sub-account-transfer";
inline constexpr char kSubAccountTransferRecord[] = "/deepcoin/sub-account/sub-account-transfer-record";
inline constexpr char kSubAccountList[] = "/deepcoin/sub-account/sub-account-list";

inline constexpr char kRebateConfig[] = "/deepcoin/rebate/config";
inline constexpr char kAgentsUsers[] = "/deepcoin/agents/users";
inline constexpr char kRebateList[] = "/deepcoin/agents/users/rebate-list";
inline constexpr char kRebateTotal[] = "/deepcoin/agents/users/rebates";

}  // namespace deepcoin::constants
