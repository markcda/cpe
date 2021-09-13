#ifndef ACLOCK_H
#define ACLOCK_H

#include <QPaintEvent>
#include <QPainter>
#include <QTime>
#include <QTimer>
#include <QWidget>

class AnalogClock : public QWidget {
  Q_OBJECT

public:
  AnalogClock(QTime ctime, QWidget *parent = nullptr)
      : QWidget(parent), _ctime(ctime) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
      _ctime = _ctime.addMSecs(1000);
      update();
      timer->start(1000);
    });
    timer->start(1000);
    resize(200, 200);
  }

  ~AnalogClock() { delete timer; }

protected:
  void paintEvent(QPaintEvent *event) {
    static const QPoint hourHand[3] = {QPoint(7, 8), QPoint(-7, 8),
                                       QPoint(0, -40)};
    static const QPoint minuteHand[3] = {QPoint(7, 8), QPoint(-7, 8),
                                         QPoint(0, -70)};

    QColor rideColor(0, 0, 0);
    QColor greyColor("#dedede");

    int side = qMin(width(), height());
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);
    
    painter.setPen(Qt::NoPen);
    painter.setBrush(greyColor);
    painter.save();
    painter.drawEllipse(-100, -100, 200, 200);
    
    painter.setPen(Qt::NoPen);
    painter.setBrush(rideColor);
    painter.save();
    painter.rotate(30.0 * ((_ctime.hour() + _ctime.minute() / 60.0)));
    painter.drawConvexPolygon(hourHand, 3);
    painter.restore();
    painter.setPen(rideColor);

    for (int i = 0; i < 12; ++i) {
      painter.drawLine(88, 0, 96, 0);
      painter.rotate(30.0);
    }
    painter.setPen(Qt::NoPen);
    painter.setBrush(rideColor);

    painter.save();
    painter.rotate(6.0 * (_ctime.minute() + _ctime.second() / 60.0));
    painter.drawConvexPolygon(minuteHand, 3);
    painter.restore();
    painter.setPen(rideColor);

    for (int j = 0; j < 60; ++j) {
      if ((j % 5) != 0)
        painter.drawLine(92, 0, 96, 0);
      painter.rotate(6.0);
    }
    
    painter.setPen(Qt::NoPen);
    painter.setBrush(greyColor);
    painter.save();
    painter.drawEllipse(-2, -2, 4, 4);
    
    event->accept();
  }

private:
  QTimer *timer = nullptr;
  QTime _ctime;
};

#endif // ACLOCK_H
