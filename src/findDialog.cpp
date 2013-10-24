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

#include "findDialog.h"

findDialog::findDialog(MainForm * parent)
{
  ui.setupUi(this);
  mw = parent;
  setWindowFlags(Qt::Widget);
  connect(ui.findButton, SIGNAL(clicked()), this, SLOT(find()));
  connect(ui.closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

void findDialog::find()
{

  QString exp = ui.lineEdit->text();
  if((ui.checkBox->checkState() == Qt::Checked) &&
     (ui.checkBox_2->checkState() == Qt::Checked))
    mw->ui.textEdit->find(exp,
			  QTextDocument::FindBackward |
			  QTextDocument::FindCaseSensitively);

  if((ui.checkBox->checkState() == Qt::Checked) &&
     (ui.checkBox_2->checkState() == Qt::Unchecked))
    mw->ui.textEdit->find(exp, QTextDocument::FindCaseSensitively);

  if((ui.checkBox->checkState() == Qt::Unchecked) &&
     (ui.checkBox_2->checkState() == Qt::Checked))
    mw->ui.textEdit->find(exp, QTextDocument::FindBackward);

  if((ui.checkBox->checkState() == Qt::Unchecked) &&
     (ui.checkBox_2->checkState() == Qt::Unchecked))
    mw->ui.textEdit->find(exp);

}
