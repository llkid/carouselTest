#pragma once

#include <QWidget>

class FannerArrow : public QWidget {
  Q_OBJECT

 public:
  enum ArrowType { Left, Right };
  FannerArrow(ArrowType type, QWidget *parent = 0);
  ~FannerArrow();

 signals:
  void clicked();

 protected:
  virtual void mousePressEvent(QMouseEvent *ev) override;
  virtual void mouseReleaseEvent(QMouseEvent *ev) override;

  virtual void enterEvent(QEvent *event) override;

  virtual void leaveEvent(QEvent *event) override;

  virtual void paintEvent(QPaintEvent *ev) override;

 private:
  bool mouse_hover_;
  bool mouse_pressed_;
  ArrowType type_;
};
