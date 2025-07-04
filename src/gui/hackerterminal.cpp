#include "hackerterminal.h"
#include "../indicatorManager/indicatorManager.hpp"
#include "../maliciousHash/maliciousHash.hpp"
#include "../maliciousIP/maliciousIP.hpp"
#include "../maliciousURL/maliciousURL.hpp"
#include <QPainter>
#include <QKeyEvent>
#include <QScrollBar>
#include <QRandomGenerator>
#include <QDateTime>
#include <QFont>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QThread>

HackerTerminal::HackerTerminal(QWidget *parent)
    : QWidget(parent)
    , m_glowIntensity(0.5)
    , blinkState(false)
    , commandHistory(0)
    , matrixModeActive(false)
    , iocManager(nullptr)
{
    // Initialize colors
    primaryColor = "#00FF41";       // Matrix green
    secondaryColor = "#008F11";     // Darker green
    backgroundColor = "#0A0A0A";    // Very dark background
    errorColor = "#FF0040";         // Red
    warningColor = "#FFD700";       // Yellow
    successColor = "#00FFFF";       // Cyan
    
    // Initialize command lists
    hackingCommands = {
        "nmap -sS -O target.sys",
        "metasploit exploit/multi/handler",
        "john --wordlist=rockyou.txt hash.txt",
        "hydra -l admin -P passwords.txt ssh://target",
        "sqlmap -u 'http://target/page?id=1' --dbs",
        "nikto -h http://target.com",
        "aircrack-ng -w wordlist.txt capture.cap",
        "hashcat -m 0 hash.txt rockyou.txt",
        "ncat -lvp 4444",
        "msfvenom -p windows/meterpreter/reverse_tcp"
    };
    
    systemResponses = {
        "[SYSTEM] Establishing secure connection...",
        "[SYSTEM] Bypassing firewall protocols...",
        "[SYSTEM] Decrypting data stream...",
        "[SYSTEM] Analyzing threat vectors...",
        "[SYSTEM] Deploying countermeasures...",
        "[SYSTEM] Scanning for vulnerabilities...",
        "[SYSTEM] Initializing neural network...",
        "[SYSTEM] Activating defense grid...",
        "[SYSTEM] Monitoring network traffic...",
        "[SYSTEM] Running deep packet inspection..."
    };
    
    setupUI();
    applyHackerTheme();
    
    // Setup timers
    promptTimer = new QTimer(this);
    connect(promptTimer, &QTimer::timeout, this, &HackerTerminal::updatePrompt);
    promptTimer->start(1000); // Blink every second
    
    networkTimer = new QTimer(this);
    connect(networkTimer, &QTimer::timeout, this, &HackerTerminal::simulateNetworkActivity);
    networkTimer->start(3000); // Network activity every 3 seconds
    
    // Setup glow animation
    glowAnimation = new QPropertyAnimation(this, "glowIntensity");
    glowAnimation->setDuration(2000);
    glowAnimation->setStartValue(0.3);
    glowAnimation->setEndValue(1.0);
    glowAnimation->setLoopCount(-1);
    glowAnimation->start();
    
    // Add initial welcome message
    addSystemMessage("SENTINEL IOC SECURITY TERMINAL INITIALIZED");
    addSystemMessage("MATRIX PROTOCOL: ACTIVE");
    addSystemMessage("SECURITY LEVEL: MAXIMUM");
    addSystemMessage("TYPE 'help' FOR AVAILABLE COMMANDS");
}

void HackerTerminal::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(5);
    
    // Status label
    statusLabel = new QLabel("◤ SECURE TERMINAL - STATUS: OPERATIONAL ◥");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet(QString(
        "color: %1; background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 transparent, stop:0.5 %2, stop:1 transparent); "
        "padding: 5px; font-weight: bold; border: 1px solid %1;"
    ).arg(primaryColor, backgroundColor));
    mainLayout->addWidget(statusLabel);
    
    // Terminal output
    terminalOutput = new QTextEdit();
    terminalOutput->setReadOnly(true);
    terminalOutput->setFont(QFont("Consolas", 10, QFont::Bold));
    mainLayout->addWidget(terminalOutput);
    
    // Command input section
    inputLayout = new QHBoxLayout();
    
    promptLabel = new QLabel("root@sentinel:~#");
    promptLabel->setFont(QFont("Consolas", 10, QFont::Bold));
    
    commandInput = new QLineEdit();
    commandInput->setFont(QFont("Consolas", 10, QFont::Bold));
    commandInput->installEventFilter(this);
    connect(commandInput, &QLineEdit::returnPressed, this, &HackerTerminal::onCommandEntered);
    
    inputLayout->addWidget(promptLabel);
    inputLayout->addWidget(commandInput);
    
    mainLayout->addLayout(inputLayout);
    
    // Setup glow effect
    glowEffect = new QGraphicsDropShadowEffect();
    glowEffect->setBlurRadius(20);
    glowEffect->setColor(QColor(primaryColor));
    glowEffect->setOffset(0, 0);
    terminalOutput->setGraphicsEffect(glowEffect);
}

