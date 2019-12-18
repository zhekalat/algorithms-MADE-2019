// Латышев Евгений. Задача 9
//
// Реализуйте алгоритм построения оптимального префиксного кода Хаффмана.
// При помощи алгоритма реализуйте две функции для создания архива из одного файла и извлечения файла из архива.

//#include "Huffman.h"
#include <iostream>
#include <vector>
#include <map>
#include <queue>

typedef unsigned char byte;

struct IOutputStream {
    std::vector<byte> contents;

    void Write(byte value) {
        contents.push_back(value);
    }
};

struct IInputStream {
    std::vector<byte> contents;
    int position = 0;

    IInputStream(std::vector<byte>& v) : contents(v) {};
    IInputStream(IOutputStream& s) : contents(s.contents) {};

    bool Read(byte& value) {
        if (position < contents.size()) {
            value = contents[position++];
            return true;
        }
        return false;
    }
};

// Битовая обертка над байтовыми потоками
class BitsIO {
public:
    explicit BitsIO(IInputStream& _s1, IOutputStream& _s2) : inputStream(_s1), outputStream(_s2) {};
    ~BitsIO() = default;
    BitsIO(const BitsIO&) = delete;
    BitsIO(BitsIO&&) = delete;
    BitsIO& operator=(const BitsIO&) = delete;
    BitsIO& operator=(BitsIO&&) = delete;

    void WriteBit(bool bit); // Осуществляет побитовую запись в поток
    void WriteByte(byte byte); // Осуществляет побайтовую запись в поток
    void WriteRemainingBits(); // Осуществляет сброс буфера

    bool ReadBit(bool& value); // Осуществляет побитовое чтение из потока
    bool ReadByte(byte& value); // Осуществляет побайтовое чтение из потока
private:
    std::vector<byte> buffer;
    IInputStream& inputStream;
    IOutputStream& outputStream;
    bool streamEnded = false;
    int position = 0;

    byte accumulatorOutput = 0;
    int bitsCountOutput = 0;
    byte accumulatorInput = 0;
    int bitsCountInput = 8;
};

// Добавляем бит в аккумулятор. Если аккумулятор заполнен, пишем его и сбрасываем
void BitsIO::WriteBit(bool bit) {
    accumulatorOutput |= static_cast<byte>(bit) << bitsCountOutput;
    bitsCountOutput++;
    if (bitsCountOutput == 8) {
        bitsCountOutput = 0;
        outputStream.Write(accumulatorOutput);
        accumulatorOutput = 0;
    }
}

// Если аккумулятор пустой, пишем байт. Иначе дополняем аккумулятор, пишем его, а остаток сохраняем в аккумулятор
void BitsIO::WriteByte(byte byte) {
    if (bitsCountOutput == 0) {
        outputStream.Write(byte);
    } else {
        accumulatorOutput |= byte << bitsCountOutput;
        outputStream.Write(accumulatorOutput);
        accumulatorOutput = byte >> (8 - bitsCountOutput);
    }
}

// Дописываем оставшиеся в конце биты
void BitsIO::WriteRemainingBits() {
    if (bitsCountOutput != 0) {
        bitsCountOutput = 0;
        outputStream.Write(accumulatorOutput);
        accumulatorOutput = 0;
    }
}

//
bool BitsIO::ReadBit(bool& value) {
    if (bitsCountInput == 8) {
        if (!streamEnded) {
            if (!inputStream.Read(accumulatorInput)) {
                streamEnded = true;
                return false;
            }
            buffer.push_back(accumulatorInput);
            position++;
        } else {
            if (position == buffer.size()) {
                return false;
            }
            accumulatorInput = buffer[position++];
        }
        bitsCountInput = 0;
    }
    value = (accumulatorInput & (1 << bitsCountInput)) >> bitsCountInput;
    bitsCountInput++;
    return true;
}

bool BitsIO::ReadByte(byte& value) {
    if (bitsCountInput == 0) {
        value = accumulatorInput;
        return true;
    } else if (bitsCountInput == 8) {
        if (!streamEnded) {
            if (!inputStream.Read(value)) {
                streamEnded = true;
                position = 0;
                bitsCountInput = 8;
                accumulatorInput = 0;
                return false;
            }
            buffer.push_back(value);
            position++;
            return true;
        } else {
            if (position == buffer.size()) {
                return false;
            }
            value = buffer[position++];
            return true;
        }
    } else {
        byte newValue = 0;
        if (!streamEnded) {
            if (inputStream.Read(newValue)) {
                buffer.push_back(newValue);
            }
            else {
                streamEnded = true;
            }
        } else {
            if (position < buffer.size()) {
                newValue = buffer[position++];
            }
        }
        value = (newValue << (8 - bitsCountInput)) | (accumulatorInput >> bitsCountInput);
        if (streamEnded) {
            position = 0;
            bitsCountInput = 8;
            accumulatorInput = 0;
        } else {
            accumulatorInput = newValue;
        }
        return true;
    }
}


