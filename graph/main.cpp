#include "graph.h"
#include "widget.h"
#include <QApplication>
#include <QDebug>
#include <QRegularExpression>
#include <QString>
#include <QTextStream>

bool parseInput(QString &s);

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QTextStream out(stdout);
  QTextStream inp(stdin);

  QString s;
  QVector<QString> v;
  out << "Enter a sentence: ";
  out.flush();
  s = inp.readLine();
  while (!s.isEmpty()) {
    out << "> ";
    out.flush();
    if (!parseInput(s)) {
      qDebug() << "Wrong input formate " << s;
      s = inp.readLine();
      continue;
    }
    v.push_back(s);
    s = inp.readLine();
  }

  Graph window(v);
  window.setWindowTitle("Graph");
  window.resize(1000, 800);
  window.setStyleSheet("background-color: white;");
  window.show();
  return a.exec();
}

bool parseInput(QString &s) {
  s = s.replace(" ", "");
  QRegularExpression re("(\\d+)--(\\d+)");
  QRegularExpressionMatch match = re.match(s);
  if (!match.hasMatch()) {
    return false;
  }
  return true;
}
