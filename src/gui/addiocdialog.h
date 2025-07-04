#ifndef ADDIOCDIALOG_H
#define ADDIOCDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QGroupBox>
#include <QStackedWidget>
#include "../indicatorManager/indicatorManager.hpp"

class AddIOCDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddIOCDialog(IndicatorManager *manager, QWidget *parent = nullptr);

private slots:
    void onTypeChanged();
    void addIOC();
    void resetForm();

private:
    void setupUI();
    void applyMatrixTheme();
    void createTypeSpecificWidgets();
    void showTypeSpecificFields(const QString &type);
    bool validateInput();

    IndicatorManager *manager;
    
    // Main layout
    QVBoxLayout *mainLayout;
    QFormLayout *formLayout;
    
    // Common fields
    QComboBox *typeCombo;
    QSpinBox *severitySpinBox;
    QLineEdit *descriptionEdit;
    QLineEdit *originEdit;
    
    // Type-specific widgets container
    QStackedWidget *typeSpecificStack;
    
    // Hash-specific widgets
    QWidget *hashWidget;
    QLineEdit *hashEdit;
    QComboBox *algorithmCombo;
    
    // IP-specific widgets
    QWidget *ipWidget;
    QLineEdit *ipEdit;
    QLineEdit *countryEdit;
    QLineEdit *ispEdit;
    
    // URL-specific widgets
    QWidget *urlWidget;
    QLineEdit *urlEdit;
    QComboBox *protocolCombo;
    
    // Buttons
    QPushButton *addButton;
    QPushButton *resetButton;
    QPushButton *cancelButton;
    
    // Theme colors
    QString primaryColor;
    QString secondaryColor;
    QString accentColor;
    QString backgroundColor;
    QString textColor;
};

#endif // ADDIOCDIALOG_H