class Huffman {
public:
    Huffman() = default;
    ~Huffman() = default;
    Huffman(const Huffman&) = delete;
    Huffman(Huffman&&) = delete;
    Huffman& operator=(const Huffman&) = delete;
    Huffman& operator=(Huffman&&) = delete;

    void BuildHuffmanTree(BitsIO& bitStream);  // Строит канонический код Хаффмана по исходному потоку бит
    void RestoreHuffmanTree(BitsIO& bitStream);  // Восстанавливает дерево Хаффмана по закодированному потоку бит
    void Encode(BitsIO& bitStream);  // Кодирует входную последовательность
    void Decode(BitsIO& bitStream);  // Декодирует закодированную последовательность в исходную
private:
    // Узел дерева Хаффмана
    struct TreeNode {
        explicit TreeNode(byte _symbol, int _count) : symbol(_symbol), count(_count) {}
        byte symbol = '|';
        int count = 0;
        std::shared_ptr<TreeNode> left = nullptr, right = nullptr;
    };
    // Компаратор для сравнения объектов TreeNode для реализации minHeap
    class myComparator {
    public:
        bool operator() (std::shared_ptr<TreeNode>& p1, std::shared_ptr<TreeNode>& p2)
        {
            return p1->count > p2->count;
        }
    };
    
    std::shared_ptr<TreeNode> tree = nullptr; // Дерево Хаффмана
    std::map<byte, int> counts; // Таблица частот встречаемости байтов
    std::map<int, std::unique_ptr<std::vector<byte>>> viewInternal; // Внутреннее представление дерева Хаффмана
    std::map<byte, std::pair<int, int>> viewCanonical; // Каноническое представление дерева Хаффмана
    std::map<byte, std::pair<int, int>> viewCanonical2; // Каноническое представление дерева Хаффмана

    void streamToCountsMap(BitsIO& bitStream);  // Строит таблицу частот встречаемости байт во входном потоке
    void countsMapToTree();  // Строит дерево Хаффмана по таблице частот
    void treeToCanonicalCode();  // Строит таблицу оптимальных кодов Хаффмана по дереву
    void treeToInternalView() { // Строит внутреннее представление по дереву Хаффмана
        _treeToInternalView(tree, 0);
    }
    void _treeToInternalView(std::shared_ptr<TreeNode>& tree, int length);
    void internalViewToCanonicalCode(); // Строит канонический код Хаффмана по внутреннему представлению
    void encodeInternalView(BitsIO& bitStream); // Кодирует внутреннее представление в поток байт
    void decodeInternalView(BitsIO& bitStream); // Декодирует внутреннее представление из потока байт
    void canonicalCodeToTree(); // Строит дерево Хаффмана по каноническому коду Хаффмана
};

void Huffman::streamToCountsMap(BitsIO& bitStream) {
    byte symbol = 0;
    while (bitStream.ReadByte(symbol)) {
        if(counts.find(symbol) == counts.end()) {
            counts[symbol] = 1;
        } else {
            counts[symbol] += 1;
        }
    }
}

void Huffman::countsMapToTree() {
    // Объёдиняем элементы начиная с самых редко встречаемых с помощью кучи
    std::priority_queue <std::shared_ptr<TreeNode>, std::vector<std::shared_ptr<TreeNode>>, myComparator> minHeap;
    for (auto f : counts) {
        minHeap.push(std::make_shared<TreeNode>(f.first, f.second));
//        std::cout<<"Symbol: "<<f.first<<' '<<"Int: "<<f.second<<'\n';
    }
    while (minHeap.size() > 1) {
        std::shared_ptr<TreeNode> left = minHeap.top();
        minHeap.pop();
        std::shared_ptr<TreeNode> right = minHeap.top();
        minHeap.pop();
        auto NewNode = std::make_shared<TreeNode>('|', left->count+right->count);
        NewNode->left = left;
        NewNode->right = right;
        minHeap.push(NewNode);
    }
    tree = minHeap.top();
}

void Huffman::treeToCanonicalCode() {
    std::queue<std::pair<std::shared_ptr<TreeNode>, int>> nodeQueue;
    nodeQueue.push(std::make_pair(tree, 0));
    int code = 0;
    int prevCodeLen = 0;
    while (!nodeQueue.empty()) {
        std::pair<std::shared_ptr<TreeNode>, int> currNode = nodeQueue.front();
        nodeQueue.pop();
        if (currNode.first->left == nullptr and currNode.first->right == nullptr) {
            if (currNode.second != prevCodeLen) {
                code = code << (currNode.second - prevCodeLen);
            }
            viewCanonical.emplace(currNode.first->symbol, std::make_pair(prevCodeLen, code));
            code += 1;
        } else {
            if (currNode.first->left != nullptr) {
                nodeQueue.push(std::make_pair(currNode.first->left, currNode.second + 1));
            }
            if (currNode.first->right != nullptr) {
                nodeQueue.push(std::make_pair(currNode.first->right, currNode.second + 1));
            }
        }
        prevCodeLen = currNode.second;
    }
    for (auto &v : viewCanonical) {
        byte symbol = v.first;
        int codeLen = v.second.first;
        int code = v.second.second;
        std::cout << "Symbol: " << symbol << " codeLen: " << codeLen << " code: " << code<<'\n';
    }
}

