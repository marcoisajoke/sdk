#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <vector>
namespace mega
{
class Trie {
private:
    struct TrieNode {
        std::unique_ptr<TrieNode> children[26];  // 26个小写字母的定长数组
        bool isEndOfWord; // 标记是否是一个单词的结尾
        int value;
        
        TrieNode() : isEndOfWord(false),value(-1) {
            // 初始化所有子节点为nullptr
            for (int i = 0; i < 26; ++i) {
                children[i] = nullptr;
            }
        }
    };
    
    std::unique_ptr<TrieNode> root;

    // 字符到索引的转换
    int charToIndex(char c) const {
        return c - 'a';
    }
    
    // 索引到字符的转换
    char indexToChar(int index) const {
        return 'a' + (char)index;
    }

public:
    Trie() : root(std::make_unique<TrieNode>()) {}
    
    // 插入单词
    void insert(const std::string& word, int value) {
        TrieNode* current = root.get();
        
        for (char c : word) {
            int index = charToIndex(c);
            
            if (current->children[index] == nullptr) {
                current->children[index] = std::make_unique<TrieNode>();
            }
            
            current = current->children[index].get();
        }
        
        current->isEndOfWord = true;
        current->value = value;
    }
    
    // 搜索单词是否存在
    int search(const std::string& word) const {
        TrieNode* node = findNode(word);
        if(node != nullptr && node->isEndOfWord) {
            return node->value;
        }
        return -1;
    }
    
    // 检查是否有以prefix为前缀的单词
    bool startsWith(const std::string& prefix) const {
        return findNode(prefix) != nullptr;
    }
    
    // 删除单词
    //bool remove(const std::string& word) {
    //    return removeHelper(root.get(), word, 0);
    //}
    
    // 获取所有存储的单词
    std::vector<std::string> getAllWords() const {
        std::vector<std::string> words;
        std::string currentWord;
        collectWords(root.get(), currentWord, words);
        return words;
    }
    
    // 打印Trie树结构
    void printTrie() const {
        //printTrieHelper(root.get(), "", 0);
    }

private:
    // 查找单词对应的节点
    TrieNode* findNode(const std::string& str) const {
        TrieNode* current = root.get();
        
        for (char c : str) {
            int index = charToIndex(c);
            
            if (current->children[index] == nullptr) {
                return nullptr;
            }
            
            current = current->children[index].get();
        }
        
        return current;
    }
    
    // 递归删除辅助函数
    /*
    bool removeHelper(TrieNode* node, const std::string& word, int depth) {
        if (node == nullptr) {
            return false;
        }
        
        // 到达单词末尾
        if (depth == word.length()) {
            if (!node->isEndOfWord) {
                return false;  // 单词不存在
            }
            
            node->isEndOfWord = false;
            
            // 如果节点没有子节点，则可以删除
            return isEmptyNode(node);
        }
        
        // 递归处理
        int index = charToIndex(word[depth]);
        if (removeHelper(node->children[index].get(), word, depth + 1)) {
            // 子节点可以删除，释放内存
            node->children[index].reset();
            
            // 如果当前节点不是单词结尾且没有其他子节点，也可以删除
            return !node->isEndOfWord && isEmptyNode(node);
        }
        
        return false;
    }*/
    
    // 检查节点是否为空（没有子节点）
    bool isEmptyNode(TrieNode* node) const {
        for (int i = 0; i < 26; ++i) {
            if (node->children[i] != nullptr) {
                return false;
            }
        }
        return true;
    }
    
    // 收集所有单词
    void collectWords(TrieNode* node, std::string& currentWord, 
                     std::vector<std::string>& words) const {
        if (node == nullptr) {
            return;
        }
        
        if (node->isEndOfWord) {
            words.push_back(currentWord);
        }
        
        for (int i = 0; i < 26; ++i) {
            if (node->children[i] != nullptr) {
                currentWord.push_back(indexToChar(i));
                collectWords(node->children[i].get(), currentWord, words);
                currentWord.pop_back();
            }
        }
    }
    
    // 打印Trie树辅助函数
    /*
    void printTrieHelper(TrieNode* node, const std::string& prefix, int level) const {
        if (node == nullptr) {
            return;
        }
        
        std::string indent(level * 2, ' ');
        
        if (node->isEndOfWord) {
            std::cout << indent << "[" << prefix << "] (word end)" << std::endl;
        }
        
        for (int i = 0; i < 26; ++i) {
            if (node->children[i] != nullptr) {
                char c = indexToChar(i);
                std::cout << indent << "└─ '" << c << "'" << std::endl;
                printTrieHelper(node->children[i].get(), prefix + c, level + 1);
            }
        }
    }
    */
};
}