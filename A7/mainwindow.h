#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <QColorDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    bool eraseMode = false;
    int horizontalOffset = 150;
    int pixelSize = 10;
    int canvasWidth = 16;
    int canvasHeight = 16;
    QColor pixels[16][16];
    QColor brushColor = QColor(0,0,0,255);
    QColor brush2Color = QColor(255,255,255,255);
    QColor tempColorHolder = QColor(0,0,0,255);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *e);
    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_pencilButton_clicked();
    void on_eraserButton_clicked();
    void on_swapBrushesButton_clicked();
    void on_primaryBrushButton_clicked();
    void on_resetBrushesButton_clicked();
    void on_secondaryBrushButton_clicked();
};
#endif // MAINWINDOW_H
