#include "carouselwidget.h"

#include <qcolor.h>
#include <qdebug.h>
#include <qmutex.h>
#include <qpainter.h>
#include <qtimer.h>
#include <qevent.h>
#include <qlabel.h>

#include <QHBoxLayout>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QMouseEvent>

#pragma execution_character_set("utf-8")

CarouselWidget::CarouselWidget(QWidget* parent) : QWidget(parent) {
  initWidget();
  initForm();
  initQss();
}

CarouselWidget::~CarouselWidget() {
  if (timer->isActive()) {
    timer->stop();
  }

  for (auto lab : labs) {
    lab->removeEventFilter(this);
  }
  qDeleteAll(labs);
}

bool CarouselWidget::getHoverStop() const { return hoverStop; }

bool CarouselWidget::getShowNumber() const { return showNumber; }

IndicatorProperty CarouselWidget::getIndicatorProperty() const {
  auto ip = this->ip_.get();
  return *ip;
}

int CarouselWidget::getInterval() const { return intervalGap; }

NavPosition CarouselWidget::getNavPosition() const { return navPosition; }

NavStyle CarouselWidget::getNavStyle() const { return navStyle; }

QSize CarouselWidget::sizeHint() const { return QSize(640, 360); }

QSize CarouselWidget::minimumSizeHint() const { return QSize(640, 360); }

void CarouselWidget::start() {
  if (!timer->isActive()) {
    timer->start(intervalGap);
  }
}

void CarouselWidget::setShowNumber(bool showNumber) {
  this->showNumber = showNumber;
}

void CarouselWidget::setInterval(int interval) {
  this->intervalGap = interval;
  if (timer->isActive()) {
    timer->stop();
    timer->start(interval);
  }
}

void CarouselWidget::setIndicatorProperty(IndicatorProperty& ip) {
  this->ip_.reset(&ip);
}

void CarouselWidget::addImage(const QString& imageNames) {
  names.append(imageNames);

  auto lab = new QLabel;
  labs.append(lab);
  // lab->setStyleSheet(labs.count() == 1 ? qssCurrent : qssNormal);
  if (labs.count() == 1) {
    lab->setStyleSheet("background-color:red");
    lab->setFixedWidth(ip_->indicatorMaxWidth);
  } else {
    lab->setStyleSheet("background-color:green");
    lab->setFixedWidth(ip_->indicatorMinWidth);
  }
  // lab->setFixedHeight(ip_->indicatorMaxHeight);

  indicatorLayout->insertWidget(leftExists ? labs.size() : labs.size() - 1,
                                lab);
  lab->installEventFilter(this);
}

void CarouselWidget::setNavPosition(const NavPosition& navPosition) {
  this->navPosition = navPosition;

  if (navPosition == NavPosition::Center) {
    if (!leftExists) indicatorLayout->insertSpacerItem(0, spacerLeft);
    if (!rightExists) indicatorLayout->addItem(spacerRight);
  } else if (navPosition == NavPosition::Left) {
    if (!rightExists) indicatorLayout->addItem(spacerRight);
    indicatorLayout->removeItem(spacerLeft);
    leftExists = false;
  } else if (navPosition == NavPosition::Right) {
    if (!leftExists) indicatorLayout->insertSpacerItem(0, spacerLeft);
    indicatorLayout->removeItem(spacerRight);
    rightExists = false;
  }
}

void CarouselWidget::setNavStyle(const NavStyle& navStyle) {
  bool needRestart = false;
  if (timer->isActive()) {
    timer->stop();
    needRestart = true;
  }

  this->navStyle = navStyle;

  if (navStyle != NavStyle::Bar) {
    initQss();
    for (int i = 0; i < labs.size(); ++i) {
      auto lab = labs.at(i);
      lab->setFixedSize(ip_->indicatorMaxWidth, ip_->indicatorMaxHeight);
      lab->setStyleSheet(i == currentIndex ? qssCurrent : qssNormal);
    }
  } else {
    ip_->indicatorRadius = 3;
    initQss();
    for (int i = 0; i < labs.size(); ++i) {
      auto lab = labs.at(i);
      lab->setFixedHeight(ip_->indicatorMinHeight);
      lab->setStyleSheet(i == currentIndex ? qssCurrent : qssNormal);
    }
  }

  if (needRestart) {
    timer->start(intervalGap);
  }
}