void Huffman::_treeToInternalView(std::shared_ptr<TreeNode>& tree, int length = 0) {
    if (tree->left != nullptr)
        _treeToInternalView(tree->left, length + 1);

    if ((tree->left == nullptr) && (tree->right == nullptr)) {
        if (viewInternal.find(length) == viewInternal.end())
            viewInternal.emplace(length, std::make_unique<std::vector<byte>>());
        viewInternal[length]->push_back(tree->symbol);
    }

    if (tree->right != nullptr)
        _treeToInternalView(tree->right, length + 1);
}

void Huffman::internalViewToCanonicalCode() {
    int code = 0;
    auto it = viewInternal.begin();
    while (it != viewInternal.end()) {
        std::cout<<it->first<<' '<<it->second<<'\n';
        int currentBitLength = it->first;

        for (int i = 0; i < it->second->size(); i++) {
            std::cout<<'\t'<<it->second->at(i)<<'\n';
            viewCanonical.emplace(it->second->at(i), std::make_pair(it->first, code));
            code += 1;
        }

        it = std::next(it);
        if (it != viewInternal.end())
            code = code << (it->first - currentBitLength);
    }
    std::cout << "Normal code:"<<'\n';
    for (auto &v : viewCanonical) {
        byte symbol = v.first;
        int codeLen = v.second.first;
        int code = v.second.second;
        std::cout << "Symbol: " << symbol << " codeLen: " << codeLen << " code: " << code<<'\n';
    }
}

void Huffman::canonicalCodeToTree() {
    tree = std::make_shared<TreeNode>(0, 0);
    for (auto &v : viewCanonical) {
        byte symbol = v.first;
        int codeLen = v.second.first;
        int code = v.second.second;
        std::shared_ptr<TreeNode> root = tree;

        for (int i = codeLen - 1; i >= 0; i--) {
            if ((code >> i) & 1) {  // Направо
                if (root->right == nullptr)
                    root->right = std::make_shared<TreeNode>(0, 0);
                root = root->right;
            } else {  // Иначе налево
                if (root->left == nullptr)
                    root->left = std::make_shared<TreeNode>(0, 0);
                root = root->left;
            }
        }
        root->symbol = symbol;
    }
}

void Huffman::encodeInternalView(BitsIO& bitStream) {
    byte nBitLengths = viewInternal.rbegin()->first;
    bitStream.WriteByte(nBitLengths);
    // Записываем, сколько элементов каждой битовой длины и считаем их общее количество
    byte countUniqueBytes = 0;
    for (int i = 1; i <= nBitLengths; i++) {
        byte nElems = 0;
        if (viewInternal.find(i) != viewInternal.end())
            nElems = viewInternal[i]->size();
        bitStream.WriteByte(nElems);
        countUniqueBytes += nElems;
    }
    // Записываем байты по длинам
    for (const auto& v : viewInternal)
        for (byte i : *v.second)
            bitStream.WriteByte(i);
}

void Huffman::decodeInternalView(BitsIO& bitStream) {
    // Число битовых длин
    byte nBitLengths = 0;
    bitStream.ReadByte(nBitLengths);
    // Читаем число элементов каждой битовой длины и считаем их общее количество
    std::map<int, int> counts;
    byte countUniqueBytes = 0;
    for (int i = 1; i <= nBitLengths; i++) {
        byte nElems = 0;
        bitStream.ReadByte(nElems);
        if (nElems != 0)
            counts.emplace(i, nElems);
        countUniqueBytes += nElems;
    }
    // Читаем полноценные байты из потока
    for (auto& v : counts) {
        for (int i = 0; i < counts[v.first]; i++) {
            byte value = 0;
            bitStream.ReadByte(value);
            if (viewInternal.find(v.first) == viewInternal.end())
                viewInternal.emplace(v.first, std::make_unique<std::vector<byte>>());
            viewInternal[v.first]->push_back(value);
        }
    }
}

void Huffman::BuildHuffmanTree(BitsIO& bitStream) {
    streamToCountsMap(bitStream);
    countsMapToTree();
    treeToInternalView();
//    internalViewToCanonicalCode();
    treeToCanonicalCode();
}

