#include "carouseltest.h"
#include "carouselwidget.h"

#include <qdebug.h>

#pragma execution_character_set("utf-8")

carouselTest::carouselTest(QWidget* parent) : QMainWindow(parent) {

  CarouselWidget* cw = new CarouselWidget(this);
  cw->setNavPosition(Left);
  cw->setHoverStop(false);

  connect(cw, &CarouselWidget::clicked,
          [](int index) { qWarning() << "??ǰ??????" << index; });

  setCentralWidget(cw);

  for (int i = 0; i < 8; ++i)
    cw->addImage(QString("./images/Wall%1.png").arg(i));

  cw->setNavPosition(Right);
  adjustSize();

  cw->setNavStyle(NavStyle::Bar);
  cw->start();
}
