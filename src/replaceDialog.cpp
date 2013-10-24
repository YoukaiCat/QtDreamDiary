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

#include "replaceDialog.h"

replaceDialog::replaceDialog(MainForm * parent)
{
  ui.setupUi(this);
  mw = parent;
  setWindowFlags(Qt::Widget);
  connect(ui.replaceButton, SIGNAL(clicked()), this, SLOT(replace()));
  connect(ui.closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

void replaceDialog::replace()
{
  int r = 0;
  QString exp = ui.findEdit->text();
  QString newExp = ui.replaceEdit->text();
  QString text = mw->ui.textEdit->toPlainText();
  if(ui.checkBox->checkState() == Qt::Unchecked)
  {
    int i = text.indexOf(exp, 0, Qt::CaseInsensitive);
    while(i != -1)
    {
      text.replace(i, exp.size(), newExp);
      i = text.indexOf(exp, i + exp.size(), Qt::CaseInsensitive);
      r++;
    }
  }
  else
  {
    int i = text.indexOf(exp, 0, Qt::CaseSensitive);
    while(i != -1)
    {
      text.replace(i, exp.size(), newExp);
      i = text.indexOf(exp, i + exp.size(), Qt::CaseSensitive);
      r++;
    }
  }
  mw->ui.textEdit->setPlainText(text);

  exp = exp.setNum(r);
  QMessageBox::information(this, tr("Zamieniono"),
			   tr("Zamieniono ") + exp + tr(" wyrażeń."),
			   QMessageBox::Ok);
}
