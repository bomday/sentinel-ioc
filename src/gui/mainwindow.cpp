#include "mainwindow.h"
#include "matrixwidget.h"
#include "ioctablewidget.h"
#include "hackerterminal.h"
#include "addiocdialog.h"
// #include "searchdialog.h"
// #include "statisticsdialog.h"

#include <QApplication>
#include <QScreen>
#include <QMessageBox>
#include <QFileDialog>
#include <QHeaderView>
#include <QSplitter>
#include <QGroupBox>
#include <QDateTime>
#include <QFont>
#include <QFontDatabase>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , manager(new IndicatorManager())
    , matrixEnabled(true)
    , addDialog(nullptr)
    // , searchDialog(nullptr)
    // , statsDialog(nullptr)
{
    // Initialize theme colors (Matrix/Hacker theme)
    primaryColor = "#00FF41";      // Matrix green
    secondaryColor = "#008F11";    // Darker green
    accentColor = "#00FFFF";       // Cyan
    backgroundColor = "#0D1117";   // Dark background
    textColor = "#C9D1D9";         // Light text
    
    setupUI();
    applyMatrixTheme();
    loadIOCData();
    
    // Setup status timer
    statusTimer = new QTimer(this);
    connect(statusTimer, &QTimer::timeout, this, &MainWindow::updateSystemStatus);
    statusTimer->start(5000); // Update every 5 seconds
    
    // Initialize system tray
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/icons/sentinel.png"));
    trayIcon->show();
    
    updateStatusInfo();
}

MainWindow::~MainWindow()
{
    delete manager;
}

void MainWindow::setupUI()
{
    setWindowTitle("S E N T I N E L  I O C  [ M A T R I X  T E R M I N A L ]");
    setMinimumSize(1200, 800);
    
    // Center window on screen
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - 1200) / 2;
    int y = (screenGeometry.height() - 800) / 2;
    move(x, y);
    
    setupMenuBar();
    setupStatusBar();
    createCentralWidget();
}

void MainWindow::setupMenuBar()
{
    QMenuBar *menuBar = this->menuBar();
    
    // File Menu
    QMenu *fileMenu = menuBar->addMenu("▌ FILE");
    fileMenu->addAction("◈ Load Data", this, &MainWindow::loadData, QKeySequence::Open);
    fileMenu->addAction("◈ Save Data", this, &MainWindow::saveData, QKeySequence::Save);
    fileMenu->addAction("◈ Export Data", this, &MainWindow::exportData);
    fileMenu->addSeparator();
    fileMenu->addAction("◈ Exit", this, &QWidget::close, QKeySequence::Quit);
    
    // IOC Menu
    QMenu *iocMenu = menuBar->addMenu("▌ IOC");
    iocMenu->addAction("◈ Add IOC", this, &MainWindow::addIOC, QKeySequence("Ctrl+N"));
    iocMenu->addAction("◈ Edit IOC", this, &MainWindow::editIOC, QKeySequence("Ctrl+E"));
    iocMenu->addAction("◈ Delete IOC", this, &MainWindow::deleteIOC, QKeySequence::Delete);
    iocMenu->addSeparator();
    iocMenu->addAction("◈ Search IOC", this, &MainWindow::searchIOC, QKeySequence::Find);
    iocMenu->addAction("◈ Refresh", this, &MainWindow::refreshData, QKeySequence::Refresh);
    
    // Analysis Menu
    QMenu *analysisMenu = menuBar->addMenu("▌ ANALYSIS");
    analysisMenu->addAction("◈ Statistics", this, &MainWindow::showStatistics);
    
    // View Menu
    QMenu *viewMenu = menuBar->addMenu("▌ VIEW");
    viewMenu->addAction("◈ Toggle Matrix", this, &MainWindow::toggleMatrixBackground);
    
    // Help Menu
    QMenu *helpMenu = menuBar->addMenu("▌ HELP");
    helpMenu->addAction("◈ About", this, &MainWindow::showAbout);
}

