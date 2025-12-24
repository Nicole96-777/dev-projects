/*
# MIT License. See readme file.
*/
#include <iostream>
#include <string>
#include <queue>

#include <vector>

using namespace std;

class Node{
public:
    char chr;
    int freq;
    Node* left;
    Node* right;

    Node(char c, int f);
    ~Node();
};


class Compare{
public:
    bool operator()(Node* a, Node* b);
};

class Huffman{
private:
    Node* root;
    string codes[256];


    void buildCodes(Node* node, const  string& code);
    void freeTree(Node* node);

public:
    Huffman();
    ~Huffman();

    void BuildTree(const  string& text);
    string encode(const  string& text);
    string decode(const  string& encoded);

    void printCodes()const;
};
    
	


Node::Node(char c, int f)
{
    chr = c;
    freq = f;
    left = nullptr;
    right = nullptr;
}

Node::~Node() {}

bool Compare::operator()(Node* a, Node* b)
{
    return a->freq > b->freq;
}

Huffman::Huffman()
{
    root = nullptr;
}

Huffman::~Huffman()
{
    freeTree(root);
}

void Huffman::freeTree(Node* node)
{
    if (!node)
        return;
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}
void Huffman::buildCodes(Node *node, const string &code)
{
    if (!node)
        return;
    if (!node->left && !node->right)
    {
        codes[(unsigned char)node->chr] = code;
        return;
    }
    buildCodes(node->left, code + "0");
    buildCodes(node->right, code + "1");
}

void Huffman::BuildTree(const string &text)
{
    int freq[256] = {0};

    for (char c : text)
        freq[(unsigned char)c]++;

    priority_queue<Node* , vector<Node *>, Compare> pq;
    for (int i = 0; i < 256; i++)
    {
        if (freq[i] > 0){
            pq.push(new Node((char)i, freq[i]));
        }
    }

    while (pq.size() > 1)
    {
        Node *left = pq.top();
        pq.pop();
        Node *right = pq.top();
        pq.pop();

        Node *parent = new Node('\0', left->freq + right->freq);
        parent->left = left;
        parent->right = right;

        pq.push(parent);
    }

    root = pq.top();

    for (int i = 0; i < 256; i++)
        codes[i] = "";

    buildCodes(root, "");
}

string Huffman::encode(const string &text)
{
    string encoded = "";
    for (char c : text)
        encoded += codes[(unsigned char)c];
    return encoded;
}
string Huffman::decode(const string &encoded)
{
    string decoded = "";
    Node *current = root;

    for (char bit : encoded)
    {
        current = (bit == '0') ? current->left : current->right;

        if (!current->left && !current->right)
        {
            decoded += current->chr;
            current = root;
        }
    }
    return decoded;
}   
void Huffman::printCodes()const{
   for (int i = 0; i < 256; i++) {
       if (!codes[i].empty()) {
            cout << (char)i << " : " << codes[i] << "\n";
            }
        }
    }
    


int main() {
     Huffman h;

    string text = "hello huffman coding";

    h.BuildTree(text);

     string encoded = h.encode(text);
     string decoded = h.decode(encoded);

   cout << "Original: " << text << "\n";
     cout << "Encoded:  " << encoded << "\n";
     cout << "Decoded:  " << decoded << "\n\n";

    h.printCodes();
    return 0;
}
