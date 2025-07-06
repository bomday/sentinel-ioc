#include "matrixtitle.h"
#include <QPaintEvent>
#include <QResizeEvent>
#include <QShowEvent>
#include <QtMath>

MatrixTitle::MatrixTitle(const QString &title, QWidget *parent)
    : QWidget(parent)
    , titleText(title)
    , columnWidth(12)
    , charHeight(16)
    , frameCounter(0)
    , matrixSpeed(150)
    , matrixEnabled(true)
    , m_glowIntensity(1.0)
{
    // Initialize theme colors
    primaryColor = QColor("#00FF00");      // Terminal green
    secondaryColor = QColor("#004400");    // Dark green
    backgroundColor = QColor("#000000");   // Black background
    matrixGreen = QColor("#00FF41");       // Bright matrix green
    brightGreen = QColor("#FFFFFF");       // White for leading characters
    
    // Matrix character sets
    matrixChars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+-=[]{}|;:,.<>?";
    japaneseChars = "アイウエオカキクケコサシスセソタチツテトナニヌネノハヒフヘホマミムメモヤユヨラリルレロワヲン";
    
    // Setup fonts
    matrixFont = QFont("Courier New", 12, QFont::Bold); // Increased from 10 to 12 for better visibility
    matrixFont.setStyleHint(QFont::Monospace);
    
    titleFont = QFont("Courier New", 24, QFont::Bold);
    titleFont.setStyleHint(QFont::Monospace);
    
    fontMetrics = new QFontMetrics(matrixFont);
    columnWidth = fontMetrics->horizontalAdvance("M");
    charHeight = fontMetrics->height();
    
    // Set minimum size for the title
    setMinimumHeight(80);
    setMaximumHeight(80);
    
    // No graphics effects to prevent QPainter errors
    // The glow will be applied via stylesheets in the paintEvent
    
    // NO GLOW ANIMATION - completely removed to prevent title shaking
    // glowAnimation removed from class - no animation whatsoever
    
    // Setup timer for matrix updates (only matrix, not title)
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MatrixTitle::updateMatrix);
    
    // Enable matrix animation for background effect
    matrixEnabled = true;
    
    // Initialize matrix columns
    initializeColumns();
    
    // Delay timer start to ensure widget is fully initialized
    QTimer::singleShot(200, this, [this]() {
        // Force reinitialization of columns when timer starts
        initializeColumns();
        if (timer && !timer->isActive()) {
            timer->start(100); // Faster updates for more visible matrix effect
        }
    });
}

MatrixTitle::~MatrixTitle()
{
    delete fontMetrics;
    // No glowAnimation to delete since it was removed
}

void MatrixTitle::setTitle(const QString &title)
{
    titleText = title;
    update();
}

void MatrixTitle::setGlowIntensity(qreal intensity)
{
    // NO DYNAMIC GLOW UPDATES - keep title completely stable
    m_glowIntensity = intensity; // Store value but don't use it for visual changes
    // updateGlow(); // DISABLED - no glow updates to prevent shaking
}

void MatrixTitle::updateGlow()
{
    // DISABLED - no glow updates to prevent title shaking
    // This function is intentionally left empty
}

void MatrixTitle::initializeColumns()
{
    if (width() <= 0 || height() <= 0) return;
    
    int numColumns = qMax(1, width() / columnWidth); // Ensure at least 1 column
    columns.clear();
    columns.reserve(numColumns);
    
    for (int i = 0; i < numColumns; ++i) {
        TitleMatrixColumn column;
        column.x = i * columnWidth;
        column.y = QRandomGenerator::global()->bounded(charHeight, height() + charHeight); // Start within visible area
        column.speed = QRandomGenerator::global()->bounded(2, 5); // Faster movement
        column.maxChars = QRandomGenerator::global()->bounded(4, 8); // Longer trails for more density
        column.chars = "";
        column.charAges.clear();
        
        // Generate random characters for this column
        for (int j = 0; j < column.maxChars; ++j) {
            column.chars += getRandomChar();
            column.charAges.append(0);
        }
        
        columns.push_back(column);
    }
}

QString MatrixTitle::getRandomChar()
{
    // Mix different character sets
    if (QRandomGenerator::global()->bounded(100) < 20) {
        // 20% chance for Japanese characters
        int index = QRandomGenerator::global()->bounded(japaneseChars.length());
        return QString(japaneseChars.at(index));
    } else {
        // 80% chance for matrix characters
        int index = QRandomGenerator::global()->bounded(matrixChars.length());
        return QString(matrixChars.at(index));
    }
}

QColor MatrixTitle::getCharColor(int age, int maxAge)
{
    if (age == 0) {
        return QColor(255, 255, 255, 255); // Bright white for the head
    } else if (age == 1) {
        return QColor(200, 255, 200, 255); // Very bright green
    } else if (age < 3) {
        return QColor(150, 255, 180, 255); // Bright matrix green
    } else {
        // Fade to darker green with much higher visibility
        int alpha = qMax(180, 255 - (age * 20)); // Much higher minimum alpha
        return QColor(100, 240, 120, alpha); // Very bright base color
    }
}

