#ifndef MATRIXWIDGET_H
#define MATRIXWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QRandomGenerator>
#include <QVector>
#include <QString>
#include <QPropertyAnimation>
#include <QGraphicsEffect>
#include <QTime>

struct MatrixColumn {
    int x;
    int y;
    int speed;
    QString chars;
    QVector<int> charAges;
    int maxChars;
    bool isGlitched;
    int glitchTimer;
};

struct GlitchEffect {
    int x, y, width, height;
    int duration;
    QColor color;
    bool active;
};

struct ScanLine {
    int y;
    int speed;
    int opacity;
    bool direction; // true = down, false = up
};

class MatrixWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal glitchIntensity READ glitchIntensity WRITE setGlitchIntensity)

public:
    explicit MatrixWidget(QWidget *parent = nullptr);
    ~MatrixWidget();
    
    qreal glitchIntensity() const { return m_glitchIntensity; }
    void setGlitchIntensity(qreal intensity) { m_glitchIntensity = intensity; update(); }

public slots:
    void startCyberAttackSimulation();
    void setMatrixSpeed(int speed);
    void toggleScanlines(bool enabled);
    void triggerGlitchEffect();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateMatrix();
    void updateGlitchEffects();

private:
    void initializeColumns();
    void initializeScanlines();
    QString getRandomChar();
    QString getRandomJapaneseChar();
    QString getRandomHackerSymbol();
    QColor getCharColor(int age, int maxAge);
    void drawScanlines(QPainter &painter);
    void drawGlitchEffects(QPainter &painter);
    void drawMatrixRain(QPainter &painter);
    void drawCyberGrid(QPainter &painter);
    void createRandomGlitch();

    // Timers
    QTimer *timer;
    QTimer *glitchTimer;
    QTimer *scanlineTimer;
    
    // Matrix data
    QVector<MatrixColumn> columns;
    QVector<GlitchEffect> glitchEffects;
    QVector<ScanLine> scanlines;
    
    // Fonts
    QFont matrixFont;
    QFont japaneseFont;
    QFontMetrics *fontMetrics;
    
    // Matrix character sets
    QString matrixChars;
    QString japaneseChars;
    QString hackerSymbols;
    QString binaryChars;
    
    // Visual properties
    int fontSize;
    int columnWidth;
    int charHeight;
    QColor primaryGreen;
    QColor secondaryGreen;
    QColor backgroundColor;
    QColor glitchColor;
    QColor scanlineColor;
    
    // Effects control
    qreal m_glitchIntensity;
    bool scanlinesEnabled;
    bool cyberAttackMode;
    int matrixSpeed;
    int frameCounter;
    
    // Animation properties
    QPropertyAnimation *glitchAnimation;
    QTime lastUpdateTime;
};

#endif // MATRIXWIDGET_H
