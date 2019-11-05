#include <QDebug>
#include <QVBoxLayout>
#include <QDebug>
#include <QShortcut>

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

MainWindow::MainWindow(Canvas* copyCanvas, QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusbar->hide();      //For macOS user.

    QWidget *client = ui->framesScrollWidget;

    //set icons
    ui->pencilButton->setIcon(QIcon (QPixmap (":/paint.png")));             //pencil
    ui->pencilButton->setIconSize(QSize(33,33));
    ui->bucketButton->setIcon(QIcon (QPixmap (":/bucket.png")));            //bucket
    ui->bucketButton->setIconSize(QSize(33,33));
    ui->eraserButton->setIcon(QIcon (QPixmap (":/erase.png")));             //eraser
    ui->eraserButton->setIconSize(QSize(33,33));
    ui->colorPicker->setIcon(QIcon (QPixmap (":/colorPicker.png")));        //colorPicker
    ui->colorPicker->setIconSize(QSize(33,33));
    ui->findAndReplaceButton->setIcon(QIcon (QPixmap (":/brush.png")));     //findAndReplace
    ui->findAndReplaceButton->setIconSize(QSize(33,33));
    ui->openButton->setIcon(QIcon (QPixmap (":/open.png")));                //open
    ui->openButton->setIconSize(QSize(33,33));
    ui->saveButton->setIcon(QIcon (QPixmap (":/save.png")));                //save
    ui->saveButton->setIconSize(QSize(33,33));
    ui->redoButton->setIcon(QIcon (QPixmap (":/redo.png")));                //redo
    ui->redoButton->setIconSize(QSize(33,33));
    ui->undoButton->setIcon(QIcon (QPixmap (":/undo.png")));                //undo
    ui->undoButton->setIconSize(QSize(33,33));
    ui->background_2->setPixmap(QPixmap(":/background.png"));
    ui->background_3->setPixmap(QPixmap(":/background.png"));

    //set button tool tips
    ui->pencilButton->setToolTip("pencil(HotKey_P)");
    ui->eraserButton->setToolTip("eraser(HotKey_E");
    ui->bucketButton->setToolTip("bucket(HotKey_B)");
    ui->findAndReplaceButton->setToolTip("findAndReplace(HotKey_M)");
    ui->colorPicker->setToolTip("pencil.HotKey_C");
    ui->swapBrushesButton->setToolTip("swapBrushes(HotKey_X)");
    ui->resetBrushesButton->setToolTip("resetBrushes");
    ui->addFrameButton->setToolTip("addFrames");
    ui->saveButton->setToolTip("save");
    ui->openButton->setToolTip("open");
    ui->redoButton->setToolTip("redo(HotKey_])");
    ui->undoButton->setToolTip("undo(HotKey_[)");

    // Create frame prview area
    QScrollArea *scrollArea = ui->framesScroll;
    scrollArea->setWidgetResizable(true);

    frameGridLayout = new QGridLayout;
    frameGridLayout->setContentsMargins(0, 0, 0, 0);
    frameGridLayout->setSpacing(5);

    //Shortcuts support using ctrl
    //On macOS, CTRL is Command.
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z), this, SLOT(on_undoButton_clicked()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Z), this, SLOT(on_redoButton_clicked()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this, SLOT(on_saveButton_clicked()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_O), this, SLOT(on_openButton_clicked()));
    // add another for exporting a sprite to a gif

    client->setLayout(frameGridLayout);

    initialize(copyCanvas);
}

MainWindow::~MainWindow()
{
    deinitalize();

    delete ui;
}

void MainWindow::initialize(Canvas *copyCanvas)
{
    if (copyCanvas == nullptr)
        canvas = new Canvas(32, 32);
    else
        canvas = copyCanvas;

    for(int i = 0; i < canvas->sizeFrame(); i++)
        addFramePreview();

    canvas->moveFrame(0);

    //Scaling pixel editer.
    resizeEvent(nullptr);

    //Set tool to pencil
    on_pencilButton_clicked();
}