void MatrixTitle::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    
    // Basic safety checks to prevent painter errors
    if (width() <= 0 || height() <= 0) {
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
    
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Fill background with black
    painter.fillRect(rect(), backgroundColor);
    
    // Draw matrix rain effect behind the title
    if (matrixEnabled && columns.size() > 0) {
        painter.setFont(matrixFont);
        
        for (const auto& column : columns) {
            if (column.chars.isEmpty()) continue; // Safety check
            
            for (int i = 0; i < column.chars.length(); ++i) {
                int charY = column.y - (i * charHeight); // Draw upward from the column position
                
                // Only draw if within visible area (be more lenient with bounds)
                if (charY < -charHeight || charY > height() + charHeight) continue;
                
                // Get character and its age
                QString ch = QString(column.chars.at(i));
                int age = (i < column.charAges.size()) ? column.charAges[i] : 0;
                
                // Set color based on age
                QColor charColor = getCharColor(age, column.maxChars);
                
                // Make matrix much more visible by increasing alpha significantly
                int newAlpha = qMax(120, qMin(255, static_cast<int>(charColor.alpha()))); 
                charColor.setAlpha(newAlpha);
                painter.setPen(charColor);
                
                painter.drawText(column.x, charY, ch);
            }
        }
    }
    
    // Draw title with COMPLETELY STATIC effects - NO ANIMATIONS OR MOVEMENT
    painter.setFont(titleFont);
    
    // Calculate title rect once for absolute stability
    QRect titleRect = rect();
    
    // STATIC shadow for depth (never changes position or color)
    painter.setPen(QColor(0, 0, 0, 200));
    painter.drawText(titleRect.adjusted(2, 2, 0, 0), Qt::AlignCenter, titleText);
    
    // STATIC glow layers - Enhanced for better visibility (completely fixed colors and positions)
    QColor staticGlowColor = QColor(primaryColor);
    
    // Outermost glow layer for enhanced visibility (STATIC)
    staticGlowColor.setAlpha(40);
    painter.setPen(staticGlowColor);
    painter.drawText(titleRect.adjusted(-3, -3, 3, 3), Qt::AlignCenter, titleText);
    
    // Second outer glow layer (STATIC)
    staticGlowColor.setAlpha(70);
    painter.setPen(staticGlowColor);
    painter.drawText(titleRect.adjusted(-2, -2, 2, 2), Qt::AlignCenter, titleText);
    
    // Inner glow layer (STATIC)
    staticGlowColor.setAlpha(120);
    painter.setPen(staticGlowColor);
    painter.drawText(titleRect.adjusted(-1, -1, 1, 1), Qt::AlignCenter, titleText);
    
    // Bright accent glow for enhanced visibility (STATIC)
    QColor accentGlow = QColor(0, 255, 65); // Bright green accent
    accentGlow.setAlpha(80);
    painter.setPen(accentGlow);
    painter.drawText(titleRect.adjusted(0, 0, 0, 0), Qt::AlignCenter, titleText);
    
    // Main title text (COMPLETELY STATIC - never changes)
    QColor staticMainColor = QColor(primaryColor);
    staticMainColor = staticMainColor.lighter(150); // Enhanced brightness for better visibility
    painter.setPen(staticMainColor);
    painter.drawText(titleRect, Qt::AlignCenter, titleText);
    
    // STATIC border (completely fixed)
    QColor staticBorderColor = QColor(primaryColor);
    staticBorderColor = staticBorderColor.lighter(125); // Fixed brightness
    painter.setPen(QPen(staticBorderColor, 2));
    painter.drawRect(titleRect.adjusted(1, 1, -1, -1));
}

void MatrixTitle::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    initializeColumns();
}

void MatrixTitle::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    
    // Ensure timer is running when widget becomes visible
    if (timer && !timer->isActive()) {
        // Force reinitialize columns and start timer
        initializeColumns();
        timer->start(100);
    }
}

void MatrixTitle::updateMatrix()
{
    if (width() <= 0 || height() <= 0) {
        return; // Don't update if widget has invalid size
    }
    
    // Reinitialize columns if they're empty (widget was resized or just created)
    if (columns.isEmpty()) {
        initializeColumns();
        if (columns.isEmpty()) {
            return; // Still no columns, can't proceed
        }
    }
    
    frameCounter++;
    
    for (auto& column : columns) {
        column.y += column.speed;
        
        // Age all characters in the column
        for (int& age : column.charAges) {
            age++;
        }
        
        // Reset column when it goes off screen (bottom)
        if (column.y > height() + charHeight) {
            column.y = QRandomGenerator::global()->bounded(-charHeight * column.maxChars, 0); // Start above the widget
            column.speed = QRandomGenerator::global()->bounded(2, 5); // Faster speeds
            column.maxChars = QRandomGenerator::global()->bounded(4, 8); // Longer trails
            
            // Generate new random characters
            column.chars.clear();
            column.charAges.clear();
            for (int j = 0; j < column.maxChars; ++j) {
                column.chars += getRandomChar();
                column.charAges.append(0);
            }
        }
        
        // More frequent character changes for dynamic effect
        if (QRandomGenerator::global()->bounded(100) < 8) { // Increased from 5% to 8%
            int charIndex = QRandomGenerator::global()->bounded(column.chars.length());
            column.chars[charIndex] = getRandomChar().at(0);
        }
    }
    
    // Trigger repaint
    update();
}
