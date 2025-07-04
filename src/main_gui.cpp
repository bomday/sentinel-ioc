#include <QApplication>
#include <QStyleFactory>
#include <QDir>
#include <QFontDatabase>
#include <QSplashScreen>
#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include <QLinearGradient>
#include <QRandomGenerator>

#include "gui/mainwindow.h"

QPixmap createMatrixSplashScreen()
{
    QPixmap splash(400, 300);
    splash.fill(QColor(13, 17, 23)); // Dark background
    
    QPainter painter(&splash);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Create gradient background
    QLinearGradient gradient(0, 0, 400, 300);
    gradient.setColorAt(0, QColor(0, 255, 65, 50));
    gradient.setColorAt(1, QColor(0, 143, 17, 50));
    painter.fillRect(splash.rect(), gradient);
    
    // Draw border
    painter.setPen(QPen(QColor(0, 255, 65), 3));
    painter.drawRect(splash.rect().adjusted(5, 5, -5, -5));
    
    // Draw title
    QFont titleFont("Courier New", 18, QFont::Bold);
    painter.setFont(titleFont);
    painter.setPen(QColor(0, 255, 65));
    painter.drawText(splash.rect(), Qt::AlignCenter, "◤ SENTINEL IOC ◥\\nMATRIX LOADING...");
    
    // Draw loading effect
    painter.setPen(QColor(0, 255, 255));
    for (int i = 0; i < 20; ++i) {
        int x = (i * 20) % 400;
        int y = 250 + (i % 3) * 10;
        painter.drawText(x, y, QString::number(QRandomGenerator::global()->bounded(2)));
    }
    
    return splash;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("Sentinel IOC Matrix Terminal");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Cybersecurity Division");
    
    // Set application style
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // Load custom fonts if available
    QFontDatabase::addApplicationFont(":/fonts/matrix.ttf");
    
    // Create and show splash screen
    QPixmap splashPixmap = createMatrixSplashScreen();
    QSplashScreen splash(splashPixmap);
    splash.show();
    
    // Process events to show splash
    app.processEvents();
    
    // Create main window
    MainWindow window;
    
    // Simulate loading time
    QTimer::singleShot(2000, [&]() {
        splash.finish(&window);
        window.show();
    });
    
    return app.exec();
}
