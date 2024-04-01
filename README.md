# Keyser Protocol

The Keyser Protocol is a decentralized, P2P, network protocol that allows for peers to maintain
the Keyser block chain.

## Keyser Core

Keyser Core is a "full-node" software for connecting to the Keyser P2P network
and partaking in the validation of the blockchain.

## Port Configs

Upon startup of the software, the user will be prompted to provide 2 port numbers. The first
will be the port on which the node will listen for incoming TCP connections from other nodes on the P2P
network. The second will be the port on which the node will listen for incoming HTTP requests
from clients making calls to the RPC API. Once provided with open ports, the software will initialize
multiple processes in order to discover its peers on the network and begin contributing to the validation
of chain data.

## Peer Discovery

Upon startup of the node, it has no knowledge of any peers on the network and must be provided the IP address
and port number of at least one node on the network. After making a successful connection, the node will recieve
info of other peers on the network and begin making additional connections.

## Block Chain

Each peer on the Keyser network stores and maintains a copy of the Keyser block chain. The block chain is an immutable,
append-only ledger of transactions that is validated by a set of consensus mechanisms as determined by the protocol. Each
block within the block chain contains both a header a body.

### Block Header structure

<table>
    <tr>
        <th>Name</th>
        <th>Description</th>
    </tr>
    <tr>
        <td>Index</td>
        <td>The position of the current block within the blockchain.</td>
    </tr>
    <tr>
        <td>Time</td>
        <td>
            A Unix epoch time when the block was mined and distributed to the rest of the network.
        </td>
    </tr>
    <tr>
        <td>Nonce</td>
        <td>
            An arbitrary number that when changed, will result in a different hash output for the overall block.
        </td>
    </tr>
    <tr>
        <td>Prev Hash</td>
        <td>The hash of the previous block in the chain.</td>
    </tr>
    <tr>
        <td>Hash</td>
        <td>The hash of the current block.</td>
    </tr>
</table>

## RPC API

The Keyser RPC API is a set of API's that allow for developers to interact with a "full-node". There are several
reasons why a developer would want to interact with a node ranging from wanting to create a custom
GUI/CLI to simply wanting to monitor chain or network data. Developers can send an HTTP POST request to a node,
containing a body that conforms to the following JSON structure.

### RPC API JSON structure

```json
{
    "id": "...",
    "method": "...",
    "params": ["..."]
}
```

The "id" field is for a developer provided id to aid in tracking outgoing API requests and their corresponding
responses. The "method" field is used to specify which particular RPC is to be called. The "params" field is
used to supply parameters to the particular RPC that is being called. Below is an example of a JSON object that
could be used to call the "getBalance" RPC.

```json
{
    "id": "10000",
    "method": "getBalance",
    "params": ["0xc6d8a2c830495d07318212e9f2cad16f"]
}
```

### Curl RPC API request example

```bash
curl --data-binary '{"id": "10000", "method": "getBalance", "params": ["0xc6d8a2c830495d07318212e9f2cad16f"]}' -H 'content-type: text/plain' http://127.0.0.1:8080
```

## Test wallets

| AJ             |                                                                                                                                    |
|----------------|------------------------------------------------------------------------------------------------------------------------------------|
| Public address | 0xc6d8a2c830495d07318212e9f2cad16f                                                                                                 |
| Private Key    | 38B15E3C2210827DEB42C39FBC04D2D8268B5F7B7F1DC2DA75FD620BBD2F4E01                                                                   |
| UPublic Key    | 04D82B2CAF4C6D53916F7B2A65F5F90D2F36A8CA5A232F215A696FC79AF96B4344B8683C44B28983DC6816AA31B3394C3860269E476D246F6AD9046C0CD7040B50 |

| Guy            |                                                                                                                                    |
|----------------|------------------------------------------------------------------------------------------------------------------------------------|
| Public address | 0x183944191006324a447bdb2d98d4b408                                                                                                 |
| Private Key    | 18E14A7B6A307F426A94F8114701E7C8E774E7F9A47E2C2035DB29A206321725                                                                   |
| UPublic Key    | 0450863AD64A87AE8A2FE83C1AF1A8403CB53F53E486D8511DAD8A04887E5B23522CD470243453A299FA9E77237716103ABC11A1DF38855ED6F2EE187E9C582BA6 |

| Genesis        |                                                                                                                                    |
|----------------|------------------------------------------------------------------------------------------------------------------------------------|
| Public address | 0x33a1f6d9f637df8a805f6cc659d2366b                                                                                                 |
| Private Key    | 279E1F972138A63F3EB46B0513B20ECCFCBD6CAF80443E0A076D456556666BEC                                                                   |
| UPublic Key    | 046E1C1B9214B0E59C89C0C4CB9A14723178157141C0BE7FA08469F12572BF839BB5EBC4C3AFB853792E67E7EDC9CD141881BF6403CB25FE62942FC20CED34F476 |
