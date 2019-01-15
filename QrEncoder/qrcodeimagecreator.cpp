#include "qrcodeimagecreator.h"

QrCodeImageCreator::QrCodeImageCreator() {

}

QImage QrCodeImageCreator::createQrCode(BitArray data, uchar qrVersion) {
    uint qrSize = _defaultQrCodeSize + 4 * (qrVersion - 1) + _offsetsSize;
    _qrCodeSize = qrSize;
    QImage qrCode(_qrCodeSize, _qrCodeSize, QImage::Format_Mono);
    qrCode.fill(Qt::black);
    _imageMatrix = new bool* [_qrCodeSize];
    _isPixelPainted = new bool* [_qrCodeSize];
    for(int i = 0; i < _qrCodeSize; i++) {
        _imageMatrix[i] = new bool [_qrCodeSize];
        _isPixelPainted[i] = new bool [_qrCodeSize];
        memset(_imageMatrix[i], 0, sizeof(bool) * _qrCodeSize);
        memset(_isPixelPainted[i], 0, sizeof(bool) * _qrCodeSize);
    }
    prepareImageMatrix(data, qrVersion);
    for(int x = 0; x < _qrCodeSize; x++) {
        for(int y = 0; y < _qrCodeSize; y++) {
            if(_imageMatrix[x][y]) {
                colorPixel(qrCode, x, y);
            }
        }
    }
    for(int i = 0; i < _qrCodeSize; i++) {
        delete [] _imageMatrix[i];
        delete [] _isPixelPainted[i];
    }
    delete [] _imageMatrix;
    delete [] _isPixelPainted;
    return qrCode;
}

void QrCodeImageCreator::prepareImageMatrix(BitArray &data, uchar version) {
    addQuietZone();
    addFinders(2, 2);
    addFinders(_qrCodeSize - 11, 2);
    addFinders(2, _qrCodeSize - 11);
    addSyncLine(11, 9, _qrCodeSize - 12, 9);
    addSyncLine(9, 11, 9, _qrCodeSize - 12);
    addAlignmentPatterns(version);
    addFormatInfo();
    addVersionInfo(version);
    addData(data);
}

void QrCodeImageCreator::colorPixel(QImage & image, int x, int y, QColor color) {
    _painter.begin(&image);
    _painter.setPen(color);
    _painter.setBrush(color);
    _painter.drawPoint(x, y);
    _painter.end();
}

void QrCodeImageCreator::addQuietZone() {
    addLine(0, 0, _qrCodeSize - 1, 0, WHITE);
    addLine(0, 1, _qrCodeSize - 1, 1, WHITE);
    addLine(0, 2, _qrCodeSize - 1, 2, WHITE);

    addLine(0, 0, 0, _qrCodeSize - 1, WHITE);
    addLine(1, 0, 1, _qrCodeSize - 1, WHITE);
    addLine(2, 0, 2, _qrCodeSize - 1, WHITE);

    addLine(0, _qrCodeSize - 3, _qrCodeSize - 1, _qrCodeSize - 3, WHITE);
    addLine(0, _qrCodeSize - 2, _qrCodeSize - 1, _qrCodeSize - 2, WHITE);
    addLine(0, _qrCodeSize - 1, _qrCodeSize - 1, _qrCodeSize - 1, WHITE);

    addLine(_qrCodeSize - 3, 0, _qrCodeSize - 3, _qrCodeSize - 1, WHITE);
    addLine(_qrCodeSize - 2, 0, _qrCodeSize - 2, _qrCodeSize - 1, WHITE);
    addLine(_qrCodeSize - 1, 0, _qrCodeSize - 1, _qrCodeSize - 1, WHITE);
}

void QrCodeImageCreator::addFinders(QPoint topLeft) {
    return addFinders(topLeft.x(), topLeft.y());
}

void QrCodeImageCreator::addFinders(int x, int y) { // from top-left
    addLine(x + 1, y + 1, x + 1, y + 7);            // line top-left to bottom-left
    addLine(x + 1, y + 1, x + 7, y + 1);            // line top-left to top-right
    addLine(x + 7, y + 1, x + 7, y + 7);            // line top-right to bottom-right
    addLine(x + 1, y + 7, x + 7, y + 7);            // line bottom-left to bottom-right

    for(int x1 = x + 3; x1 <= x + 5; x1++) {
        for(int y1 = y + 3; y1 <= y + 5; y1++) {
            _imageMatrix[x1][y1] = 1;                 // inner block 3x3
        }
    }

    for(int x1 = x; x1 <= x + 8; x1++) {
        for(int y1 = y; y1 <= y + 8; y1++) {
            _isPixelPainted[x1][y1] = 1;
        }
    }
}

void QrCodeImageCreator::addLine(int x1, int y1, int x2, int y2, Color color) {
    if(x1 == x2) {
        if(y1 > y2) {
            std::swap(y1, y2);
        }
        for(int y = y1; y <= y2; y++) {
            _imageMatrix[x1][y] = color;
            _isPixelPainted[x1][y] = 1;
        }
    } else if (y1 == y2) {
        if(x1 > x2) {
            std::swap(x1, x2);
        }
        for(int x = x1; x <= x2; x++) {
            _imageMatrix[x][y1] = color;
            _isPixelPainted[x][y1] = 1;
        }
    } else {
        qDebug() << "err adding line, check coords";
    }
}

