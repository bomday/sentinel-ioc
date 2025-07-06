#include "matrixsplash.h"
#include <QPaintEvent>
#include <QScreen>

MatrixSplash::MatrixSplash(const QPixmap &pixmap, QWidget *parent)
    : QSplashScreen(QPixmap(500, 400))
    , frameCount(0)
{
    // Set up the splash screen size
    resize(500, 400);
    
    // Create a transparent pixmap since we'll do custom painting
    QPixmap transparentPixmap(500, 400);
    transparentPixmap.fill(Qt::transparent);
    setPixmap(transparentPixmap);
    
    // Center on screen
    if (QScreen *screen = QApplication::primaryScreen()) {
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - width()) / 2;
        int y = (screenGeometry.height() - height()) / 2;
        move(x, y);
    }
    
    // Initialize colors
    backgroundColor = QColor(0, 0, 0); // Pure black
    matrixGreen = QColor(0, 255, 65);
    brightGreen = QColor(150, 255, 150);
    textColor = QColor(255, 255, 255);
    
    // Set up font
    matrixFont = QFont("Courier New", 12, QFont::Bold);
    
    // Initialize matrix columns
    initializeColumns();
    
    // Set up animation timer but delay start
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &MatrixSplash::updateAnimation);
    
    // Delay timer start to ensure widget is fully initialized
    QTimer::singleShot(200, this, [this]() {
        if (animationTimer && isVisible()) {
            animationTimer->start(100); // Update every 100ms for smooth animation
        }
    });
}

MatrixSplash::~MatrixSplash()
{
    if (animationTimer) {
        animationTimer->stop();
    }
}

void MatrixSplash::initializeColumns()
{
    int columnWidth = 20;
    int numColumns = width() / columnWidth;
    
    columns.clear();
    columns.reserve(numColumns);
    
    for (int i = 0; i < numColumns; ++i) {
        MatrixColumn column;
        column.x = i * columnWidth;
        column.y = QRandomGenerator::global()->bounded(-height(), 0);
        column.speed = QRandomGenerator::global()->bounded(2, 6);
        column.length = QRandomGenerator::global()->bounded(5, 15);
        column.chars = "";
        
        // Generate random characters for this column
        for (int j = 0; j < column.length; ++j) {
            column.chars += getRandomMatrixChar();
        }
        
        columns.push_back(column);
    }
}

QString MatrixSplash::getRandomMatrixChar()
{
    // Mix of digits, letters, and some special characters for matrix effect
    static const QString chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#$%^&*()_+-=[]{}|;:,.<>?";
    int index = QRandomGenerator::global()->bounded(chars.length());
    return QString(chars.at(index));
}

void MatrixSplash::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    
    // Enhanced safety checks to prevent painter errors
    if (width() <= 0 || height() <= 0 || !isVisible()) {
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
    
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Fill background
    painter.fillRect(rect(), backgroundColor);
    
    // Draw matrix rain
    painter.setFont(matrixFont);
    
    for (const auto& column : columns) {
        for (int i = 0; i < column.length; ++i) {
            int charY = column.y + (i * 15);
            
            // Skip if outside visible area
            if (charY < -15 || charY > height() + 15) continue;
            
            // Get character for this position
            int charIndex = i % column.chars.length();
            QString ch = QString(column.chars.at(charIndex));
            
            // Color gradient effect - brightest at the head, fading down
            if (i == 0) {
                painter.setPen(brightGreen); // Bright white/green for the head
            } else if (i < 3) {
                painter.setPen(matrixGreen); // Standard matrix green
            } else {
                // Fade to darker green
                int alpha = 255 - (i * 30);
                if (alpha < 50) alpha = 50;
                QColor fadedGreen = matrixGreen;
                fadedGreen.setAlpha(alpha);
                painter.setPen(fadedGreen);
            }
            
            painter.drawText(column.x, charY, ch);
        }
    }
    
    // Draw title and loading text in the center
    QRect centerRect = rect().adjusted(50, 50, -50, -50);
    
    // Semi-transparent background for text
    painter.fillRect(centerRect, QColor(0, 0, 0, 180));
    painter.setPen(QPen(matrixGreen, 2));
    painter.drawRect(centerRect);
    
    // Title
    QFont titleFont("Courier New", 16, QFont::Bold);
    painter.setFont(titleFont);
    painter.setPen(textColor);
    
    QString title = "SENTINEL IOC";
    QRect titleRect = centerRect.adjusted(0, 20, 0, -centerRect.height() + 60);
    painter.drawText(titleRect, Qt::AlignCenter, title);
    
    // Loading text with animation
    QString loadingText = "LOADING";
    for (int i = 0; i < (frameCount / 3) % 4; ++i) {
        loadingText += ".";
    }
    
    QFont loadingFont("Courier New", 12);
    painter.setFont(loadingFont);
    painter.setPen(matrixGreen);
    
    QRect loadingRect = centerRect.adjusted(0, centerRect.height() - 80, 0, -20);
    painter.drawText(loadingRect, Qt::AlignCenter, loadingText);
    
    // Progress indicator
    int progressWidth = centerRect.width() - 40;
    int progressHeight = 8;
    QRect progressRect((width() - progressWidth) / 2, 
                      centerRect.bottom() - 40, 
                      progressWidth, 
                      progressHeight);
    
    painter.setPen(matrixGreen);
    painter.drawRect(progressRect);
    
    // Animated progress fill
    int fillWidth = (progressWidth * ((frameCount / 2) % 100)) / 100;
    QRect fillRect = progressRect.adjusted(1, 1, -progressWidth + fillWidth - 1, -1);
    painter.fillRect(fillRect, matrixGreen);
}

void MatrixSplash::updateAnimation()
{
    frameCount++;
    
    // Update matrix columns
    for (auto& column : columns) {
        column.y += column.speed;
        
        // Reset column when it goes off screen
        if (column.y > height() + (column.length * 15)) {
            column.y = QRandomGenerator::global()->bounded(-height(), -50);
            column.speed = QRandomGenerator::global()->bounded(2, 6);
            column.length = QRandomGenerator::global()->bounded(5, 15);
            
            // Generate new random characters
            column.chars.clear();
            for (int j = 0; j < column.length; ++j) {
                column.chars += getRandomMatrixChar();
            }
        }
        
        // Occasionally change some characters for dynamic effect
        if (QRandomGenerator::global()->bounded(100) < 10) {
            int charIndex = QRandomGenerator::global()->bounded(column.chars.length());
            column.chars[charIndex] = getRandomMatrixChar().at(0);
        }
    }
    
    // Trigger repaint
    update();
}
