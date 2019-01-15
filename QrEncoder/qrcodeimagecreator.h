#ifndef QRCODEIMAGECREATOR_H
#define QRCODEIMAGECREATOR_H

#include <QPainter>
#include <QImage>
#include <bitarray.h>

class QrCodeImageCreator
{
public:
    QrCodeImageCreator();

    QImage createQrCode(BitArray data, uchar qrVersion);

private:

    enum Color {
        WHITE = 0,
        BLACK = 1
    };

    QVector <uchar> _aligmentPatterns[41] = {
        {}, {}, {18}, {22}, {26}, {30}, {34},
        {6, 22, 38}, {6, 24, 42}, {6, 26, 46}, {6, 28, 50},
        {6, 30, 54}, {6, 32, 58}, {6, 34, 62}, {6, 26, 46, 66},
        {6, 26, 48, 70}, {6, 26, 50, 74}, {6, 30, 54, 78},
        {6, 30, 56, 82}, {6, 30, 58, 86}, {6, 34, 62, 90},
        {6, 28, 50, 72, 94}, {6, 26, 50, 74, 98}, {6, 30, 54, 78, 102},
        {6, 28, 54, 80, 106}, {6, 32, 58, 84, 110}, {6, 30, 58, 86, 114},
        {6, 34, 62, 90, 118}, {6, 26, 50, 74, 98, 122}, {6, 30, 54, 78, 102, 126},
        {6, 26, 52, 78, 104, 130}, {6, 30, 56, 82, 108, 134}, {6, 34, 60, 86, 112, 138},
        {6, 30, 58, 86, 114, 142}, {6, 34, 62, 90, 118, 146}, {6, 30, 54, 78, 102, 126, 150},
        {6, 24, 50, 76, 102, 128, 154}, {6, 28, 54, 80, 106, 132, 158}, {6, 32, 58, 84, 110, 136, 162},
        {6, 26, 54, 82, 110, 138, 166}, {6, 30, 58, 86, 114, 142, 170},
    };

    BitArray _versions[41][3] {
        {}, {}, {}, {}, {}, {}, {},
        {(QString)"000010", (QString)"011110", (QString)"100110"},
        {(QString)"010001", (QString)"011100", (QString)"111000"},
        {(QString)"110111", (QString)"011000", (QString)"000100"},
        {(QString)"101001", (QString)"111110", (QString)"000000"},
        {(QString)"001111", (QString)"111010", (QString)"111100"},
        {(QString)"001101", (QString)"100100", (QString)"011010"},
        {(QString)"101011", (QString)"100000", (QString)"100110"},
        {(QString)"110101", (QString)"000110", (QString)"100010"},
        {(QString)"010011", (QString)"000010", (QString)"011110"},
        {(QString)"011100", (QString)"010001", (QString)"011100"},
        {(QString)"111010", (QString)"010101", (QString)"100000"},
        {(QString)"100100", (QString)"110011", (QString)"100100"},
        {(QString)"000010", (QString)"110111", (QString)"011000"},
        {(QString)"000000", (QString)"101001", (QString)"111110"},
        {(QString)"100110", (QString)"101101", (QString)"000010"},
        {(QString)"111000", (QString)"001011", (QString)"000110"},
        {(QString)"011110", (QString)"001111", (QString)"111010"},
        {(QString)"001101", (QString)"001101", (QString)"100100"},
        {(QString)"101011", (QString)"001001", (QString)"011000"},
        {(QString)"110101", (QString)"101111", (QString)"011100"},
        {(QString)"010011", (QString)"101011", (QString)"100000"},
        {(QString)"010001", (QString)"110101", (QString)"000110"},
        {(QString)"110111", (QString)"110001", (QString)"111010"},
        {(QString)"101001", (QString)"010111", (QString)"111110"},
        {(QString)"001111", (QString)"010011", (QString)"000010"},
        {(QString)"101000", (QString)"011000", (QString)"101101"},
        {(QString)"001110", (QString)"011100", (QString)"010001"},
        {(QString)"010000", (QString)"111010", (QString)"010101"},
        {(QString)"110110", (QString)"111110", (QString)"101001"},
        {(QString)"110100", (QString)"100000", (QString)"001111"},
        {(QString)"010010", (QString)"100100", (QString)"110011"},
        {(QString)"001100", (QString)"000010", (QString)"110111"},
        {(QString)"101010", (QString)"000110", (QString)"001011"},
        {(QString)"111001", (QString)"000100", (QString)"010101"}
    };

    bool **_imageMatrix;
    bool **_isPixelPainted;

    QPainter _painter;

    uint _qrCodeSize;
    uint _defaultQrCodeSize = 21;
    uint _offsetsSize = 6;

    void prepareImageMatrix(BitArray &data, uchar version);

    void colorPixel(QImage & image, int x, int y, QColor color = QColor(Qt::black));

    void addQuietZone();
    void addFinders(QPoint center);
    void addFinders(int x, int y);
    void addLine(int x1, int y1, int x2, int y2, Color = BLACK);
    void addSyncLine(int x1, int y1, int x2, int y2);
    void addAlignmentPatterns(uchar version);
    void addAlignmentPattern(int x, int y);
    void addFormatInfo();
    void addVersionInfo(uchar version);
    void addData(BitArray &data);
};

#endif // QRCODEIMAGECREATOR_H
