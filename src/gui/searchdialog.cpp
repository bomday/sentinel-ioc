#include "searchdialog.h"
#include <QMessageBox>
#include <QDateTime>
#include "../maliciousIP/maliciousIP.hpp"
#include "../maliciousURL/maliciousURL.hpp"
#include "../maliciousHash/maliciousHash.hpp"

SearchDialog::SearchDialog(IndicatorManager* manager, QWidget *parent)
    : QDialog(parent)
    , manager(manager)
    , primaryColor("#00FF41")
    , backgroundColor("#0D1117")
    , secondaryColor("#008F11")
{
    setWindowTitle("◤ SEARCH IOC MATRIX ◥");
    setMinimumSize(800, 600);
    setupUI();
    applyMatrixTheme();
}

void SearchDialog::setupUI()
{
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Title label
    QLabel *titleLabel = new QLabel("◤ IOC SEARCH INTERFACE ◥");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; padding: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Search criteria group
    QGroupBox *criteriaGroup = new QGroupBox("Search Criteria");
    QGridLayout *criteriaLayout = new QGridLayout(criteriaGroup);
    
    // Search type
    criteriaLayout->addWidget(new QLabel("Search Type:"), 0, 0);
    searchTypeCombo = new QComboBox();
    searchTypeCombo->addItems({"All Types", "IP", "URL", "Hash"});
    searchTypeCombo->setToolTip("Select the type of IOC to search for");
    criteriaLayout->addWidget(searchTypeCombo, 0, 1);
    
    // Search value
    criteriaLayout->addWidget(new QLabel("Search Value:"), 1, 0);
    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Enter IOC value to search...");
    searchEdit->setToolTip("Enter text to search in IOC descriptions and origins");
    criteriaLayout->addWidget(searchEdit, 1, 1);
    
    // Threat level
    criteriaLayout->addWidget(new QLabel("Threat Level:"), 2, 0);
    severityCombo = new QComboBox();
    severityCombo->addItems({"All Levels", "1 - Low", "2 - Medium", "3 - High", "4 - Critical", "5 - Emergency"});
    severityCombo->setToolTip("Filter by threat severity level");
    criteriaLayout->addWidget(severityCombo, 2, 1);
    
    // Type filter
    criteriaLayout->addWidget(new QLabel("IOC Type:"), 3, 0);
    iocTypeCombo = new QComboBox();
    iocTypeCombo->addItems({"All Types", "Hash", "IP", "URL"});
    iocTypeCombo->setToolTip("Filter by IOC type");
    criteriaLayout->addWidget(iocTypeCombo, 3, 1);
    
    // Date range
    criteriaLayout->addWidget(new QLabel("Date From:"), 4, 0);
    dateFromEdit = new QDateEdit(QDate::currentDate().addDays(-30));
    dateFromEdit->setCalendarPopup(true);
    criteriaLayout->addWidget(dateFromEdit, 4, 1);
    
    criteriaLayout->addWidget(new QLabel("Date To:"), 5, 0);
    dateToEdit = new QDateEdit(QDate::currentDate());
    dateToEdit->setCalendarPopup(true);
    criteriaLayout->addWidget(dateToEdit, 5, 1);
    
    mainLayout->addWidget(criteriaGroup);
    
    // Search buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    searchButton = new QPushButton("◈ SEARCH");
    clearButton = new QPushButton("◈ CLEAR");
    exportButton = new QPushButton("◈ EXPORT RESULTS");
    
    connect(searchButton, &QPushButton::clicked, this, &SearchDialog::performSearch);
    connect(clearButton, &QPushButton::clicked, this, &SearchDialog::clearSearch);
    connect(exportButton, &QPushButton::clicked, this, &SearchDialog::exportResults);
    
    buttonLayout->addWidget(searchButton);
    buttonLayout->addWidget(clearButton);
    buttonLayout->addWidget(exportButton);
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);
    
    // Results table
    resultsTable = new QTableWidget();
    resultsTable->setColumnCount(6);
    QStringList headers = {"Type", "Value", "Threat Level", "Source", "Date Added", "Description"};
    resultsTable->setHorizontalHeaderLabels(headers);
    resultsTable->horizontalHeader()->setStretchLastSection(true);
    resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    resultsTable->setAlternatingRowColors(true);
    
    mainLayout->addWidget(resultsTable);
    
    // Status label
    resultsCountLabel = new QLabel("Ready to search...");
    mainLayout->addWidget(resultsCountLabel);
}

