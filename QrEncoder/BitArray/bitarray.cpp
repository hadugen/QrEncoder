#include "bitarray.h"

BitArray::BitArray() {

}

BitArray::BitArray(uint size, bool value) {
    _content.fill(value, size);
}

BitArray::BitArray(QString bits) {
    _content = getBitsFromString(bits);
}

BitArray::BitArray(uchar value) {
    _content = valueToBits(value);
}

BitArray::BitArray(QVector<bool> bits) {
    _content = bits;
}

bool &BitArray::operator[](uint i) {
    return _content[i];
}

void BitArray::operator=(QVector<bool> bytes) {
    this->_content = bytes;
}

void BitArray::setBit(uint i, bool value) {
    _content[i] = value;
}

void BitArray::append(const BitArray &array) {
    _content.append(array._content);
}

void BitArray::append(const uchar &value) {
    _content.append(valueToBits(value));
}

void BitArray::append(const ushort &value) {
    _content.append(valueToBits(value));
}

void BitArray::append(QString bits) {
    _content.append(getBitsFromString(bits));
}

void BitArray::append(const QByteArray &data) {
    for(const uchar & value : data) {
        _content.append(valueToBits(value));
    }
}

void BitArray::append(const QVector<bool> &bits) {
    _content.append(bits);
}

uint BitArray::size() {
    return _content.size();
}

bool BitArray::isEmpty() {
    return _content.isEmpty();
}

void BitArray::clear() {
    _content.clear();
}

QVector <bool> ::iterator BitArray::begin() {
    return _content.begin();
}

QVector <bool> ::iterator BitArray::end() {
    return _content.end();
}

BitArray BitArray::mid(int i, int len) {
    return BitArray(_content.mid(i, len));
}

QString BitArray::toString() {
    QString result;
    for(bool bit : _content) {
        result.append(bit ? '1' : '0');
    }
    return result;
}

QByteArray BitArray::toByteArray() {
    QByteArray result(_content.size() / 8, 0);
    for(int i = 0; i < _content.size(); i++) {
        result[i / 8] = (result.at(i / 8) | ((_content[i] ? 128 : 0) >> (i % 8)));
    }
    return result;
}

QVector<bool> BitArray::valueToBits(const uchar & value) {
    QVector <bool> bits;
    for(uchar mask = 128; mask != 0; mask >>= 1) {
        bits.push_back(((value & mask) == 0) ? 0 : 1);
    }
    return bits;
}

QVector<bool> BitArray::valueToBits(const ushort &value) {
    QVector <bool> bits;
    for(ushort mask = 32768; mask != 0; mask >>= 1) {
        bits.push_back(((value & mask) == 0) ? 0 : 1);
    }
    return bits;
}

QVector<bool> BitArray::getBitsFromString(QString bits) {
    QVector <bool> result;
    for(QChar bit : bits) {
        result.append((bit == '1') ? 1 : 0);
    }
    return result;
}

