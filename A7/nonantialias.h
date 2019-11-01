#ifndef NONANTIALIAS_H
#define NONANTIALIAS_H
#include <QWidget>
#include <QPainter>
#include <QStyle>


class nonantialias : public QWidget{
    Q_OBJECT
    Q_DISABLE_COPY(nonantialias)
public:
    explicit nonantialias(QWidget* parent = Q_NULLPTR)
        : QWidget(parent)
    {}
    const QPixmap& pixmap() const;
    void setPixmap(const QPixmap& px);
protected:
    void paintEvent(QPaintEvent*);
private:
    QPixmap m_pixmap;
};

#endif // NONANTIALIAS_H
