#ifndef __HUFFMAN__
#define __HUFFMAN__ 1
#include <cstdio>

typedef unsigned char byte;

struct IInputStream {
    virtual bool Read(byte& value) = 0;
};

struct IOutputStream {
    virtual void Write(byte value) = 0;
};

// Метод архивирует данные из потока original
void Encode(IInputStream& original, IOutputStream& compressed);
// Метод восстанавливает оригинальные данные
void Decode(IInputStream& compressed, IOutputStream& original);

struct MyInputStream: public IInputStream {
	// Возвращает false, если поток закончился
    explicit MyInputStream(const char* filename);
    bool Read(byte& value) override;
    FILE * file;
};

MyInputStream::MyInputStream(const char* filename) {
    file = fopen(filename,"r");
}

bool MyInputStream::Read(byte& value) {
    int c = fgetc(file);
    bool res = c != EOF;
    value = c;
    return res;
}

struct MyOutputStream: public IOutputStream {
    explicit MyOutputStream(const char* filename);
	void Write(byte value) override;
    FILE * file;
};

MyOutputStream::MyOutputStream(const char* filename) {
    file = fopen(filename,"w");
}

void MyOutputStream::Write(byte value) {
    fputc(value, file);
}

#endif