#ifndef MATRIXSPLASH_H
#define MATRIXSPLASH_H

#include <QSplashScreen>
#include <QTimer>
#include <QPainter>
#include <QFont>
#include <QRandomGenerator>
#include <QApplication>
#include <vector>

class MatrixSplash : public QSplashScreen
{
    Q_OBJECT

public:
    explicit MatrixSplash(const QPixmap &pixmap = QPixmap(), QWidget *parent = nullptr);
    ~MatrixSplash();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updateAnimation();

private:
    struct MatrixColumn {
        int x;
        int y;
        int speed;
        int length;
        QString chars;
    };

    void initializeColumns();
    QString getRandomMatrixChar();

    QTimer *animationTimer;
    std::vector<MatrixColumn> columns;
    QFont matrixFont;
    int frameCount;
    
    // Colors
    QColor backgroundColor;
    QColor matrixGreen;
    QColor brightGreen;
    QColor textColor;
};

#endif // MATRIXSPLASH_H