void Huffman::RestoreHuffmanTree(BitsIO& bitStream) {
    decodeInternalView(bitStream);
    internalViewToCanonicalCode();
    canonicalCodeToTree();
}

void Huffman::Encode(BitsIO& bitStream) {
    encodeInternalView(bitStream);
    uint32_t bitsAfterEncoding = 0;  // Считаем количество бит, записываемых в поток
    for (const auto& f : counts) {
        byte value = f.first;
        int frequency = f.second;
        int nbits = viewCanonical[value].first;
        bitsAfterEncoding += frequency * nbits;
    }
    byte bitCountByte = 0;
    for (int i=0; i<4;i++) {  // uint32_t = 32 бита = 4 байта
        bitCountByte = (bitsAfterEncoding >> i*8) & 0xFF;
        bitStream.WriteByte(bitCountByte);
    }
    byte value = 0;
    // читаем байты, пока не кончатся, преобразовываем в битовый код и пишем в выходной поток
    while (bitStream.ReadByte(value)) {
        auto e = viewCanonical[value];
        int nBits = e.first;
        int code = e.second;
        for (int i = nBits - 1; i >= 0; --i) {
            bool right = (code >> i) & 1;
            bitStream.WriteBit(right);
        }
    }
    bitStream.WriteRemainingBits();  // дописываем в выходной поток последний неполный байт
}

void Huffman::Decode(BitsIO& bitStream) {
    uint32_t bitsAfterEncoding = 0; // Считаем количество бит, считываемых из потока
    byte bitCountByte = 0;
    for (int i=0; i<4;i++) {  // uint32_t = 32 бита = 4 байта
        bitStream.ReadByte(bitCountByte);
        bitsAfterEncoding |= bitCountByte << (i*8);
    }
    bool bit = false;
    std::shared_ptr<TreeNode> root = tree;
    // читаем биты, пока не кончатся. Идём по дереву Хаффмата до листа и пишем соответствующий байт
    while ((bitsAfterEncoding--) && bitStream.ReadBit(bit)) {
        if (!bit)
            root = root->left;
        else
            root = root->right;
        if ((root->left == nullptr) && (root->right == nullptr)) {
            bitStream.WriteByte(root->symbol);
            root = tree;
        }
    }
}

void Encode(IInputStream& original, IOutputStream& compressed) {
    BitsIO bitStream(original, compressed);
    Huffman h1;
    h1.BuildHuffmanTree(bitStream);
    h1.Encode(bitStream);
}

void Decode(IInputStream& compressed, IOutputStream& original) {
    BitsIO bitStream2(compressed, original);
    Huffman h2;
    h2.RestoreHuffmanTree(bitStream2);
    h2.Decode(bitStream2);
}

int main(const int argc, const char* argv[]) {
    std::string original = "Спасибо, что скачали книгу в бесплатной электронной библиотеке Royallib.ru: http://royallib.ru\n";
//                    "\n"
//                    "Все книги автора: http://royallib.ru/author/bez_avtora_.html\n"
//                    "\n"
//                    "Эта же книга в других форматах: http://royallib.ru/book/bez_avtora_/slovo_o_polku_igoreve.html\n"
//                    "\n"
//                    "Приятного чтения!\n"
//                    "\n"
//                    "\n"
//                    "\n"
//                    "\n"
//                    "\t\tНе пора ль нам, братия, начать\n"
//                    "\t\tО походе Игоревом слово,\n"
//                    "\t\tЧтоб старинной речью рассказать\n"
//                    "\t\tПро деянья князя удалого?\n"
//                    "\t\tА воспеть нам, братия, его -\n"
//                    "\t\tВ похвалу трудам его и ранам -\n"
//                    "\t\tПо былинам времени сего,\n"
//                    "\t\tНе гоняясь в песне за Бояном.\n"
//                    "\t\tТот Боян, исполнен дивных сил,\n"
//                    "\t\tПриступая к вещему напеву,\n"
//                    "\t\tСерым волком по полю кружил,\n"
//                    "\t\tКак орел, под облаком парил,\n"
//                    "\t\tРастекался мыслию по древу.\n";
    std::vector<byte> byteInput;
    std::copy(original.begin(), original.end(), std::back_inserter(byteInput));

    IInputStream inputStream(byteInput);
    IOutputStream outputStream;
    BitsIO bitStream(inputStream, outputStream);

    Huffman h1;
    h1.BuildHuffmanTree(bitStream);
    h1.Encode(bitStream);

    IInputStream inputStream2(outputStream);
    IOutputStream outputStream2;
    BitsIO bitStream2(inputStream2, outputStream2);

    Huffman h2;
    h2.RestoreHuffmanTree(bitStream2);
    h2.Decode(bitStream2);

    original = std::string(outputStream2.contents.begin(), outputStream2.contents.end());
    std::cout << original << std::endl;
}
