#include "imagelabel.h"

#include <QMouseEvent>
#include <QPainter>

ImageLabel::ImageLabel(QWidget *parent, Qt::WindowFlags f)
    : QLabel(parent, f), pressed_(false), actived_(false) {}

ImageLabel::~ImageLabel() {}

void ImageLabel::setActive(bool actived) { this->actived_ = actived; }

void ImageLabel::mousePressEvent(QMouseEvent *ev) {
  if (ev->button() == Qt::LeftButton) {
    pressed_ = true;
  }
}

void ImageLabel::mouseReleaseEvent(QMouseEvent *ev) {
  if (ev->button() == Qt::LeftButton) {
    pressed_ = false;
    emit clicked();
  }
}

void ImageLabel::paintEvent(QPaintEvent *ev) {
  QLabel::paintEvent(ev);
  if (!actived_) {
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(0, 0, 0, 150));
    painter.drawRect(rect());
  }
}
