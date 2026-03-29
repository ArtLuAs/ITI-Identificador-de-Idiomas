#include "../headers/ContextModel.hpp"
#include <stdexcept>

ContextModel::ContextModel(int k) : maxOrder(k) {
    if (k < 0)
        throw std::invalid_argument("Erro: A ordem (k) do modelo PPM deve ser >= 0.");

    root = std::make_unique<TrieNode>();

    // Ordem 0: raiz conhece todos os símbolos do alfabeto (0-255) + EOF (257)
    for (uint32_t i = 0; i < 256; i++)
        root->incrementSymbol(i);
    root->incrementSymbol(257);
}

std::vector<TrieNode*> ContextModel::getActiveContextNodes() const {
    std::vector<TrieNode*> nodes;

    for (size_t start = 0; start < history.size(); ++start) {
        TrieNode* current = root.get();
        bool found = true;

        for (size_t i = start; i < history.size(); ++i) {
            current = current->getChild(history[i]);
            if (current == nullptr) { found = false; break; }
        }

        if (found) nodes.push_back(current);
    }

    nodes.push_back(root.get()); // Ordem-0 como fallback
    return nodes;
}

void ContextModel::updateAndShift(uint32_t symbol) {
    for (size_t start = 0; start <= history.size(); ++start) {
        TrieNode* current = root.get();

        for (size_t i = start; i < history.size(); ++i) {
            TrieNode* next = current->getChild(history[i]);
            if (next == nullptr) next = current->createChild(history[i]);
            current = next;
        }

        current->incrementSymbol(symbol);
    }

    history.push_back(symbol);
    if (history.size() > static_cast<size_t>(maxOrder))
        history.pop_front();
}

void ContextModel::forceHistory(uint32_t symbol) {
    history.push_back(symbol);
    if (history.size() > static_cast<size_t>(maxOrder))
        history.pop_front();
}
