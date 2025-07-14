#include "matrixwidget.h"
#include <QPaintEvent>
#include <QResizeEvent>
#include <QParallelAnimationGroup>
#include <QEasingCurve>
#include <QtMath>

MatrixWidget::MatrixWidget(QWidget *parent)
    : QWidget(parent)
    , fontSize(14)
    , primaryGreen(0, 255, 65)      // #00FF41
    , secondaryGreen(0, 143, 17)    // #008F11
    , backgroundColor(13, 17, 23)   // #0D1117
    , glitchColor(255, 0, 255)      // Magenta glitch
    , scanlineColor(0, 255, 255)    // Cyan scanlines
    , m_glitchIntensity(0.1)
    , scanlinesEnabled(true)
    , cyberAttackMode(false)
    , matrixSpeed(100)
    , frameCounter(0)
{
    // Enhanced Matrix characters with more variety
    matrixChars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+-=[]{}|;:,.<>?";
    
    // Japanese Katakana for authentic Matrix feel
    japaneseChars = "アイウエオカキクケコサシスセソタチツテトナニヌネノハヒフヘホマミムメモヤユヨラリルレロワヲン"
                   "ガギグゲゴザジズゼゾダヂヅデドバビブベボパピプペポ";
    
    // Hacker/cyber symbols
    hackerSymbols = "░▒▓█▄▀■□▲►◄▼○●◆◇★☆※→←↑↓═║╔╗╚╝╠╣╦╩╬";
    
    // Binary characters
    binaryChars = "01";
    
    // Setup fonts
    matrixFont = QFont("Consolas", fontSize, QFont::Bold);
    matrixFont.setStyleHint(QFont::Monospace);
    
    japaneseFont = QFont("MS Gothic", fontSize, QFont::Bold);
    japaneseFont.setStyleHint(QFont::Monospace);
    
    fontMetrics = new QFontMetrics(matrixFont);
    columnWidth = fontMetrics->horizontalAdvance("M");
    charHeight = fontMetrics->height();
    
    // Setup timers but delay start
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MatrixWidget::updateMatrix);
    
    glitchTimer = new QTimer(this);
    connect(glitchTimer, &QTimer::timeout, this, &MatrixWidget::updateGlitchEffects);
    
    scanlineTimer = new QTimer(this);
    connect(scanlineTimer, &QTimer::timeout, [this]() {
        // Update scanlines position
        for (auto &scanline : scanlines) {
            if (scanline.direction) {
                scanline.y += scanline.speed;
                if (scanline.y > height()) {
                    scanline.y = -10;
                    scanline.opacity = QRandomGenerator::global()->bounded(50, 150);
                }
            } else {
                scanline.y -= scanline.speed;
                if (scanline.y < -10) {
                    scanline.y = height();
                    scanline.opacity = QRandomGenerator::global()->bounded(50, 150);
                }
            }
        }
        update();
    });
    
    // Set background
    setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, backgroundColor);
    setPalette(palette);
    
    // Initialize animations
    glitchAnimation = new QPropertyAnimation(this, "glitchIntensity");
    glitchAnimation->setDuration(500);
    glitchAnimation->setEasingCurve(QEasingCurve::InOutQuad);
    
    // Delay timer start to ensure widget is fully initialized
    QTimer::singleShot(200, this, [this]() {
        if (timer && isVisible()) {
            timer->start(matrixSpeed);
        }
        if (glitchTimer && isVisible()) {
            glitchTimer->start(50); // Faster updates for glitch effects
        }
        if (scanlineTimer && isVisible()) {
            scanlineTimer->start(100);
        }
    });
    
    initializeColumns();
    initializeScanlines();
    
    lastUpdateTime = QTime::currentTime();
}

MatrixWidget::~MatrixWidget()
{
    delete fontMetrics;
}

void MatrixWidget::startCyberAttackSimulation()
{
    cyberAttackMode = !cyberAttackMode;
    if (cyberAttackMode) {
        setMatrixSpeed(30); // Faster matrix rain
        glitchAnimation->setStartValue(0.1);
        glitchAnimation->setEndValue(0.8);
        glitchAnimation->start();
        
        // Create multiple glitch effects
        for (int i = 0; i < 10; ++i) {
            createRandomGlitch();
        }
    } else {
        setMatrixSpeed(100); // Normal speed
        glitchAnimation->setStartValue(m_glitchIntensity);
        glitchAnimation->setEndValue(0.1);
        glitchAnimation->start();
    }
}

