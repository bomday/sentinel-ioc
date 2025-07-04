#include "addiocdialog.h"
#include "../utils/utils.hpp"
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

AddIOCDialog::AddIOCDialog(IndicatorManager *manager, QWidget *parent)
    : QDialog(parent)
    , manager(manager)
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
    setMinimumSize(500, 600);
    
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);
    
    // Title
    QLabel *titleLabel = new QLabel("◤ IOC CREATION MATRIX ◥");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; padding: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Form layout
    QGroupBox *formGroup = new QGroupBox("◤ IOC PROPERTIES ◥");
    formLayout = new QFormLayout(formGroup);
    formLayout->setSpacing(10);
    
    // IOC Type
    typeCombo = new QComboBox();
    typeCombo->addItem("Hash");
    typeCombo->addItem("IP");
    typeCombo->addItem("URL");
    connect(typeCombo, QOverload<const QString &>::of(&QComboBox::currentTextChanged), 
            this, &AddIOCDialog::onTypeChanged);
    formLayout->addRow("◈ Type:", typeCombo);
    
    // Severity
    severitySpinBox = new QSpinBox();
    severitySpinBox->setRange(1, 5);
    severitySpinBox->setValue(3);
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
    formLayout->addRow("◈ Severity:", severitySpinBox);
    
    // Description
    descriptionEdit = new QLineEdit();
    descriptionEdit->setPlaceholderText("Enter IOC description...");
    formLayout->addRow("◈ Description:", descriptionEdit);
    
    // Origin
    originEdit = new QLineEdit();
    originEdit->setPlaceholderText("Enter IOC origin/source...");
    formLayout->addRow("◈ Origin:", originEdit);
    
    mainLayout->addWidget(formGroup);
    
    // Type-specific fields
    createTypeSpecificWidgets();
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    addButton = new QPushButton("◈ ADD IOC");
    resetButton = new QPushButton("◈ RESET");
    cancelButton = new QPushButton("◈ CANCEL");
    
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
    
    hashEdit = new QLineEdit();
    hashEdit->setPlaceholderText("Enter hash value...");
    hashLayout->addRow("◈ Hash:", hashEdit);
    
    algorithmCombo = new QComboBox();
    algorithmCombo->addItems({"SHA256", "SHA1", "MD5", "SHA512"});
    hashLayout->addRow("◈ Algorithm:", algorithmCombo);
    
    typeSpecificStack->addWidget(hashWidget);
    
    // IP widget
    ipWidget = new QWidget();
    QFormLayout *ipLayout = new QFormLayout(ipWidget);
    
    ipEdit = new QLineEdit();
    ipEdit->setPlaceholderText("xxx.xxx.xxx.xxx");
    // Add IP validation
    QRegularExpression ipRegex("^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    ipEdit->setValidator(new QRegularExpressionValidator(ipRegex, this));
    ipLayout->addRow("◈ IP Address:", ipEdit);
    
    countryEdit = new QLineEdit();
    countryEdit->setPlaceholderText("Country name...");
    ipLayout->addRow("◈ Country:", countryEdit);
    
    ispEdit = new QLineEdit();
    ispEdit->setPlaceholderText("ISP name...");
    ipLayout->addRow("◈ ISP:", ispEdit);
    
    typeSpecificStack->addWidget(ipWidget);
    
    // URL widget
    urlWidget = new QWidget();
    QFormLayout *urlLayout = new QFormLayout(urlWidget);
    
    urlEdit = new QLineEdit();
    urlEdit->setPlaceholderText("https://example.com/malicious");
    urlLayout->addRow("◈ URL:", urlEdit);
    
    protocolCombo = new QComboBox();
    protocolCombo->addItems({"HTTPS", "HTTP", "FTP", "FTPS"});
    urlLayout->addRow("◈ Protocol:", protocolCombo);
    
    typeSpecificStack->addWidget(urlWidget);
    
    // Add stack to main layout
    QGroupBox *typeSpecificGroup = new QGroupBox("◤ TYPE-SPECIFIC PROPERTIES ◥");
    QVBoxLayout *stackLayout = new QVBoxLayout(typeSpecificGroup);
    stackLayout->addWidget(typeSpecificStack);
    
    mainLayout->addWidget(typeSpecificGroup);
}

