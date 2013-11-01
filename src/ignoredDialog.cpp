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

#include "ignoredDialog.h"

ignoredDialog::ignoredDialog(MyDreamBase * Base, bool encoding)
{
  base = Base;
  enc = encoding;
  QVBoxLayout *lay = new QVBoxLayout(this);
  listWidget = new QListWidget(this);
  QPushButton *addButton = new QPushButton(this);
  addButton->setText(tr("Add"));
  QPushButton *removeButton = new QPushButton(this);
  removeButton->setText(tr("Delete"));
  QPushButton *closeButton = new QPushButton(this);
  closeButton->setText(tr("Close"));

  fillList();

  lay->addWidget(listWidget);
  lay->addWidget(addButton);
  lay->addWidget(removeButton);
  lay->addWidget(closeButton);
  setWindowFlags(Qt::Widget);
  connect(addButton, SIGNAL(clicked()), this, SLOT(add()));
  connect(removeButton, SIGNAL(clicked()), this, SLOT(remove()));
  connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

void ignoredDialog::add()
{
  bool ok;
  QString
    str = QInputDialog::getText(this, tr("Suggestion ignored"),
                tr("Please enter ignored expression:"),
                QLineEdit::Normal, "", &ok);
  if(!ok)
    return;
  base->addIgnored(str);
  base->saveKeys(enc);
  fillList();
}

void ignoredDialog::remove()
{
  int r = listWidget->currentRow();
  if(r == -1)
    return;
  QString str = listWidget->item(r)->text();
  base->removeIgnored(str);
  base->saveKeys(enc);
  fillList();
}

void ignoredDialog::fillList()
{
  listWidget->clear();
  for(int i = 0; i < base->ignoredSuggestions->count(); i++)
    listWidget->addItem(base->ignoredSuggestions->at(i));
  listWidget->sortItems();
}
