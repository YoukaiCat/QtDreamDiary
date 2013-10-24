/***************************************************************************
 *   Copyright (C) 2008 by Łukasz Chodyła                                  *
 *   chodak166@op.pl                                                       *
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

#include "fromKeyDialog.h"

fromKeyDialog::fromKeyDialog(symbolDialog * Parent)
{
  ui.setupUi(this);
  parent = Parent;

  for(int i = 0; i < parent->mw->dreamBase->keys->count(); i++)
    for(int j = 0; j < parent->mw->dreamBase->keys->at(i)->keys.count(); j++)
      ui.listWidget->addItem(parent->mw->dreamBase->keys->at(i)->keys.at(j));
  setWindowFlags(Qt::Widget);
  connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(pushButton()));
  connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(close()));
}

void fromKeyDialog::pushButton()
{

  if(ui.listWidget->currentRow() != -1)
  {
    QString exp = ui.listWidget->currentItem()->text();

    for(int i = 0; i < parent->symbolList.count(); i++)
      if(exp == parent->symbolList.at(i).name)
      {
	QMessageBox::warning(this, tr("Ostrzeżenie"),
			     tr
			     ("Podany symbol już istnieje!"),
			     QMessageBox::Ok);
	return;
      }

    TSymbol tmp;
    tmp.name = exp;
    parent->symbolList.push_back(tmp);
    parent->current = exp;
    parent->fillList();
    parent->openSelected();

    this->close();
  }
  else
    QMessageBox::warning(this, tr("Ostrzeżenie"),
			 tr("Nie wybrano wyrażenia!"), QMessageBox::Ok);
}