void AddIOCDialog::applyMatrixTheme()
{
    setStyleSheet(QString(
        "QDialog {"
        "    background-color: %1;"
        "    color: %2;"
        "}"
        "QGroupBox {"
        "    font-weight: bold;"
        "    border: 2px solid %3;"
        "    border-radius: 6px;"
        "    margin-top: 10px;"
        "    padding-top: 10px;"
        "    background-color: %4;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    left: 10px;"
        "    padding: 0 8px 0 8px;"
        "    color: %3;"
        "    font-size: 12px;"
        "}"
        "QLineEdit {"
        "    background-color: %4;"
        "    color: %2;"
        "    border: 2px solid %3;"
        "    border-radius: 4px;"
        "    padding: 8px;"
        "    font-family: 'Courier New', monospace;"
        "    selection-background-color: %3;"
        "}"
        "QLineEdit:focus {"
        "    border-color: %5;"
        "    background-color: %1;"
        "}"
        "QComboBox {"
        "    background-color: %4;"
        "    color: %2;"
        "    border: 2px solid %3;"
        "    border-radius: 4px;"
        "    padding: 8px;"
        "    font-family: 'Courier New', monospace;"
        "}"
        "QComboBox:drop-down {"
        "    border: none;"
        "    width: 20px;"
        "}"
        "QComboBox::down-arrow {"
        "    image: none;"
        "    border-left: 5px solid transparent;"
        "    border-right: 5px solid transparent;"
        "    border-top: 6px solid %2;"
        "}"
        "QSpinBox {"
        "    background-color: %4;"
        "    color: %2;"
        "    border: 2px solid %3;"
        "    border-radius: 4px;"
        "    padding: 8px;"
        "    font-family: 'Courier New', monospace;"
        "}"
        "QPushButton {"
        "    background-color: %4;"
        "    color: %2;"
        "    border: 2px solid %3;"
        "    border-radius: 6px;"
        "    padding: 10px 20px;"
        "    font-weight: bold;"
        "    font-size: 11px;"
        "    min-width: 100px;"
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
        "    font-family: 'Courier New', monospace;"
        "    font-weight: bold;"
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
    
    // Generate unique ID and timestamp
    int id = generateUniqueId();
    std::string timestamp = getTimestamp();
    
    std::string type = typeCombo->currentText().toStdString();
    int severity = severitySpinBox->value();
    std::string description = descriptionEdit->text().trimmed().toStdString();
    std::string origin = originEdit->text().trimmed().toStdString();
    
    try {
        if (type == "Hash") {
            manager->addMaliciousHash(id, severity, type, description, origin, timestamp,
                                    hashEdit->text().trimmed().toStdString(),
                                    algorithmCombo->currentText().toStdString());
        } else if (type == "IP") {
            manager->addMaliciousIP(id, severity, type, description, origin, timestamp,
                                  ipEdit->text().trimmed().toStdString(),
                                  countryEdit->text().trimmed().toStdString(),
                                  ispEdit->text().trimmed().toStdString());
        } else if (type == "URL") {
            manager->addMaliciousURL(id, severity, type, description, origin, timestamp,
                                   urlEdit->text().trimmed().toStdString(),
                                   protocolCombo->currentText().toStdString());
        }
        
        QMessageBox::information(this, "Success", 
            QString("◤ IOC ADDED TO MATRIX ◥\\n\\nID: %1\\nType: %2\\nSeverity: %3\\n\\nIOC has been successfully added to the database!")
            .arg(id).arg(type).arg(severity));
        
        accept();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to add IOC: %1").arg(e.what()));
    }
}

void AddIOCDialog::resetForm()
{
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