void CarouselWidget::setHoverStop(bool hoverStop) {
  this->hoverStop = hoverStop;
}

bool CarouselWidget::eventFilter(QObject* obj, QEvent* ev) {
  auto it = std::find_if(labs.begin(), labs.end(),
                         [obj](auto ptr) { return ptr == obj; });

  if (it != labs.end()) {
    if (ev->type() == QEvent::MouseButtonRelease) {
      auto mouseEvent = static_cast<QMouseEvent*>(ev);
      if (mouseEvent->button() == Qt::LeftButton) {
        int index = it - labs.begin();
        if (index != currentIndex) {
          previousIndex = currentIndex;
          currentIndex = index;
          changedAds(labs.at(currentIndex));
        }
      }
    } else if (ev->type() == QEvent::Enter) {
      if (timer->isActive()) {
        timer->stop();
      }

      /*int index = it - labs.begin();
      if (index != currentIndex) {
        previousIndex = currentIndex;
        currentIndex = index;
        changedAds(labs.at(currentIndex));
      }*/
    } else if (ev->type() == QEvent::Leave) {
      if (!timer->isActive()) {
        timer->start(intervalGap);
      }
    }
  }

  return false;
}

void CarouselWidget::enterEvent(QEvent*) {
  if (timer->isActive() && hoverStop) {
    timer->stop();
  }
}

void CarouselWidget::leaveEvent(QEvent*) {
  if (!timer->isActive() && hoverStop) {
    timer->start(intervalGap);
  }
}

void CarouselWidget::showEvent(QShowEvent*) {
  if (!timer->isActive()) {
    timer->start(intervalGap);
  }
}

void CarouselWidget::hideEvent(QHideEvent* event) {
  qDebug() << "hide!!!";
  if (timer->isActive()) {
    timer->stop();
  }
}

void CarouselWidget::paintEvent(QPaintEvent*) {
  if (names.count() == 0) {
    return;
  }

  int width = this->width();
  int height = this->height();

  QPainter painter(this);
  painter.setRenderHints(QPainter::SmoothPixmapTransform |
                         QPainter::TextAntialiasing);
  QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
  painter.setPen(ip_->indicatorTipColor);

  if (isResizing) {
    QPixmap currentPix(names.at(currentIndex));
    currentPix = currentPix.scaled(width, height, Qt::IgnoreAspectRatio,
                          Qt::SmoothTransformation);
    painter.drawPixmap(0, 0, currentPix);
    isResizing = false;
    timer->start();
  } else {
    //????????
    QFont font;
    font.setPixelSize(15);
    painter.setFont(font);

    //??????????????+????????,??????????
    QPixmap previousPix(names.at(previousIndex));
    QPixmap currentPix(names.at(currentIndex));
    previousPix = previousPix.scaled(width, height, Qt::IgnoreAspectRatio,
                           Qt::SmoothTransformation);
    currentPix = currentPix.scaled(width, height, Qt::IgnoreAspectRatio,
                          Qt::SmoothTransformation);

    int widthOffset = offset + width;
    if (leftToRight) {
      widthOffset = offset - width;
    }

    //??????????????
    painter.drawPixmap(offset, 0, previousPix);
    //????????????
    painter.drawPixmap(widthOffset, 0, currentPix);

    //??????????????????????,????????????????????????????
    if (previousIndex <= tips.count() - 1) {
      painter.drawText(QRect(offset + 10, height - ip_->indicatorMinHeight - 40,
                             width - 20, 30),
                       tips.at(previousIndex), option);
    }

    //????????????????????,??????????????????????????
    if (currentIndex <= tips.count() - 1) {
      painter.drawText(
          QRect(widthOffset + 10, height - ip_->indicatorMinHeight - 40,
                width - 20, 30),
          tips.at(currentIndex), option);
    }
  }
}

