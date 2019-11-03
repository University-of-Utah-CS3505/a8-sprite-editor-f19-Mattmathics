#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <QColorDialog>
#include <QLabel>
#include <QFileDialog>
#include <QScrollArea>
#include <QGridLayout>
#include <QWidget>

#include "canvas.h"
#include "tool.h"
#include "eraser.h"
#include "pencil.h"
#include "bucket.h"
#include "colorpicker.h"
#include "projectmanager.h"
#include "qimagebutton.h"
#include "json.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Canvas *copyCanvas = nullptr, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    Canvas *canvas = nullptr;
    Tool *tool = nullptr;

    QVector<QImageButton*> framePreviews;
    QGridLayout *frameGridLayout;

    int horizontalOffset = 150;
    int pixelSize = 10;

    QColor brushColor = QColor(0,0,0,255);
    QColor brushSubColor = QColor(255,255,255,255);

    bool initialized = false;
    QString projectLocation = "";

    void initialize(Canvas *canvas);
    void deinitalize();

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
    void addFramePreview();

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
    void on_framePriview_clicked();
    void on_openButton_clicked();
    void on_redoButton_clicked();
    void on_undoButton_clicked();
};
#endif // MAINWINDOW_H