void MainWindow::deinitalize()
{
    foreach (QImageButton* obj, framePreviews) {
        layout()->removeWidget(obj);
        delete obj;
    }
    framePreviews.clear();

    delete canvas;
    canvas = nullptr;

    delete tool;
    tool = nullptr;
}

void MainWindow::mousePressEvent(QMouseEvent *e) {
    this->canvasPressed(e->x(), e->y());
}

void MainWindow::mouseMoveEvent(QMouseEvent *e) {
    if(canvasClicked)
        this->canvasMoved(e->x(), e->y());
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e) {
    Q_UNUSED(e)

    if(canvasClicked)
        this->canvasReleased();
}

void MainWindow::canvasPressed(int posX, int posY) {
    // Check inside of canvas clicked
    if(posX >= horizontalOffset &&
       posX <= horizontalOffset + (pixelSize * canvas->getWidth()-1) &&
       posY <= pixelSize * canvas->getHeight() - 1 &&
       posY >= 0) {

        //When user starts click canvas, capture current image for undo redo.
        //When canvasClicked is false, it means user starts edit pixels.
        //When canvasClicked is true, it means user is still editing pixels.
        if(!canvasClicked)
            canvas->getCurrentFrame()->captureHistory();

        int pointX = (posX - horizontalOffset) / pixelSize;
        int pointY = posY / pixelSize;

        // Perform tool
        tool->perform(pointX, pointY);
        this->aftrerToolPerform(pointX, pointY);

        // If tool is Colorpicker, update primary color
        if(ColorPicker* picker = dynamic_cast<ColorPicker*>(tool))
             primaryBrushColorUpdate(tool->getBrushColor());

        // Update Screen
        this->repaint();

        canvasClicked = true;
        lastPointX = pointX;
        lastPointY = pointY;
    }
}

void MainWindow::canvasMoved(int posX, int posY) {
    // Check inside of canvas clicked
    if(posX >= horizontalOffset &&
       posX <= horizontalOffset + (pixelSize * canvas->getWidth()-1) &&
       posY <= pixelSize * canvas->getHeight() - 1 &&
       posY >= 0) {

        int pointX = (posX - horizontalOffset) / pixelSize;
        int pointY = posY / pixelSize;

        // Draw line from last point to current point.
        tool->preformLine(lastPointX, lastPointY, pointX, pointY);
        this->aftrerToolPerform(pointX, pointY);

        // Update Screen
        this->repaint();

        canvasClicked = true;
        lastPointX = pointX;
        lastPointY = pointY;
    }
}

void MainWindow::canvasReleased() {
    canvasClicked = false;

    if(Pencil* p = dynamic_cast<Pencil*>(tool))
        p->resetStrokes();
}

void MainWindow::aftrerToolPerform(int pointX, int pointY) {
    // If tool is Colorpicker, update primary color
    if(ColorPicker* picker = dynamic_cast<ColorPicker*>(tool))
         primaryBrushColorUpdate(tool->getBrushColor());
}

