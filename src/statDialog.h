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


#ifndef STATDIALOG_H
#define STATDIALOG_H

#include "ui_statDialog.h"
#include "MyDreamBase.h"
#include <QTableWidget>

class detailDialog;
class MainForm;
struct DreamDocument;

class statDialog:public QDialog
{
Q_OBJECT public:
    statDialog(const QList < DreamDocument > *);
   ~statDialog();
private:
    QList < DreamDocument > *scoredList;
  const QList < DreamDocument > *docsList;
    Ui::statDialog ui;
  bool weekEnable;
  detailDialog *dd;
  int allDocs;
  int lucidDocs;
  int allScored;
  int allAvg;
  int allMax;
  int allMin;
  void drawGraph(int, int, int, int, int[7], int[7]);
  void paintEvent(QPaintEvent *);
  private slots:void showDetailDialog();

};

class detailDialog:public QDialog
{
Q_OBJECT public:
    detailDialog(const QList < DreamDocument > *);
};

#endif
