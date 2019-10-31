#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusbar->hide();      //For macOS user.

    //Scaling pixel editer.
    resizeEvent(nullptr);

    //Set tool to pencil
    on_pencilButton_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete tool;
}

void MainWindow::mousePressEvent(QMouseEvent *e) {
    this->windowClicked(e->x(), e->y());
}

void MainWindow::mouseMoveEvent(QMouseEvent *e) {    
    this->windowClicked(e->x(), e->y());
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e) {
    if(Pencil* p = dynamic_cast<Pencil*>(tool)) {
        p->resetStrokes();
    }
}

void MainWindow::windowClicked(int posX, int posY) {
    // Check inside of canvas clicked
    if(posX >= horizontalOffset &&
       posX <= horizontalOffset + (pixelSize * canvas.getWidth()-1) &&
       posY <= pixelSize * canvas.getHeight() - 1 &&
       posY >= 0) {

        int pointX = (posX - horizontalOffset) / pixelSize;
        int pointY = posY / pixelSize;

        tool->perform(pointX, pointY);

        // Update Screen
        this->repaint();
    }
}

void MainWindow::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e)

    QPainter painter(this);
    Frame* currentFrame = canvas.getCurrentFrame();

    // Draw background pixels;
    bool colorFlag = false;
    QColor grayColor(97, 97, 97, 255);
    QColor lightGrayColor(125, 125, 125, 255);

    for(int x = 0; x < canvas.getWidth(); x++) {
        for(int y = 0; y < canvas.getHeight(); y++) {
            QColor fillColor = currentFrame->getPixel(x, y);

            // Get point of pixel.
            int pointX = (x * pixelSize) + horizontalOffset;
            int pointY = y * pixelSize;

            // Get middle point of pixel.
            int middleX = pointX + (pixelSize / 2);
            int middleY = pointY + (pixelSize / 2);

            int sizeHalf = pixelSize / 2;

            painter.fillRect(QRect(pointX, pointY, sizeHalf, sizeHalf), grayColor);
            painter.fillRect(QRect(middleX, pointY, pixelSize - sizeHalf, sizeHalf), lightGrayColor);

            painter.fillRect(QRect(pointX, middleY, sizeHalf, pixelSize - sizeHalf), lightGrayColor);
            painter.fillRect(QRect(middleX, middleY, pixelSize - sizeHalf, pixelSize - sizeHalf), grayColor);
        }
    }

    for(int y = 0; y < canvas.getHeight() * 2; y++) {
        for(int x = 0; x < canvas.getWidth() * 2; x++) {
            int pointX = (x * (pixelSize / 2)) + horizontalOffset;
            int pointY = y * (pixelSize / 2);

            painter.fillRect(QRect(pointX, pointY, pixelSize/2, pixelSize/2),
                             (colorFlag = !colorFlag) ? grayColor : lightGrayColor);
        }

        colorFlag = !colorFlag;
    }

    // Draw all pixels.
    for(int x = 0; x < canvas.getWidth(); x++) {
        for(int y = 0; y < canvas.getHeight(); y++) {
            QColor fillColor = currentFrame->getPixel(x, y);

            int pointX = (x * pixelSize) + horizontalOffset;
            int pointY = y * pixelSize;

            painter.fillRect(QRect(pointX, pointY, pixelSize, pixelSize), fillColor);
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
    Q_UNUSED(event)

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
    if (tool != nullptr)
        delete tool;

    tool = new Pencil(brushColor, canvas.getCurrentFrame());

    setCursor(Qt::PointingHandCursor);
}

void MainWindow::on_eraserButton_clicked()
{
    if (tool != nullptr)
        delete tool;

    tool = new Eraser(canvas.getCurrentFrame());

    setCursor(Qt::OpenHandCursor);
}

void MainWindow::on_swapBrushesButton_clicked()
{
    std::string colorString = "background-color: rgb(" + std::to_string(brushSubColor.red()) + ", " + std::to_string(brushSubColor.green()) + ", " + std::to_string(brushSubColor.blue()) + ");";
    this->ui->primaryBrushButton->setStyleSheet(QString::fromStdString(colorString));
    colorString = "background-color: rgb(" + std::to_string(brushColor.red()) + ", " + std::to_string(brushColor.green()) + ", " + std::to_string(brushColor.blue()) + ");";
    this->ui->secondaryBrushButton->setStyleSheet(QString::fromStdString(colorString));
    QColor temp = brushColor;
    brushColor = brushSubColor;
    brushSubColor = temp;

    tool->setBrushColor(brushColor);
}

void MainWindow::on_primaryBrushButton_clicked()
{
    QColor newColor = QColorDialog::getColor(brushColor,this,QString("Choose Color"), QColorDialog::ShowAlphaChannel);
    if(newColor.isValid()) {
        brushColor = newColor;
        std::string colorString = "background-color: rgb(" + std::to_string(newColor.red()) + ", " + std::to_string(newColor.green()) + ", " + std::to_string(newColor.blue()) + ");";
        this->ui->primaryBrushButton->setStyleSheet(QString::fromStdString(colorString));
    }

    tool->setBrushColor(brushColor);
}

void MainWindow::on_resetBrushesButton_clicked()
{
    brushColor = QColor(0,0,0,255);
    brushSubColor = QColor(255,255,255,255);
    this->ui->primaryBrushButton->setStyleSheet(QString("background-color: rgba(0,0,0,255);"));
    this->ui->secondaryBrushButton->setStyleSheet(QString("background-color: rgba(255,255,255,255);"));

    tool->setBrushColor(brushColor);
}

void MainWindow::on_secondaryBrushButton_clicked()
{
    QColor newColor = QColorDialog::getColor(brushColor,this,QString("Choose Color"), QColorDialog::ShowAlphaChannel);
    if(newColor.isValid()) {
        brushSubColor = newColor;
        std::string colorString = "background-color: rgb(" + std::to_string(newColor.red()) + ", " + std::to_string(newColor.green()) + ", " + std::to_string(newColor.blue()) + ");";
        this->ui->secondaryBrushButton->setStyleSheet(QString::fromStdString(colorString));
    }
}



