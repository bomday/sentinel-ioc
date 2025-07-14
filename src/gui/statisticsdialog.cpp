#include "statisticsdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QProgressBar>
#include <QMessageBox>
#include <QTextEdit>
#include <QSplitter>

StatisticsDialog::StatisticsDialog(IndicatorManager* manager, QWidget *parent)
    : QDialog(parent)
    , manager(manager)
    , primaryColor("#00FF00")
    , backgroundColor("#000000")
    , secondaryColor("#004400")
{
    setWindowTitle("IOC Statistics - CLI Output");
    setMinimumSize(900, 750);  // Optimized size for CLI focus
    setupUI();
    applyTerminalTheme();
    updateStatistics();
}

void StatisticsDialog::setupUI()
{
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Title label
    QLabel *titleLabel = new QLabel("IOC Statistics Dashboard - CLI Option 6");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; padding: 10px; color: #00FF00; font-family: 'Courier New', monospace;");
    mainLayout->addWidget(titleLabel);
    
    // Create a splitter to divide space evenly between overview and CLI output
    QSplitter *splitter = new QSplitter(Qt::Vertical, this);
    splitter->setStyleSheet(
        "QSplitter::handle {"
        "    background-color: #00FF00;"
        "    height: 3px;"
        "    margin: 2px 0px;"
        "}"
        "QSplitter::handle:hover {"
        "    background-color: #00FF41;"
        "}"
    );
    
    // Statistics overview group - Dashboard style
    QGroupBox *overviewGroup = new QGroupBox("◤ THREAT INTELLIGENCE DASHBOARD ◥");
    QVBoxLayout *overviewMainLayout = new QVBoxLayout(overviewGroup);
    
    // System status and key metrics
    QHBoxLayout *statusLayout = new QHBoxLayout();
    
    // Total IOCs with visual indicator
    QVBoxLayout *totalLayout = new QVBoxLayout();
    totalCountLabel = new QLabel("Loading...");
    totalCountLabel->setAlignment(Qt::AlignCenter);
    totalCountLabel->setStyleSheet(
        "font-size: 24px; font-weight: bold; color: #00FF00; "
        "background-color: #001100; border: 2px solid #00FF00; "
        "border-radius: 8px; padding: 10px; margin: 5px;"
    );
    QLabel *totalTitle = new QLabel("TOTAL THREATS");
    totalTitle->setAlignment(Qt::AlignCenter);
    totalTitle->setStyleSheet("font-size: 10px; color: #00FF00; font-weight: bold;");
    totalLayout->addWidget(totalCountLabel);
    totalLayout->addWidget(totalTitle);
    
    // Threat level indicator
    QVBoxLayout *threatLayout = new QVBoxLayout();
    threatLevelLabel = new QLabel("SECURE");
    threatLevelLabel->setAlignment(Qt::AlignCenter);
    threatLevelLabel->setStyleSheet(
        "font-size: 14px; font-weight: bold; color: #00FF00; "
        "background-color: #001100; border: 2px solid #00FF00; "
        "border-radius: 8px; padding: 10px; margin: 5px;"
    );
    QLabel *threatTitle = new QLabel("THREAT LEVEL");
    threatTitle->setAlignment(Qt::AlignCenter);
    threatTitle->setStyleSheet("font-size: 10px; color: #00FF00; font-weight: bold;");
    threatLayout->addWidget(threatLevelLabel);
    threatLayout->addWidget(threatTitle);
    
    // Activity indicator
    QVBoxLayout *activityLayout = new QVBoxLayout();
    activityLabel = new QLabel("STABLE");
    activityLabel->setAlignment(Qt::AlignCenter);
    activityLabel->setStyleSheet(
        "font-size: 14px; font-weight: bold; color: #00FF00; "
        "background-color: #001100; border: 2px solid #00FF00; "
        "border-radius: 8px; padding: 10px; margin: 5px;"
    );
    QLabel *activityTitle = new QLabel("ACTIVITY");
    activityTitle->setAlignment(Qt::AlignCenter);
    activityTitle->setStyleSheet("font-size: 10px; color: #00FF00; font-weight: bold;");
    activityLayout->addWidget(activityLabel);
    activityLayout->addWidget(activityTitle);
    
    statusLayout->addLayout(totalLayout);
    statusLayout->addLayout(threatLayout);
    statusLayout->addLayout(activityLayout);
    overviewMainLayout->addLayout(statusLayout);
    
    // Type distribution with progress bars
    QLabel *distributionTitle = new QLabel("◢ THREAT TYPE DISTRIBUTION ◣");
    distributionTitle->setAlignment(Qt::AlignCenter);
    distributionTitle->setStyleSheet("font-size: 12px; font-weight: bold; color: #00FF00; margin: 10px 0px 5px 0px;");
    overviewMainLayout->addWidget(distributionTitle);
    
    QGridLayout *distLayout = new QGridLayout();
    
    // Hash indicators
    QLabel *hashLabel = new QLabel("MALICIOUS HASHES:");
    hashLabel->setStyleSheet("font-size: 11px; color: #00FF00; font-weight: bold;");
    hashProgressBar = new QProgressBar();
    hashProgressBar->setStyleSheet(
        "QProgressBar { border: 1px solid #00FF00; background-color: #001100; "
        "text-align: center; color: #00FF00; font-weight: bold; } "
        "QProgressBar::chunk { background-color: #00FF00; }"
    );
    hashCountLabel = new QLabel("0");
    hashCountLabel->setStyleSheet("font-size: 11px; color: #00FF00; font-weight: bold; min-width: 30px;");
    hashCountLabel->setAlignment(Qt::AlignCenter);
    
    distLayout->addWidget(hashLabel, 0, 0);
    distLayout->addWidget(hashProgressBar, 0, 1);
    distLayout->addWidget(hashCountLabel, 0, 2);
    
    // IP indicators
    QLabel *ipLabel = new QLabel("MALICIOUS IPs:");
    ipLabel->setStyleSheet("font-size: 11px; color: #00FF00; font-weight: bold;");
    ipProgressBar = new QProgressBar();
    ipProgressBar->setStyleSheet(
        "QProgressBar { border: 1px solid #00FF00; background-color: #001100; "
        "text-align: center; color: #00FF00; font-weight: bold; } "
        "QProgressBar::chunk { background-color: #00FF00; }"
    );
    ipCountLabel = new QLabel("0");
    ipCountLabel->setStyleSheet("font-size: 11px; color: #00FF00; font-weight: bold; min-width: 30px;");
    ipCountLabel->setAlignment(Qt::AlignCenter);
    
    distLayout->addWidget(ipLabel, 1, 0);
    distLayout->addWidget(ipProgressBar, 1, 1);
    distLayout->addWidget(ipCountLabel, 1, 2);
    
    // URL indicators
    QLabel *urlLabel = new QLabel("MALICIOUS URLs:");
    urlLabel->setStyleSheet("font-size: 11px; color: #00FF00; font-weight: bold;");
    urlProgressBar = new QProgressBar();
    urlProgressBar->setStyleSheet(
        "QProgressBar { border: 1px solid #00FF00; background-color: #001100; "
        "text-align: center; color: #00FF00; font-weight: bold; } "
        "QProgressBar::chunk { background-color: #00FF00; }"
    );
    urlCountLabel = new QLabel("0");
    urlCountLabel->setStyleSheet("font-size: 11px; color: #00FF00; font-weight: bold; min-width: 30px;");
    urlCountLabel->setAlignment(Qt::AlignCenter);
    
    distLayout->addWidget(urlLabel, 2, 0);
    distLayout->addWidget(urlProgressBar, 2, 1);
    distLayout->addWidget(urlCountLabel, 2, 2);
    
    overviewMainLayout->addLayout(distLayout);
    
    // High severity alerts section
    QLabel *alertTitle = new QLabel("◢ HIGH SEVERITY ALERTS ◣");
    alertTitle->setAlignment(Qt::AlignCenter);
    alertTitle->setStyleSheet("font-size: 12px; font-weight: bold; color: #FF4444; margin: 10px 0px 5px 0px;");
    overviewMainLayout->addWidget(alertTitle);
    
    QHBoxLayout *alertLayout = new QHBoxLayout();
    
    // Critical (Severity 5) alerts
    QVBoxLayout *criticalLayout = new QVBoxLayout();
    severity5Label = new QLabel("0");
    severity5Label->setAlignment(Qt::AlignCenter);
    severity5Label->setStyleSheet(
        "font-size: 18px; font-weight: bold; color: #FF0000; "
        "background-color: #110000; border: 2px solid #FF0000; "
        "border-radius: 6px; padding: 8px; margin: 2px;"
    );
    QLabel *criticalTitle = new QLabel("CRITICAL");
    criticalTitle->setAlignment(Qt::AlignCenter);
    criticalTitle->setStyleSheet("font-size: 9px; color: #FF0000; font-weight: bold;");
    criticalLayout->addWidget(severity5Label);
    criticalLayout->addWidget(criticalTitle);
    
    // High (Severity 4) alerts
    QVBoxLayout *highLayout = new QVBoxLayout();
    severity4Label = new QLabel("0");
    severity4Label->setAlignment(Qt::AlignCenter);
    severity4Label->setStyleSheet(
        "font-size: 18px; font-weight: bold; color: #FF4444; "
        "background-color: #110000; border: 2px solid #FF4444; "
        "border-radius: 6px; padding: 8px; margin: 2px;"
    );
    QLabel *highTitle = new QLabel("HIGH");
    highTitle->setAlignment(Qt::AlignCenter);
    highTitle->setStyleSheet("font-size: 9px; color: #FF4444; font-weight: bold;");
    highLayout->addWidget(severity4Label);
    highLayout->addWidget(highTitle);
    
    // Recent activity (current/last month comparison)
    QVBoxLayout *recentLayout = new QVBoxLayout();
    QHBoxLayout *monthlyLayout = new QHBoxLayout();
    currentMonthLabel = new QLabel("0");
    currentMonthLabel->setAlignment(Qt::AlignCenter);
    currentMonthLabel->setStyleSheet(
        "font-size: 14px; font-weight: bold; color: #00FF00; "
        "background-color: #001100; border: 1px solid #00FF00; "
        "border-radius: 4px; padding: 6px; margin: 2px;"
    );
    lastMonthLabel = new QLabel("0");
    lastMonthLabel->setAlignment(Qt::AlignCenter);
    lastMonthLabel->setStyleSheet(
        "font-size: 14px; font-weight: bold; color: #00AA00; "
        "background-color: #001100; border: 1px solid #00AA00; "
        "border-radius: 4px; padding: 6px; margin: 2px;"
    );
    monthlyLayout->addWidget(currentMonthLabel);
    monthlyLayout->addWidget(lastMonthLabel);
    QLabel *monthlyTitle = new QLabel("THIS/LAST MONTH");
    monthlyTitle->setAlignment(Qt::AlignCenter);
    monthlyTitle->setStyleSheet("font-size: 9px; color: #00FF00; font-weight: bold;");
    recentLayout->addLayout(monthlyLayout);
    recentLayout->addWidget(monthlyTitle);
    
    alertLayout->addLayout(criticalLayout);
    alertLayout->addLayout(highLayout);
    alertLayout->addLayout(recentLayout);
    overviewMainLayout->addLayout(alertLayout);
    
    // Hidden labels for backward compatibility (not displayed but updated)
    severity1Label = new QLabel();
    severity1Label->hide();
    severity2Label = new QLabel();
    severity2Label->hide();
    severity3Label = new QLabel();
    severity3Label->hide();
    
    // Add overview group to splitter
    splitter->addWidget(overviewGroup);
    
    // CLI-style detailed statistics - Main focus
    QGroupBox *detailedGroup = new QGroupBox("◤ CLI STATISTICS OUTPUT ◥");
    QVBoxLayout *detailedLayout = new QVBoxLayout(detailedGroup);
    
    detailedTextLabel = new QTextEdit();
    detailedTextLabel->setReadOnly(true);
    detailedTextLabel->setStyleSheet(
        "font-family: 'Courier New', monospace; "
        "background-color: #001100; "
        "padding: 15px; "  // Increased padding
        "border: 2px solid #00FF00; "  // Thicker border
        "color: #00FF00; "
        "font-size: 12px; "  // Slightly larger font
        "line-height: 1.2;"  // Better line spacing
    );
    detailedLayout->addWidget(detailedTextLabel);
    
    // Add detailed group to splitter
    splitter->addWidget(detailedGroup);
    
    // Set sizes to give CLI output slightly more space (40/60 split)
    QList<int> sizes;
    sizes << 2 << 3;  // Overview gets 40%, CLI gets 60% of the space
    splitter->setSizes(sizes);
    
    // Add splitter to main layout
    mainLayout->addWidget(splitter);
    
    // Only refresh button - remove non-functional export button
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    refreshButton = new QPushButton("Refresh Statistics");
    
    connect(refreshButton, &QPushButton::clicked, this, &StatisticsDialog::updateStatistics);
    
    buttonLayout->addWidget(refreshButton);
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);
}

