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
  Left,    //���
  Center,  //�м�
  Right    //�Ҳ�
};

enum NavStyle {
  Ellipse,  //��Բ
  Circle,   //Բ��
  Rect,     //����
  Dot,      //��
  Bar       //��״
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
  //���������ֹͣͣ�ֲ�
  void setHoverStop(bool hoverStop);
  //�����Ƿ���ʾ���
  void setShowNumber(bool showNumber);

  //�����л����
  void setInterval(int interval);
  void setIndicatorProperty(IndicatorProperty& ip);

  //����ͼƬ����
  void setImageNames(const QString& imageNames);
  //������ʾ��Ϣ
  void setImageTips(const QString& imageTips);

  //����ָʾ��λ��
  void setNavPosition(const NavPosition& navPosition);
  //����ָʾ����ʽ
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

  QString imageNames;  //ͼƬ����
  QString imageTips;   //��ʾ��Ϣ

  NavPosition navPosition;  //ָʾ��λ��
  NavStyle navStyle;        //ָʾ����ʽ

  QList<QString> names;  //չʾ��ͼƬ·��
  QList<QString> tips;   //��ʾ��
  QList<QLabel*> labs;   //ָʾ������

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
