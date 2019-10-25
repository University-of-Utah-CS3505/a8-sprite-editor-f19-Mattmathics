#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for(int i = 0; i < canvasWidth; i++){
        for(int j = 0; j < canvasHeight; j++) {
            pixels[i][j] = QColor(0,0,0,0);
        }
    }
    int horizontalScaling = (this->width()-300)/canvasWidth;
    int verticalScaling = this->height()/canvasHeight;
    pixelSize = std::min(horizontalScaling, verticalScaling);
    horizontalOffset = (this->width()/2)-(canvasWidth*pixelSize/2);
    setCursor(Qt::PointingHandCursor);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *e) {
    this->repaint();
}
void MainWindow::mouseMoveEvent(QMouseEvent *e) {
    this->repaint();
}

void MainWindow::paintEvent(QPaintEvent *e) {
    QPoint globalCursorPos = QCursor::pos();
    QWidget::mapFromGlobal(QCursor::pos());
    QPoint p = this->mapFromGlobal(globalCursorPos);
    QPainter painter(this);
    QPen pointpen(Qt::black);
    pointpen.setWidth(1);
    painter.setPen(pointpen);
    if(p.x()>=horizontalOffset && p.x()<=horizontalOffset+pixelSize*canvasWidth-1 && p.y()<=pixelSize*canvasHeight-1 && p.y()>=0){
//        QColor backColor = pixels[(p.x()-horizontalOffset)/pixelSize][p.y()/pixelSize];
//        if(backColor == QColor(0,0,0,0)){
//            pixels[(p.x()-horizontalOffset)/pixelSize][p.y()/pixelSize] = brushColor;
//        }
//        else {
//            int brushAlpha = brushColor.alpha();
//            double oneMinus = 1.0-((double)brushAlpha/255.0);
//            int outA = (double)brushColor.alpha() + ((double)backColor.alpha()*oneMinus);
//            int outR = (brushColor.red()*brushColor.alpha()+backColor.red()*backColor.alpha()*oneMinus)/outA;
//            int outG = (brushColor.green()*brushColor.alpha()+backColor.green()*backColor.alpha()*oneMinus)/outA;
//            int outB = (brushColor.blue()*brushColor.alpha()+backColor.blue()*backColor.alpha()*oneMinus)/outA;
//            pixels[(p.x()-horizontalOffset)/pixelSize][p.y()/pixelSize] = QColor(outR, outG, outB, outA);
//        }
        pixels[(p.x()-horizontalOffset)/pixelSize][p.y()/pixelSize] = brushColor;
    }
    for(int i = 0; i < canvasWidth; i++){
        for(int j = 0; j < canvasHeight; j++) {
            pointpen.setColor(pixels[i][j]);
            painter.setPen(pointpen);
            for(int h = 0; h < pixelSize/2+1; h++) {
                painter.drawRect(i*pixelSize+horizontalOffset+h, j*pixelSize+h, pixelSize-2*h, pixelSize-2*h);
            }
            painter.drawPoint(i*pixelSize + pixelSize/2+horizontalOffset,j*pixelSize + pixelSize/2);
        }
    }
    QPen linepen(Qt::gray);
    linepen.setWidth(1);
    painter.setPen(linepen);
    for(int i = horizontalOffset; i < (canvasWidth+1)*pixelSize+horizontalOffset; i+=pixelSize){
        painter.drawLine(i, 0, i, canvasHeight*pixelSize);
    }
    for(int i = 0; i < canvasHeight+1; i++){
        painter.drawLine(horizontalOffset, pixelSize*i, horizontalOffset+canvasWidth*pixelSize, pixelSize*i);
    }
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   this->ui->rightPanel->setGeometry(this->width()-150,0, 150, this->height());
   this->ui->leftPanel->setGeometry(0,0, 150, this->height());
   int horizontalScaling = (this->width()-300)/canvasWidth;
   int verticalScaling = this->height()/canvasHeight;
   pixelSize = std::min(horizontalScaling, verticalScaling);
   horizontalOffset = (this->width()/2)-(canvasWidth*pixelSize/2);
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_X) {
        on_swapBrushesButton_clicked();
    }
    if(event->key() == Qt::Key_E) {
        on_eraserButton_clicked();
    }
    if(event->key() == Qt::Key_P) {
        on_pencilButton_clicked();
    }
}

