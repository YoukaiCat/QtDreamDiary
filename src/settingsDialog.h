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


#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "ui_settingsDialog.h"
#include "MainForm.h"

class MainForm;

class settingsDialog:public QDialog
{
Q_OBJECT public:
    settingsDialog(MainForm * parent);
   ~settingsDialog()
  {
  };
private:
  Ui::settingsDialog ui;
  MainForm *mw;
  QColor hcolor;
  XConfig *config;
  bool obs;			//oryginal base state
  bool oks;
  bool oss;
  bool closed;
  void loadConfigData();
  private slots:void pushButtonClicked();
  void pushButton2Clicked();
  void pushButton3Clicked();
  void toolButtonClicked();
  void okButtonClicked();
  void checkBox4Changed(int);
  void checkBox7Changed(int);
  void checkBox8Changed(int);

};


#endif
