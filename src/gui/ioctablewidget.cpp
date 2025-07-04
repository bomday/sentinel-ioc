#include "ioctablewidget.h"
#include "../maliciousHash/maliciousHash.hpp"
#include "../maliciousIP/maliciousIP.hpp"
#include "../maliciousURL/maliciousURL.hpp"

#include <QHeaderView>
#include <QMessageBox>
#include <QDateTime>

IOCTableWidget::IOCTableWidget(QWidget *parent)
    : QWidget(parent)
    , currentManager(nullptr)
    , blinkState(false)
{
    // Initialize theme colors
    primaryColor = "#00FF41";
    secondaryColor = "#008F11";
    accentColor = "#00FFFF";
    backgroundColor = "#0D1117";
    textColor = "#C9D1D9";
    
    setupUI();
    applyMatrixTheme();
    
    // Setup blinking effect timer
    blinkTimer = new QTimer(this);
    connect(blinkTimer, &QTimer::timeout, this, &IOCTableWidget::updateBlinkingCells);
    blinkTimer->start(1000); // Blink every second
}

void IOCTableWidget::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);
    
    // Filter section
    filterLayout = new QHBoxLayout();
    
    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Search IOCs...");
    searchEdit->setMaximumWidth(200);
    
    typeCombo = new QComboBox();
    typeCombo->addItem("All Types");
    typeCombo->addItem("Hash");
    typeCombo->addItem("IP");
    typeCombo->addItem("URL");
    typeCombo->setMaximumWidth(120);
    
    severityCombo = new QComboBox();
    severityCombo->addItem("All Severities");
    severityCombo->addItem("1 - Low");
    severityCombo->addItem("2 - Medium");
    severityCombo->addItem("3 - High");
    severityCombo->addItem("4 - Critical");
    severityCombo->addItem("5 - Emergency");
    severityCombo->setMaximumWidth(140);
    
    filterButton = new QPushButton("◈ FILTER");
    filterButton->setMaximumWidth(80);
    clearButton = new QPushButton("◈ CLEAR");
    clearButton->setMaximumWidth(80);
    
    connect(filterButton, &QPushButton::clicked, this, &IOCTableWidget::filterData);
    connect(clearButton, &QPushButton::clicked, this, &IOCTableWidget::clearFilter);
    connect(searchEdit, &QLineEdit::returnPressed, this, &IOCTableWidget::filterData);
    
    filterLayout->addWidget(new QLabel("◤ SEARCH:"));
    filterLayout->addWidget(searchEdit);
    filterLayout->addWidget(new QLabel("TYPE:"));
    filterLayout->addWidget(typeCombo);
    filterLayout->addWidget(new QLabel("SEVERITY:"));
    filterLayout->addWidget(severityCombo);
    filterLayout->addWidget(filterButton);
    filterLayout->addWidget(clearButton);
    filterLayout->addStretch();
    
    mainLayout->addLayout(filterLayout);
    
    // Setup table
    setupTable();
    
    // Status label
    statusLabel = new QLabel("◤ IOC DATABASE STATUS: READY ◥");
    statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(statusLabel);
}

void IOCTableWidget::setupTable()
{
    table = new QTableWidget();
    
    // Set column headers
    QStringList headers = {
        "ID", "SEVERITY", "TYPE", "DESCRIPTION", "ORIGIN", 
        "TIMESTAMP", "HASH/IP/URL", "ALGORITHM/COUNTRY/PROTOCOL", "DETAILS"
    };
    table->setColumnCount(headers.size());
    table->setHorizontalHeaderLabels(headers);
    
    // Configure table properties
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setAlternatingRowColors(true);
    table->setSortingEnabled(true);
    table->verticalHeader()->setVisible(false);
    
    // Configure column widths
    QHeaderView *header = table->horizontalHeader();
    header->setStretchLastSection(true);
    header->resizeSection(0, 60);   // ID
    header->resizeSection(1, 80);   // Severity
    header->resizeSection(2, 60);   // Type
    header->resizeSection(3, 200);  // Description
    header->resizeSection(4, 120);  // Origin
    header->resizeSection(5, 150);  // Timestamp
    header->resizeSection(6, 200);  // Hash/IP/URL
    header->resizeSection(7, 120);  // Algorithm/Country/Protocol
    
    connect(table, &QTableWidget::cellDoubleClicked, this, &IOCTableWidget::onCellDoubleClicked);
    
    mainLayout->addWidget(table);
}