void StatisticsDialog::applyTerminalTheme()
{
    setStyleSheet(QString(
        "QDialog {"
        "    background-color: #000000;"
        "    color: #00FF00;"
        "    font-family: 'Courier New', monospace;"
        "}"
        "QGroupBox {"
        "    font-weight: bold;"
        "    border: 1px solid #00FF00;"
        "    border-radius: 4px;"
        "    margin-top: 1ex;"
        "    padding: 10px;"
        "    color: #00FF00;"
        "    font-family: 'Courier New', monospace;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    left: 10px;"
        "    padding: 0 5px 0 5px;"
        "    color: #00FF00;"
        "}"
        "QPushButton {"
        "    background-color: #001100;"
        "    color: #00FF00;"
        "    border: 1px solid #00FF00;"
        "    border-radius: 4px;"
        "    padding: 8px 16px;"
        "    font-weight: normal;"
        "    font-family: 'Courier New', monospace;"
        "}"
        "QPushButton:hover {"
        "    background-color: #004400;"
        "    color: #00FF00;"
        "    border: 2px solid #00FF00;"
        "}"
        "QLabel {"
        "    color: #00FF00;"
        "    font-weight: normal;"
        "    padding: 5px;"
        "    font-family: 'Courier New', monospace;"
        "}"
        "QTextEdit {"
        "    background-color: #001100;"
        "    color: #00FF00;"
        "    border: 2px solid #004400;"
        "    font-family: 'Courier New', monospace;"
        "    font-size: 12px;"  // Updated to match the inline style
        "    padding: 8px;"
        "}"
        "QScrollBar:vertical {"
        "    background-color: #001100;"  // Darker background
        "    width: 16px;"  // Slightly wider
        "    border: 2px solid #00FF00;"  // Thicker border
        "    border-radius: 4px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background-color: #00FF00;"
        "    border-radius: 6px;"
        "    min-height: 30px;"  // Taller handle
        "    margin: 2px;"  // Small margin for better appearance
        "}"
        "QScrollBar::handle:vertical:hover {"
        "    background-color: #00FF41;"  // Brighter on hover
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    border: none;"  // Remove arrow buttons for cleaner look
        "    background: none;"
        "}"
    ));
}

