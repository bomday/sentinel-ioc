#ifndef MATRIXTITLE_H
#define MATRIXTITLE_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QRandomGenerator>
#include <QVector>
#include <QString>
#include <QLabel>
#include <QVBoxLayout>
#include <QPropertyAnimation>

struct TitleMatrixColumn {
    int x;
    int y;
    int speed;
    QString chars;
    QVector<int> charAges;
    int maxChars;
};

class MatrixTitle : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal glowIntensity READ glowIntensity WRITE setGlowIntensity)

public:
    explicit MatrixTitle(const QString &title, QWidget *parent = nullptr);
    ~MatrixTitle();

    void setTitle(const QString &title);
    
    // Glow animation properties
    qreal glowIntensity() const { return m_glowIntensity; }
    void setGlowIntensity(qreal intensity);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void updateMatrix();
    void updateGlow();

private:
    void initializeColumns();
    void startGlowAnimation();
    QString getRandomChar();
    QColor getCharColor(int age, int maxAge);

    // Matrix animation
    QTimer *timer;
    QVector<TitleMatrixColumn> columns;
    
    // NO GLOW ANIMATION - removed to prevent title shaking
    // QPropertyAnimation *glowAnimation; // REMOVED
    // QGraphicsDropShadowEffect *glowEffect; // REMOVED to prevent QPainter errors
    qreal m_glowIntensity; // Keep for compatibility but don't use for animations
    
    // Fonts and metrics
    QFont matrixFont;
    QFont titleFont;
    QFontMetrics *fontMetrics;
    
    // Matrix character sets
    QString matrixChars;
    QString japaneseChars;
    
    // Colors
    QColor primaryColor;
    QColor secondaryColor;
    QColor backgroundColor;
    QColor matrixGreen;
    QColor brightGreen;
    
    // Layout and content
    QString titleText;
    int columnWidth;
    int charHeight;
    int frameCounter;
    
    // Matrix settings
    int matrixSpeed;
    bool matrixEnabled;
};

#endif // MATRIXTITLE_H
