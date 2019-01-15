#include "qrencoder.h"
#include <QTime>
QREncoder::QREncoder() {
}

QImage QREncoder::getQrCodeImage(QByteArray data) {
    CorrectionLevel correctionLevel = LEVEL_M;
    uchar qrVersion = getQrVersion(data.size() * 8, correctionLevel);
    BitArray encodedData = encodeString(data, qrVersion, correctionLevel);
    return _imageCreator.createQrCode(encodedData, qrVersion);
}

BitArray QREncoder::encodeString(QByteArray data, uchar qrVersion, CorrectionLevel correctionLevel) {
    int maxDataSize = _maxDataSize[correctionLevel][qrVersion];
    uchar amountDataBlocks = _amountDataBlocks[correctionLevel][qrVersion];
    uchar amountCorrectionBytes = _amountCorrectionBytes[correctionLevel][qrVersion];
    QVector <uchar> genPolynom = _genPolynoms.value(amountCorrectionBytes);
    BitArray encodedString;
    encodedString.append(QString("0100"));                              // encoding type
    if(qrVersion == 0) {
        qDebug() << "no matching version to encode";
        return BitArray();
    } else if (qrVersion < 10) {
        encodedString.append((uchar)data.size());
    } else {
        encodedString.append((ushort)data.size());
    }
    encodedString.append(data);                                    // append data

    for(int i = 8 - (encodedString.size() % 8); i > 0 && i < 8; i--) {
        encodedString.append(QString("0"));                             // 8x multiple
    }
    int bytesToAddLeft = (maxDataSize - (int)encodedString.size()) / 8;
    for(int i = 0; i < bytesToAddLeft; i++) {
        encodedString.append((i % 2) == 0 ? (uchar)236 : (uchar)17);
    }
    QByteArray *dataBlocks = new QByteArray[amountDataBlocks];
    QByteArray *correctionBlocks = new QByteArray[amountDataBlocks];
    QByteArray byteData = encodedString.toByteArray();
    uint maxBlockSize;
    if(byteData.size() % amountDataBlocks == 0) {
        uint oneBlockSize = byteData.size() / amountDataBlocks;
        for(int i = 0; i < amountDataBlocks; i++) {
            dataBlocks[i] = byteData.mid(i * oneBlockSize, oneBlockSize);
        }
        maxBlockSize = oneBlockSize;
    } else {
        uint oneBlockSize = byteData.size() / amountDataBlocks;
        uint bytesLeft = byteData.size() % amountDataBlocks;
        uint currentByte = 0;
        for(int i = 0; i < amountDataBlocks - bytesLeft; i++) {
            dataBlocks[i] = byteData.mid(currentByte, oneBlockSize);
            currentByte += oneBlockSize;
        }
        oneBlockSize++;
        for(int i = amountDataBlocks - bytesLeft ; i < amountDataBlocks; i++) {
            dataBlocks[i] = byteData.mid(currentByte, oneBlockSize);
            currentByte += oneBlockSize;
        }
        maxBlockSize = oneBlockSize;
    }
    for(int i = 0; i < amountDataBlocks; i++) {
        QByteArray correctionBytes = dataBlocks[i];
        correctionBytes = correctionBytes.leftJustified(std::max(correctionBytes.size(), genPolynom.size()), 0);
        for(int j = 0; j < correctionBytes.size(); j++) {
            QVector <uchar> currentGenPolynom = genPolynom;
            uchar first = correctionBytes.at(0);
            correctionBytes.remove(0, 1);
            correctionBytes.push_back((char)0);
            if(first == 0) {
                continue;
            }
            uchar valueFromReversGF = _reverseGaloisField[first];
            for(int k = 0; k < currentGenPolynom.size(); k++) {
                currentGenPolynom[k] = (currentGenPolynom[k] + valueFromReversGF) % 255;
                correctionBytes[k] = correctionBytes[k] ^ _galoisField[currentGenPolynom[k]];
            }
        }
        correctionBlocks[i] = correctionBytes;
    }
    BitArray result;
    for(int byte = 0; byte < maxBlockSize; byte++) {
        for(int i = 0; i < amountDataBlocks; i++) {
            if(dataBlocks[i].size() < (byte + 1)) {
                continue;
            }
            result.append((uchar)dataBlocks[i][byte]);
        }
    }
    for(int byte = 0; byte < maxBlockSize; byte++) {
        for(int i = 0; i < amountDataBlocks; i++) {
            if(correctionBlocks[i].size() < (byte + 1)) {
                continue;
            }
            result.append((uchar)correctionBlocks[i][byte]);
        }
    }
    delete [] dataBlocks;
    delete [] correctionBlocks;
    return result;
}

uchar QREncoder::getQrVersion(uint dataSize, CorrectionLevel correctionLvl) {
    for(uchar version = 1; version < 40; version++) {
        if(_maxDataSize[correctionLvl][version] - 8 > dataSize ) {
            return version;
        }
    }
    return 0;
}
