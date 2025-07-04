#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QSpinBox>
#include <QGroupBox>
#include <QCheckBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QSplitter>
#include <QTextEdit>
#include "../indicatorManager/indicatorManager.hpp"

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(IndicatorManager *manager, QWidget *parent = nullptr);

private slots:
    void performSearch();
    void clearSearch();
    void applyFilters();
    void onSearchTypeChanged();
    void onResultSelectionChanged();
    void exportResults();

private:
    void setupUI();
    void applyMatrixTheme();
    void createSearchControls();
    void createResultsArea();
    void updateResultsTable();
    void populateSearchResults(const std::vector<std::unique_ptr<Indicator>>& results);

    IndicatorManager *manager;
    
    // Layout
    QVBoxLayout *mainLayout;
    QSplitter *splitter;
    
    // Search controls
    QGroupBox *searchGroup;
    QGroupBox *filtersGroup;
    
    QLineEdit *searchEdit;
    QComboBox *searchTypeCombo;
    QComboBox *iocTypeCombo;
    QComboBox *severityCombo;
    QDateEdit *dateFromEdit;
    QDateEdit *dateToEdit;
    QCheckBox *enableDateFilter;
    
    QPushButton *searchButton;
    QPushButton *clearButton;
    QPushButton *exportButton;
    
    // Results area
    QTableWidget *resultsTable;
    QTextEdit *detailsText;
    QLabel *resultsCountLabel;
    
    // Theme colors
    QString primaryColor;
    QString secondaryColor;
    QString accentColor;
    QString backgroundColor;
    QString textColor;
};

#endif // SEARCHDIALOG_H
