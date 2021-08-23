#include "fannerarrow.h"

#include <qpainter.h>

#include <QMouseEvent>

FannerArrow::FannerArrow(ArrowType type, QWidget *parent)
    : QWidget(parent),
      mouse_hover_(false),
      mouse_pressed_(false), type_(type) {
  setFixedSize(12, 20);
}

FannerArrow::~FannerArrow() {}

void FannerArrow::mousePressEvent(QMouseEvent *ev) {
  if (ev->button() == Qt::LeftButton) {
    mouse_pressed_ = true;
  }
}

void FannerArrow::mouseReleaseEvent(QMouseEvent *ev) {
  if (ev->button() == Qt::LeftButton) {
    mouse_pressed_ = false;
    emit clicked();
  }
}

void FannerArrow::enterEvent(QEvent */*event*/) {
  mouse_hover_ = true;
  update();
}

void FannerArrow::leaveEvent(QEvent * /*event*/) {
  mouse_hover_ = false;
  update();
}

void FannerArrow::paintEvent(QPaintEvent *ev) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);
  QPen pen;
  pen.setWidth(3);
  if (mouse_hover_) {
    pen.setBrush(QColor(255, 255, 255));
  } else {
    pen.setBrush(QColor(150, 150, 150));
  }
  painter.setPen(pen);
  int margin = 2;
  switch (type_) {
    case Right: {
      painter.drawLine(QPointF(margin, margin),
                       QPointF(this->width() - margin, this->height() / 2));
      painter.drawLine(QPointF(margin, this->height() - margin),
                       QPointF(this->width() - margin, this->height() / 2));
    } break;
    case Left: {
      painter.drawLine(QPointF(this->width() - margin, margin),
                       QPointF(margin, this->height() / 2));
      painter.drawLine(QPointF(this->width() - margin, this->height() - margin),
                       QPointF(margin, this->height() / 2));
    } break;
  }
  QWidget::paintEvent(ev);
}
