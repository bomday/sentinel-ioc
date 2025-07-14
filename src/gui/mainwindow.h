#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QTabWidget>
#include <QTextEdit>
#include <QStatusBar>
#include <QMenuBar>
#include <QAction>
#include <QSplitter>
#include <QFrame>
#include <QTimer>
#include <QProgressBar>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QPropertyAnimation>

#include "../indicatorManager/indicatorManager.hpp"

class IOCTableWidget;
class AddIOCDialog;
class StatisticsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addIOC();
    void editIOC();
    void deleteIOC();
    void showStatistics();
    void loadData();
    void saveData();
    void exportData();
    void showAbout();
    void updateSystemStatus();
    void onIOCAdded(const QString &logMessage);
    void applyGraphicsEffects(); // Apply graphics effects after UI initialization

private:
    void setupUI();
    void setupMenuBar();
    void setupStatusBar();
    void applyTerminalTheme();
    void createCentralWidget();
    void createControlPanel();
    void createMainContent();
    void loadIOCData();
    void updateIOCTable();
    void updateStatusInfo();
    
    // Visual effects
    void addButtonGlowEffect(QPushButton* button);
    
    // Message box helpers
    void showResizableInformation(const QString &title, const QString &text);
    void showResizableWarning(const QString &title, const QString &text);
    void showResizableCritical(const QString &title, const QString &text);
    QMessageBox::StandardButton showResizableQuestion(const QString &title, const QString &text);

    // Core components
    IndicatorManager* manager;
    
    // UI Components
    QWidget* centralWidget;
    QSplitter* mainSplitter;
    QTabWidget* tabWidget;
    
    // IOC management
    IOCTableWidget* iocTable;
    QTextEdit* logTextEdit;
    
    // Control panels
    QFrame* controlPanel;
    
    // Buttons
    QPushButton* addButton;
    QPushButton* editButton;
    QPushButton* deleteButton;
    QPushButton* statsButton;
    
    // Status components
    QLabel* statusLabel;
    QLabel* countLabel;
    QLabel* timeLabel;
    QProgressBar* progressBar;
    QTimer* statusTimer;
    
    // Dialogs
    AddIOCDialog* addDialog;
    StatisticsDialog* statsDialog;
    
    // System tray
    QSystemTrayIcon* trayIcon;
    
    // Theme colors
    QString primaryColor;
    QString secondaryColor;
    QString accentColor;
    QString backgroundColor;
    QString textColor;
};

#endif // MAINWINDOW_H