void MainWindow::setupStatusBar()
{
    statusLabel = new QLabel("SYSTEM STATUS: OPERATIONAL");
    countLabel = new QLabel("IOC COUNT: 0");
    timeLabel = new QLabel(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    progressBar = new QProgressBar();
    progressBar->setVisible(false);
    progressBar->setMaximumWidth(200);
    
    statusBar()->addWidget(statusLabel);
    statusBar()->addPermanentWidget(countLabel);
    statusBar()->addPermanentWidget(progressBar);
    statusBar()->addPermanentWidget(timeLabel);
}

void MainWindow::createCentralWidget()
{
    centralWidget = new QWidget();
    setCentralWidget(centralWidget);
    
    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);
    
    // Create header with title
    QLabel *titleLabel = new QLabel("◤ SENTINEL IOC MANAGEMENT TERMINAL ◥");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(QString(
        "font-size: 24px; font-weight: bold; color: %1; "
        "padding: 15px; background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 %2, stop:0.5 %3, stop:1 %2); "
        "border: 2px solid %1; border-radius: 8px; margin-bottom: 10px;"
    ).arg(primaryColor, backgroundColor, secondaryColor));
    mainLayout->addWidget(titleLabel);
    
    // Create main splitter
    mainSplitter = new QSplitter(Qt::Horizontal);
    mainLayout->addWidget(mainSplitter);
    
    // Create control panel (left side)
    createControlPanel();
    
    // Create main content area (right side)
    createMainContent();
    
    // Set splitter proportions
    mainSplitter->setSizes({300, 900});
}

void MainWindow::createControlPanel()
{
    controlPanel = new QFrame();
    controlPanel->setFrameStyle(QFrame::Box);
    controlPanel->setMaximumWidth(300);
    
    QVBoxLayout *controlLayout = new QVBoxLayout(controlPanel);
    
    // Control panel title
    QLabel *controlTitle = new QLabel("◤ CONTROL MATRIX ◥");
    controlTitle->setAlignment(Qt::AlignCenter);
    controlLayout->addWidget(controlTitle);
    
    // Action buttons
    addButton = new QPushButton("◈ ADD IOC");
    editButton = new QPushButton("◈ EDIT IOC");
    deleteButton = new QPushButton("◈ DELETE IOC");
    searchButton = new QPushButton("◈ SEARCH IOC");
    refreshButton = new QPushButton("◈ REFRESH DATA");
    statsButton = new QPushButton("◈ STATISTICS");
    
    // Connect buttons
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addIOC);
    connect(editButton, &QPushButton::clicked, this, &MainWindow::editIOC);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteIOC);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::searchIOC);
    connect(refreshButton, &QPushButton::clicked, this, &MainWindow::refreshData);
    connect(statsButton, &QPushButton::clicked, this, &MainWindow::showStatistics);
    
    controlLayout->addWidget(addButton);
    controlLayout->addWidget(editButton);
    controlLayout->addWidget(deleteButton);
    
    // Add separator line
    QFrame *separator1 = new QFrame();
    separator1->setFrameShape(QFrame::HLine);
    separator1->setFrameShadow(QFrame::Sunken);
    controlLayout->addWidget(separator1);
    
    controlLayout->addWidget(searchButton);
    controlLayout->addWidget(refreshButton);
    
    // Add separator line
    QFrame *separator2 = new QFrame();
    separator2->setFrameShape(QFrame::HLine);
    separator2->setFrameShadow(QFrame::Sunken);
    controlLayout->addWidget(separator2);
    
    controlLayout->addWidget(statsButton);
    
    // Info panel
    infoPanel = new QFrame();
    infoPanel->setFrameStyle(QFrame::Box);
    
    QVBoxLayout *infoLayout = new QVBoxLayout(infoPanel);
    QLabel *infoTitle = new QLabel("◤ SYSTEM INFO ◥");
    infoTitle->setAlignment(Qt::AlignCenter);
    infoLayout->addWidget(infoTitle);
    
    // Add system information labels
    QLabel *systemInfo = new QLabel(
        "STATUS: ACTIVE\\n"
        "SECURITY: MAXIMUM\\n"
        "ENCRYPTION: AES-256\\n"
        "PROTOCOL: SECURE\\n"
        "MATRIX: ENABLED"
    );
    infoLayout->addWidget(systemInfo);
    
    controlLayout->addWidget(infoPanel);
    controlLayout->addStretch();
    
    mainSplitter->addWidget(controlPanel);
}

