#ifndef CIRCUITOPAMP_H
#define CIRCUITOPAMP_H

#include "circuitobject.h"

class CircuitOpAmp : public CircuitObject
{
public:
  CircuitOpAmp(Coordinate begin, float scale, quint16 rotation, quint32 num);
  CircuitOpAmp(QDataStream& in);
  ~CircuitOpAmp();
  virtual void draw(QPainter&p, float scale);
  virtual bool save(QDataStream& out);
  virtual void smallRotate(){}
  virtual bool isSmallRotate() { return false; }
  static QFont oxy;
  virtual K::info info();
};

#endif // CIRCUITOPAMP_H
