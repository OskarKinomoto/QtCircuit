#ifndef CIRCUITWIDGET_H
#define CIRCUITWIDGET_H

#include <QtWidgets>

#include "circuit.h"
#include "include.h"

class CircuitScrollArea;

class CircuitWidget : public QWidget
{
  Q_OBJECT
private:
  Circuit * circuit;
  CircuitScrollArea * scroll;
  bool ignoreMouse = false;
public:
  explicit CircuitWidget(Circuit * _circuit, CircuitScrollArea *parent);
  ~CircuitWidget();

  inline void scaleUp() { circuit->scaleUp(); }
  inline void scaleDown() { circuit->scaleDown(); }
  inline void rotate() { circuit->rotate(); }
  inline void smallRotate() { circuit->smallRotate(); }
  inline void resetRotation() { circuit->resetRotation(); }

  inline void exportToPNG(QString path) { circuit->exportToPNG(path); }
  inline void saveFileAs(QString file) { circuit->saveFileAs(file); }

  inline void saveFile() { circuit->saveFile(); }
  inline bool hasPath() { return circuit->hasPath(); }
  inline bool isModyfied() { return circuit->isModyfied(); }
  inline bool isNew() { return circuit->newFile; }
  inline void destroyDrawingObject() { circuit->destroyDrawingObject(); }
  inline void updateInfoBox() { circuit->updateInfoBox(); }
  inline void undo() { circuit->undo(); }
  inline void redo() { circuit->redo(); }

  void centering();

  void updateTitle();
  void grabMouse();
  void releaseMouse();
  bool grabedMouse = false;
  void forceMouseMoveEvent();
private:
  friend class Circuit;
  QRect visibleRect = QRect(0,0,0,0);
  void updateName();
  void updateSize(bool i = true);
  bool inVisibleRect(int x, int y);
protected:
  void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
  void mouseMoveEvent ( QMouseEvent * event ) Q_DECL_OVERRIDE;
  void mouseReleaseEvent( QMouseEvent * event ) Q_DECL_OVERRIDE;
  void mousePressEvent( QMouseEvent * event ) Q_DECL_OVERRIDE;
  void mouseDoubleClickEvent ( QMouseEvent * );

signals:

public slots:
};

#endif // CIRCUITWIDGET_H