void SearchDialog::applyMatrixTheme()
{
    setStyleSheet(QString(
        "QDialog {"
        "    background-color: %1;"
        "    color: %2;"
        "}"
        "QGroupBox {"
        "    font-weight: bold;"
        "    border: 2px solid %2;"
        "    border-radius: 6px;"
        "    margin-top: 1ex;"
        "    padding: 10px;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    left: 10px;"
        "    padding: 0 5px 0 5px;"
        "}"
        "QPushButton {"
        "    background-color: %3;"
        "    color: %2;"
        "    border: 2px solid %2;"
        "    border-radius: 6px;"
        "    padding: 8px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: %2;"
        "    color: %1;"
        "}"
        "QLineEdit, QComboBox, QDateEdit {"
        "    background-color: %3;"
        "    color: %2;"
        "    border: 1px solid %2;"
        "    border-radius: 4px;"
        "    padding: 6px;"
        "}"
        "QTableWidget {"
        "    background-color: %1;"
        "    color: %2;"
        "    border: 1px solid %2;"
        "    gridline-color: %2;"
        "}"
        "QHeaderView::section {"
        "    background-color: %3;"
        "    color: %2;"
        "    border: 1px solid %2;"
        "    padding: 6px;"
        "    font-weight: bold;"
        "}"
    ).arg(backgroundColor, primaryColor, secondaryColor));
}

void SearchDialog::performSearch()
{
    if (!manager) {
        QMessageBox::warning(this, "Error", "No IOC manager available for search.");
        return;
    }
    
    resultsCountLabel->setText("Searching...");
    resultsTable->setRowCount(0);
    
    // Get search criteria
    QString searchValue = searchEdit->text().trimmed();
    QString searchType = searchTypeCombo->currentText();
    QString severityText = severityCombo->currentText();
    
    // Convert severity to numeric value
    int severity = 0;
    if (severityText.contains("Low (1)")) severity = 1;
    else if (severityText.contains("Medium (2)")) severity = 2;
    else if (severityText.contains("High (3)")) severity = 3;
    else if (severityText.contains("Critical (4)")) severity = 4;
    else if (severityText.contains("Extreme (5)")) severity = 5;
    
    std::vector<const Indicator*> results;
    
    // Perform search based on criteria
    if (searchValue.isEmpty() && searchType == "All Types" && severity == 0) {
        // Show all indicators if no specific criteria
        for (size_t i = 0; i < manager->getIndicatorCount(); ++i) {
            const Indicator* ioc = manager->getIndicator(i);
            if (ioc) {
                results.push_back(ioc);
            }
        }
    } else {
        // Use the combined search with filters
        std::string valueStr = searchValue.toStdString();
        std::string typeStr = (searchType == "All Types") ? "" : searchType.toStdString();
        
        results = manager->searchWithFilters(valueStr, typeStr, severity, "", "");
    }
    
    // Populate results table
    populateSearchResults(results);
    
    // Update status
    resultsCountLabel->setText(QString("Found %1 IOC(s) matching your criteria").arg(results.size()));
}

void SearchDialog::populateSearchResults(const std::vector<const Indicator*>& results)
{
    resultsTable->setRowCount(results.size());
    
    for (size_t i = 0; i < results.size(); ++i) {
        const Indicator* ioc = results[i];
        
        // Type
        resultsTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(ioc->getType())));
        
        // Value (type-specific)
        QString value;
        if (const MaliciousIP* ip = dynamic_cast<const MaliciousIP*>(ioc)) {
            value = QString::fromStdString(ip->getIP());
        } else if (const MaliciousURL* url = dynamic_cast<const MaliciousURL*>(ioc)) {
            value = QString::fromStdString(url->getURL());
        } else if (const MaliciousHash* hash = dynamic_cast<const MaliciousHash*>(ioc)) {
            value = QString::fromStdString(hash->getHash());
        }
        resultsTable->setItem(i, 1, new QTableWidgetItem(value));
        
        // Threat Level
        QString threatLevel;
        int severity = ioc->getSeverity();
        switch (severity) {
            case 1: threatLevel = "Low"; break;
            case 2: threatLevel = "Medium"; break;
            case 3: threatLevel = "High"; break;
            case 4: threatLevel = "Critical"; break;
            case 5: threatLevel = "Extreme"; break;
            default: threatLevel = "Unknown"; break;
        }
        resultsTable->setItem(i, 2, new QTableWidgetItem(threatLevel));
        
        // Source
        resultsTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(ioc->getOrigin())));
        
        // Date Added
        resultsTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(ioc->getTimestamp())));
        
        // Description
        resultsTable->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(ioc->getDescription())));
        
        // Color code rows based on severity
        QColor rowColor;
        switch (severity) {
            case 4:
            case 5:
                rowColor = QColor(40, 0, 0); // Dark red for critical
                break;
            case 3:
                rowColor = QColor(40, 20, 0); // Dark orange for high
                break;
            case 2:
                rowColor = QColor(40, 40, 0); // Dark yellow for medium
                break;
            default:
                rowColor = QColor(0, 20, 0); // Dark green for low
                break;
        }
        
        for (int col = 0; col < 6; ++col) {
            if (resultsTable->item(i, col)) {
                resultsTable->item(i, col)->setBackground(rowColor);
            }
        }
    }
    
    // Resize columns to content
    resultsTable->resizeColumnsToContents();
}

