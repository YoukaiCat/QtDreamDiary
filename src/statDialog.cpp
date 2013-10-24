/***************************************************************************
 *   Copyright (C) 2008 by Łukasz Chodyła   *
 *   chodak166@op.pl   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "statDialog.h"

statDialog::statDialog(const QList < DreamDocument > *docList)
{
  ui.setupUi(this);
  int sum = 0;
  weekEnable = false;
  dd = NULL;
  scoredList = new QList < DreamDocument >;
  docsList = docList;
  connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(close()));
  connect(ui.tableButton, SIGNAL(clicked()), this, SLOT(showDetailDialog()));

  allDocs = docList->count();	// WSZYSTKICH SNOW OGOLNIE

  lucidDocs = 0;
  for(int i = 0; i < docList->count(); i++)
    if(docList->at(i).lucid)
      lucidDocs++;		// SNOW SWIADOMYCH
  // przepisujemy elementy
  for(int i = 0; i < docList->count(); i++)
    scoredList->push_back(docList->at(i));
  // usuwamy nieocenione dokumenty
  for(int i = 0; i < scoredList->count(); i++)
    if((scoredList->at(i).sleep == scoredList->at(i).wakeup) ||
       (scoredList->at(i).score == 0))
    {
      scoredList->removeAt(i);
      i--;
    }
  allMax = 1;
  allMin = 10;

  // statystyki dla ocenionych
  for(int i = 0; i < scoredList->count(); i++)
  {
    if(scoredList->at(i).score > allMax)
      allMax = scoredList->at(i).score;	// NAJWYZSZA OCENA OGOLNIE
    if(scoredList->at(i).score < allMin)
      allMin = scoredList->at(i).score;	// NAJNIZSZA OCENA OGOLNIE

    sum += scoredList->at(i).score;
  }
  allScored = scoredList->count();	// OCENIONYCH SNOW OGOLNIE
  allAvg = sum / allScored;	// SREDNIA OCEN OGOLNIE 

  // sortowanie (rosnaco)
  for(int i = 0; i < scoredList->count(); i++)
    for(int j = i + 1; j < scoredList->count(); j++)
      if(scoredList->at(i).date > scoredList->at(j).date)
	scoredList->swap(i, j);
  // zostawiamy po jednym z kazdej daty
  for(int i = 0; i < scoredList->count(); i++)
    for(int j = i + 1; j < scoredList->count(); j++)
      if(scoredList->at(i).date == scoredList->at(j).date)
	if(scoredList->at(i).score > scoredList->at(j).score)
	{
	  scoredList->removeAt(j);
	  j--;
	}			// cofamy sie aby nie przeskoczyc elementu
	else
	{
	  scoredList->removeAt(i);
	  i--;
	  break;
	}			// cofamy sie aby nie przeskoczyc elementu

  // wypelniamy dane:
  QString n;
  n.setNum(allDocs);
  ui.label->setText(tr("wszystkich snów: ") + n);
  n.setNum(lucidDocs);
  ui.label_6->setText(tr("świadomych snów: ") + n);
  n.setNum(allScored);
  ui.label_2->setText(tr("ocenionych snów: ") + n);
  n.setNum(allAvg);
  ui.label_3->setText(tr("średnia ocen: ") + n + "/10");
  n.setNum(allMax);
  ui.label_4->setText(tr("najwyższa ocena: ") + n + "/10");
  n.setNum(allMin);
  ui.label_5->setText(tr("najniższa ocena: ") + n + "/10");
  n.setNum(scoredList->count());
  ui.groupBox_2->setTitle(tr("Dane na podstawie ") + n + tr(" dni"));
  // udostepniamy grafik jezeli jest 7 roznych dat
  if(scoredList->count() >= 7)
  {
    weekEnable = true;
    ui.label_7->hide();
    // zostawiamy tylko 7 ostatnich dni
    while(scoredList->count() != 7)
      scoredList->removeAt(0);
  }

  /*
   * //wyswietlanie starej listy std::cout<<"stara
   * lista:"<<std::wakeupl; for(int i=0;i<docList->count();i++)
   * std::cout<<docList->at(i).date.toString().toStdString()<<std::wakeupl;
   * //wyswietlanie nowej listy std::cout<<"nowa lista:"<<std::wakeupl;
   * for(int i=0;i<scoredList->count();i++)
   * std::cout<<scoredList->at(i).date.toString().toStdString()<<std::wakeupl; 
   */

}

statDialog::~statDialog()
{
  scoredList->clear();
  delete scoredList;
}

// -----------------------------------------------------------------------------------------------

