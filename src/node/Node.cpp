#include "./Node.hpp"


// keyser::Node::Node(MainMenu* menu, uint16_t port)
// {
//     _startTime = time(nullptr);
//
//     _chain            = std::make_shared<ChainIndex>();
//     // _mempool          = std::make_shared<Mempool>();
//     // _txoSet           = std::make_shared<TxoSet>(this);
//     _network          = std::make_shared<NetInterface>(this, menu, port);
// }

keyser::Node::Status keyser::Node::status() const
{
    return _status;
}

void keyser::Node::status(Status status)
{
    _status = status;
}

time_t keyser::Node::getUptime() const
{
    return time(NULL) - _startTime;
}

// void keyser::Node::beginMining(uint numBlocks)
// {
//     if (_miner.use_count() == 0)
//     {
//         _miner = std::make_shared<Miner>(this);
//     }
//
//     _miner->start(numBlocks);
// }
//
// void keyser::Node::stopMining()
// {
//     if (_miner.use_count() == 1)
//     {
//         _miner.reset();
//     }
// }

bool keyser::Node::submitTransaction(Transaction transaction)
{
    // if (_validationEngine->validateTransaction(transaction))
    // {
    //     _network->distributeTransaction(transaction);
    //     return true;
    // }
    return false;
}