void SearchDialog::clearSearch()
{
    searchEdit->clear();
    searchTypeCombo->setCurrentIndex(0);
    severityCombo->setCurrentIndex(0);
    dateFromEdit->setDate(QDate::currentDate().addDays(-30));
    dateToEdit->setDate(QDate::currentDate());
    resultsTable->setRowCount(0);
    resultsCountLabel->setText("Ready to search...");
}

void SearchDialog::exportResults()
{
    QMessageBox::information(this, "Export", 
        "Export functionality will be available once search results are implemented.");
}

void SearchDialog::applyFilters()
{
    if (!manager) {
        resultsCountLabel->setText("No IOC data available");
        return;
    }
    
    resultsTable->setRowCount(0);
    QVector<const Indicator*> filteredResults;
    
    QString searchText = searchEdit->text().toLower();
    QString selectedType = iocTypeCombo->currentText();
    int selectedSeverity = severityCombo->currentIndex();
    
    for (size_t i = 0; i < manager->getIndicatorCount(); ++i) {
        const Indicator* indicator = manager->getIndicator(i);
        if (!indicator) continue;
        
        bool matchesSearch = searchText.isEmpty() || 
                           QString::fromStdString(indicator->getDescription()).toLower().contains(searchText) ||
                           QString::fromStdString(indicator->getOrigin()).toLower().contains(searchText);
        
        bool matchesType = selectedType == "All Types" || 
                          QString::fromStdString(indicator->getType()) == selectedType;
        
        bool matchesSeverity = selectedSeverity == 0 || 
                              indicator->getSeverity() == selectedSeverity;
        
        if (matchesSearch && matchesType && matchesSeverity) {
            filteredResults.append(indicator);
        }
    }
    
    // Populate results table
    resultsTable->setRowCount(filteredResults.size());
    for (int i = 0; i < filteredResults.size(); ++i) {
        const Indicator* indicator = filteredResults[i];
        
        resultsTable->setItem(i, 0, new QTableWidgetItem(QString::number(indicator->getIndicatorId())));
        resultsTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(indicator->getType())));
        resultsTable->setItem(i, 2, new QTableWidgetItem(QString::number(indicator->getSeverity())));
        resultsTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(indicator->getDescription())));
        resultsTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(indicator->getOrigin())));
        resultsTable->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(indicator->getTimestamp())));
    }
    
    resultsCountLabel->setText(QString("Found %1 matching IOCs").arg(filteredResults.size()));
}

void SearchDialog::onSearchTypeChanged()
{
    // Clear search results when type changes
    resultsTable->setRowCount(0);
    resultsCountLabel->setText("Search type changed - apply filters to see results");
    detailsText->clear();
}

void SearchDialog::onResultSelectionChanged()
{
    int currentRow = resultsTable->currentRow();
    if (currentRow < 0 || !manager) {
        detailsText->clear();
        return;
    }
    
    // Get IOC ID from the first column
    QTableWidgetItem* idItem = resultsTable->item(currentRow, 0);
    if (!idItem) return;
    
    bool ok;
    int iocId = idItem->text().toInt(&ok);
    if (!ok) return;
    
    // Find and display detailed information
    for (size_t i = 0; i < manager->getIndicatorCount(); ++i) {
        const Indicator* indicator = manager->getIndicator(i);
        if (indicator && indicator->getIndicatorId() == iocId) {
            QString details = QString(
                "◤ IOC DETAILED ANALYSIS ◥\n\n"
                "ID: %1\n"
                "Type: %2\n"
                "Severity: %3/5\n"
                "Description: %4\n"
                "Origin: %5\n"
                "Timestamp: %6\n\n"
                "◤ SECURITY ASSESSMENT ◥\n"
                "Threat Level: %7\n"
                "Risk Category: %8\n"
                "Recommended Action: %9\n"
            ).arg(
                QString::number(indicator->getIndicatorId()),
                QString::fromStdString(indicator->getType()),
                QString::number(indicator->getSeverity()),
                QString::fromStdString(indicator->getDescription()),
                QString::fromStdString(indicator->getOrigin()),
                QString::fromStdString(indicator->getTimestamp()),
                indicator->getSeverity() >= 4 ? "HIGH" : indicator->getSeverity() >= 3 ? "MEDIUM" : "LOW",
                indicator->getSeverity() >= 4 ? "CRITICAL THREAT" : indicator->getSeverity() >= 3 ? "MODERATE RISK" : "LOW RISK",
                indicator->getSeverity() >= 4 ? "IMMEDIATE CONTAINMENT" : indicator->getSeverity() >= 3 ? "MONITOR & ANALYZE" : "ROUTINE MONITORING"
            );
            
            detailsText->setText(details);
            break;
        }
    }
}