void MainWindow::on_pencilButton_clicked()
{
    setCursor(Qt::PointingHandCursor);
    eraseMode = false;
    brushColor = tempColorHolder;
}

void MainWindow::on_eraserButton_clicked()
{
    setCursor(Qt::OpenHandCursor);
    if(!eraseMode) {
        eraseMode = true;
        tempColorHolder = brushColor;
        brushColor = QColor(0,0,0,0);
    }
}

void MainWindow::on_swapBrushesButton_clicked()
{
    if(eraseMode){
        std::string colorString = "background-color: rgb(" + std::to_string(brush2Color.red()) + ", " + std::to_string(brush2Color.green()) + ", " + std::to_string(brush2Color.blue()) + ");";
        this->ui->primaryBrushButton->setStyleSheet(QString::fromStdString(colorString));
        colorString = "background-color: rgb(" + std::to_string(tempColorHolder.red()) + ", " + std::to_string(tempColorHolder.green()) + ", " + std::to_string(tempColorHolder.blue()) + ");";
        this->ui->secondaryBrushButton->setStyleSheet(QString::fromStdString(colorString));
        QColor temp = brush2Color;
        brush2Color = tempColorHolder;
        tempColorHolder = temp;
    }
    else {
        std::string colorString = "background-color: rgb(" + std::to_string(brush2Color.red()) + ", " + std::to_string(brush2Color.green()) + ", " + std::to_string(brush2Color.blue()) + ");";
        this->ui->primaryBrushButton->setStyleSheet(QString::fromStdString(colorString));
        colorString = "background-color: rgb(" + std::to_string(brushColor.red()) + ", " + std::to_string(brushColor.green()) + ", " + std::to_string(brushColor.blue()) + ");";
        this->ui->secondaryBrushButton->setStyleSheet(QString::fromStdString(colorString));
        QColor temp = brushColor;
        brushColor = brush2Color;
        brush2Color = temp;
    }
}

void MainWindow::on_primaryBrushButton_clicked()
{
    QColor newColor = QColorDialog::getColor(brushColor,this,QString("Choose Color"), QColorDialog::ShowAlphaChannel);
    if(newColor.isValid()) {
        if(eraseMode){
            tempColorHolder = newColor;
        }
        else {
            brushColor = newColor;
        }
        std::string colorString = "background-color: rgb(" + std::to_string(newColor.red()) + ", " + std::to_string(newColor.green()) + ", " + std::to_string(newColor.blue()) + ");";
        this->ui->primaryBrushButton->setStyleSheet(QString::fromStdString(colorString));
    }
}

void MainWindow::on_resetBrushesButton_clicked()
{
    if(eraseMode){
        tempColorHolder = QColor(0,0,0,255);

    }
    else {
        brushColor = QColor(0,0,0,255);
    }
    brush2Color = QColor(255,255,255,255);
    this->ui->primaryBrushButton->setStyleSheet(QString("background-color: rgba(0,0,0,255);"));
    this->ui->secondaryBrushButton->setStyleSheet(QString("background-color: rgba(255,255,255,255);"));
}

void MainWindow::on_secondaryBrushButton_clicked()
{
    QColor newColor = QColorDialog::getColor(brushColor,this,QString("Choose Color"), QColorDialog::ShowAlphaChannel);
    if(newColor.isValid()) {
        brush2Color = newColor;
        std::string colorString = "background-color: rgb(" + std::to_string(newColor.red()) + ", " + std::to_string(newColor.green()) + ", " + std::to_string(newColor.blue()) + ");";
        this->ui->secondaryBrushButton->setStyleSheet(QString::fromStdString(colorString));
    }
}