void HackerTerminal::applyHackerTheme()
{
    QString terminalStyle = QString(
        "QTextEdit {"
        "    background-color: %1;"
        "    color: %2;"
        "    border: 2px solid %2;"
        "    border-radius: 5px;"
        "    padding: 10px;"
        "    selection-background-color: %3;"
        "}"
        "QTextEdit::scrollbar:vertical {"
        "    background-color: %1;"
        "    width: 12px;"
        "    border: 1px solid %2;"
        "}"
        "QTextEdit::scrollbar::handle:vertical {"
        "    background-color: %2;"
        "    border-radius: 6px;"
        "}"
    ).arg(backgroundColor, primaryColor, secondaryColor);
    
    QString inputStyle = QString(
        "QLineEdit {"
        "    background-color: %1;"
        "    color: %2;"
        "    border: 2px solid %2;"
        "    border-radius: 3px;"
        "    padding: 5px;"
        "    font-family: 'Consolas';"
        "}"
        "QLineEdit:focus {"
        "    border-color: %3;"
        "    background-color: %4;"
        "}"
    ).arg(backgroundColor, primaryColor, successColor, "#001100");
    
    QString labelStyle = QString(
        "QLabel {"
        "    color: %1;"
        "    font-family: 'Consolas';"
        "    font-weight: bold;"
        "}"
    ).arg(primaryColor);
    
    terminalOutput->setStyleSheet(terminalStyle);
    commandInput->setStyleSheet(inputStyle);
    promptLabel->setStyleSheet(labelStyle);
    
    setStyleSheet(QString("HackerTerminal { background-color: %1; }").arg(backgroundColor));
}

void HackerTerminal::addSystemMessage(const QString &message)
{
    QString timestamp = getCurrentTimestamp();
    QString fullMessage = QString("[%1] %2").arg(timestamp, message);
    
    QTextCursor cursor = terminalOutput->textCursor();
    cursor.movePosition(QTextCursor::End);
    
    QTextCharFormat format;
    format.setForeground(QColor(primaryColor));
    cursor.setCharFormat(format);
    cursor.insertText(fullMessage + "\\n");
    
    terminalOutput->setTextCursor(cursor);
    terminalOutput->ensureCursorVisible();
}

void HackerTerminal::addSecurityAlert(const QString &alert)
{
    QString timestamp = getCurrentTimestamp();
    QString fullMessage = QString("[%1] [SECURITY ALERT] %2").arg(timestamp, alert);
    
    QTextCursor cursor = terminalOutput->textCursor();
    cursor.movePosition(QTextCursor::End);
    
    QTextCharFormat format;
    format.setForeground(QColor(errorColor));
    format.setFontWeight(QFont::Bold);
    cursor.setCharFormat(format);
    cursor.insertText(fullMessage + "\\n");
    
    terminalOutput->setTextCursor(cursor);
    terminalOutput->ensureCursorVisible();
}

void HackerTerminal::addThreatDetection(const QString &threat)
{
    QString timestamp = getCurrentTimestamp();
    QString fullMessage = QString("[%1] [THREAT DETECTED] %2").arg(timestamp, threat);
    
    QTextCursor cursor = terminalOutput->textCursor();
    cursor.movePosition(QTextCursor::End);
    
    QTextCharFormat format;
    format.setForeground(QColor(warningColor));
    format.setFontWeight(QFont::Bold);
    cursor.setCharFormat(format);
    cursor.insertText(fullMessage + "\\n");
    
    terminalOutput->setTextCursor(cursor);
    terminalOutput->ensureCursorVisible();
}

