#include <QApplication>
#include <QStyleFactory>
#include <QtCore/QDir>
#include <QtGui/QFontDatabase>
#include <QTimer>
#include "gui/mainwindow.h"
#include "gui/matrixsplash.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("Sentinel IOC Manager");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Cybersecurity Division");
    
    // Set application style
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // Load custom fonts if available
    QFontDatabase::addApplicationFont(":/fonts/matrix.ttf");
    
    // Create and show animated matrix splash screen
    MatrixSplash *splash = new MatrixSplash();
    splash->show();
    
    // Process events to show splash
    app.processEvents();
    
    // Create main window
    MainWindow window;
    
    // Show main window maximized after loading time and hide splash
    QTimer::singleShot(4000, [&]() {
        splash->finish(&window);
        window.showMaximized();  // Open maximized by default
        splash->deleteLater();
    });
    
    return app.exec();
}