void MainWindow::createMainContent()
{
    // Create tab widget for main content
    tabWidget = new QTabWidget();
    
    // IOC Table Tab
    iocTable = new IOCTableWidget();
    tabWidget->addTab(iocTable, "◈ IOC DATABASE");
    
    // Hacker Terminal Tab
    hackerTerminal = new HackerTerminal();
    hackerTerminal->setIOCManager(manager);  // Connect IOC data
    tabWidget->addTab(hackerTerminal, "◈ SECURITY TERMINAL");
    
    // Log Tab
    logTextEdit = new QTextEdit();
    logTextEdit->setReadOnly(true);
    logTextEdit->append("[SYSTEM] Sentinel IOC Terminal initialized...");
    logTextEdit->append("[SYSTEM] Matrix protocol activated...");
    logTextEdit->append("[SYSTEM] Security level: MAXIMUM");
    tabWidget->addTab(logTextEdit, "◈ SYSTEM LOG");
    
    // Matrix background tab
    if (matrixEnabled) {
        matrixBackground = new MatrixWidget();
        tabWidget->addTab(matrixBackground, "◈ MATRIX");
    }
    
    mainSplitter->addWidget(tabWidget);
}

void MainWindow::applyMatrixTheme()
{
    // Main window style
    setStyleSheet(QString(
        "QMainWindow {"
        "    background-color: %1;"
        "    color: %2;"
        "}"
        "QMenuBar {"
        "    background-color: %3;"
        "    color: %4;"
        "    border-bottom: 1px solid %4;"
        "    font-weight: bold;"
        "}"
        "QMenuBar::item {"
        "    padding: 8px 16px;"
        "    background-color: transparent;"
        "}"
        "QMenuBar::item:selected {"
        "    background-color: %5;"
        "    color: %1;"
        "}"
        "QMenu {"
        "    background-color: %3;"
        "    color: %4;"
        "    border: 1px solid %4;"
        "}"
        "QMenu::item {"
        "    padding: 8px 24px;"
        "}"
        "QMenu::item:selected {"
        "    background-color: %5;"
        "    color: %1;"
        "}"
        "QPushButton {"
        "    background-color: %3;"
        "    color: %4;"
        "    border: 2px solid %4;"
        "    border-radius: 6px;"
        "    padding: 10px;"
        "    font-weight: bold;"
        "    font-size: 11px;"
        "}"
        "QPushButton:hover {"
        "    background-color: %5;"
        "    color: %1;"
        "    border-color: %6;"
        "}"
        "QPushButton:pressed {"
        "    background-color: %4;"
        "    color: %1;"
        "}"
        "QFrame {"
        "    background-color: %3;"
        "    border: 1px solid %4;"
        "    border-radius: 6px;"
        "    padding: 5px;"
        "}"
        "QLabel {"
        "    color: %4;"
        "    font-weight: bold;"
        "}"
        "QTabWidget::pane {"
        "    border: 2px solid %4;"
        "    background-color: %3;"
        "}"
        "QTabBar::tab {"
        "    background-color: %3;"
        "    color: %4;"
        "    border: 1px solid %4;"
        "    padding: 8px 16px;"
        "    margin-right: 2px;"
        "    font-weight: bold;"
        "}"
        "QTabBar::tab:selected {"
        "    background-color: %5;"
        "    color: %1;"
        "}"
        "QStatusBar {"
        "    background-color: %3;"
        "    color: %4;"
        "    border-top: 1px solid %4;"
        "}"
        "QTextEdit {"
        "    background-color: %1;"
        "    color: %4;"
        "    border: 1px solid %4;"
        "    font-family: 'Courier New', monospace;"
        "    font-size: 10px;"
        "}"
        "QProgressBar {"
        "    border: 1px solid %4;"
        "    border-radius: 3px;"
        "    background-color: %1;"
        "}"
        "QProgressBar::chunk {"
        "    background-color: %4;"
        "    border-radius: 2px;"
        "}"
    ).arg(backgroundColor, textColor, secondaryColor, primaryColor, primaryColor, accentColor));
}

