#ifndef CONTEXTMODEL_HPP
#define CONTEXTMODEL_HPP

#include <deque>
#include <vector>
#include <memory>
#include <cstdint>
#include "TrieNode.hpp"

class ContextModel {
private:
    int maxOrder;
    std::unique_ptr<TrieNode> root;
    std::deque<uint32_t> history;

public:
    ContextModel(int k);

    // Retorna lista de nós ativos do maior contexto para o menor
    std::vector<TrieNode*> getActiveContextNodes() const;

    // Atualiza frequências na Trie e avança a janela deslizante (uso do Interpretador)
    void updateAndShift(uint32_t symbol);

    // ---- Acesso para serialização (ModeloPersistente) ----
    int getMaxOrder() const { return maxOrder; }
    const std::deque<uint32_t>& getHistory() const { return history; }
    TrieNode* getRoot() const { return root.get(); }

    // Reconstrói o histórico durante o carregamento
    void forceHistory(uint32_t symbol);
};

#endif // CONTEXTMODEL_HPP
