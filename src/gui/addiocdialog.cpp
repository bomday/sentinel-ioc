#include "addiocdialog.h"
#include "../utils/utils.hpp"
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

AddIOCDialog::AddIOCDialog(IndicatorManager *manager, QWidget *parent)
    : QDialog(parent)
    , manager(manager)
    , editingMode(false)
    , editingIOCId(-1)
{
    // Initialize theme colors
    primaryColor = "#00FF41";
    secondaryColor = "#008F11";
    accentColor = "#00FFFF";
    backgroundColor = "#0D1117";
    textColor = "#C9D1D9";
    
    setupUI();
    applyMatrixTheme();
}

void AddIOCDialog::setupUI()
{
    setWindowTitle("◤ ADD NEW IOC TO MATRIX ◥");
    setModal(true);
    setMinimumSize(600, 700);  // Increased minimum size
    resize(650, 750);  // Set a good default size
    
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(25, 25, 25, 25);  // Increased margins
    mainLayout->setSpacing(20);  // Increased spacing
    
    // Title
    QLabel *titleLabel = new QLabel("◤ IOC CREATION MATRIX ◥");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; padding: 15px; color: #00FF41;");
    mainLayout->addWidget(titleLabel);
    
    // Form layout
    QGroupBox *formGroup = new QGroupBox("◤ IOC PROPERTIES ◥");
    formLayout = new QFormLayout(formGroup);
    formLayout->setSpacing(15);  // Increased spacing between rows
    formLayout->setContentsMargins(20, 25, 20, 20);  // Better margins
    formLayout->setLabelAlignment(Qt::AlignLeft);  // Ensure labels are left-aligned
    formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);  // Allow fields to grow
    
    // IOC Type
    typeCombo = new QComboBox();
    typeCombo->addItem("Hash");
    typeCombo->addItem("IP");
    typeCombo->addItem("URL");
    typeCombo->setMinimumHeight(40);  // Increased height
    typeCombo->setToolTip("Select the type of Indicator of Compromise (IOC) you want to add");
    connect(typeCombo, QOverload<const QString &>::of(&QComboBox::currentTextChanged), 
            this, &AddIOCDialog::onTypeChanged);
    formLayout->addRow("◈ IOC Type:", typeCombo);
    
    // Severity
    severitySpinBox = new QSpinBox();
    severitySpinBox->setRange(1, 5);
    severitySpinBox->setValue(3);
    severitySpinBox->setMinimumHeight(40);  // Increased height
    severitySpinBox->setToolTip("Set threat severity: 1=Low, 2=Medium, 3=High, 4=Critical, 5=Emergency");
    severitySpinBox->setSuffix(" - " + QString(severitySpinBox->value() == 1 ? "Low" : 
                                              severitySpinBox->value() == 2 ? "Medium" :
                                              severitySpinBox->value() == 3 ? "High" :
                                              severitySpinBox->value() == 4 ? "Critical" : "Emergency"));
    connect(severitySpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
        QString suffix = QString(" - %1").arg(value == 1 ? "Low" : 
                                             value == 2 ? "Medium" :
                                             value == 3 ? "High" :
                                             value == 4 ? "Critical" : "Emergency");
        severitySpinBox->setSuffix(suffix);
    });
    formLayout->addRow("◈ Threat Severity:", severitySpinBox);
    
    // Description
    descriptionEdit = new QLineEdit();
    descriptionEdit->setPlaceholderText("Enter detailed IOC description...");
    descriptionEdit->setMinimumHeight(40);  // Increased height
    descriptionEdit->setToolTip("Provide a detailed description of this IOC and its potential threats");
    formLayout->addRow("◈ Description:", descriptionEdit);
    
    // Origin
    originEdit = new QLineEdit();
    originEdit->setPlaceholderText("Enter IOC origin/source...");
    originEdit->setMinimumHeight(40);  // Increased height
    originEdit->setToolTip("Specify the source or origin of this IOC (e.g., threat intelligence feed, security team)");
    formLayout->addRow("◈ Origin/Source:", originEdit);
    
    mainLayout->addWidget(formGroup);
    
    // Type-specific fields
    createTypeSpecificWidgets();
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);  // Better spacing between buttons
    
    addButton = new QPushButton("◈ ADD IOC");
    resetButton = new QPushButton("◈ RESET FORM");
    cancelButton = new QPushButton("◈ CANCEL");
    
    // Make buttons bigger and more readable
    addButton->setMinimumHeight(45);
    resetButton->setMinimumHeight(45);
    cancelButton->setMinimumHeight(45);
    
    // Add tooltips for better accessibility
    addButton->setToolTip("Add this IOC to the database");
    resetButton->setToolTip("Clear all form fields and reset to defaults");
    cancelButton->setToolTip("Cancel and close the dialog without saving");
    
    connect(addButton, &QPushButton::clicked, this, &AddIOCDialog::addIOC);
    connect(resetButton, &QPushButton::clicked, this, &AddIOCDialog::resetForm);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(resetButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Initialize with Hash type
    onTypeChanged();
}

