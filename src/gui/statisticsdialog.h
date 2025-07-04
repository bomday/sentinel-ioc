#ifndef STATISTICSDIALOG_H
#define STATISTICSDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QProgressBar>
#include <QChart>
#include <QChartView>
#include <QPieSeries>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QPieSlice>
#include <QTimer>
#include "../indicatorManager/indicatorManager.hpp"

QT_CHARTS_USE_NAMESPACE

class StatisticsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StatisticsDialog(IndicatorManager *manager, QWidget *parent = nullptr);

private slots:
    void refreshStatistics();
    void exportStatistics();
    void updateAnimations();

private:
    void setupUI();
    void applyMatrixTheme();
    void createStatisticsWidgets();
    void createCharts();
    void updateStatistics();
    void animateCharts();

    IndicatorManager *manager;
    
    // Layout
    QVBoxLayout *mainLayout;
    QGridLayout *statsGrid;
    
    // Statistics labels
    QLabel *totalCountLabel;
    QLabel *currentMonthLabel;
    QLabel *lastMonthLabel;
    QLabel *criticalCountLabel;
    QLabel *hashCountLabel;
    QLabel *ipCountLabel;
    QLabel *urlCountLabel;
    
    // Progress bars for visual effect
    QProgressBar *hashProgress;
    QProgressBar *ipProgress;
    QProgressBar *urlProgress;
    QProgressBar *severityProgress;
    
    // Charts
    QChartView *typeChartView;
    QChartView *severityChartView;
    QChart *typeChart;
    QChart *severityChart;
    QPieSeries *typeSeries;
    QBarSeries *severitySeries;
    
    // Animation timer
    QTimer *animationTimer;
    int animationStep;
    
    // Buttons
    QPushButton *refreshButton;
    QPushButton *exportButton;
    QPushButton *closeButton;
    
    // Theme colors
    QString primaryColor;
    QString secondaryColor;
    QString accentColor;
    QString backgroundColor;
    QString textColor;
    
    // Statistics data
    struct Stats {
        int totalCount = 0;
        int currentMonth = 0;
        int lastMonth = 0;
        int criticalCount = 0;
        int hashCount = 0;
        int ipCount = 0;
        int urlCount = 0;
        int severity1 = 0;
        int severity2 = 0;
        int severity3 = 0;
        int severity4 = 0;
        int severity5 = 0;
    } currentStats;
};

#endif // STATISTICSDIALOG_H