void HackerTerminal::simulateHackingActivity()
{
    matrixModeActive = true;
    
    // Simulate various hacking commands
    QStringList activities = {
        "Scanning network topology...",
        "Analyzing packet headers...",
        "Detecting anomalous behavior...",
        "Correlating threat intelligence...",
        "Updating IOC database...",
        "Running behavioral analysis...",
        "Monitoring system integrity...",
        "Validating security protocols..."
    };
    
    for (const QString &activity : activities) {
        addSystemMessage(activity);
        QThread::msleep(200);
    }
    
    // Add some fake command execution
    QString command = hackingCommands.at(QRandomGenerator::global()->bounded(hackingCommands.size()));
    executeCommand(command);
}

void HackerTerminal::startMatrixMode()
{
    matrixModeActive = !matrixModeActive;
    
    if (matrixModeActive) {
        addSystemMessage("MATRIX MODE ACTIVATED");
        addSystemMessage("NEURAL LINK ESTABLISHED");
        addSystemMessage("ENTERING THE GRID...");
        
        // Speed up network activity
        networkTimer->setInterval(1000);
        
        // Start more intense glow
        glowAnimation->setStartValue(0.8);
        glowAnimation->setEndValue(1.0);
    } else {
        addSystemMessage("MATRIX MODE DEACTIVATED");
        addSystemMessage("RETURNING TO NORMAL OPERATIONS");
        
        // Return to normal speed
        networkTimer->setInterval(3000);
        
        // Normal glow
        glowAnimation->setStartValue(0.3);
        glowAnimation->setEndValue(0.7);
    }
}

void HackerTerminal::executeCommand(const QString &command)
{
    // Display the command
    QTextCursor cursor = terminalOutput->textCursor();
    cursor.movePosition(QTextCursor::End);
    
    QTextCharFormat promptFormat;
    promptFormat.setForeground(QColor(successColor));
    cursor.setCharFormat(promptFormat);
    cursor.insertText(QString("root@sentinel:~# %1\\n").arg(command));
    
    // Process and respond
    processCommand(command);
    
    terminalOutput->setTextCursor(cursor);
    terminalOutput->ensureCursorVisible();
}

void HackerTerminal::processCommand(const QString &command)
{
    QString cmd = command.trimmed().toLower();
    QStringList parts = command.trimmed().split(' ', Qt::SkipEmptyParts);
    QString mainCmd = parts.isEmpty() ? "" : parts[0].toLower();
    
    if (mainCmd == "help") {
        showIOCCommands();
    } else if (mainCmd == "stats") {
        displayIOCStats();
    } else if (mainCmd == "search") {
        if (parts.size() > 1) {
            QString query = parts.mid(1).join(' ');
            searchIOC(query);
        } else {
            addSecurityAlert("Usage: search <query>");
        }
    } else if (mainCmd == "matrix") {
        startMatrixMode();
    } else if (mainCmd == "scan") {
        addSystemMessage("Initiating comprehensive security scan...");
        addSystemMessage("Scanning IOC database for threats...");
        if (iocManager) {
            int total = iocManager->getIndicatorCount();
            int highSeverity = 0;
            for (int i = 0; i < total; ++i) {
                const Indicator* ioc = iocManager->getIndicator(i);
                if (ioc && ioc->getSeverity() >= 4) {
                    highSeverity++;
                }
            }
            addSystemMessage(QString("Analyzed %1 indicators").arg(total));
            addSystemMessage(QString("High severity threats: %1").arg(highSeverity));
            addSystemMessage("Security scan complete");
        } else {
            addSecurityAlert("IOC database not available for scan");
        }
    } else if (mainCmd == "status") {
        addSystemMessage("System Status: OPERATIONAL");
        addSystemMessage("Security Level: MAXIMUM");
        addSystemMessage("Matrix Protocol: ACTIVE");
        if (iocManager) {
            addSystemMessage(QString("IOC Database: CONNECTED (%1 indicators)").arg(iocManager->getIndicatorCount()));
        } else {
            addSystemMessage("IOC Database: DISCONNECTED");
        }
    } else if (mainCmd == "threats") {
        if (iocManager) {
            int total = iocManager->getIndicatorCount();
            int displayed = 0;
            for (int i = 0; i < total && displayed < 5; ++i) {
                const Indicator* ioc = iocManager->getIndicator(i);
                if (ioc && ioc->getSeverity() >= 3) {
                    QString typeStr = "UNKNOWN";
                    if (dynamic_cast<const MaliciousHash*>(ioc)) typeStr = "HASH";
                    else if (dynamic_cast<const MaliciousIP*>(ioc)) typeStr = "IP";
                    else if (dynamic_cast<const MaliciousURL*>(ioc)) typeStr = "URL";
                    
                    addThreatDetection(QString("[%1] %2").arg(typeStr, QString::fromStdString(ioc->getValue())));
                    displayed++;
                }
            }
            if (displayed == 0) {
                addSystemMessage("No high-severity threats detected");
            }
        } else {
            addThreatDetection("Sample threat data not available");
        }
    } else if (mainCmd == "clear") {
        clearTerminal();
    } else if (mainCmd == "exit") {
        addSystemMessage("Terminal session ended");
    } else if (cmd.startsWith("nmap") || cmd.startsWith("metasploit") || 
               cmd.startsWith("hydra") || cmd.startsWith("john")) {
        addSystemMessage("Executing security tool...");
        addSystemMessage("Scan complete. Results logged.");
    } else if (!cmd.isEmpty()) {
        addSystemMessage(QString("Command not found: %1").arg(command));
        addSystemMessage("Type 'help' for available commands");
    }
}