void statDialog::paintEvent(QPaintEvent *)
{
  if(weekEnable)
  {
    int scr[7];
    int slp[7];
    // tablica punktow:
    for(int i = 0; i < 7; i++)
      scr[i] = scoredList->at(i).score;
    // tablica godzin snu:
    for(int ss, sh, sm, i = 0; i < 7; i++)
    {

      ss = scoredList->at(i).sleep.secsTo(scoredList->at(i).wakeup);
      ss /= 60;

      sh = ss / 60;		// godziny
      sm = ss % 60;		// minuty

      if(sh < 0)
	sh += 24;
      if(sm < 0)
	sm += 60;

      if(sm >= 30)
	sh++;
      if((sh > 12) || (sh < 1))
	sh = 12;
      slp[i] = sh;

    }
    // ===================================================================
    int x = ui.groupBox->x() + ui.frame->x();
    int y = ui.groupBox->y() + ui.frame->y();
    int w = ui.frame->width();
    int h = ui.frame->height();

    QPainter paint(this);
    QPen pen;
    pen.setColor(QColor(0xdd, 0xdd, 0xdd));
    paint.setPen(pen);
    paint.fillRect(x, y, w, h, QBrush(QColor(0, 0, 0)));

    drawGraph(x, y + 20, w, h - 50, scr, slp);
    paint.drawText(x + w - 50, y + h - 50, tr("data"));
    paint.drawText(x + 5, y + 15, tr("godz./ocena"));
    pen.setColor(QColor(0, 255, 0));
    pen.setWidth(2);
    paint.setPen(pen);
    paint.drawLine(x + 20, y + h - 40, x + 60, y + h - 40);
    pen.setColor(QColor(255, 0, 0));
    paint.setPen(pen);
    paint.drawLine(x + 20, y + h - 20, x + 60, y + h - 20);

    pen.setColor(QColor(255, 255, 255));
    paint.setPen(pen);
    paint.drawText(x + 70, y + h - 20, tr("czas snu"));
    paint.drawText(x + 70, y + h - 40, tr("oceny"));

  }

}

// -----------------------------------------------------------------------------------------------

void
  statDialog::drawGraph(int x, int y, int w, int h, int scores[7],
			int sleep_time[7])
{
  for(int i = 0; i < 7; i++)
  {
    if(scores[i] > 10)
      scores[i] = 10;
    if(sleep_time[i] > 12)
      sleep_time[i] = 12;
  }

  int V[13];
  int H[8];
  QPainter paint(this);
  QPen pen;
  paint.fillRect(x, y, w, h, QBrush(QColor(0, 0, 0)));
  paint.setPen(QColor(0xdd, 0xdd, 0xdd));
  // ===============================================
  paint.drawLine(x + 30, y, x + 30, y + h - 25);	// pionowa
  int s = (h - 40) / 12;
  int c = s;
  for(int i = 12; i >= 1; i--)
  {
    QString n;
    n.setNum(i);
    paint.drawText(x + 5, y + s, n);
    paint.drawLine(x + 25, y + s - 5, x + 35, y + s - 5);
    V[i] = y + s - 5;
    s += c;

  }
  // ================================================
  paint.drawLine(x + 25, V[1], w, V[1]);	// pozioma
  c = (w - 60) / 7;
  s = 0;
  for(int i = 1; i <= 7; i++)
  {
    QString n;
    n = scoredList->at(i - 1).date.toString("dd.MM");
    paint.drawLine(x + 30 + s, V[1] - 5, x + 30 + s, V[1] + 5);
    H[i] = x + 30 + s;
    paint.drawText(x + 30 + s - 3, V[1] + 20, n);
    s += c;
  }
  // =================================================

  pen.setColor(QColor(0, 255, 0));
  pen.setWidth(2);
  paint.setPen(pen);
  paint.drawLine(H[1], V[scores[0]], H[2], V[scores[1]]);
  paint.drawLine(H[2], V[scores[1]], H[3], V[scores[2]]);
  paint.drawLine(H[3], V[scores[2]], H[4], V[scores[3]]);
  paint.drawLine(H[4], V[scores[3]], H[5], V[scores[4]]);
  paint.drawLine(H[5], V[scores[4]], H[6], V[scores[5]]);
  paint.drawLine(H[6], V[scores[5]], H[7], V[scores[6]]);

  pen.setColor(QColor(255, 0, 0));
  paint.setPen(pen);
  paint.drawLine(H[1], V[sleep_time[0]], H[2], V[sleep_time[1]]);
  paint.drawLine(H[2], V[sleep_time[1]], H[3], V[sleep_time[2]]);
  paint.drawLine(H[3], V[sleep_time[2]], H[4], V[sleep_time[3]]);
  paint.drawLine(H[4], V[sleep_time[3]], H[5], V[sleep_time[4]]);
  paint.drawLine(H[5], V[sleep_time[4]], H[6], V[sleep_time[5]]);
  paint.drawLine(H[6], V[sleep_time[5]], H[7], V[sleep_time[6]]);

  int max = scores[0];
  int pos = 1;
  for(int i = 0; i < 7; i++)
    if(max < scores[i])
    {
      max = scores[i];
      pos = i;
    }
  int s_max = sleep_time[pos];

  pen.setWidth(1);
  int X = H[1];
  pen.setColor(QColor(190, 0, 0));
  paint.setPen(pen);
  while(X + 5 <= H[pos + 1])
  {
    paint.drawLine(X, V[s_max], X + 10, V[s_max]);
    X += 20;
  }

  X = V[1];
  pen.setColor(QColor(0, 190, 0));
  paint.setPen(pen);
  while(X - 5 >= V[s_max])
  {
    paint.drawLine(H[pos + 1], X, H[pos + 1], X - 10);
    X -= 20;
  }

}

