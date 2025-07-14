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
#include <QPaintEvent>
#include <QPainter>
#include "../indicatorManager/indicatorManager.hpp"

class IOCTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IOCTableWidget(QWidget *parent = nullptr);
    void updateData(IndicatorManager *manager);
    int getSelectedRow() const;

private slots:
    void filterData();
    void clearFilter();
    void updateBlinkingCells();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void setupUI();
    void setupTable();
    void applyTerminalTheme();
    void populateTable(IndicatorManager *manager);
    void addTableRow(int row, const Indicator *ioc);
    QString getSeverityText(int severity);
    QColor getSeverityColor(int severity);
    void updateRowNumbers();

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