void CarouselWidget::resizeEvent(QResizeEvent* event) {
  isResizing = true;
  timer->stop();
  update();
}

void CarouselWidget::mouseReleaseEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton && this->underMouse()) {
    emit clicked(currentIndex);
  }
}

void CarouselWidget::initWidget() {
  //????????????????????
  indicatorContainer = new QWidget(this);
  indicatorContainer->setObjectName(QString::fromUtf8("indicatorContainer"));

  //????????????????????????
  indicatorLayout = new QHBoxLayout(indicatorContainer);
  indicatorLayout->setSpacing(3);

  //??????,????????
  QVBoxLayout* verticalLayout = new QVBoxLayout(this);
  verticalLayout->setSpacing(0);
  verticalLayout->setContentsMargins(0, 0, 0, 0);

  //????????,????????????????????????
  QSpacerItem* verticalSpacer =
      new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
  verticalLayout->addItem(verticalSpacer);

  //??????????????????????????????
  verticalLayout->addWidget(indicatorContainer);

  //??????????????????
  spacerLeft =
      new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
  spacerRight =
      new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);

  indicatorLayout->addSpacerItem(spacerLeft);
  indicatorLayout->addSpacerItem(spacerRight);
}

void CarouselWidget::initForm() {
  hoverStop = true;
  showNumber = false;

  ip_.reset(new IndicatorProperty);

  ip_->indicatorMinHeight = 6;
  ip_->indicatorMaxHeight = 24;
  ip_->indicatorMinWidth = 6;
  ip_->indicatorMaxWidth = 24;
  ip_->indicatorRadius = 12;
  intervalGap = 3000;

  ip_->indicatorColor = QColor(220, 220, 220);
  ip_->indicatorTextColor = QColor(20, 20, 20);
  ip_->indicatorTipColor = QColor(255, 255, 255);
  ip_->indicatorCurrentColor = QColor(255, 255, 255);

  navPosition = Left;
  navStyle = Bar;

  leftToRight = true;
  offset = 0;
  currentIndex = 0;
  previousIndex = 0;

  //??????????????
  timer = new QTimer(this);
  timer->setInterval(intervalGap);
  connect(timer, SIGNAL(timeout()), this, SLOT(changedAds()));

  this->setMouseTracking(true);

  //??????????
  animationGroup = new QParallelAnimationGroup(this);

  //????????????????
  animationImage = new QPropertyAnimation(this, "");
  connect(animationImage, SIGNAL(valueChanged(const QVariant&)), this,
          SLOT(changedImage(const QVariant&)));
  animationImage->setEasingCurve(QEasingCurve::OutCirc);
  animationImage->setDuration(1000);
  animationGroup->addAnimation(animationImage);

  //??????????????????
  animationIndicator = new QPropertyAnimation(this, "");
  connect(animationIndicator, SIGNAL(valueChanged(const QVariant&)), this,
          SLOT(changeIndicator(const QVariant&)));
  animationIndicator->setEasingCurve(QEasingCurve::OutCubic);
  animationIndicator->setDuration(500);

  animationGroup->addAnimation(animationIndicator);
}

