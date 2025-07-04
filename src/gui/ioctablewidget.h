#ifndef IOCTABLEWIDGET_H
#define IOCTABLEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTimer>
#include "../indicatorManager/indicatorManager.hpp"

class IOCTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IOCTableWidget(QWidget *parent = nullptr);
    void updateData(IndicatorManager *manager);

private slots:
    void filterData();
    void clearFilter();
    void onCellDoubleClicked(int row, int column);
    void updateBlinkingCells();

private:
    void setupUI();
    void setupTable();
    void applyMatrixTheme();
    void populateTable(IndicatorManager *manager);
    void addTableRow(int row, const Indicator *ioc);
    QString getSeverityText(int severity);
    QColor getSeverityColor(int severity);

    QVBoxLayout *mainLayout;
    QHBoxLayout *filterLayout;
    
    // Filter controls
    QLineEdit *searchEdit;
    QComboBox *typeCombo;
    QComboBox *severityCombo;
    QPushButton *filterButton;
    QPushButton *clearButton;
    
    // Table
    QTableWidget *table;
    QLabel *statusLabel;
    
    // Matrix effects
    QTimer *blinkTimer;
    QVector<int> blinkingRows;
    bool blinkState;
    
    // Data
    IndicatorManager *currentManager;
    
    // Theme colors
    QString primaryColor;
    QString secondaryColor;
    QString accentColor;
    QString backgroundColor;
    QString textColor;
};

#endif // IOCTABLEWIDGET_H