void StatisticsDialog::updateStatistics()
{
    if (!manager) {
        // Fallback if manager is null
        totalCountLabel->setText("N/A");
        hashCountLabel->setText("0");
        ipCountLabel->setText("0");
        urlCountLabel->setText("0");
        currentMonthLabel->setText("0");
        lastMonthLabel->setText("0");
        severity4Label->setText("0");
        severity5Label->setText("0");
        threatLevelLabel->setText("OFFLINE");
        activityLabel->setText("OFFLINE");
        
        hashProgressBar->setValue(0);
        ipProgressBar->setValue(0);
        urlProgressBar->setValue(0);
        return;
    }

    // Get basic counts
    size_t totalIOCs = manager->getIndicatorCount();
    totalCountLabel->setText(QString::number(totalIOCs));

    // Get type distribution
    auto typeDistribution = manager->getTypeDistribution();
    int hashCount = typeDistribution["Hash"];
    int ipCount = typeDistribution["IP"];
    int urlCount = typeDistribution["URL"];
    
    hashCountLabel->setText(QString::number(hashCount));
    ipCountLabel->setText(QString::number(ipCount));
    urlCountLabel->setText(QString::number(urlCount));

    // Update progress bars (percentage of total)
    if (totalIOCs > 0) {
        hashProgressBar->setMaximum(100);
        ipProgressBar->setMaximum(100);
        urlProgressBar->setMaximum(100);
        
        hashProgressBar->setValue((hashCount * 100) / totalIOCs);
        ipProgressBar->setValue((ipCount * 100) / totalIOCs);
        urlProgressBar->setValue((urlCount * 100) / totalIOCs);
        
        hashProgressBar->setFormat(QString("%1%").arg((hashCount * 100) / totalIOCs));
        ipProgressBar->setFormat(QString("%1%").arg((ipCount * 100) / totalIOCs));
        urlProgressBar->setFormat(QString("%1%").arg((urlCount * 100) / totalIOCs));
    } else {
        hashProgressBar->setValue(0);
        ipProgressBar->setValue(0);
        urlProgressBar->setValue(0);
    }

    // Get monthly statistics
    int currentMonth = manager->getIOCsRegisteredCurrentMonth();
    int lastMonth = manager->getIOCsRegisteredLastMonth();
    currentMonthLabel->setText(QString::number(currentMonth));
    lastMonthLabel->setText(QString::number(lastMonth));

    // Get severity distribution
    auto severityDistribution = manager->getSeverityDistribution();
    int severity4 = severityDistribution[4];
    int severity5 = severityDistribution[5];
    
    severity4Label->setText(QString::number(severity4));
    severity5Label->setText(QString::number(severity5));
    
    // Update hidden labels for compatibility
    severity1Label->setText(QString("Severity 1: %1").arg(severityDistribution[1]));
    severity2Label->setText(QString("Severity 2: %1").arg(severityDistribution[2]));
    severity3Label->setText(QString("Severity 3: %1").arg(severityDistribution[3]));

    // Calculate threat level based on high severity threats
    int highSeverityThreats = severity4 + severity5;
    if (severity5 > 0) {
        threatLevelLabel->setText("CRITICAL");
        threatLevelLabel->setStyleSheet(
            "font-size: 14px; font-weight: bold; color: #FF0000; "
            "background-color: #110000; border: 2px solid #FF0000; "
            "border-radius: 8px; padding: 10px; margin: 5px;"
        );
    } else if (severity4 > 5) {
        threatLevelLabel->setText("HIGH");
        threatLevelLabel->setStyleSheet(
            "font-size: 14px; font-weight: bold; color: #FF4444; "
            "background-color: #110000; border: 2px solid #FF4444; "
            "border-radius: 8px; padding: 10px; margin: 5px;"
        );
    } else if (highSeverityThreats > 0) {
        threatLevelLabel->setText("ELEVATED");
        threatLevelLabel->setStyleSheet(
            "font-size: 14px; font-weight: bold; color: #FFAA00; "
            "background-color: #111100; border: 2px solid #FFAA00; "
            "border-radius: 8px; padding: 10px; margin: 5px;"
        );
    } else {
        threatLevelLabel->setText("SECURE");
        threatLevelLabel->setStyleSheet(
            "font-size: 14px; font-weight: bold; color: #00FF00; "
            "background-color: #001100; border: 2px solid #00FF00; "
            "border-radius: 8px; padding: 10px; margin: 5px;"
        );
    }

    // Calculate activity level based on recent additions
    if (currentMonth > lastMonth * 2) {
        activityLabel->setText("HIGH");
        activityLabel->setStyleSheet(
            "font-size: 14px; font-weight: bold; color: #FF4444; "
            "background-color: #110000; border: 2px solid #FF4444; "
            "border-radius: 8px; padding: 10px; margin: 5px;"
        );
    } else if (currentMonth > lastMonth) {
        activityLabel->setText("ELEVATED");
        activityLabel->setStyleSheet(
            "font-size: 14px; font-weight: bold; color: #FFAA00; "
            "background-color: #111100; border: 2px solid #FFAA00; "
            "border-radius: 8px; padding: 10px; margin: 5px;"
        );
    } else {
        activityLabel->setText("STABLE");
        activityLabel->setStyleSheet(
            "font-size: 14px; font-weight: bold; color: #00FF00; "
            "background-color: #001100; border: 2px solid #00FF00; "
            "border-radius: 8px; padding: 10px; margin: 5px;"
        );
    }

    // Update detailed CLI-style text
    updateDetailedStatistics();
}

