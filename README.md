# DeepCoin OpenAPI C++ Example

C++17 版本的 DeepCoin OpenAPI 示例，目录结构与 Go/Python 示例一致，分为 REST API 和 WebSocket API。

```text
rest/
  config/master.yaml
  include/deepcoin/
  src/
ws/
  include/deepcoin/
  src/
```

## 依赖

- C++17 编译器
- libcurl
- OpenSSL
- make

macOS 通常可直接使用系统 libcurl；OpenSSL 可通过 Homebrew 安装，`Makefile` 默认查找 `/opt/homebrew`。

## 配置

在 `rest/config/master.yaml` 填入 API 凭据：

```yaml
api:
  url: "https://api.deepcoin.com"
  key: ""
  secret_key: ""
  passphrase: ""
```

也可以用环境变量覆盖：

```sh
export DEEPCOIN_API_KEY="..."
export DEEPCOIN_SECRET_KEY="..."
export DEEPCOIN_PASSPHRASE="..."
export DEEPCOIN_API_URL="https://api.deepcoin.com"
```

## 构建

```sh
make
```

## 运行

查看命令列表：

```sh
./build/deepcoin_rest help
```

公共行情接口不需要 API 凭据：

```sh
./build/deepcoin_rest market:time
./build/deepcoin_rest market:tickers
./build/deepcoin_ws
```

私有接口需要配置 API 凭据：

```sh
./build/deepcoin_rest account:balance
./build/deepcoin_rest trade:fills
./build/deepcoin_rest listenkey:acquire
```

注意：`trade:*`、`account:set-leverage`、`asset:*`、`copytrading:*`、`rebate:*` 中部分命令会提交真实 POST 请求。运行前请检查 `rest/src/examples.cpp` 中的示例参数，避免对真实账户产生非预期操作。

## 签名规则

签名明文与 Go/Python 示例保持一致：

```text
timestamp + method + requestPath + body
```

- `timestamp` 格式：UTC ISO8601 毫秒，例如 `2026-05-11T08:00:00.123Z`
- `method` 使用大写 `GET` 或 `POST`
- `requestPath` 包含 path 和 GET query，例如 `/deepcoin/account/balances?instType=SWAP&ccy=USDT`
- `body` 是 POST 的 JSON 字符串，GET 为空
- 签名算法：HMAC-SHA256 后 Base64
