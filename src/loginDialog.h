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


#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include "ui_loginDialog.h"
#include "MainForm.h"
#include "newDialog.h"
#include <QCryptographicHash>

class MainForm;
class newDialogClass;

class loginDialog:public QDialog
{
Q_OBJECT public:
    loginDialog(MainForm * parent);
   ~loginDialog()
  {
  };
  MainForm *mw;
  bool AutoLogin;
private:
  newDialogClass * nd;
  Ui::Dialog ui;
  bool savePass;
  QString login;
  QString password;
  private slots:void loginButtonClicked();
  void newButtonClicked();
  void checkBoxChanged(int);

};


#endif