void IOCTableWidget::applyMatrixTheme()
{
    setStyleSheet(QString(
        "QTableWidget {"
        "    background-color: %1;"
        "    color: %2;"
        "    gridline-color: %3;"
        "    selection-background-color: %3;"
        "    selection-color: %1;"
        "    border: 2px solid %3;"
        "    font-family: 'Courier New', monospace;"
        "    font-size: 10px;"
        "}"
        "QTableWidget::item {"
        "    padding: 5px;"
        "    border-bottom: 1px solid %4;"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: %3;"
        "    color: %1;"
        "}"
        "QHeaderView::section {"
        "    background-color: %4;"
        "    color: %2;"
        "    border: 1px solid %3;"
        "    padding: 8px;"
        "    font-weight: bold;"
        "    font-size: 11px;"
        "}"
        "QLineEdit {"
        "    background-color: %4;"
        "    color: %2;"
        "    border: 2px solid %3;"
        "    border-radius: 4px;"
        "    padding: 6px;"
        "    font-family: 'Courier New', monospace;"
        "}"
        "QLineEdit:focus {"
        "    border-color: %5;"
        "}"
        "QComboBox {"
        "    background-color: %4;"
        "    color: %2;"
        "    border: 2px solid %3;"
        "    border-radius: 4px;"
        "    padding: 6px;"
        "    font-family: 'Courier New', monospace;"
        "}"
        "QComboBox:drop-down {"
        "    border: none;"
        "}"
        "QComboBox::down-arrow {"
        "    image: none;"
        "    border-left: 5px solid transparent;"
        "    border-right: 5px solid transparent;"
        "    border-top: 5px solid %2;"
        "}"
        "QLabel {"
        "    color: %2;"
        "    font-weight: bold;"
        "    font-family: 'Courier New', monospace;"
        "}"
    ).arg(backgroundColor, textColor, primaryColor, secondaryColor, accentColor));
}

void IOCTableWidget::updateData(IndicatorManager *manager)
{
    currentManager = manager;
    populateTable(manager);
}

void IOCTableWidget::populateTable(IndicatorManager *manager)
{
    if (!manager) return;
    
    table->setRowCount(0);
    
    // Get indicators from manager
    // Note: You'll need to add a method to IndicatorManager to get all indicators
    // For now, we'll simulate with placeholder data
    
    // This would need to be implemented in IndicatorManager:
    // const auto& indicators = manager->getAllIndicators();
    
    // Placeholder implementation
    table->setRowCount(0); // Will be populated when IndicatorManager provides access to indicators
    
    statusLabel->setText(QString("◤ IOC DATABASE: %1 ENTRIES LOADED ◥").arg(table->rowCount()));
}

void IOCTableWidget::addTableRow(int row, const Indicator *ioc)
{
    if (!ioc) return;
    
    // ID
    table->setItem(row, 0, new QTableWidgetItem(QString::number(ioc->getIndicatorId())));
    
    // Severity
    QTableWidgetItem *severityItem = new QTableWidgetItem(getSeverityText(ioc->getSeverity()));
    severityItem->setBackground(getSeverityColor(ioc->getSeverity()));
    table->setItem(row, 1, severityItem);
    
    // Type
    table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(ioc->getType())));
    
    // Description
    table->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(ioc->getDescription())));
    
    // Origin
    table->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(ioc->getOrigin())));
    
    // Timestamp
    table->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(ioc->getTimestamp())));
    
    // Type-specific data
    QString mainValue, secondaryValue, details;
    
    if (const MaliciousHash* hash = dynamic_cast<const MaliciousHash*>(ioc)) {
        mainValue = QString::fromStdString(hash->getHash());
        secondaryValue = QString::fromStdString(hash->getAlgorithm());
        details = QString("Hash: %1").arg(QString::fromStdString(hash->getAlgorithm()));
    } else if (const MaliciousIP* ip = dynamic_cast<const MaliciousIP*>(ioc)) {
        mainValue = QString::fromStdString(ip->getIP());
        secondaryValue = QString::fromStdString(ip->getCountry());
        details = QString("ISP: %1").arg(QString::fromStdString(ip->getISP()));
    } else if (const MaliciousURL* url = dynamic_cast<const MaliciousURL*>(ioc)) {
        mainValue = QString::fromStdString(url->getURL());
        secondaryValue = QString::fromStdString(url->getProtocol());
        details = QString("Protocol: %1").arg(QString::fromStdString(url->getProtocol()));
    }
    
    table->setItem(row, 6, new QTableWidgetItem(mainValue));
    table->setItem(row, 7, new QTableWidgetItem(secondaryValue));
    table->setItem(row, 8, new QTableWidgetItem(details));
    
    // Add to blinking rows if high severity
    if (ioc->getSeverity() >= 4) {
        blinkingRows.append(row);
    }
}

