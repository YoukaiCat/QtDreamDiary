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


#ifndef FROMKEYDIALOG_H
#define FROMKEYDIALOG_H

#include "ui_fromKeyDialog.h"
#include "MainForm.h"
#include "symbolDialog.h"
class symbolDialog;

class fromKeyDialog:public QDialog
{
Q_OBJECT public:
    fromKeyDialog(symbolDialog * Parent);
   ~fromKeyDialog()
  {
  };
private:
  Ui::fromKeyDialog ui;
  symbolDialog *parent;
  private slots:void pushButton();

};


#endif