void HackerTerminal::clearTerminal()
{
    terminalOutput->clear();
    addSystemMessage("Terminal cleared");
}

void HackerTerminal::onCommandEntered()
{
    QString command = commandInput->text();
    if (!command.isEmpty()) {
        executeCommand(command);
        commandInput->clear();
        commandHistory++;
        emit commandEntered(command);
    }
}

void HackerTerminal::updatePrompt()
{
    blinkState = !blinkState;
    QString promptText = blinkState ? "root@sentinel:~#" : "root@sentinel:~# _";
    promptLabel->setText(promptText);
}

void HackerTerminal::updateGlow()
{
    if (glowEffect) {
        QColor glowColor = QColor(primaryColor);
        glowColor.setAlphaF(m_glowIntensity);
        glowEffect->setColor(glowColor);
        glowEffect->setBlurRadius(20 * m_glowIntensity);
    }
}

void HackerTerminal::simulateNetworkActivity()
{
    if (QRandomGenerator::global()->bounded(100) < 30) { // 30% chance
        QString response = systemResponses.at(QRandomGenerator::global()->bounded(systemResponses.size()));
        addSystemMessage(response);
    }
}

QString HackerTerminal::getCurrentTimestamp()
{
    return QDateTime::currentDateTime().toString("hh:mm:ss");
}

bool HackerTerminal::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == commandInput && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        
        // Handle special keys for terminal-like behavior
        if (keyEvent->key() == Qt::Key_Tab) {
            // Auto-complete (simplified)
            QString text = commandInput->text();
            if (text.startsWith("h")) {
                commandInput->setText("help");
            } else if (text.startsWith("m")) {
                commandInput->setText("matrix");
            } else if (text.startsWith("s")) {
                commandInput->setText("scan");
            }
            return true;
        }
    }
    
    return QWidget::eventFilter(obj, event);
}

void HackerTerminal::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    
    // Draw additional cyber effects
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw corner brackets for cyber aesthetic
    QColor accentColor(primaryColor);
    accentColor.setAlpha(100);
    painter.setPen(QPen(accentColor, 2));
    
    int bracketSize = 20;
    QRect rect = this->rect().adjusted(5, 5, -5, -5);
    
    // Top-left bracket
    painter.drawLine(rect.topLeft(), rect.topLeft() + QPoint(bracketSize, 0));
    painter.drawLine(rect.topLeft(), rect.topLeft() + QPoint(0, bracketSize));
    
    // Top-right bracket
    painter.drawLine(rect.topRight(), rect.topRight() + QPoint(-bracketSize, 0));
    painter.drawLine(rect.topRight(), rect.topRight() + QPoint(0, bracketSize));
    
    // Bottom-left bracket
    painter.drawLine(rect.bottomLeft(), rect.bottomLeft() + QPoint(bracketSize, 0));
    painter.drawLine(rect.bottomLeft(), rect.bottomLeft() + QPoint(0, -bracketSize));
    
    // Bottom-right bracket
    painter.drawLine(rect.bottomRight(), rect.bottomRight() + QPoint(-bracketSize, 0));
    painter.drawLine(rect.bottomRight(), rect.bottomRight() + QPoint(0, -bracketSize));
}