void MatrixWidget::setMatrixSpeed(int speed)
{
    matrixSpeed = qMax(10, qMin(500, speed));
    timer->setInterval(matrixSpeed);
}

void MatrixWidget::toggleScanlines(bool enabled)
{
    scanlinesEnabled = enabled;
    update();
}

void MatrixWidget::triggerGlitchEffect()
{
    for (int i = 0; i < 5; ++i) {
        createRandomGlitch();
    }
}

void MatrixWidget::initializeScanlines()
{
    scanlines.clear();
    
    // Create 3-5 scanlines
    int numScanlines = QRandomGenerator::global()->bounded(3, 6);
    for (int i = 0; i < numScanlines; ++i) {
        ScanLine scanline;
        scanline.y = QRandomGenerator::global()->bounded(height());
        scanline.speed = QRandomGenerator::global()->bounded(1, 3);
        scanline.opacity = QRandomGenerator::global()->bounded(30, 100);
        scanline.direction = QRandomGenerator::global()->bounded(2);
        scanlines.append(scanline);
    }
}

void MatrixWidget::initializeColumns()
{
    columns.clear();
    
    if (columnWidth <= 0) return;
    
    int numColumns = width() / columnWidth + 1;
    
    for (int i = 0; i < numColumns; ++i) {
        MatrixColumn column;
        column.x = i * columnWidth;
        column.y = QRandomGenerator::global()->bounded(-height(), 0);
        column.speed = QRandomGenerator::global()->bounded(1, 4);
        column.maxChars = QRandomGenerator::global()->bounded(5, 25);
        column.isGlitched = false;
        column.glitchTimer = 0;
        
        // Initialize character string with mixed character sets
        column.chars.clear();
        column.charAges.clear();
        for (int j = 0; j < column.maxChars; ++j) {
            // Mix different character types for variety
            QString charToAdd;
            int charType = QRandomGenerator::global()->bounded(4);
            switch (charType) {
                case 0: charToAdd = getRandomChar(); break;
                case 1: charToAdd = getRandomJapaneseChar(); break;
                case 2: charToAdd = getRandomHackerSymbol(); break;
                case 3: charToAdd = binaryChars.at(QRandomGenerator::global()->bounded(2)); break;
            }
            column.chars += charToAdd;
            column.charAges.append(j);
        }
        
        columns.append(column);
    }
}

void MatrixWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    
    // Enhanced safety checks to prevent painter errors
    if (width() <= 0 || height() <= 0 || !isVisible()) {
        return;
    }
    
    // Check if widget is properly initialized
    if (!fontMetrics) {
        return;
    }
    
    QPainter painter(this);
    if (!painter.isActive()) {
        return; // Painter failed to initialize
    }
    
    // Additional check for paint device
    if (!painter.device() || painter.device()->width() <= 0 || painter.device()->height() <= 0) {
        return;
    }
    
    painter.setRenderHint(QPainter::Antialiasing, false); // Pixel-perfect for retro feel
    painter.fillRect(rect(), backgroundColor);
    
    // Draw cyber grid background
    drawCyberGrid(painter);
    
    // Draw matrix rain
    drawMatrixRain(painter);
    
    // Draw scanlines effect
    if (scanlinesEnabled) {
        drawScanlines(painter);
    }
    
    // Draw glitch effects
    drawGlitchEffects(painter);
    
    frameCounter++;
}

void MatrixWidget::drawMatrixRain(QPainter &painter)
{
    painter.setFont(matrixFont);
    
    // Draw matrix columns
    for (const MatrixColumn &column : columns) {
        for (int i = 0; i < column.chars.length(); ++i) {
            int charY = column.y + (i * charHeight);
            
            // Skip characters outside visible area
            if (charY < -charHeight || charY > height()) {
                continue;
            }
            
            // Calculate color based on character age and glitch effects
            QColor charColor = getCharColor(column.charAges[i], column.maxChars);
            
            // Apply glitch effect if column is glitched
            if (column.isGlitched) {
                charColor = QColor::fromHsv(
                    QRandomGenerator::global()->bounded(360),
                    255,
                    charColor.value()
                );
            }
            
            painter.setPen(charColor);
            
            // Add slight random offset for glitch effect
            int offsetX = column.isGlitched ? QRandomGenerator::global()->bounded(-2, 3) : 0;
            int offsetY = column.isGlitched ? QRandomGenerator::global()->bounded(-1, 2) : 0;
            
            // Draw character
            painter.drawText(column.x + offsetX, charY + offsetY, QString(column.chars[i]));
        }
    }
    
    // Draw some random floating characters for extra atmosphere
    painter.setPen(QColor(primaryGreen.red(), primaryGreen.green(), primaryGreen.blue(), 100));
    for (int i = 0; i < 30; ++i) {
        int x = QRandomGenerator::global()->bounded(width());
        int y = QRandomGenerator::global()->bounded(height());
        if (QRandomGenerator::global()->bounded(100) < 3) { // 3% chance
            painter.drawText(x, y, getRandomChar());
        }
    }
}

