#include "mainwindow.h"
#include "matrixwidget.h"
#include "ioctablewidget.h"
#include "hackerterminal.h"
#include "addiocdialog.h"
#include "searchdialog.h"
#include "statisticsdialog.h"
#include "../maliciousHash/maliciousHash.hpp"
#include "../maliciousIP/maliciousIP.hpp"
#include "../maliciousURL/maliciousURL.hpp"

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
    , searchDialog(nullptr)
    , statsDialog(nullptr)
{
    // Initialize theme colors (Terminal theme)
    primaryColor = "#00FF00";      // Terminal green
    secondaryColor = "#004400";    // Dark green
    accentColor = "#00FF41";       // Bright green
    backgroundColor = "#000000";   // Black background
    textColor = "#00FF00";         // Green text
    
    setupUI();
    applyTerminalTheme();
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
    setWindowTitle("SENTINEL IOC TERMINAL");
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
    fileMenu->addAction("◈ Load Data", QKeySequence::Open, this, &MainWindow::loadData);
    fileMenu->addAction("◈ Save Data", QKeySequence::Save, this, &MainWindow::saveData);
    fileMenu->addAction("◈ Export Data", this, &MainWindow::exportData);
    fileMenu->addSeparator();
    fileMenu->addAction("◈ Exit", QKeySequence::Quit, this, &QWidget::close);
    
    // IOC Menu
    QMenu *iocMenu = menuBar->addMenu("▌ IOC");
    iocMenu->addAction("◈ Add IOC", QKeySequence("Ctrl+N"), this, &MainWindow::addIOC);
    iocMenu->addAction("◈ Edit IOC", QKeySequence("Ctrl+E"), this, &MainWindow::editIOC);
    iocMenu->addAction("◈ Delete IOC", QKeySequence::Delete, this, &MainWindow::deleteIOC);
    iocMenu->addSeparator();
    iocMenu->addAction("◈ Search IOC", QKeySequence::Find, this, &MainWindow::searchIOC);
    iocMenu->addAction("◈ Refresh", QKeySequence::Refresh, this, &MainWindow::refreshData);
    
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
    
    // Add tooltips for better accessibility
    addButton->setToolTip("Add a new Indicator of Compromise to the database");
    editButton->setToolTip("Edit the selected IOC in the table");
    deleteButton->setToolTip("Delete the selected IOC from the database");
    searchButton->setToolTip("Open search dialog to find specific IOCs");
    refreshButton->setToolTip("Reload IOC data from storage");
    statsButton->setToolTip("View detailed statistics and analytics");
    
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

void MainWindow::applyTerminalTheme()
{
    // Main window style with terminal theme
    setStyleSheet(QString(
        "* {"
        "    font-family: 'Courier New', monospace;"
        "}"
        "QMainWindow {"
        "    background-color: #000000;"
        "    color: #00FF00;"
        "}"
        "QMenuBar {"
        "    background-color: #000000;"
        "    color: #00FF00;"
        "    border-bottom: 2px solid #00FF00;"
        "    font-weight: bold;"
        "    font-family: 'Courier New', monospace;"
        "}"
        "QMenuBar::item {"
        "    padding: 8px 16px;"
        "    background-color: transparent;"
        "    color: #00FF00;"
        "}"
        "QMenuBar::item:selected {"
        "    background-color: #004400;"
        "    color: #00FF00;"
        "    border: 1px solid #00FF00;"
        "}"
        "QMenu {"
        "    background-color: #000000;"
        "    color: #00FF00;"
        "    border: 2px solid #00FF00;"
        "    font-family: 'Courier New', monospace;"
        "}"
        "QMenu::item {"
        "    padding: 8px 24px;"
        "    color: #00FF00;"
        "}"
        "QMenu::item:selected {"
        "    background-color: #004400;"
        "    color: #00FF00;"
        "}"
        "QPushButton {"
        "    background-color: #001100;"
        "    color: #00FF00;"
        "    border: 2px solid #00FF00;"
        "    border-radius: 4px;"
        "    padding: 8px 16px;"
        "    font-weight: bold;"
        "    font-family: 'Courier New', monospace;"
        "}"
        "QPushButton:hover {"
        "    background-color: #004400;"
        "    color: #00FF00;"
        "    border: 3px solid #00FF41;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #006600;"
        "    color: #000000;"
        "}"
        "QFrame {"
        "    background-color: #000000;"
        "    border: 1px solid #00FF00;"
        "    border-radius: 4px;"
        "    padding: 5px;"
        "}"
        "QLabel {"
        "    color: #00FF00;"
        "    font-weight: bold;"
        "    font-family: 'Courier New', monospace;"
        "}"
        "QTabWidget::pane {"
        "    border: 2px solid #00FF00;"
        "    background-color: #000000;"
        "}"
        "QTabBar::tab {"
        "    background-color: #001100;"
        "    color: #00FF00;"
        "    border: 1px solid #00FF00;"
        "    padding: 8px 16px;"
        "    margin-right: 2px;"
        "    font-weight: bold;"
        "    font-family: 'Courier New', monospace;"
        "}"
        "QTabBar::tab:selected {"
        "    background-color: #004400;"
        "    color: #00FF00;"
        "    border: 2px solid #00FF41;"
        "}"
        "QStatusBar {"
        "    background-color: #000000;"
        "    color: #00FF00;"
        "    border-top: 2px solid #00FF00;"
        "    font-family: 'Courier New', monospace;"
        "}"
        "QTextEdit {"
        "    background-color: #000000;"
        "    color: #00FF00;"
        "    border: 2px solid #00FF00;"
        "    font-family: 'Courier New', monospace;"
        "    font-size: 10px;"
        "}"
        "QProgressBar {"
        "    border: 2px solid #00FF00;"
        "    border-radius: 3px;"
        "    background-color: #000000;"
        "    color: #00FF00;"
        "    font-family: 'Courier New', monospace;"
        "}"
        "QProgressBar::chunk {"
        "    background-color: #00FF00;"
        "    border-radius: 2px;"
        "}"
    ));
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
    // Update IOC count using the manager
    if (manager) {
        size_t count = manager->getIndicatorCount();
        countLabel->setText(QString("IOC COUNT: %1").arg(count));
    } else {
        countLabel->setText("IOC COUNT: N/A");
    }
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
    // Get current selection from IOC table
    int selectedRow = iocTable->getSelectedRow();
    if (selectedRow < 0) {
        QMessageBox::warning(this, "Edit IOC", "Please select an IOC to edit.");
        return;
    }
    
    if (selectedRow >= static_cast<int>(manager->getIndicatorCount())) {
        QMessageBox::warning(this, "Edit IOC", "Invalid selection.");
        return;
    }
    
    const Indicator* selectedIOC = manager->getIndicator(selectedRow);
    if (!selectedIOC) {
        QMessageBox::warning(this, "Edit IOC", "Failed to retrieve selected IOC.");
        return;
    }
    
    // Create and configure edit dialog (reuse AddIOCDialog)
    if (!addDialog) {
        addDialog = new AddIOCDialog(manager, this);
        connect(addDialog, &AddIOCDialog::iocAdded, this, &MainWindow::onIOCAdded);
    }
    
    // Store original values for logging
    int originalSeverity = selectedIOC->getSeverity();
    QString originalOrigin = QString::fromStdString(selectedIOC->getOrigin());
    
    // Pre-populate dialog with current values
    addDialog->setEditMode(true, selectedIOC);
    
    if (addDialog->exec() == QDialog::Accepted) {
        // Log the successful edit with detailed information
        QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString logMessage = QString("[IOC_EDIT] IOC ID:%1 modified at %2")
                                .arg(selectedIOC->getIndicatorId())
                                .arg(currentTime);
        
        // Add details about what was changed
        logMessage += QString(" | Type: %1 | Original Severity: %2 | Original Origin: %3")
                        .arg(QString::fromStdString(selectedIOC->getType()))
                        .arg(originalSeverity)
                        .arg(originalOrigin);
        
        logTextEdit->append(logMessage);
        
        // Refresh the IOC table to show changes
        loadIOCData();
        updateStatusInfo();
        
        logTextEdit->append("[SYSTEM] IOC table refreshed after edit operation");
    } else {
        logTextEdit->append("[SYSTEM] IOC edit operation cancelled by user");
    }
}

void MainWindow::deleteIOC()
{
    // Get current selection from IOC table
    int selectedRow = iocTable->getSelectedRow();
    if (selectedRow < 0) {
        QMessageBox::warning(this, "Delete IOC", "Please select an IOC to delete.");
        return;
    }
    
    if (selectedRow >= static_cast<int>(manager->getIndicatorCount())) {
        QMessageBox::warning(this, "Delete IOC", "Invalid selection.");
        return;
    }
    
    const Indicator* selectedIOC = manager->getIndicator(selectedRow);
    if (!selectedIOC) {
        QMessageBox::warning(this, "Delete IOC", "Failed to retrieve selected IOC.");
        return;
    }
    
    // Store IOC details for logging before deletion
    int iocId = selectedIOC->getIndicatorId();
    QString iocType = QString::fromStdString(selectedIOC->getType());
    QString iocDescription = QString::fromStdString(selectedIOC->getDescription());
    int iocSeverity = selectedIOC->getSeverity();
    QString iocOrigin = QString::fromStdString(selectedIOC->getOrigin());
    QString iocTimestamp = QString::fromStdString(selectedIOC->getTimestamp());
    
    // Get type-specific information for detailed logging
    QString specificInfo;
    if (iocType == "IP") {
        if (auto* ipIOC = dynamic_cast<const MaliciousIP*>(selectedIOC)) {
            specificInfo = QString("IP: %1, Country: %2, ISP: %3")
                             .arg(QString::fromStdString(ipIOC->getIP()))
                             .arg(QString::fromStdString(ipIOC->getCountry()))
                             .arg(QString::fromStdString(ipIOC->getISP()));
        }
    } else if (iocType == "URL") {
        if (auto* urlIOC = dynamic_cast<const MaliciousURL*>(selectedIOC)) {
            specificInfo = QString("URL: %1, Protocol: %2")
                             .arg(QString::fromStdString(urlIOC->getURL()))
                             .arg(QString::fromStdString(urlIOC->getProtocol()));
        }
    } else if (iocType == "Hash") {
        if (auto* hashIOC = dynamic_cast<const MaliciousHash*>(selectedIOC)) {
            specificInfo = QString("Hash: %1, Algorithm: %2")
                             .arg(QString::fromStdString(hashIOC->getHash()))
                             .arg(QString::fromStdString(hashIOC->getAlgorithm()));
        }
    }
    
    // Confirmation dialog
    QString confirmMessage = QString("Are you sure you want to delete this IOC?\n\n"
                                   "ID: %1\n"
                                   "Type: %2\n"
                                   "Description: %3\n"
                                   "Severity: %4\n"
                                   "%5")
                               .arg(iocId)
                               .arg(iocType)
                               .arg(iocDescription)
                               .arg(iocSeverity)
                               .arg(specificInfo);
    
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Deletion", 
                                                            confirmMessage,
                                                            QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        // Attempt to remove the IOC
        if (manager->removeIndicatorById(iocId)) {
            // Log successful deletion with comprehensive details
            QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            QString logMessage = QString("[IOC_REMOVE] IOC deleted at %1")
                                   .arg(currentTime);
            
            logMessage += QString(" | ID: %1 | Type: %2 | Severity: %3 | Origin: %4 | Created: %5")
                            .arg(iocId)
                            .arg(iocType)
                            .arg(iocSeverity)
                            .arg(iocOrigin)
                            .arg(iocTimestamp);
            
            if (!specificInfo.isEmpty()) {
                logMessage += QString(" | Details: %1").arg(specificInfo);
            }
            
            logMessage += QString(" | Description: %1").arg(iocDescription);
            
            logTextEdit->append(logMessage);
            
            // Refresh the IOC table
            loadIOCData();
            updateStatusInfo();
            
            logTextEdit->append("[SYSTEM] IOC table refreshed after deletion");
            
            QMessageBox::information(this, "Delete IOC", "IOC successfully deleted.");
        } else {
            QString errorMessage = QString("[ERROR] Failed to delete IOC ID:%1 - %2")
                                     .arg(iocId)
                                     .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            logTextEdit->append(errorMessage);
            QMessageBox::critical(this, "Delete IOC", "Failed to delete the selected IOC.");
        }
    } else {
        logTextEdit->append(QString("[SYSTEM] IOC deletion cancelled by user - ID:%1 at %2")
                              .arg(iocId)
                              .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
    }
}

void MainWindow::searchIOC()
{
    if (!searchDialog) {
        searchDialog = new SearchDialog(manager, this);
    }
    
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    logTextEdit->append(QString("[SYSTEM] Search dialog opened at %1").arg(currentTime));
    
    if (searchDialog->exec() == QDialog::Accepted) {
        logTextEdit->append("[SYSTEM] Search operation completed");
    } else {
        logTextEdit->append("[SYSTEM] Search operation cancelled by user");
    }
}

void MainWindow::showStatistics()
{
    if (!statsDialog) {
        statsDialog = new StatisticsDialog(manager, this);
    }
    // Update statistics with current data before showing
    statsDialog->refreshStatistics();
    
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    logTextEdit->append(QString("[SYSTEM] Statistics dialog opened at %1 - IOC count: %2")
                          .arg(currentTime)
                          .arg(manager->getIndicatorCount()));
    
    statsDialog->exec();
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
        logTextEdit->append(QString("[DATA_LOAD] Loading data from: %1 at %2")
                              .arg(fileName)
                              .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
        
        try {
            manager->loadIndicatorsFromFile(fileName.toStdString());
            loadIOCData();
            updateStatusInfo();
            logTextEdit->append(QString("[DATA_LOAD] ✓ Successfully loaded %1 IOCs from file")
                                  .arg(manager->getIndicatorCount()));
        } catch (const std::exception& e) {
            logTextEdit->append(QString("[DATA_LOAD] ✗ Failed to load data: %1").arg(e.what()));
            QMessageBox::critical(this, "Load Error", QString("Failed to load data: %1").arg(e.what()));
        }
    }
}

void MainWindow::saveData()
{
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    logTextEdit->append(QString("[DATA_SAVE] Saving %1 IOCs to default location at %2")
                          .arg(manager->getIndicatorCount())
                          .arg(currentTime));
    
    try {
        manager->saveIndicatorsToFile("data/ioc.csv");
        logTextEdit->append("[DATA_SAVE] ✓ Database saved successfully to data/ioc.csv");
    } catch (const std::exception& e) {
        logTextEdit->append(QString("[DATA_SAVE] ✗ Failed to save database: %1").arg(e.what()));
        QMessageBox::critical(this, "Save Error", QString("Failed to save database: %1").arg(e.what()));
    }
}

void MainWindow::exportData()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export IOC Data", "ioc_export.csv", "CSV Files (*.csv)");
    if (!fileName.isEmpty()) {
        QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        logTextEdit->append(QString("[DATA_EXPORT] Exporting %1 IOCs to: %2 at %3")
                              .arg(manager->getIndicatorCount())
                              .arg(fileName)
                              .arg(currentTime));
        
        try {
            manager->saveIndicatorsToFile(fileName.toStdString());
            logTextEdit->append("[DATA_EXPORT] ✓ IOC data successfully exported");
            QMessageBox::information(this, "Export Complete", "IOC data has been successfully exported.");
        } catch (const std::exception& e) {
            logTextEdit->append(QString("[DATA_EXPORT] ✗ Failed to export data: %1").arg(e.what()));
            QMessageBox::critical(this, "Export Error", QString("Failed to export data: %1").arg(e.what()));
        }
    } else {
        logTextEdit->append("[SYSTEM] Export operation cancelled by user");
    }
}