void QrCodeImageCreator::addSyncLine(int x1, int y1, int x2, int y2) {
    bool nextPixelColor = 1;                // 0 - white / 1 - black
    if(x1 == x2) {
        if(y1 > y2) {
            std::swap(y1, y2);
        }
        for(int y = y1; y <= y2; y++) {
            _imageMatrix[x1][y] = nextPixelColor;
            _isPixelPainted[x1][y] = 1;
            nextPixelColor = !nextPixelColor;
        }
    } else if (y1 == y2) {
        if(x1 > x2) {
            std::swap(x1, x2);
        }
        for(int x = x1; x <= x2; x++) {
            _imageMatrix[x][y1] = nextPixelColor;
            _isPixelPainted[x][y1] = 1;
            nextPixelColor = !nextPixelColor;
        }
    } else {
        qDebug() << "err adding syncline, check coords";
    }
}

void QrCodeImageCreator::addAlignmentPatterns(uchar version) {
    QVector <uchar> patternCoords = _aligmentPatterns[version];
    for(const uchar & xCoord : patternCoords) {
        for(const uchar & yCoord : patternCoords) {
            if((xCoord == 6 && yCoord == 6)  ||
               (xCoord == 6 && yCoord == _qrCodeSize - 13) ||
               (xCoord == _qrCodeSize - 13 && yCoord == 6)) {
                continue;
            }
            addAlignmentPattern(xCoord + 3, yCoord + 3);
        }
    }

}

void QrCodeImageCreator::addAlignmentPattern(int x, int y) {
    _imageMatrix[x][y] = 1;
    addLine(x - 2, y - 2, x - 2, y + 2);
    addLine(x - 2, y - 2, x + 2, y - 2);
    addLine(x + 2, y + 2, x - 2, y + 2);
    addLine(x + 2, y + 2, x + 2, y - 2);
    for(int x1 = x - 2; x1 <= x + 2; x1++) {
        for(int y1 = y - 2; y1 <= y + 2; y1++) {
            _isPixelPainted[x1][y1] = 1;                 // inner block 3x3
        }
    }
}

void QrCodeImageCreator::addFormatInfo() {
    BitArray info("101010000010010");
    int curInfoBit = 0;
    for(int x = 3, y = 11; x <= 11; x++) {
        if(!_isPixelPainted[x][y]) {
            _imageMatrix[x][y] = info[curInfoBit];
            _isPixelPainted[x][y] = 1;
            curInfoBit++;
        }
    }
    for(int x = 11, y = 11; y >= 3; y--) {
        if(!_isPixelPainted[x][y]) {
            _imageMatrix[x][y] = info[curInfoBit];
            _isPixelPainted[x][y] = 1;
            curInfoBit++;
        }
    }
    curInfoBit = 0;
    _imageMatrix[11][_qrCodeSize - 11] = 1;
    _isPixelPainted[11][_qrCodeSize - 11] = 1;
    for(int x = 11, y = _qrCodeSize - 3; y >= _qrCodeSize - 10; y--) {
        if(!_isPixelPainted[x][y]) {
            _imageMatrix[x][y] = info[curInfoBit];
            _isPixelPainted[x][y] = 1;
            curInfoBit++;
        }
    }
    for(int x = _qrCodeSize - 11, y = 11; x <= _qrCodeSize - 3; x++) {
        if(!_isPixelPainted[x][y]) {
            _imageMatrix[x][y] = info[curInfoBit];
            _isPixelPainted[x][y] = 1;
            curInfoBit++;
        }
    }
}

void QrCodeImageCreator::addVersionInfo(uchar version) {
    if(version < 7) {
        return;
    }
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < _versions[version][i].size(); j++) {
            _imageMatrix[3 + j][_qrCodeSize - 14 + i] = _versions[version][i][j];
            _isPixelPainted[3 + j][_qrCodeSize - 14 + i] = 1;
        }
    }
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < _versions[version][i].size(); j++) {
            _imageMatrix[_qrCodeSize - 14 + i][3 + j] = _versions[version][i][j];
            _isPixelPainted[_qrCodeSize - 14 + i][3 + j] = 1;
        }
    }
}

void QrCodeImageCreator::addData(BitArray & data) {
    bool fromTop = false;
    int currentDataIndex = 0;
    for(int x = _qrCodeSize - 4; x > 1; x -= 2) {
        if(x == 9) {
            x++;
            continue;
        }
        for(int y = fromTop ? 0 : _qrCodeSize - 4; fromTop ? y < _qrCodeSize : y >= 0;) {
            if(!_isPixelPainted[x][y]) {
                _imageMatrix[x][y] = (((x + y) % 2 == 0)) ? !data[currentDataIndex] : data[currentDataIndex];
                _isPixelPainted[x][y] = 1;
                currentDataIndex++;
            }
            if(!_isPixelPainted[x - 1][y]) {
                _imageMatrix[x - 1][y] = (((x + y - 1) % 2 == 0)) ? !data[currentDataIndex] : data[currentDataIndex];
                _isPixelPainted[x - 1][y] = 1;
                currentDataIndex++;
            }
            fromTop ? y++ : y--;
        }
        fromTop = !fromTop;
    }
}