QString IOCTableWidget::getSeverityText(int severity)
{
    switch (severity) {
        case 1: return "◈ LOW";
        case 2: return "◈ MEDIUM";
        case 3: return "◈ HIGH";
        case 4: return "◈ CRITICAL";
        case 5: return "◈ EMERGENCY";
        default: return "◈ UNKNOWN";
    }
}

QColor IOCTableWidget::getSeverityColor(int severity)
{
    switch (severity) {
        case 1: return QColor(0, 255, 0, 50);      // Green
        case 2: return QColor(255, 255, 0, 50);    // Yellow
        case 3: return QColor(255, 165, 0, 50);    // Orange
        case 4: return QColor(255, 0, 0, 50);      // Red
        case 5: return QColor(139, 0, 139, 50);    // Dark Magenta
        default: return QColor(128, 128, 128, 50); // Gray
    }
}

void IOCTableWidget::filterData()
{
    QString searchText = searchEdit->text().toLower();
    QString typeFilter = typeCombo->currentText();
    QString severityFilter = severityCombo->currentText();
    
    for (int row = 0; row < table->rowCount(); ++row) {
        bool showRow = true;
        
        // Search filter
        if (!searchText.isEmpty()) {
            bool matchFound = false;
            for (int col = 0; col < table->columnCount(); ++col) {
                QTableWidgetItem *item = table->item(row, col);
                if (item && item->text().toLower().contains(searchText)) {
                    matchFound = true;
                    break;
                }
            }
            if (!matchFound) showRow = false;
        }
        
        // Type filter
        if (typeFilter != "All Types" && showRow) {
            QTableWidgetItem *typeItem = table->item(row, 2);
            if (typeItem && typeItem->text() != typeFilter) {
                showRow = false;
            }
        }
        
        // Severity filter
        if (severityFilter != "All Severities" && showRow) {
            QTableWidgetItem *severityItem = table->item(row, 1);
            if (severityItem) {
                QString expectedSeverity = getSeverityText(severityFilter.left(1).toInt());
                if (severityItem->text() != expectedSeverity) {
                    showRow = false;
                }
            }
        }
        
        table->setRowHidden(row, !showRow);
    }
    
    // Update status
    int visibleRows = 0;
    for (int row = 0; row < table->rowCount(); ++row) {
        if (!table->isRowHidden(row)) visibleRows++;
    }
    statusLabel->setText(QString("◤ FILTER: %1 OF %2 ENTRIES VISIBLE ◥").arg(visibleRows).arg(table->rowCount()));
}

void IOCTableWidget::clearFilter()
{
    searchEdit->clear();
    typeCombo->setCurrentIndex(0);
    severityCombo->setCurrentIndex(0);
    
    for (int row = 0; row < table->rowCount(); ++row) {
        table->setRowHidden(row, false);
    }
    
    statusLabel->setText(QString("◤ IOC DATABASE: %1 ENTRIES LOADED ◥").arg(table->rowCount()));
}

void IOCTableWidget::onCellDoubleClicked(int row, int column)
{
    Q_UNUSED(column)
    
    QTableWidgetItem *idItem = table->item(row, 0);
    if (idItem) {
        QString iocId = idItem->text();
        QMessageBox::information(this, "IOC Details", 
            QString("◤ IOC DETAILS ◥\\n\\nID: %1\\n\\nDouble-click detected.\\nDetailed view would open here.").arg(iocId));
    }
}

void IOCTableWidget::updateBlinkingCells()
{
    blinkState = !blinkState;
    
    for (int row : blinkingRows) {
        if (row < table->rowCount()) {
            for (int col = 0; col < table->columnCount(); ++col) {
                QTableWidgetItem *item = table->item(row, col);
                if (item) {
                    if (blinkState) {
                        item->setBackground(QColor(255, 0, 0, 100)); // Red blink
                    } else {
                        // Restore original severity color
                        if (col == 1) { // Severity column
                            QTableWidgetItem *severityItem = table->item(row, 1);
                            if (severityItem) {
                                QString severityText = severityItem->text();
                                if (severityText.contains("CRITICAL")) {
                                    item->setBackground(getSeverityColor(4));
                                } else if (severityText.contains("EMERGENCY")) {
                                    item->setBackground(getSeverityColor(5));
                                }
                            }
                        } else {
                            item->setBackground(QColor(0, 0, 0, 0)); // Transparent
                        }
                    }
                }
            }
        }
    }
}
