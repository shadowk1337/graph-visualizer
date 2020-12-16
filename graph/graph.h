#ifndef GRAPH_H
#define GRAPH_H
#include <QVector>
#include <QWidget>
#include <map>
#include <set>

class Graph : public QWidget {
public:
  explicit Graph(QWidget * = 0);
  explicit Graph(QVector<QString>, QWidget * = 0);
  void dataInit(QPainter *, std::map<qint32, QPointF> &,
                std::map<qint32, QPointF> &, std::map<qint32, QPainterPath> &);
  qint32 countFreq();
  friend void quickSort(QVector<QVector<qint32>>::iterator,
                        QVector<QVector<qint32>>::iterator);

protected:
  void addVertex(QVector<QString> &);
  void printVertex(QPainter *);
  void paintEvent(QPaintEvent *);

private:
  std::map<qint32, std::multiset<qint32>> m_map;
  QVector<QVector<qint32>> m_sorted;
  const qint32 rect_length, rect_width;
};

#endif // GRAPH_H