void MainWindow::showAbout()
{
    QString secretMessage = 
        "<pre style='color: #00FF00; font-family: Courier New, monospace; font-size: 11px; line-height: 1.2;'>"
        "◤════════════════════════════════════════◥\n"
        "║          CLASSIFIED TRANSMISSION          ║\n"
        "◥════════════════════════════════════════◤\n\n"
        "┌─ DECRYPTION SEQUENCE INITIATED ─┐\n"
        "│ STATUS: [██████████] 100% COMPLETE │\n"
        "└───────────────────────────────────┘\n\n"
        "/* =============== SECRET CODE =============== */\n"
        "// SENTINEL IOC MANAGEMENT SYSTEM v1.0.0\n"
        "// CLASSIFIED PROJECT: MATRIX PROTOCOL\n"
        "//\n"
        "// DEVELOPER AUTHENTICATION:\n"
        "// ┌─ SECURITY CLEARANCE: OMEGA ─┐\n"
        "//\n"
        "// [LEAD ARCHITECT]     : <span style='color: #FF0000; font-weight: bold;'>DAY</span>\n"
        "// [SECURITY SPECIALIST]: <span style='color: #FF0000; font-weight: bold;'>BELENA</span>\n"
        "// [CORE DEVELOPER]     : <span style='color: #FF0000; font-weight: bold;'>rafitels</span>\n"
        "// [SYSTEM ANALYST]     : <span style='color: #FF0000; font-weight: bold;'>MARY</span>\n"
        "//\n"
        "// COMPILATION_DATE: 2025.07.04\n"
        "// THREAT_LEVEL: CLASSIFIED\n"
        "// MATRIX_STATUS: ACTIVE\n"
        "//\n"
        "// WARNING: UNAUTHORIZED ACCESS DETECTED\n"
        "// INITIATING COUNTERMEASURES...\n"
        "// [████████████████████████] SECURE\n"
        "//\n"
        "/* ======================================== */\n\n"
        "◢ TRANSMISSION ENDS ◣\n"
        "▓▓▓ CONNECTION TERMINATED ▓▓▓"
        "</pre>";

    // Create a custom dialog for the secret message
    QDialog *aboutDialog = new QDialog(this);
    aboutDialog->setWindowTitle("◤ CLASSIFIED ACCESS ◥");
    aboutDialog->setMinimumSize(500, 400);
    aboutDialog->setModal(true);
    
    QVBoxLayout *layout = new QVBoxLayout(aboutDialog);
    
    QTextEdit *textArea = new QTextEdit();
    textArea->setHtml(secretMessage);  // Changed from setPlainText to setHtml
    textArea->setReadOnly(true);
    textArea->setStyleSheet(
        "QTextEdit {"
        "    background-color: #000000;"
        "    color: #00FF00;"
        "    font-family: 'Courier New', monospace;"
        "    font-size: 11px;"
        "    border: 2px solid #00FF00;"
        "    padding: 10px;"
        "    line-height: 1.2;"
        "}"
        "QScrollBar:vertical {"
        "    background-color: #001100;"
        "    border: 1px solid #00FF00;"
        "    width: 12px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background-color: #00FF00;"
        "    border-radius: 3px;"
        "}"
    );
    
    QPushButton *closeButton = new QPushButton("◈ TERMINATE CONNECTION");
    closeButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #001100;"
        "    color: #00FF00;"
        "    border: 2px solid #00FF00;"
        "    border-radius: 6px;"
        "    padding: 8px;"
        "    font-weight: bold;"
        "    font-family: 'Courier New', monospace;"
        "}"
        "QPushButton:hover {"
        "    background-color: #00FF00;"
        "    color: #000000;"
        "}"
    );
    
    connect(closeButton, &QPushButton::clicked, aboutDialog, &QDialog::accept);
    
    layout->addWidget(textArea);
    layout->addWidget(closeButton);
    
    // Apply dialog styling
    aboutDialog->setStyleSheet(
        "QDialog {"
        "    background-color: #000000;"
        "    border: 3px solid #00FF00;"
        "}"
    );
    
    aboutDialog->exec();
    delete aboutDialog;
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

void MainWindow::onIOCAdded(const QString &logMessage)
{
    logTextEdit->append(logMessage);
    loadIOCData();  // Refresh the IOC table
    updateStatusInfo();  // Update status information
}
