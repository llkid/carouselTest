#pragma once

#include <QLabel>

class ImageLabel : public QLabel {
  Q_OBJECT

 public:
  ImageLabel(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
  ~ImageLabel();

 public:
  void setActive(bool actived);

 signals:
  void clicked();

 protected:
  virtual void mousePressEvent(QMouseEvent *ev) override;
  virtual void mouseReleaseEvent(QMouseEvent *ev) override;

  virtual void paintEvent(QPaintEvent *ev) override;

 private:
  bool pressed_;
  bool actived_;
};
