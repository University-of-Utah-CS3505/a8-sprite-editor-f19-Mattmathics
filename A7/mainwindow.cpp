#include <QDebug>
#include <QVBoxLayout>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas.h"
#include "tool.h"
#include "eraser.h"
#include "pencil.h"
#include "bucket.h"
#include "projectmanager.h"
#include "paintAllSameColor.h"

#define PREVIEW_IMAGE_SIZE 110

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusbar->hide();      //For macOS user.

    //Set Frames View
    QWidget *client = ui->framesScrollWidget;

    QScrollArea *scrollArea = ui->framesScroll;
    scrollArea->setWidgetResizable(true);

    frameGridLayout = new QGridLayout;
    frameGridLayout->setContentsMargins(0, 0, 0, 0);
    frameGridLayout->setSpacing(5);

    client->setLayout(frameGridLayout);

    QImageButton *previewLabel = new QImageButton(client);
    previewLabel->setStyleSheet("border: 1px solid black");
    previewLabel->setFixedSize(PREVIEW_IMAGE_SIZE, PREVIEW_IMAGE_SIZE);
    previewLabel->setObjectName("previewLabel-0");
    connect(previewLabel, &QImageButton::clicked, this, &MainWindow::on_framePriview_clicked);

    frameGridLayout->addWidget(previewLabel);
    framePreviews.push_back(previewLabel);
    on_addFrameButton_clicked();

    //Scaling pixel editer.
    resizeEvent(nullptr);

    //Set tool to pencil
    on_pencilButton_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete tool;
    delete frameGridLayout;

    foreach (auto obj, framePreviews) {
        delete obj;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *e) {
    this->windowClicked(e->x(), e->y());
}

void MainWindow::mouseMoveEvent(QMouseEvent *e) {
    this->windowClicked(e->x(), e->y());
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e) {
    Q_UNUSED(e)

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

        // If tool's brush color is diffrent with brush color, update primary color
        if (tool->getBrushColor() != brushColor)
            primaryBrushColorUpdate(tool->getBrushColor());

        // Update Screen
        this->repaint();
    }
}

void MainWindow::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e)

    QPainter painter(this);
    Frame* currentFrame = canvas.getCurrentFrame();

    for(int i = 0; i < canvas.sizeFrame(); i++) {
        QPixmap pixmap = QPixmap::fromImage(canvas.getFrame(i)->getImage());
        pixmap = pixmap.fromImage(canvas.getFrame(i)->getImage());
        pixmap = pixmap.scaled(framePreviews[i]->size(), Qt::KeepAspectRatio);

        framePreviews[i]->setPixmap(pixmap);
    }

    // Draw background pixels;
    QColor grayColor(97, 97, 97, 255);
    QColor lightGrayColor(125, 125, 125, 255);

    for(int x = 0; x < canvas.getWidth(); x++) {
        for(int y = 0; y < canvas.getHeight(); y++) {\
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
    if(event->key() == Qt::Key_M) {
        on_findAndReplaceButton_clicked();
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

void MainWindow::on_findAndReplaceButton_clicked()
{
    if (tool != nullptr)
        delete tool;

    tool = new PaintAllSameColor(brushColor, canvas.getCurrentFrame());

    setCursor(Qt::PointingHandCursor);
}

void MainWindow::on_bucketButton_clicked()
{
    if (tool != nullptr)
        delete tool;

    tool = new Bucket(brushColor, canvas.getCurrentFrame());

    setCursor(Qt::PointingHandCursor);
}

void MainWindow::on_colorPicker_clicked()
{
    if (tool != nullptr)
        delete tool;

    tool = new ColorPicker(brushColor, canvas.getCurrentFrame());

    setCursor(Qt::CrossCursor);
}


void MainWindow::on_swapBrushesButton_clicked()
{
    QColor swap = brushColor;

    primaryBrushColorUpdate(brushSubColor);
    secondaryBrushColorUpdate(swap);
}

void MainWindow::on_primaryBrushButton_clicked()
{
    QColor newColor = QColorDialog::getColor(brushColor,this,QString("Choose Color"), QColorDialog::ShowAlphaChannel);

    primaryBrushColorUpdate(newColor);
}

void MainWindow::on_resetBrushesButton_clicked()
{
    primaryBrushColorUpdate(QColor(0,0,0,255));
    secondaryBrushColorUpdate(QColor(255,255,255,255));
}

void MainWindow::on_secondaryBrushButton_clicked()
{
    QColor newColor = QColorDialog::getColor(brushColor,this,QString("Choose Color"), QColorDialog::ShowAlphaChannel);

    secondaryBrushColorUpdate(newColor);
}

void MainWindow::primaryBrushColorUpdate(QColor color)
{
    if(color.isValid()) {
        brushColor = color;
        this->ui->primaryBrushButton->setStyleSheet(getColorString(color));

        tool->setBrushColor(color);
    }
}

void MainWindow::secondaryBrushColorUpdate(QColor color)
{
    if(color.isValid()) {
        brushSubColor = color;
        this->ui->secondaryBrushButton->setStyleSheet(getColorString(color));
    }
}

void MainWindow::on_addFrameButton_clicked()
{
    canvas.addFrame();

    QWidget *client = ui->framesScrollWidget;

    QImageButton *previewLabel = new QImageButton(client);
    previewLabel->setStyleSheet("border: 1px solid black");
    previewLabel->setFixedSize(PREVIEW_IMAGE_SIZE, PREVIEW_IMAGE_SIZE);
    previewLabel->setObjectName("previewLabel-" + QString::number(canvas.sizeFrame() - 1));
    connect(previewLabel, &QImageButton::clicked, this, &MainWindow::on_framePriview_clicked);

    frameGridLayout->addWidget(previewLabel);
    framePreviews.push_back(previewLabel);
}

void MainWindow::on_framePriview_clicked()
{
    // Get which button is clicekd
    QImageButton* buttonSender = qobject_cast<QImageButton*>(sender());
    int index = buttonSender->objectName().split("-")[1].toInt();

    canvas.moveFrame(index);
    tool->setCurrentFrame(canvas.getCurrentFrame());
    repaint();
}

QString MainWindow::getColorString(QColor color)
{
    std::string colorString = "background-color: rgb(" + std::to_string(color.red()) + ", " + std::to_string(color.green()) + ", " + std::to_string(color.blue()) + ");";
    return QString::fromStdString(colorString);
}

void MainWindow::on_saveButton_clicked()
{
    if (projectLocation.isEmpty())
        projectLocation = QDir::homePath() + "/" + "untitled.ssp";

    QString filter = "SIMP Project file (*.ssp);; PNG image file (*.png)";
    QString filePath = QFileDialog::getSaveFileName(this, "Choose file to save", projectLocation, filter, &filter);

    if (filePath.isEmpty()) return;

    if (filePath.toLower().endsWith(".ssp"))
        ProjectManager::saveProject(&canvas, filePath);

    else if (filePath.toLower().endsWith(".png"))
        ProjectManager::saveAsPng(canvas.getCurrentFrame(), filePath);
    else
        ProjectManager::saveProject(&canvas, filePath);

    projectLocation = filePath;
}