void MainWindow::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e)

    if(canvas == nullptr) return;

    QPainter painter(this);
    Frame* currentFrame = canvas->getCurrentFrame();

    // Update preview pixels.
    for(int i = 0; i < canvas->sizeFrame(); i++) {
        QPixmap pixmap = QPixmap::fromImage(canvas->getFrame(i)->getImage());
        pixmap = pixmap.fromImage(canvas->getFrame(i)->getImage());
        pixmap = pixmap.scaled(framePreviews[i]->size(), Qt::KeepAspectRatio);

        framePreviews[i]->setPixmap(pixmap);

        // Show to user certain frame is using now.
        if(i == canvas->currentIndex())
            framePreviews[i]->setStyleSheet("border: 1px solid white");
        else
            framePreviews[i]->setStyleSheet("border: 1px solid black");
    }

    // Draw background pixels;
    QColor grayColor(97, 97, 97, 255);
    QColor lightGrayColor(125, 125, 125, 255);

    for(int x = 0; x < canvas->getWidth(); x++) {
        for(int y = 0; y < canvas->getHeight(); y++) {\
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
    for(int x = 0; x < canvas->getWidth(); x++) {
        for(int y = 0; y < canvas->getHeight(); y++) {
            QColor fillColor = currentFrame->getPixel(x, y);

            int pointX = (x * pixelSize) + horizontalOffset;
            int pointY = y * pixelSize;

            painter.fillRect(QRect(pointX, pointY, pixelSize, pixelSize), fillColor);
        }
    }

    // Draw pixel border lines.
    QPen linepen(Qt::gray);
    linepen.setWidth(1);
    painter.setPen(linepen);

    for(int i = horizontalOffset; i < (canvas->getWidth() + 1)*pixelSize+horizontalOffset; i+=pixelSize){
        painter.drawLine(i, 0, i, canvas->getHeight()*pixelSize);
    }

    for(int i = 0; i < canvas->getHeight() + 1; i++){
        painter.drawLine(horizontalOffset, pixelSize*i, horizontalOffset+canvas->getWidth()*pixelSize, pixelSize*i);
    }

    painter.end();

    // Update redo, undo button
    ui->undoButton->setEnabled(currentFrame->isUndoable());
    ui->redoButton->setEnabled(currentFrame->isRedoable());

    // Update frame delete button
    ui->deleteFrameButton->setEnabled(canvas->sizeFrame() != 1);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event)

    this->ui->rightPanel->setGeometry(this->width() -150, 0, 150, this->height());
    this->ui->leftPanel->setGeometry(0, 0, 150, this->height());

    int horizontalScaling = (this->width() - 300) / canvas->getWidth();
    int verticalScaling = this->height() / canvas->getHeight();

    pixelSize = std::min(horizontalScaling, verticalScaling);
    horizontalOffset = (this->width() / 2) - (canvas->getWidth() * pixelSize / 2);
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
    if(event->key() == Qt::Key_B) {
        on_bucketButton_clicked();
    }
    if(event->key() == Qt::Key_M) {
        on_findAndReplaceButton_clicked();
    }
    if(event->key() == Qt::Key_C) {
         on_colorPicker_clicked();
    }
    if(event->key() ==Qt::Key_BracketLeft) {
        on_undoButton_clicked();
    }
    if(event->key() ==Qt::Key_BracketRight) {
        on_redoButton_clicked();
    }


}

void MainWindow::on_pencilButton_clicked()
{
    if (tool != nullptr)
        delete tool;

    tool = new Pencil(brushColor, canvas);

    setCursor(Qt::PointingHandCursor);
}

void MainWindow::on_eraserButton_clicked()
{
    if (tool != nullptr)
        delete tool;

    tool = new Eraser(canvas);

    //custom cursor
    QCursor eraserCursor = QCursor(QPixmap(":/erase.png"),0,-50);
    setCursor(eraserCursor);
}

void MainWindow::on_findAndReplaceButton_clicked()
{
    if (tool != nullptr)
        delete tool;

    tool = new PaintAllSameColor(brushColor, canvas);

    //custom cursor
    QCursor findAndReplaceCursor = QCursor(QPixmap(":/brush.png"),0,-10);
    setCursor(findAndReplaceCursor);
}

void MainWindow::on_bucketButton_clicked()
{
    if (tool != nullptr)
        delete tool;

    tool = new Bucket(brushColor, canvas);

    //custom cursor
    QCursor bucketCursor = QCursor(QPixmap(":/bucket.png"),0,-10);
    setCursor(bucketCursor);
}

void MainWindow::on_colorPicker_clicked()
{
    if (tool != nullptr)
        delete tool;

    tool = new ColorPicker(brushColor, canvas);

    //custom cursor
    QCursor colorPicker = QCursor(QPixmap(":/colorPicker.png"),0,-70);
    setCursor(colorPicker);
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

    setCursor(Qt::PointingHandCursor);
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

    setCursor(Qt::PointingHandCursor);
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
    canvas->addFrame();
    addFramePreview();
    canvas->moveFrame(canvas->sizeFrame() - 1);
    repaint();
}