void MatrixWidget::drawScanlines(QPainter &painter)
{
    for (const ScanLine &scanline : scanlines) {
        QColor color = scanlineColor;
        color.setAlpha(scanline.opacity);
        painter.setPen(QPen(color, 1));
        painter.drawLine(0, scanline.y, width(), scanline.y);
        
        // Draw additional effect lines
        color.setAlpha(scanline.opacity / 3);
        painter.setPen(QPen(color, 1));
        painter.drawLine(0, scanline.y - 1, width(), scanline.y - 1);
        painter.drawLine(0, scanline.y + 1, width(), scanline.y + 1);
    }
}

void MatrixWidget::drawGlitchEffects(QPainter &painter)
{
    for (const GlitchEffect &glitch : glitchEffects) {
        if (!glitch.active) continue;
        
        painter.fillRect(glitch.x, glitch.y, glitch.width, glitch.height, glitch.color);
        
        // Add some random noise
        for (int i = 0; i < 20; ++i) {
            int x = glitch.x + QRandomGenerator::global()->bounded(glitch.width);
            int y = glitch.y + QRandomGenerator::global()->bounded(glitch.height);
            QColor noiseColor = glitch.color;
            noiseColor.setAlpha(QRandomGenerator::global()->bounded(50, 200));
            painter.setPen(noiseColor);
            painter.drawPoint(x, y);
        }
    }
}

void MatrixWidget::drawCyberGrid(QPainter &painter)
{
    // Draw subtle grid lines for cyber aesthetic
    QColor gridColor = primaryGreen;
    gridColor.setAlpha(20);
    painter.setPen(QPen(gridColor, 1));
    
    // Vertical lines
    for (int x = 0; x < width(); x += columnWidth * 4) {
        painter.drawLine(x, 0, x, height());
    }
    
    // Horizontal lines
    for (int y = 0; y < height(); y += charHeight * 4) {
        painter.drawLine(0, y, width(), y);
    }
}

void MatrixWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    initializeColumns();
    initializeScanlines();
}

void MatrixWidget::updateMatrix()
{
    QTime currentTime = QTime::currentTime();
    int deltaTime = lastUpdateTime.msecsTo(currentTime);
    lastUpdateTime = currentTime;
    
    for (MatrixColumn &column : columns) {
        // Move column down
        column.y += column.speed;
        
        // Update glitch timer
        if (column.isGlitched) {
            column.glitchTimer--;
            if (column.glitchTimer <= 0) {
                column.isGlitched = false;
            }
        }
        
        // Random chance to glitch
        if (!column.isGlitched && QRandomGenerator::global()->bounded(1000) < m_glitchIntensity * 100) {
            column.isGlitched = true;
            column.glitchTimer = QRandomGenerator::global()->bounded(5, 20);
        }
        
        // Reset column when it goes off screen
        if (column.y > height() + (column.maxChars * charHeight)) {
            column.y = QRandomGenerator::global()->bounded(-height(), -charHeight);
            column.speed = QRandomGenerator::global()->bounded(1, 5);
            column.maxChars = QRandomGenerator::global()->bounded(5, 30);
            
            // Regenerate characters with mixed types
            column.chars.clear();
            column.charAges.clear();
            for (int j = 0; j < column.maxChars; ++j) {
                QString charToAdd;
                int charType = QRandomGenerator::global()->bounded(4);
                switch (charType) {
                    case 0: charToAdd = getRandomChar(); break;
                    case 1: charToAdd = getRandomJapaneseChar(); break;
                    case 2: charToAdd = getRandomHackerSymbol(); break;
                    case 3: charToAdd = binaryChars.at(QRandomGenerator::global()->bounded(2)); break;
                }
                column.chars += charToAdd;
                column.charAges.append(j);
            }
        }
        
        // Randomly change some characters
        for (int i = 0; i < column.chars.length(); ++i) {
            if (QRandomGenerator::global()->bounded(100) < 3) { // 3% chance
                int charType = QRandomGenerator::global()->bounded(4);
                switch (charType) {
                    case 0: column.chars[i] = getRandomChar().at(0); break;
                    case 1: column.chars[i] = getRandomJapaneseChar().at(0); break;
                    case 2: column.chars[i] = getRandomHackerSymbol().at(0); break;
                    case 3: column.chars[i] = binaryChars.at(QRandomGenerator::global()->bounded(2)); break;
                }
            }
        }
    }
    
    update(); // Trigger repaint
}