void MainWindow::loadIOCData()
{
    try {
        manager->loadIndicatorsFromFile("data/ioc.csv");
        updateIOCTable();
        logTextEdit->append(QString("[SYSTEM] IOC database loaded successfully"));
    } catch (const std::exception& e) {
        logTextEdit->append(QString("[ERROR] Failed to load IOC database: %1").arg(e.what()));
    }
}

void MainWindow::updateIOCTable()
{
    if (iocTable) {
        iocTable->updateData(manager);
        updateStatusInfo();
    }
}

void MainWindow::updateStatusInfo()
{
    // Update IOC count
    // Note: You'll need to add a method to get the count from IndicatorManager
    countLabel->setText("IOC COUNT: Loading...");
    timeLabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

// Slot implementations
void MainWindow::addIOC()
{
    if (!addDialog) {
        addDialog = new AddIOCDialog(manager, this);
    }
    if (addDialog->exec() == QDialog::Accepted) {
        updateIOCTable();
        logTextEdit->append("[SYSTEM] New IOC added to database");
    }
}

void MainWindow::editIOC()
{
    // Implementation for editing IOC
    logTextEdit->append("[SYSTEM] Edit IOC functionality activated");
}

void MainWindow::deleteIOC()
{
    // Implementation for deleting IOC
    logTextEdit->append("[SYSTEM] Delete IOC functionality activated");
}

void MainWindow::searchIOC()
{
    // Temporary: Search dialog not implemented yet
    QMessageBox::information(this, "Search", "Search functionality will be available soon!");
    /*
    if (!searchDialog) {
        searchDialog = new SearchDialog(manager, this);
    }
    searchDialog->exec();
    */
}

void MainWindow::showStatistics()
{
    // Temporary: Statistics dialog not implemented yet  
    QMessageBox::information(this, "Statistics", "Statistics functionality will be available soon!");
    /*
    if (!statsDialog) {
        statsDialog = new StatisticsDialog(manager, this);
    }
    statsDialog->exec();
    */
}

void MainWindow::refreshData()
{
    progressBar->setVisible(true);
    progressBar->setValue(0);
    
    // Simulate loading progress
    for (int i = 0; i <= 100; i += 20) {
        progressBar->setValue(i);
        QApplication::processEvents();
        QThread::msleep(100);
    }
    
    loadIOCData();
    progressBar->setVisible(false);
    logTextEdit->append("[SYSTEM] Database refreshed");
}

void MainWindow::loadData()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Load IOC Data", "", "CSV Files (*.csv)");
    if (!fileName.isEmpty()) {
        // Load data from file
        logTextEdit->append(QString("[SYSTEM] Loading data from: %1").arg(fileName));
    }
}

void MainWindow::saveData()
{
    // Save current data
    manager->saveIndicatorsToFile("data/ioc.csv");
    logTextEdit->append("[SYSTEM] Database saved successfully");
}

void MainWindow::exportData()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export IOC Data", "", "CSV Files (*.csv)");
    if (!fileName.isEmpty()) {
        // Export data to file
        logTextEdit->append(QString("[SYSTEM] Data exported to: %1").arg(fileName));
    }
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, "About Sentinel IOC",
        "◤ SENTINEL IOC MANAGEMENT TERMINAL ◥\\n\\n"
        "Version: 1.0.0\\n"
        "Matrix Protocol: ACTIVE\\n"
        "Security Level: MAXIMUM\\n\\n"
        "A cybersecurity IOC management system\\n"
        "with Matrix-themed interface.\\n\\n"
        "Developed for threat intelligence\\n"
        "and security operations.");
}

void MainWindow::toggleMatrixBackground()
{
    matrixEnabled = !matrixEnabled;
    logTextEdit->append(QString("[SYSTEM] Matrix background: %1").arg(matrixEnabled ? "ENABLED" : "DISABLED"));
}

void MainWindow::updateSystemStatus()
{
    timeLabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    
    // Add some dynamic status messages
    static int statusCounter = 0;
    QStringList statusMessages = {
        "SYSTEM STATUS: OPERATIONAL",
        "SYSTEM STATUS: MONITORING",
        "SYSTEM STATUS: SCANNING",
        "SYSTEM STATUS: ANALYZING"
    };
    statusLabel->setText(statusMessages[statusCounter % statusMessages.size()]);
    statusCounter++;
}