void StatisticsDialog::updateDetailedStatistics()
{
    if (!manager) {
        detailedTextLabel->setText("Error: Manager not available");
        return;
    }
    
    QString detailedText;
    detailedText += "=========== IOCs Statistics ===========\n\n";
    
    // Total count (exactly matching CLI format)
    size_t totalIOCs = manager->getIndicatorCount();
    detailedText += QString("Total: %1 IOCs\n\n").arg(totalIOCs);
    
    // Distribution by type (exactly matching CLI format)
    detailedText += "Distribution by Type:\n";
    auto typeDistribution = manager->getTypeDistribution();
    for (auto it = typeDistribution.begin(); it != typeDistribution.end(); ++it) {
        detailedText += QString("  %1: %2\n").arg(QString::fromStdString(it->first)).arg(it->second);
    }
    
    // Distribution by severity (exactly matching CLI format - show ALL severities like CLI does)
    detailedText += "\nDistribution by Severity:\n";
    auto severityDistribution = manager->getSeverityDistribution();
    for (auto it = severityDistribution.begin(); it != severityDistribution.end(); ++it) {
        detailedText += QString("  Severity %1: %2\n").arg(it->first).arg(it->second);
    }
    
    // Monthly statistics (exactly matching CLI format)
    int lastMonth = manager->getIOCsRegisteredLastMonth();
    int currentMonth = manager->getIOCsRegisteredCurrentMonth();
    detailedText += QString("\nIOCs registered in last month: %1\n").arg(lastMonth);
    detailedText += QString("IOCs registered in current month: %1\n").arg(currentMonth);
    
    detailedText += "\n===========================================";
    
    detailedTextLabel->setText(detailedText);
}

