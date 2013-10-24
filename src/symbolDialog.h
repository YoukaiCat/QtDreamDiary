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


#ifndef SYMBOLDIALOG_H
#define SYMBOLDIALOG_H

#include "ui_symbolDialog.h"
#include "MainForm.h"
#include "fromKeyDialog.h"
class fromKeyDialog;

class MainForm;

struct TSymbol
{
  QString name;
  QString meaning;
};

class symbolDialog:public QDialog
{
Q_OBJECT public:
    symbolDialog(MainForm * parent);
   ~symbolDialog();
  QString current;
  MainForm *mw;
  void fillList();
    QList < TSymbol > symbolList;
  void load(QString filename = "default");
  void save(QString filename = "default");
private:
    bool changed;
    Ui::symbolDialog ui;
  fromKeyDialog *ad;
  public slots:void add();
  void fromKey();
  void deleteSelected();
  void openSelected();
  void saveMeaning();
  void textChanged();
  void Export();
  void import();
  void quit();
};


#endif
