#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <algorithm>
using namespace std;

// A Huffman tree node
struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;
    Node(char ch, int freq)
        : ch(ch), freq(freq), left(nullptr), right(nullptr) {}
    Node(char ch, int freq, Node* left, Node* right)
        : ch(ch), freq(freq), left(left), right(right) {}
};

// Comparison object to order the heap
struct compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

// Print Huffman Codes
void printCodes(Node* root, string str, unordered_map<char, string>& huffmanCode) {
    if (!root) return;
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }
    printCodes(root->left, str + "0", huffmanCode);
    printCodes(root->right, str + "1", huffmanCode);
}

// Free memory
void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

// Main logic
void buildHuffmanTree(const string& text) {
    unordered_map<char, int> freq;
    for (char ch : text) {
        freq[ch]++;
    }

    priority_queue<Node*, vector<Node*>, compare> pq;
    for (const auto& pair : freq) {
        pq.push(new Node(pair.first, pair.second));
    }

    while (pq.size() != 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        int sum = left->freq + right->freq;
        pq.push(new Node('\0', sum, left, right));
    }

    Node* root = pq.top();
    unordered_map<char, string> huffmanCode;
    printCodes(root, "", huffmanCode);

    // Print sorted codes
    vector<pair<char, string>> sortedCodes(huffmanCode.begin(), huffmanCode.end());
    sort(sortedCodes.begin(), sortedCodes.end());

    cout << "\nHuffman Codes:\n";
    for (auto& pair : sortedCodes) {
        cout << pair.first << ": " << pair.second << "\n";
    }

    // Encode
    string encoded = "";
    for (char ch : text) {
        encoded += huffmanCode[ch];
    }

    cout << "\nOriginal string:\n" << text;
    cout << "\n\nEncoded string:\n" << encoded;

    // Decode
    cout << "\n\nDecoded string:\n";
    Node* curr = root;
    for (char bit : encoded) {
        curr = (bit == '0') ? curr->left : curr->right;
        if (!curr->left && !curr->right) {
            cout << curr->ch;
            curr = root;
        }
    }
    cout << "\n";

    freeTree(root);
}

int main() {
    string text;
    cout << "Enter a string to encode using Huffman Coding:\n";
    getline(cin, text);

    if (text.empty()) {
        cout << "Input string is empty.\n";
        return 0;
    }

    buildHuffmanTree(text);
    return 0;
}
