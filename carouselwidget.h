#pragma once

#include <QWidget>

class QLabel;
class QHBoxLayout;
class QSpacerItem;
class QParallelAnimationGroup;
class QPropertyAnimation;
class ImageLabel;

struct IndicatorProperty {
  int indicatorMinHeight;
  int indicatorMaxHeight;
  int indicatorMinWidth;
  int indicatorMaxWidth;
  int indicatorRadius;

  QColor indicatorColor;
  QColor indicatorTextColor;
  QColor indicatorTipColor;
  QColor indicatorCurrentColor;
};

enum NavPosition {
  Left,    //左侧
  Center,  //中间
  Right    //右侧
};

enum NavStyle {
  Ellipse,  //椭圆
  Circle,   //圆形
  Rect,     //矩形
  Dot,      //点
  Bar       //条状
};

class CarouselWidget : public QWidget {
  Q_OBJECT
  Q_ENUMS(NavPosition)
  Q_ENUMS(NavStyle)

  Q_PROPERTY(bool hoverStop READ getHoverStop WRITE setHoverStop)
  Q_PROPERTY(bool showNumber READ getShowNumber WRITE setShowNumber)

  Q_PROPERTY(IndicatorProperty indicatorProperty READ getIndicatorProperty WRITE
                 setIndicatorProperty)

  Q_PROPERTY(int interval READ getInterval WRITE setInterval)

  Q_PROPERTY(QString imageNames READ getImageNames WRITE setImageNames)
  Q_PROPERTY(QString imageTips READ getImageTips WRITE setImageTips)

  Q_PROPERTY(NavPosition navPosition READ getNavPosition WRITE setNavPosition)
  Q_PROPERTY(NavStyle navStyle READ getNavStyle WRITE setNavStyle)

 public:
  CarouselWidget(QWidget* parent = nullptr);
  ~CarouselWidget();

 public:
  bool getHoverStop() const;
  bool getShowNumber() const;

  IndicatorProperty getIndicatorProperty() const;
  int getInterval() const;

  QString getImageNames() const;
  QString getImageTips() const;

  NavPosition getNavPosition() const;
  NavStyle getNavStyle() const;

  QSize sizeHint() const;
  QSize minimumSizeHint() const;

 public slots:
  //设置鼠标悬停停止轮播
  void setHoverStop(bool hoverStop);
  //设置是否显示序号
  void setShowNumber(bool showNumber);

  //设置切换间隔
  void setInterval(int interval);
  void setIndicatorProperty(IndicatorProperty& ip);

  //设置图片名称
  void setImageNames(const QString& imageNames);
  //设置提示信息
  void setImageTips(const QString& imageTips);

  //设置指示器位置
  void setNavPosition(const NavPosition& navPosition);
  //设置指示器样式
  void setNavStyle(const NavStyle& navStyle);

 protected:
  bool eventFilter(QObject* obj, QEvent* event);
  void enterEvent(QEvent*);
  void leaveEvent(QEvent*);
  void showEvent(QShowEvent*);

  virtual void hideEvent(QHideEvent* event) override;

  void paintEvent(QPaintEvent*);

  virtual void resizeEvent(QResizeEvent* event) override;

  virtual void mouseReleaseEvent(QMouseEvent* event) override;

 private:
  bool hoverStop;
  bool showNumber;

  QScopedPointer<IndicatorProperty> ip_;

  int intervalGap;

  QString imageNames;  //图片名称
  QString imageTips;   //提示信息

  NavPosition navPosition;  //指示器位置
  NavStyle navStyle;        //指示器样式

  QList<QString> names;  //展示的图片路径
  QList<QString> tips;   //提示语
  QList<QLabel*> labs;   //指示器集合

 private:
  bool leftToRight;
  int offset;
  int currentIndex;
  int previousIndex;

  QString qssNormal;
  QString qssCurrent;

  QTimer* timer;
  QHBoxLayout* indicatorLayout;
  QSpacerItem* spacerLeft;
  QSpacerItem* spacerRight;
  QWidget* indicatorContainer;

  QParallelAnimationGroup* animationGroup;
  QPropertyAnimation* animationImage;
  QPropertyAnimation* animationIndicator;

 private:
  bool leftExists = true;
  bool rightExists = true;
  bool isResizing = false;

 private slots:
  void initWidget();
  void initForm();
  void initQss();
  void changedAds();
  void changedAds(QLabel* lab);
  void changedImage(const QVariant&);
  void changedMin(const QVariant&);

 Q_SIGNALS:
  void clicked(int index);
};