void CarouselWidget::initQss() {
  //????????????????????
  if (navStyle == Dot) {
    qssNormal =
        QString(
            "border:none;background:qradialgradient(spread:pad,cx:0.5,cy:0.5,"
            "radius:0.5,fx:0.5,fy:0.5,"
            "stop:0 #00FFFFFF,stop:0.4 #00FFFFFF,"
            "stop:0.5 #00FFFFFF,stop:0.6 #00FFFFFF,stop:0.7 rgba(%1,%2,%3,%4));"
            "color:rgba(%5,%6,%7,%8);border-radius:%9px;")
            .arg(ip_->indicatorColor.red())
            .arg(ip_->indicatorColor.green())
            .arg(ip_->indicatorColor.blue())
            .arg(ip_->indicatorColor.alpha())
            .arg(ip_->indicatorTextColor.red())
            .arg(ip_->indicatorTextColor.green())
            .arg(ip_->indicatorTextColor.blue())
            .arg(ip_->indicatorColor.alpha())
            .arg(ip_->indicatorRadius);
    qssCurrent =
        QString(
            "border:none;background:qradialgradient(spread:pad,cx:0.5,cy:0.5,"
            "radius:0.5,fx:0.5,fy:0.5,"
            "stop:0 rgba(%1,%2,%3,%4),stop:0.4 rgba(%1,%2,%3,%4),"
            "stop:0.5 #00FFFFFF,stop:0.6 #00FFFFFF,stop:0.7 rgba(%1,%2,%3,%4));"
            "color:rgba(%5,%6,%7,%8);border-radius:%9px;")
            .arg(ip_->indicatorCurrentColor.red())
            .arg(ip_->indicatorCurrentColor.green())
            .arg(ip_->indicatorCurrentColor.blue())
            .arg(ip_->indicatorCurrentColor.alpha())
            .arg(ip_->indicatorTextColor.red())
            .arg(ip_->indicatorTextColor.green())
            .arg(ip_->indicatorTextColor.blue())
            .arg(ip_->indicatorColor.alpha())
            .arg(ip_->indicatorRadius);

  } else {
    qssNormal = QString(
                    "border:none;background:rgba(%1,%2,%3,%4);color:rgba(%5,%6,"
                    "%7,%8);border-radius:%9px;")
                    .arg(ip_->indicatorColor.red())
                    .arg(ip_->indicatorColor.green())
                    .arg(ip_->indicatorColor.blue())
                    .arg(ip_->indicatorColor.alpha())
                    .arg(ip_->indicatorTextColor.red())
                    .arg(ip_->indicatorTextColor.green())
                    .arg(ip_->indicatorTextColor.blue())
                    .arg(ip_->indicatorColor.alpha())
                    .arg(ip_->indicatorRadius);
    qssCurrent = QString(
                     "border:none;background:rgba(%1,%2,%3,%4);color:rgba(%5,%"
                     "6,%7,%8);border-radius:%9px;")
                     .arg(ip_->indicatorCurrentColor.red())
                     .arg(ip_->indicatorCurrentColor.green())
                     .arg(ip_->indicatorCurrentColor.blue())
                     .arg(ip_->indicatorCurrentColor.alpha())
                     .arg(ip_->indicatorTextColor.red())
                     .arg(ip_->indicatorTextColor.green())
                     .arg(ip_->indicatorTextColor.blue())
                     .arg(ip_->indicatorTextColor.alpha())
                     .arg(ip_->indicatorRadius);
  }
}

void CarouselWidget::changedAds() {
  if (names.count() == 0) {
    return;
  }

  previousIndex = currentIndex;
  if (currentIndex < names.count() - 1) {
    currentIndex++;
  } else {
    currentIndex = 0;
  }

  changedAds(labs.at(currentIndex));
}

void CarouselWidget::changedAds(QLabel* lab) {
  if (navStyle == NavStyle::Bar) {
    for (auto ele : labs) {
      /*ele->setStyleSheet(
          QString("background-color:%1").arg(ele == lab ? "red" : "green"));*/
       ele->setStyleSheet(ele == lab ? qssCurrent : qssNormal);
    }

    animationImage->setStartValue(0);
    animationImage->setEndValue(this->width());

    animationIndicator->setStartValue(ip_->indicatorMaxWidth);
    animationIndicator->setEndValue(ip_->indicatorMinWidth);

    animationImage->start();
    animationIndicator->start();
  } else {
    for (auto ele : labs) {
       ele->setStyleSheet(ele == lab ? qssCurrent : qssNormal);
    }

    animationImage->setStartValue(0);
    animationImage->setEndValue(this->width());

    animationImage->start();
  }
}

void CarouselWidget::changedImage(const QVariant& var) {
  offset = var.toInt();

  update();
}

void CarouselWidget::changeIndicator(const QVariant& var) {
  static int length = ip_->indicatorMinWidth + ip_->indicatorMaxWidth;
  int width = var.toInt();
  labs.at(previousIndex)->setFixedWidth(width);
  labs.at(currentIndex)->setFixedWidth(length - width);
}