// IOC Management Methods
void HackerTerminal::setIOCManager(IndicatorManager *manager)
{
    iocManager = manager;
    addSystemMessage("IOC DATABASE CONNECTION ESTABLISHED");
    if (manager) {
        addSystemMessage(QString("DATABASE CONTAINS %1 INDICATORS").arg(manager->getIndicatorCount()));
    }
}

void HackerTerminal::displayIOCStats()
{
    if (!iocManager) {
        addSecurityAlert("IOC DATABASE NOT CONNECTED");
        return;
    }
    
    addSystemMessage("═══════════════════════════════════════");
    addSystemMessage("           IOC STATISTICS REPORT          ");
    addSystemMessage("═══════════════════════════════════════");
    
    int totalIOCs = iocManager->getIndicatorCount();
    addSystemMessage(QString("TOTAL INDICATORS: %1").arg(totalIOCs));
    
    // Count by type
    int hashCount = 0, ipCount = 0, urlCount = 0;
    for (int i = 0; i < totalIOCs; ++i) {
        const Indicator* ioc = iocManager->getIndicator(i);
        if (ioc) {
            if (dynamic_cast<const MaliciousHash*>(ioc)) hashCount++;
            else if (dynamic_cast<const MaliciousIP*>(ioc)) ipCount++;
            else if (dynamic_cast<const MaliciousURL*>(ioc)) urlCount++;
        }
    }
    
    addSystemMessage(QString("HASH INDICATORS: %1").arg(hashCount));
    addSystemMessage(QString("IP INDICATORS: %1").arg(ipCount)); 
    addSystemMessage(QString("URL INDICATORS: %1").arg(urlCount));
    
    // Monthly statistics
    int monthlyCount = iocManager->getIOCsRegisteredLastMonth();
    addSystemMessage(QString("NEW THIS MONTH: %1").arg(monthlyCount));
    
    addSystemMessage("═══════════════════════════════════════");
}

void HackerTerminal::searchIOC(const QString &query)
{
    if (!iocManager) {
        addSecurityAlert("IOC DATABASE NOT CONNECTED");
        return;
    }
    
    if (query.isEmpty()) {
        addSecurityAlert("SEARCH QUERY CANNOT BE EMPTY");
        return;
    }
    
    addSystemMessage(QString("SCANNING DATABASE FOR: '%1'").arg(query));
    
    int found = 0;
    int totalIOCs = iocManager->getIndicatorCount();
    
    for (int i = 0; i < totalIOCs; ++i) {
        const Indicator* ioc = iocManager->getIndicator(i);
        if (ioc && QString::fromStdString(ioc->getValue()).contains(query, Qt::CaseInsensitive)) {
            found++;
            QString typeStr = "UNKNOWN";
            if (dynamic_cast<const MaliciousHash*>(ioc)) typeStr = "HASH";
            else if (dynamic_cast<const MaliciousIP*>(ioc)) typeStr = "IP";
            else if (dynamic_cast<const MaliciousURL*>(ioc)) typeStr = "URL";
            
            addThreatDetection(QString("[%1] %2 (SEVERITY: %3)")
                .arg(typeStr, QString::fromStdString(ioc->getValue())).arg(ioc->getSeverity()));
        }
    }
    
    if (found == 0) {
        addSystemMessage("NO MATCHES FOUND");
    } else {
        addSystemMessage(QString("SCAN COMPLETE - %1 THREATS DETECTED").arg(found));
    }
}

void HackerTerminal::showIOCCommands()
{
    addSystemMessage("═══════════════════════════════════════");
    addSystemMessage("          AVAILABLE IOC COMMANDS         ");
    addSystemMessage("═══════════════════════════════════════");
    addSystemMessage("stats          - Display IOC statistics");
    addSystemMessage("search <query> - Search IOC database");
    addSystemMessage("scan           - Perform security scan");
    addSystemMessage("matrix         - Toggle matrix mode");
    addSystemMessage("clear          - Clear terminal");
    addSystemMessage("help           - Show this help");
    addSystemMessage("exit           - Close terminal");
    addSystemMessage("═══════════════════════════════════════");
}