void statDialog::showDetailDialog()
{
  if(dd == NULL)
    dd = new detailDialog(docsList);
  dd->exec();
  delete dd;
  dd = NULL;
}

// ********************************************************************************
// ********************************************************************************
// ********************************************************************************

detailDialog::detailDialog(const QList < DreamDocument > *docsList)
{
  QGridLayout *grid = new QGridLayout(this);
  QTableWidget *table = new QTableWidget(this);
  grid->addWidget(table, 0, 0, 1, 1);
  setWindowTitle(tr("Zestawienie szczegółowe"));
  table->setColumnCount(8);
  table->setRowCount(docsList->count());
  table->setAlternatingRowColors(true);

  QTableWidgetItem *colItem = new QTableWidgetItem();
  colItem->setText(tr("Data"));
  table->setHorizontalHeaderItem(0, colItem);

  QTableWidgetItem *colItem2 = new QTableWidgetItem();
  colItem2->setText(tr("Tytuł"));
  table->setHorizontalHeaderItem(1, colItem2);

  QTableWidgetItem *colItem3 = new QTableWidgetItem();
  colItem3->setText(tr("Godz. zaś."));
  table->setHorizontalHeaderItem(2, colItem3);

  QTableWidgetItem *colItem4 = new QTableWidgetItem();
  colItem4->setText(tr("Godz. przeb."));
  table->setHorizontalHeaderItem(3, colItem4);

  QTableWidgetItem *colItem5 = new QTableWidgetItem();
  colItem5->setText(tr("Czas snu"));
  table->setHorizontalHeaderItem(4, colItem5);

  QTableWidgetItem *colItem6 = new QTableWidgetItem();
  colItem6->setText(tr("Ocena"));
  table->setHorizontalHeaderItem(5, colItem6);

  QTableWidgetItem *colItem7 = new QTableWidgetItem();
  colItem7->setText(tr("Świadomy"));
  table->setHorizontalHeaderItem(6, colItem7);

  QTableWidgetItem *colItem8 = new QTableWidgetItem();
  colItem8->setText(tr("Długość opisu"));
  table->setHorizontalHeaderItem(7, colItem8);

  // ------------------------------------
  QTableWidgetItem *item;
  int h, m, s;
  QString strh, strm;
  for(int i = 0; i < docsList->count(); i++)
  {
    // data
    item = new QTableWidgetItem(docsList->at(i).date.toString("yyyy-MM-dd"));
    table->setItem(i, 0, item);

    // tytul
    item = new QTableWidgetItem(docsList->at(i).title);
    table->setItem(i, 1, item);

    // g.z.
    item = new QTableWidgetItem(docsList->at(i).sleep.toString());
    table->setItem(i, 2, item);

    // g.p.
    item = new QTableWidgetItem(docsList->at(i).wakeup.toString());
    table->setItem(i, 3, item);

    // obliczanie czasu snu
    s = docsList->at(i).sleep.secsTo(docsList->at(i).wakeup);
    s /= 60;

    h = s / 60;			// godziny
    m = s % 60;			// minuty

    if(h < 0)
      h += 24;
    if(m < 0)
      m += 60;

    strh.setNum(h);
    if(h < 10)
      strh = "0" + strh;
    strm.setNum(m);
    if(m < 10)
      strm = "0" + strm;

    item = new QTableWidgetItem(strh + ":" + strm);
    table->setItem(i, 4, item);

    // ocena
    strh.setNum(docsList->at(i).score);
    item = new QTableWidgetItem(strh);
    table->setItem(i, 5, item);

    // swiadomy
    if(docsList->at(i).lucid)
      strh = tr("tak");
    else
      strh = tr("nie");
    item = new QTableWidgetItem(strh);
    table->setItem(i, 6, item);

    // dlugosc
    strh.setNum(docsList->at(i).content.size());
    item = new QTableWidgetItem(strh);
    table->setItem(i, 7, item);
  }
  // ------------------------------------
  table->setSortingEnabled(true);
  setWindowState(Qt::WindowMaximized);
}
