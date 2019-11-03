#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <QColorDialog>
#include <QLabel>
#include <QFileDialog>

#include "canvas.h"
#include "tool.h"
#include "eraser.h"
#include "pencil.h"
#include "bucket.h"
#include "colorpicker.h"
#include "projectmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Canvas canvas = Canvas(16, 16);
    bool brushStrokes[16][16];

    int horizontalOffset = 150;
    int pixelSize = 10;
    int canvasWidth = 16;
    int canvasHeight = 16;

    QColor brushColor = QColor(0,0,0,255);
    QColor brushSubColor = QColor(255,255,255,255);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Tool *tool = nullptr;
    QVector<QLabel*> framePreviews;

    QString projectLocation = "";

    /**
     * When window is cliked, update pixel with current tool.
     *
     * @param posX Clicked position X
     * @param posY Clicked posision Y
     */
    void windowClicked(int posX, int posY);

    /**
     * Get background CSS from color.
     *
     * @param color
     * @return
     */
    QString getColorString(QColor color);
    void primaryBrushColorUpdate(QColor color);
    void secondaryBrushColorUpdate(QColor color);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
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
    void on_findAndReplaceButton_clicked();
    void on_addFrameButton_clicked();
    void on_bucketButton_clicked();
    void on_colorPicker_clicked();
    void on_saveButton_clicked();
};
#endif // MAINWINDOW_H
