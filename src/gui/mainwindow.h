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

#include "../indicatorManager/indicatorManager.hpp"

class MatrixWidget;
class IOCTableWidget;
class HackerTerminal;
class AddIOCDialog;
class SearchDialog;
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
    void searchIOC();
    void showStatistics();
    void refreshData();
    void loadData();
    void saveData();
    void exportData();
    void showAbout();
    void toggleMatrixBackground();
    void updateSystemStatus();
    void onIOCAdded(const QString &logMessage);

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

    // Core components
    IndicatorManager* manager;
    
    // UI Components
    QWidget* centralWidget;
    QSplitter* mainSplitter;
    QTabWidget* tabWidget;
    
    // Matrix theme components
    MatrixWidget* matrixBackground;
    bool matrixEnabled;
    
    // IOC management
    IOCTableWidget* iocTable;
    QTextEdit* logTextEdit;
    HackerTerminal* hackerTerminal;
    
    // Control panels
    QFrame* controlPanel;
    QFrame* infoPanel;
    
    // Buttons
    QPushButton* addButton;
    QPushButton* editButton;
    QPushButton* deleteButton;
    QPushButton* searchButton;
    QPushButton* refreshButton;
    QPushButton* statsButton;
    
    // Status components
    QLabel* statusLabel;
    QLabel* countLabel;
    QLabel* timeLabel;
    QProgressBar* progressBar;
    QTimer* statusTimer;
    
    // Dialogs
    AddIOCDialog* addDialog;
    SearchDialog* searchDialog;
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