void MainWindow::on_deleteFrameButton_clicked()
{
    //If frame size is 1, ignore.
    if(canvas->sizeFrame() == 1)
        return;

    int currentIndex = canvas->currentIndex();

    //Get framePreview and delete from memory and layout.
    QImageButton *framePreview = framePreviews[currentIndex];
    layout()->removeWidget(framePreview);
    framePreviews.remove(currentIndex);
    delete framePreview;

    //Delete that frame.
    canvas->deleteFrame(currentIndex);

    //Move previous frame, if deleted farme's index is 0, move to 0.
    if(currentIndex == 0)
        canvas->moveFrame(0);
    else
        canvas->moveFrame(currentIndex - 1);

    repaint();
}


void MainWindow::on_framePriview_clicked()
{
    // Get which button is clicekd
    QImageButton* buttonSender = qobject_cast<QImageButton*>(sender());
    int index = buttonSender->objectName().split("-")[1].toInt();

    canvas->moveFrame(index);
    repaint();
}

QString MainWindow::getColorString(QColor color)
{
    std::string colorString = "background-color: rgba(" + std::to_string(color.red()) + ", " + std::to_string(color.green()) + ", " + std::to_string(color.blue()) + ", " + std::to_string(color.alpha()) + ");";
    return QString::fromStdString(colorString);
}

void MainWindow::on_saveButton_clicked()
{
    if (projectLocation.isEmpty())
        projectLocation = QDir::homePath() + "/" + "untitled";

    QString filter = "SIMP Project file (*.ssp);; PNG image file (*.png);; Graphics Interchange Format (*.gif)";
    QString filePath = QFileDialog::getSaveFileName(this, "Choose file to save", projectLocation, filter, &filter);

    if (filePath.isEmpty()) return;

    if (filePath.toLower().endsWith(".png"))
        ProjectManager::saveAsPng(canvas->getCurrentFrame(), filePath);
    else if (filePath.toLower().endsWith(".gif"))
        ProjectManager::saveAsGif(canvas, filePath, ui->spinDelay->value());
    else
    {
        ProjectManager::saveProject(&*canvas, filePath);
        projectLocation = filePath;
    }
}

void MainWindow::on_openButton_clicked()
{
    if (projectLocation.isEmpty())
        projectLocation = QDir::homePath() + "/" + "untitled.ssp";

    QString filter = "SIMP Project file (*.ssp)";
    QString filePath =  QFileDialog::getOpenFileName(this, "Choose file to open", projectLocation, filter, &filter);

    try {
        deinitalize();
        initialize(new Canvas(ProjectManager::openProject(filePath)));
        repaint();
    } catch (bool failed) {
        // TODO : Show failed to load project.
    }
}


void MainWindow::addFramePreview()
{
    //Error prevent.
    if(canvas->sizeFrame() == framePreviews.size()) return;

    QWidget *client = ui->framesScrollWidget;

    QImageButton *previewLabel = new QImageButton(client);
    previewLabel->setStyleSheet("border: 1px solid black");
    previewLabel->setFixedSize(PREVIEW_IMAGE_SIZE, PREVIEW_IMAGE_SIZE);
    previewLabel->setObjectName("previewLabel-" + QString::number(framePreviews.size()));
    connect(previewLabel, &QImageButton::clicked, this, &MainWindow::on_framePriview_clicked);

    frameGridLayout->addWidget(previewLabel);
    framePreviews.push_back(previewLabel);
}

void MainWindow::on_redoButton_clicked()
{
    canvas->getCurrentFrame()->redo();
    repaint();

    setCursor(Qt::PointingHandCursor);
}

void MainWindow::on_undoButton_clicked()
{
    canvas->getCurrentFrame()->undo();
    repaint();

    setCursor(Qt::PointingHandCursor);
}