void StatisticsDialog::refreshStatistics()
{
    updateStatistics();
}

void StatisticsDialog::updateAnimations()
{
    animationStep++;
    
    // Animate progress bars with a pulsing effect
    int pulseValue = 50 + 30 * sin(animationStep * 0.2);
    
    if (hashProgress) {
        int targetValue = (currentStats.hashCount * 100) / (currentStats.totalCount > 0 ? currentStats.totalCount : 1);
        int currentValue = hashProgress->value();
        if (currentValue < targetValue) {
            hashProgress->setValue(std::min(currentValue + 2, targetValue));
        }
    }
    
    if (ipProgress) {
        int targetValue = (currentStats.ipCount * 100) / (currentStats.totalCount > 0 ? currentStats.totalCount : 1);
        int currentValue = ipProgress->value();
        if (currentValue < targetValue) {
            ipProgress->setValue(std::min(currentValue + 2, targetValue));
        }
    }
    
    if (urlProgress) {
        int targetValue = (currentStats.urlCount * 100) / (currentStats.totalCount > 0 ? currentStats.totalCount : 1);
        int currentValue = urlProgress->value();
        if (currentValue < targetValue) {
            urlProgress->setValue(std::min(currentValue + 2, targetValue));
        }
    }
    
    if (severityProgress) {
        int avgSeverity = currentStats.totalCount > 0 ? 
                         (currentStats.severity1 + currentStats.severity2 * 2 + 
                          currentStats.severity3 * 3 + currentStats.severity4 * 4 + 
                          currentStats.severity5 * 5) / currentStats.totalCount : 0;
        int targetValue = (avgSeverity * 100) / 5;
        int currentValue = severityProgress->value();
        if (abs(currentValue - targetValue) > 1) {
            severityProgress->setValue(currentValue < targetValue ? currentValue + 1 : currentValue - 1);
        }
    }
    
    // Update threat level label with blinking effect for high severity
    if (threatLevelLabel && currentStats.severity4 + currentStats.severity5 > 0) {
        if (animationStep % 20 < 10) {
            threatLevelLabel->setStyleSheet("color: #FF0000; font-weight: bold; font-size: 14px;");
        } else {
            threatLevelLabel->setStyleSheet("color: #FF4444; font-weight: bold; font-size: 14px;");
        }
    }
    
    // Update activity label with scrolling effect
    if (activityLabel) {
        QString activity = "◆ SCANNING... ◆ ANALYZING... ◆ MONITORING... ";
        int pos = animationStep % activity.length();
        QString scrolledActivity = activity.mid(pos) + activity.left(pos);
        activityLabel->setText(scrolledActivity.left(40));
    }
}
