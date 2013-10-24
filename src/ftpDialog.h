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


#ifndef FTPDIALOG_H
#define FTPDIALOG_H

#include "ui_ftpDialog.h"
#include "MainForm.h"

class MainForm;

class ftpDialog:public QDialog
{
Q_OBJECT public:
    ftpDialog(MainForm * parent);
   ~ftpDialog();
  void saveConfig();
  void loadConfig();
private:
    Ui::ftpDialog ui;
  MainForm *mw;
  QDialog *infoDialog;
  QLabel *label;
  QFtp *ftp;
  QString host;
  QString login;
  QString password;
  private slots:void destroy();
  void download();
  void autoupload(int);
  void stateChanged(int);
  void dataTransferProgress(qint64, qint64);
  void commandFinished(int, bool);
  public slots:void upload();

};


#endif
