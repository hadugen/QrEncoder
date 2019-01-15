#ifndef BITARRAY_H
#define BITARRAY_H

#include <QObject>
#include <QVector>
#include <QDebug>

class BitArray
{
public:
    BitArray();
    BitArray(uint size, bool value);
    BitArray(QString bits);
    BitArray(uchar value);
    BitArray(QVector <bool> bits);
    bool &operator[](uint i);
    void operator=(QVector <bool> bytes);
    void setBit(uint i, bool value);
    void append(const BitArray & array);
    void append(const uchar & value);
    void append(const ushort & value);
    void append(QString bits);
    void append(const QByteArray & data);
    void append(const QVector <bool> & bits);
    uint size();
    bool isEmpty();
    void clear();
    QVector <bool>::iterator begin();
    QVector <bool>::iterator end();
    BitArray mid(int i, int len);
    QString toString();
    QByteArray toByteArray();
private:
    QVector <bool> _content;
    QVector <bool> valueToBits(const uchar &value);
    QVector <bool> valueToBits(const ushort &value);
    QVector <bool> getBitsFromString(QString bits);
};

#endif // BITARRAY_H
