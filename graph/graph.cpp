#include "graph.h"
#include <QColor>
#include <QDebug>
#include <QFont>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPointF>
#include <QString>
#include <QtMath>
#include <algorithm>
#include <ctime>

enum class Colors { RED, BLACK, BLUE, AMAX };
Q_DECLARE_METATYPE(Colors);

const QVector<QColor> colors = {Qt::red, Qt::black, Qt::blue};

struct Rect {
  qreal ellipse_x, ellipse_y;
  qreal lineto_x, lineto_y;
};

Graph::Graph(QWidget *parent)
    : QWidget(parent), rect_length(70), rect_width(50) {}

Graph::Graph(QVector<QString> v, QWidget *parent)
    : QWidget(parent), rect_length(70), rect_width(50) {
  addVertex(v);
}

void Graph::paintEvent(QPaintEvent *e) {
  Q_UNUSED(e);

  QPainter qp(this);
  printVertex(&qp);
  countFreq();
}

void quickSort(QVector<QVector<qint32>>::iterator first,
               QVector<QVector<qint32>>::iterator last);

inline bool keyCheck(qint32 k) { return (k == 0) ? false : true; }

void Graph::addVertex(QVector<QString> &v) {
  for (QString i : v) {
    auto key = i.split("--")[0].toInt();
    auto value = i.split("--")[1].toInt();
    if (!keyCheck(key))
      exit(EXIT_FAILURE);
    if (value == 0)
      m_map[key];
    else if (value == key) {
      m_map[key].insert(value);
      m_map[key].insert(value);
    } else
      m_map[key].insert(value);
    if (key != value && value != 0)
      m_map[value].insert(key);
  }

  qint32 it = 0;
  m_sorted.resize(m_map.size());
  for (auto i : m_map) {
    m_sorted[it].push_back(i.first);
    for (auto j : i.second) {
      m_sorted[it].push_back(j);
    }
    it++;
  }

  quickSort(m_sorted.begin(), m_sorted.end());
}

void Graph::printVertex(QPainter *qp) {
  std::map<qint32, QPointF> vertex_coords, lineto_coords;
  std::map<qint32, QPainterPath> vertex_paths;
  QPainterPath path;
  qint32 idx_strt, idx_curr;

  srand(time(NULL));

  dataInit(qp, vertex_coords, lineto_coords, vertex_paths);

  for (qint32 i = 0; i < m_sorted.size(); ++i) {
    idx_strt = m_sorted.at(i).at(0);
    for (qint32 it = 1; it < m_sorted[i].size(); ++it) {
      idx_curr = m_sorted.at(i).at(it);
      if (idx_strt == idx_curr) {
        auto x = vertex_coords.at(idx_curr).x();
        auto y = vertex_coords.at(idx_curr).y();
        vertex_paths.at(idx_strt).moveTo(x, y + rect_width / 2);
        vertex_paths.at(idx_strt).arcTo(x, y - rect_width / 2, rect_length, 100,
                                        180, -180);
      } else if (idx_curr != 0 && idx_strt != 0) {
        vertex_paths.at(idx_strt).moveTo(lineto_coords[idx_strt]);
        vertex_paths.at(idx_strt).lineTo(lineto_coords[idx_curr]);
      }
    }

    qp->setPen(
        QPen(colors.at(static_cast<qint32>(Colors::BLACK)), 2, Qt::SolidLine));
    qp->drawPath(vertex_paths[m_sorted.at(i).at(0)]);
  }
}

void Graph::dataInit(QPainter *q, std::map<qint32, QPointF> &v,
                     std::map<qint32, QPointF> &l,
                     std::map<qint32, QPainterPath> &p) {
  Rect r;
  QPainterPath path;
  QPen line_style(colors.at(static_cast<qint32>(Colors::BLACK)), 2,
                  Qt::SolidLine);
  qint32 ind = 1;

  q->translate(width() * .05, height() * .05);
  q->setRenderHint(QPainter::Antialiasing);
  q->setPen(line_style);

  bool first = true;

  for (qint32 i = 0; i < m_sorted.size(); ++i) {

    if (first) {
      r = {width() * .4, 0, width() * .4 + rect_length / 2, (qreal)rect_width};
      first = false;
    } else {
      auto a = width() * .85 * (i % 2) + qPow(-1, i) * qPow(i, 2);
      auto b = (qreal)70 * i;
      r = {a, b, a + rect_length * ((i + 1) % 2), b + rect_width / 2};
    }

    auto idx = m_sorted.at(i).at(0);
    q->setFont(QFont("Times", 10));
    q->drawText(r.ellipse_x + rect_length, r.ellipse_y + rect_width * .2,
                QString::number(ind++));
    q->setFont(QFont("Times", 20));
    q->drawText(r.ellipse_x + .45 * rect_length, r.ellipse_y + rect_width * .55,
                QString::number(idx));
    path.moveTo(r.lineto_x, r.lineto_y);
    path.addEllipse(r.ellipse_x, r.ellipse_y, rect_length, rect_width);
    l[idx] = QPointF(r.lineto_x, r.lineto_y);
    v[idx] = QPointF(r.ellipse_x, r.ellipse_y);
    p[idx] = path;
  }
}

qint32 Graph::countFreq() {
  std::map<qint32, qint32> count;
  for (auto it : m_sorted) {
    count[it.size() - 1]++;
  }
  qDebug() << "=======";
  for (auto p : count)
    qDebug() << "Degree " << p.first << ": " << p.second << " times";
  qDebug() << "=======";
}

auto partition(QVector<QVector<qint32>>::iterator first,
               QVector<QVector<qint32>>::iterator last) {
  auto pivot = std::prev(last, 1);
  auto i = first;
  for (auto j = first; j != pivot; ++j) {
    if (j->size() > pivot->size()) {
      std::swap(*i++, *j);
    }
  }
  std::swap(*i, *pivot);
  return i;
}

void quickSort(QVector<QVector<qint32>>::iterator first,
               QVector<QVector<qint32>>::iterator last) {
  if (std::distance(first, last) > 1) {
    auto pivot = partition(first, last);
    quickSort(first, pivot);
    quickSort(pivot + 1, last);
  }
}
