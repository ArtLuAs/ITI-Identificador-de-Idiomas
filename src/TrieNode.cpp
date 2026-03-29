#include "../headers/TrieNode.hpp"

TrieNode::TrieNode() {
    // 0-255 (chars), 256 (Escape), 257 (EOF) — sem o 258/reset
    std::vector<uint32_t> initialFreqs(258, 0);
    initialFreqs[256] = 1; // Escape começa com peso > 0
    freqTable = std::make_unique<SimpleFrequencyTable>(initialFreqs);
}

void TrieNode::incrementSymbol(uint32_t symbol) {
    if (freqTable->get(symbol) == 0)
        activeSymbols.push_back(symbol);
    freqTable->increment(symbol);

    // Aging: evita overflow (limite da literatura: 2^14)
    if (freqTable->getTotal() >= 16384)
        freqTable->halveFrequencies();
}

TrieNode* TrieNode::getChild(uint32_t symbol) const {
    auto it = children.find(symbol);
    if (it != children.end()) return it->second.get();
    return nullptr;
}

TrieNode* TrieNode::createChild(uint32_t symbol) {
    children[symbol] = std::make_unique<TrieNode>();
    return children[symbol].get();
}