void MatrixWidget::updateGlitchEffects()
{
    // Update existing glitch effects
    for (GlitchEffect &glitch : glitchEffects) {
        if (glitch.active) {
            glitch.duration--;
            if (glitch.duration <= 0) {
                glitch.active = false;
            }
        }
    }
    
    // Randomly create new glitch effects
    if (QRandomGenerator::global()->bounded(100) < m_glitchIntensity * 50) {
        createRandomGlitch();
    }
    
    update();
}

void MatrixWidget::createRandomGlitch()
{
    GlitchEffect glitch;
    glitch.x = QRandomGenerator::global()->bounded(width() - 50);
    glitch.y = QRandomGenerator::global()->bounded(height() - 20);
    glitch.width = QRandomGenerator::global()->bounded(10, 100);
    glitch.height = QRandomGenerator::global()->bounded(2, 10);
    glitch.duration = QRandomGenerator::global()->bounded(5, 30);
    
    // Random glitch colors
    QColor colors[] = {
        QColor(255, 0, 255, 150),   // Magenta
        QColor(0, 255, 255, 150),   // Cyan
        QColor(255, 255, 0, 150),   // Yellow
        QColor(255, 0, 0, 150),     // Red
        QColor(255, 255, 255, 200)  // White
    };
    glitch.color = colors[QRandomGenerator::global()->bounded(5)];
    glitch.active = true;
    
    // Replace an inactive glitch or add if space available
    bool replaced = false;
    for (GlitchEffect &existingGlitch : glitchEffects) {
        if (!existingGlitch.active) {
            existingGlitch = glitch;
            replaced = true;
            break;
        }
    }
    
    if (!replaced && glitchEffects.size() < 20) {
        glitchEffects.append(glitch);
    }
}

QString MatrixWidget::getRandomChar()
{
    int index = QRandomGenerator::global()->bounded(matrixChars.length());
    return QString(matrixChars.at(index));
}

QString MatrixWidget::getRandomJapaneseChar()
{
    int index = QRandomGenerator::global()->bounded(japaneseChars.length());
    return QString(japaneseChars.at(index));
}

QString MatrixWidget::getRandomHackerSymbol()
{
    int index = QRandomGenerator::global()->bounded(hackerSymbols.length());
    return QString(hackerSymbols.at(index));
}

QColor MatrixWidget::getCharColor(int age, int maxAge)
{
    if (age == 0) {
        // Brightest character (head of the trail)
        return QColor(255, 255, 255); // White
    } else if (age == 1) {
        // Second brightest
        return primaryGreen;
    } else {
        // Fade from primary green to secondary green to black
        float ratio = static_cast<float>(age) / maxAge;
        
        if (ratio < 0.3f) {
            // Primary to secondary green
            float localRatio = ratio / 0.3f;
            int r = static_cast<int>(primaryGreen.red() * (1 - localRatio) + secondaryGreen.red() * localRatio);
            int g = static_cast<int>(primaryGreen.green() * (1 - localRatio) + secondaryGreen.green() * localRatio);
            int b = static_cast<int>(primaryGreen.blue() * (1 - localRatio) + secondaryGreen.blue() * localRatio);
            return QColor(r, g, b);
        } else {
            // Secondary green to black
            float localRatio = (ratio - 0.3f) / 0.7f;
            int r = static_cast<int>(secondaryGreen.red() * (1 - localRatio));
            int g = static_cast<int>(secondaryGreen.green() * (1 - localRatio));
            int b = static_cast<int>(secondaryGreen.blue() * (1 - localRatio));
            return QColor(r, g, b);
        }
    }
}
