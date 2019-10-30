#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Scaling pixel editer.
    int horizontalScaling = (this->width() - 300) / canvas.getWidth();
    int verticalScaling = this->height() / canvas.getHeight();

    pixelSize = std::min(horizontalScaling, verticalScaling);
    horizontalOffset = (this->width() / 2) - (canvas.getWidth() * pixelSize/2);

    setCursor(Qt::PointingHandCursor);

    ui->statusbar->hide();
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

    Frame* currentFrame = const_cast<Frame*>(canvas.getFrame(canvas.currentIndex()));

    // Prevent drawing when program just run.
    if (!programRunFlag) {
        programRunFlag = true;
    }
    else {
        // Detect which pixel is clicked.
        if(p.x() >= horizontalOffset &&
           p.x() <= horizontalOffset + pixelSize * canvas.getWidth()-1 &&
           p.y() <= pixelSize * canvas.getHeight() - 1 &&
           p.y() >= 0) {

            int pointX = (p.x() - horizontalOffset) / pixelSize;
            int pointY = p.y() / pixelSize;

            currentFrame->setPixel(pointX, pointY, brushColor);
        }
    }


    // Draw all pixels.
    for(int x = 0; x < canvas.getWidth(); x++) {
        for(int y = 0; y < canvas.getHeight(); y++) {
            QColor fillColor = currentFrame->getPixel(x, y);

            painter.fillRect(QRect(x*pixelSize+horizontalOffset, y*pixelSize, pixelSize, pixelSize), fillColor);
        }
    }

    // Draw outline.
    QPen linepen(Qt::gray);
    linepen.setWidth(1);
    painter.setPen(linepen);

    for(int i = horizontalOffset; i < (canvas.getWidth() + 1)*pixelSize+horizontalOffset; i+=pixelSize){
        painter.drawLine(i, 0, i, canvas.getHeight()*pixelSize);
    }

    // Draw left, right menu guide line.
    for(int i = 0; i < canvas.getHeight() + 1; i++){
        painter.drawLine(horizontalOffset, pixelSize*i, horizontalOffset+canvas.getWidth()*pixelSize, pixelSize*i);
    }

    painter.end();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);

   this->ui->rightPanel->setGeometry(this->width() -150, 0, 150, this->height());
   this->ui->leftPanel->setGeometry(0, 0, 150, this->height());

   int horizontalScaling = (this->width() - 300) / canvas.getWidth();
   int verticalScaling = this->height() / canvas.getHeight();

   pixelSize = std::min(horizontalScaling, verticalScaling);
   horizontalOffset = (this->width() / 2) - (canvas.getWidth() * pixelSize / 2);
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