void AddIOCDialog::createTypeSpecificWidgets()
{
    typeSpecificStack = new QStackedWidget();
    
    // Hash widget
    hashWidget = new QWidget();
    QFormLayout *hashLayout = new QFormLayout(hashWidget);
    hashLayout->setSpacing(15);
    hashLayout->setContentsMargins(20, 15, 20, 15);
    hashLayout->setLabelAlignment(Qt::AlignLeft);
    hashLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    
    hashEdit = new QLineEdit();
    hashEdit->setPlaceholderText("Enter hash value (e.g., a1b2c3d4e5f6...)");
    hashEdit->setMinimumHeight(40);
    hashLayout->addRow("◈ Hash Value:", hashEdit);
    
    algorithmCombo = new QComboBox();
    algorithmCombo->addItems({"SHA256", "SHA1", "MD5", "SHA512"});
    algorithmCombo->setMinimumHeight(40);
    hashLayout->addRow("◈ Hash Algorithm:", algorithmCombo);
    
    typeSpecificStack->addWidget(hashWidget);
    
    // IP widget
    ipWidget = new QWidget();
    QFormLayout *ipLayout = new QFormLayout(ipWidget);
    ipLayout->setSpacing(15);
    ipLayout->setContentsMargins(20, 15, 20, 15);
    ipLayout->setLabelAlignment(Qt::AlignLeft);
    ipLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    
    ipEdit = new QLineEdit();
    ipEdit->setPlaceholderText("Enter IP address (e.g., 192.168.1.1)");
    ipEdit->setMinimumHeight(40);
    // Add IP validation
    QRegularExpression ipRegex("^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    ipEdit->setValidator(new QRegularExpressionValidator(ipRegex, this));
    ipLayout->addRow("◈ IP Address:", ipEdit);
    
    countryEdit = new QLineEdit();
    countryEdit->setPlaceholderText("Enter country (e.g., United States)");
    countryEdit->setMinimumHeight(40);
    ipLayout->addRow("◈ Country:", countryEdit);
    
    ispEdit = new QLineEdit();
    ispEdit->setPlaceholderText("Enter ISP name (e.g., Example ISP)");
    ispEdit->setMinimumHeight(40);
    ipLayout->addRow("◈ Internet Service Provider:", ispEdit);
    
    typeSpecificStack->addWidget(ipWidget);
    
    // URL widget
    urlWidget = new QWidget();
    QFormLayout *urlLayout = new QFormLayout(urlWidget);
    urlLayout->setSpacing(15);
    urlLayout->setContentsMargins(20, 15, 20, 15);
    urlLayout->setLabelAlignment(Qt::AlignLeft);
    urlLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    
    urlEdit = new QLineEdit();
    urlEdit->setPlaceholderText("Enter URL (e.g., https://malicious-site.com/payload)");
    urlEdit->setMinimumHeight(40);
    urlLayout->addRow("◈ Malicious URL:", urlEdit);
    
    protocolCombo = new QComboBox();
    protocolCombo->addItems({"HTTPS", "HTTP", "FTP", "FTPS"});
    protocolCombo->setMinimumHeight(40);
    urlLayout->addRow("◈ Protocol:", protocolCombo);
    
    typeSpecificStack->addWidget(urlWidget);
    
    // Add stack to main layout
    QGroupBox *typeSpecificGroup = new QGroupBox("◤ TYPE-SPECIFIC PROPERTIES ◥");
    typeSpecificGroup->setMinimumHeight(200);  // Ensure enough height
    QVBoxLayout *stackLayout = new QVBoxLayout(typeSpecificGroup);
    stackLayout->setContentsMargins(10, 25, 10, 15);
    stackLayout->addWidget(typeSpecificStack);
    
    mainLayout->addWidget(typeSpecificGroup);
}

void AddIOCDialog::applyMatrixTheme()
{
    setStyleSheet(QString(
        "QDialog {"
        "    background-color: %1;"
        "    color: %2;"
        "    font-family: 'Segoe UI', Arial, sans-serif;"
        "}"
        "QGroupBox {"
        "    font-weight: bold;"
        "    border: 2px solid %3;"
        "    border-radius: 8px;"
        "    margin-top: 15px;"
        "    padding-top: 10px;"
        "    background-color: rgba(0, 255, 65, 0.05);"
        "    font-size: 13px;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    left: 15px;"
        "    padding: 0 10px 0 10px;"
        "    color: %3;"
        "    font-size: 13px;"
        "    font-weight: bold;"
        "}"
        "QLineEdit {"
        "    background-color: rgba(0, 0, 0, 0.8);"
        "    color: %2;"
        "    border: 2px solid %3;"
        "    border-radius: 6px;"
        "    padding: 10px 12px;"
        "    font-family: 'Courier New', monospace;"
        "    font-size: 12px;"
        "    selection-background-color: %3;"
        "    min-height: 20px;"
        "}"
        "QLineEdit:focus {"
        "    border-color: %5;"
        "    background-color: rgba(0, 255, 255, 0.1);"
        "    color: %5;"
        "}"
        "QLineEdit::placeholder {"
        "    color: rgba(201, 209, 217, 0.6);"
        "    font-style: italic;"
        "}"
        "QComboBox {"
        "    background-color: rgba(0, 0, 0, 0.8);"
        "    color: %2;"
        "    border: 2px solid %3;"
        "    border-radius: 6px;"
        "    padding: 10px 12px;"
        "    font-family: 'Courier New', monospace;"
        "    font-size: 12px;"
        "    min-height: 20px;"
        "}"
        "QComboBox:focus {"
        "    border-color: %5;"
        "    background-color: rgba(0, 255, 255, 0.1);"
        "}"
        "QComboBox:drop-down {"
        "    border: none;"
        "    width: 25px;"
        "}"
        "QComboBox::down-arrow {"
        "    image: none;"
        "    border-left: 6px solid transparent;"
        "    border-right: 6px solid transparent;"
        "    border-top: 7px solid %2;"
        "    margin-right: 5px;"
        "}"
        "QComboBox QAbstractItemView {"
        "    background-color: rgba(0, 0, 0, 0.9);"
        "    color: %2;"
        "    border: 2px solid %3;"
        "    selection-background-color: %3;"
        "    selection-color: %1;"
        "}"
        "QSpinBox {"
        "    background-color: rgba(0, 0, 0, 0.8);"
        "    color: %2;"
        "    border: 2px solid %3;"
        "    border-radius: 6px;"
        "    padding: 10px 12px;"
        "    font-family: 'Courier New', monospace;"
        "    font-size: 12px;"
        "    min-height: 20px;"
        "}"
        "QSpinBox:focus {"
        "    border-color: %5;"
        "    background-color: rgba(0, 255, 255, 0.1);"
        "}"
        "QSpinBox::up-button, QSpinBox::down-button {"
        "    background-color: %3;"
        "    border: none;"
        "    width: 20px;"
        "}"
        "QSpinBox::up-button:hover, QSpinBox::down-button:hover {"
        "    background-color: %5;"
        "}"
        "QSpinBox::up-arrow {"
        "    border-left: 4px solid transparent;"
        "    border-right: 4px solid transparent;"
        "    border-bottom: 5px solid %1;"
        "}"
        "QSpinBox::down-arrow {"
        "    border-left: 4px solid transparent;"
        "    border-right: 4px solid transparent;"
        "    border-top: 5px solid %1;"
        "}"
        "QPushButton {"
        "    background-color: rgba(0, 255, 65, 0.2);"
        "    color: %2;"
        "    border: 2px solid %3;"
        "    border-radius: 8px;"
        "    padding: 12px 25px;"
        "    font-weight: bold;"
        "    font-size: 12px;"
        "    min-width: 120px;"
        "    font-family: 'Segoe UI', Arial, sans-serif;"
        "}"
        "QPushButton:hover {"
        "    background-color: %3;"
        "    color: %1;"
        "    border-color: %5;"
        "}"
        "QPushButton:pressed {"
        "    background-color: %5;"
        "    color: %1;"
        "}"
        "QLabel {"
        "    color: %2;"
        "    font-family: 'Segoe UI', Arial, sans-serif;"
        "    font-weight: bold;"
        "    font-size: 12px;"
        "    min-width: 150px;"  /* Ensure labels have minimum width */
        "    padding: 2px;"
        "}"
        "QFormLayout QLabel {"
        "    min-width: 160px;"  /* Specific minimum width for form labels */
        "    text-align: left;"
        "    padding-right: 10px;"
        "}"
    ).arg(backgroundColor, textColor, primaryColor, secondaryColor, accentColor));
}

void AddIOCDialog::onTypeChanged()
{
    QString type = typeCombo->currentText();
    showTypeSpecificFields(type);
}

void AddIOCDialog::showTypeSpecificFields(const QString &type)
{
    if (type == "Hash") {
        typeSpecificStack->setCurrentWidget(hashWidget);
    } else if (type == "IP") {
        typeSpecificStack->setCurrentWidget(ipWidget);
    } else if (type == "URL") {
        typeSpecificStack->setCurrentWidget(urlWidget);
    }
}

bool AddIOCDialog::validateInput()
{
    // Validate common fields
    if (descriptionEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Description cannot be empty!");
        return false;
    }
    
    if (originEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Origin cannot be empty!");
        return false;
    }
    
    // Validate type-specific fields
    QString type = typeCombo->currentText();
    
    if (type == "Hash") {
        if (hashEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Validation Error", "Hash value cannot be empty!");
            return false;
        }
        // Validate hash length based on algorithm
        QString algorithm = algorithmCombo->currentText();
        QString hash = hashEdit->text().trimmed();
        
        if (algorithm == "MD5" && hash.length() != 32) {
            QMessageBox::warning(this, "Validation Error", "MD5 hash must be 32 characters long!");
            return false;
        } else if (algorithm == "SHA1" && hash.length() != 40) {
            QMessageBox::warning(this, "Validation Error", "SHA1 hash must be 40 characters long!");
            return false;
        } else if (algorithm == "SHA256" && hash.length() != 64) {
            QMessageBox::warning(this, "Validation Error", "SHA256 hash must be 64 characters long!");
            return false;
        }
    } else if (type == "IP") {
        if (ipEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Validation Error", "IP address cannot be empty!");
            return false;
        }
        if (countryEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Validation Error", "Country cannot be empty!");
            return false;
        }
        if (ispEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Validation Error", "ISP cannot be empty!");
            return false;
        }
    } else if (type == "URL") {
        if (urlEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Validation Error", "URL cannot be empty!");
            return false;
        }
        // Basic URL validation
        QString url = urlEdit->text().trimmed();
        if (!url.startsWith("http://") && !url.startsWith("https://") && !url.startsWith("ftp://")) {
            QMessageBox::warning(this, "Validation Error", "URL must start with http://, https://, or ftp://!");
            return false;
        }
    }
    
    return true;
}

void AddIOCDialog::addIOC()
{
    if (!validateInput()) {
        return;
    }
    
    std::string type = typeCombo->currentText().toStdString();
    int severity = severitySpinBox->value();
    std::string description = descriptionEdit->text().trimmed().toStdString();
    std::string origin = originEdit->text().trimmed().toStdString();
    
    QString logMessage;
    QString severityText;
    
    // Convert severity to text
    switch (severity) {
        case 1: severityText = "LOW"; break;
        case 2: severityText = "MEDIUM"; break;
        case 3: severityText = "HIGH"; break;
        case 4: severityText = "CRITICAL"; break;
        case 5: severityText = "EXTREME"; break;
        default: severityText = "UNKNOWN"; break;
    }
    
    try {
        if (editingMode) {
            // Edit existing IOC
            bool success = false;
            
            // Update common fields
            success = manager->updateIndicatorSeverity(editingIOCId, severity) &&
                     manager->updateIndicatorDescription(editingIOCId, description) &&
                     manager->updateIndicatorOrigin(editingIOCId, origin);
            
            // Update type-specific fields
            if (success && type == "Hash") {
                QString hashValue = hashEdit->text().trimmed();
                QString algorithm = algorithmCombo->currentText();
                
                success = manager->updateMaliciousHash(editingIOCId, hashValue.toStdString()) &&
                         manager->updateMaliciousHashAlgorithm(editingIOCId, algorithm.toStdString());
                
                if (success) {
                    logMessage = QString("[IOC-EDIT] ✓ Hash IOC updated | ID:%1 | Type:%2 | Severity:%3 | Algorithm:%4 | Hash:%5... | Origin:%6")
                                .arg(editingIOCId)
                                .arg(QString::fromStdString(type))
                                .arg(severityText)
                                .arg(algorithm)
                                .arg(hashValue.left(16))
                                .arg(QString::fromStdString(origin));
                }
                        
            } else if (success && type == "IP") {
                QString ipAddress = ipEdit->text().trimmed();
                QString country = countryEdit->text().trimmed();
                QString isp = ispEdit->text().trimmed();
                
                success = manager->updateMaliciousIPAddress(editingIOCId, ipAddress.toStdString()) &&
                         manager->updateMaliciousIPCountry(editingIOCId, country.toStdString()) &&
                         manager->updateMaliciousIPISP(editingIOCId, isp.toStdString());
                
                if (success) {
                    logMessage = QString("[IOC-EDIT] ✓ IP IOC updated | ID:%1 | Type:%2 | Severity:%3 | IP:%4 | Country:%5 | ISP:%6 | Origin:%7")
                                .arg(editingIOCId)
                                .arg(QString::fromStdString(type))
                                .arg(severityText)
                                .arg(ipAddress)
                                .arg(country)
                                .arg(isp)
                                .arg(QString::fromStdString(origin));
                }
                        
            } else if (success && type == "URL") {
                QString url = urlEdit->text().trimmed();
                QString protocol = protocolCombo->currentText();
                
                success = manager->updateMaliciousURL(editingIOCId, url.toStdString()) &&
                         manager->updateMaliciousURLProtocol(editingIOCId, protocol.toStdString());
                
                if (success) {
                    logMessage = QString("[IOC-EDIT] ✓ URL IOC updated | ID:%1 | Type:%2 | Severity:%3 | Protocol:%4 | URL:%5 | Origin:%6")
                                .arg(editingIOCId)
                                .arg(QString::fromStdString(type))
                                .arg(severityText)
                                .arg(protocol)
                                .arg(url.length() > 50 ? url.left(47) + "..." : url)
                                .arg(QString::fromStdString(origin));
                }
            }
            
            if (success) {
                emit iocAdded(logMessage);
                QMessageBox::information(this, "Success", 
                    QString("◤ IOC UPDATED IN MATRIX ◥\n\nID: %1\nType: %2\nSeverity: %3\n\nIOC has been successfully updated!")
                    .arg(editingIOCId).arg(QString::fromStdString(type)).arg(severityText));
                accept();
            } else {
                QString errorMsg = QString("[IOC-EDIT] ✗ Failed to update IOC | ID:%1 | Type:%2")
                                  .arg(editingIOCId)
                                  .arg(QString::fromStdString(type));
                emit iocAdded(errorMsg);
                QMessageBox::critical(this, "Error", "Failed to update IOC. Please try again.");
            }
            
        } else {
            // Add new IOC
            int id = generateUniqueId();
            std::string timestamp = getTimestamp();
            
            if (type == "Hash") {
                QString hashValue = hashEdit->text().trimmed();
                QString algorithm = algorithmCombo->currentText();
                
                manager->addMaliciousHash(id, severity, type, description, origin, timestamp,
                                        hashValue.toStdString(), algorithm.toStdString());
                
                logMessage = QString("[IOC-ADD] ✓ Hash IOC created | ID:%1 | Type:%2 | Severity:%3 | Algorithm:%4 | Hash:%5... | Origin:%6")
                            .arg(id)
                            .arg(QString::fromStdString(type))
                            .arg(severityText)
                            .arg(algorithm)
                            .arg(hashValue.left(16))
                            .arg(QString::fromStdString(origin));
                            
            } else if (type == "IP") {
                QString ipAddress = ipEdit->text().trimmed();
                QString country = countryEdit->text().trimmed();
                QString isp = ispEdit->text().trimmed();
                
                manager->addMaliciousIP(id, severity, type, description, origin, timestamp,
                                      ipAddress.toStdString(), country.toStdString(), isp.toStdString());
                
                logMessage = QString("[IOC-ADD] ✓ IP IOC created | ID:%1 | Type:%2 | Severity:%3 | IP:%4 | Country:%5 | ISP:%6 | Origin:%7")
                            .arg(id)
                            .arg(QString::fromStdString(type))
                            .arg(severityText)
                            .arg(ipAddress)
                            .arg(country)
                            .arg(isp)
                            .arg(QString::fromStdString(origin));
                            
            } else if (type == "URL") {
                QString url = urlEdit->text().trimmed();
                QString protocol = protocolCombo->currentText();
                
                manager->addMaliciousURL(id, severity, type, description, origin, timestamp,
                                       url.toStdString(), protocol.toStdString());
                
                logMessage = QString("[IOC-ADD] ✓ URL IOC created | ID:%1 | Type:%2 | Severity:%3 | Protocol:%4 | URL:%5 | Origin:%6")
                            .arg(id)
                            .arg(QString::fromStdString(type))
                            .arg(severityText)
                            .arg(protocol)
                            .arg(url.length() > 50 ? url.left(47) + "..." : url)
                            .arg(QString::fromStdString(origin));
            }
            
            emit iocAdded(logMessage);
            QMessageBox::information(this, "Success", 
                QString("◤ IOC ADDED TO MATRIX ◥\n\nID: %1\nType: %2\nSeverity: %3\n\nIOC has been successfully added to the database!")
                .arg(id).arg(QString::fromStdString(type)).arg(severityText));
            accept();
        }
        
    } catch (const std::exception& e) {
        QString errorMsg = QString("[IOC-%1] ✗ Failed to %2 IOC | Type:%3 | Error:%4")
                          .arg(editingMode ? "EDIT" : "ADD")
                          .arg(editingMode ? "update" : "create")
                          .arg(QString::fromStdString(type))
                          .arg(e.what());
        emit iocAdded(errorMsg);
        QMessageBox::critical(this, "Error", QString("Failed to %1 IOC: %2").arg(editingMode ? "update" : "add").arg(e.what()));
    }
}

void AddIOCDialog::resetForm()
{
    // Reset edit mode
    editingMode = false;
    editingIOCId = -1;
    setWindowTitle("◤ ADD NEW IOC TO MATRIX ◥");
    addButton->setText("◤ ADD IOC ◥");
    
    // Reset common fields
    typeCombo->setCurrentIndex(0);
    severitySpinBox->setValue(3);
    descriptionEdit->clear();
    originEdit->clear();
    
    // Reset type-specific fields
    hashEdit->clear();
    algorithmCombo->setCurrentIndex(0);
    
    ipEdit->clear();
    countryEdit->clear();
    ispEdit->clear();
    
    urlEdit->clear();
    protocolCombo->setCurrentIndex(0);
    
    // Reset to Hash type
    onTypeChanged();
}

void AddIOCDialog::setEditMode(bool editMode, const Indicator* ioc)
{
    editingMode = editMode;
    
    if (editMode && ioc) {
        editingIOCId = ioc->getIndicatorId();
        
        // Update window title and button text
        setWindowTitle("◤ EDIT IOC IN MATRIX ◥");
        addButton->setText("◤ UPDATE IOC ◥");
        
        // Pre-populate common fields
        QString type = QString::fromStdString(ioc->getType());
        int typeIndex = typeCombo->findText(type);
        if (typeIndex >= 0) {
            typeCombo->setCurrentIndex(typeIndex);
        }
        
        severitySpinBox->setValue(ioc->getSeverity());
        descriptionEdit->setText(QString::fromStdString(ioc->getDescription()));
        originEdit->setText(QString::fromStdString(ioc->getOrigin()));
        
        // Trigger type change to show appropriate fields
        onTypeChanged();
        
        // Pre-populate type-specific fields
        if (type == "Hash") {
            if (const MaliciousHash* hashIOC = dynamic_cast<const MaliciousHash*>(ioc)) {
                hashEdit->setText(QString::fromStdString(hashIOC->getHash()));
                int algIndex = algorithmCombo->findText(QString::fromStdString(hashIOC->getAlgorithm()));
                if (algIndex >= 0) {
                    algorithmCombo->setCurrentIndex(algIndex);
                }
            }
        } else if (type == "IP") {
            if (const MaliciousIP* ipIOC = dynamic_cast<const MaliciousIP*>(ioc)) {
                ipEdit->setText(QString::fromStdString(ipIOC->getIP()));
                countryEdit->setText(QString::fromStdString(ipIOC->getCountry()));
                ispEdit->setText(QString::fromStdString(ipIOC->getISP()));
            }
        } else if (type == "URL") {
            if (const MaliciousURL* urlIOC = dynamic_cast<const MaliciousURL*>(ioc)) {
                urlEdit->setText(QString::fromStdString(urlIOC->getURL()));
                int protIndex = protocolCombo->findText(QString::fromStdString(urlIOC->getProtocol()));
                if (protIndex >= 0) {
                    protocolCombo->setCurrentIndex(protIndex);
                }
            }
        }
    } else {
        // Reset to add mode
        editingIOCId = -1;
        setWindowTitle("◤ ADD NEW IOC TO MATRIX ◥");
        addButton->setText("◤ ADD IOC ◥");
        resetForm();
    }
}
