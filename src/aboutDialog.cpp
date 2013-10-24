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

#include "aboutDialog.h"

aboutDialog::aboutDialog(MainForm * parent)
{
  ui.setupUi(this);
  mw = parent;
  ui.label_3->setText(tr("My Dream Diary (wersja ") + VER + tr(")"));
  img.load(mw->exePath + "/themes/abouticon.png");
  setWindowFlags(Qt::Widget);
  connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(close()));
}

aboutDialog::~aboutDialog()
{

}

void aboutDialog::paintEvent(QPaintEvent *)
{
  QPainter paint(this);
  paint.drawImage(ui.imgWidget->x(), ui.imgWidget->y(), img);
}
