#ifndef HACKERTERMINAL_H
#define HACKERTERMINAL_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>

class HackerTerminal : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal glowIntensity READ glowIntensity WRITE setGlowIntensity)

public:
    explicit HackerTerminal(QWidget *parent = nullptr);
    
    void addSystemMessage(const QString &message);
    void addSecurityAlert(const QString &alert);
    void addThreatDetection(const QString &threat);
    void simulateHackingActivity();
    
    // IOC management commands
    void setIOCManager(class IndicatorManager *manager);
    void displayIOCStats();
    void searchIOC(const QString &query);
    void showIOCCommands();
    
    qreal glowIntensity() const { return m_glowIntensity; }
    void setGlowIntensity(qreal intensity) { m_glowIntensity = intensity; updateGlow(); }

public slots:
    void startMatrixMode();
    void executeCommand(const QString &command);
    void clearTerminal();

signals:
    void commandEntered(const QString &command);

protected:
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void updatePrompt();
    void onCommandEntered();
    void updateGlow();
    void simulateNetworkActivity();

private:
    void setupUI();
    void applyHackerTheme();
    void addTypingEffect(const QString &text, const QColor &color = QColor());
    void processCommand(const QString &command);
    QString getCurrentTimestamp();
    
    // UI Components
    QVBoxLayout *mainLayout;
    QTextEdit *terminalOutput;
    QHBoxLayout *inputLayout;
    QLabel *promptLabel;
    QLineEdit *commandInput;
    QLabel *statusLabel;
    
    // Visual effects
    QGraphicsDropShadowEffect *glowEffect;
    QPropertyAnimation *glowAnimation;
    QTimer *promptTimer;
    QTimer *networkTimer;
    
    // Styling
    QString primaryColor;
    QString secondaryColor;
    QString backgroundColor;
    QString errorColor;
    QString warningColor;
    QString successColor;
    
    // Animation properties
    qreal m_glowIntensity;
    bool blinkState;
    int commandHistory;
    
    // Terminal state
    bool matrixModeActive;
    
    // IOC Management
    class IndicatorManager *iocManager;
    
    // Command lists
    QStringList hackingCommands;
    QStringList systemResponses;
};

#endif // HACKERTERMINAL_H
