#include "carouseltest.h"
#include "carouselwidget.h"

#include <qdebug.h>

#pragma execution_character_set("utf-8")

carouselTest::carouselTest(QWidget* parent) : QMainWindow(parent) {
  // ui.setupUi(this);

  CarouselWidget* cw = new CarouselWidget(this);
  cw->setNavPosition(Left);
  cw->setHoverStop(false);

  connect(cw, &CarouselWidget::clicked,
          [](int index) { qWarning() << "µ±Ç°Ë÷Òý£º" << index; });

  setCentralWidget(cw);

  for (int i = 0; i < 8; ++i)
    cw->setImageNames(
        QString(R"(D:\work\test\qt\carouselTest\images\Wall%1.png)").arg(i));

   cw->setNavPosition(Right);
  adjustSize();

  cw->setNavStyle(NavStyle::Dot);
  cw->start();
}
