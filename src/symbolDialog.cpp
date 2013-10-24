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

#include "symbolDialog.h"

symbolDialog::symbolDialog(MainForm * parent)
{
  ui.setupUi(this);
  mw = parent;
  current.clear();
  load();
  changed = false;
  ad = NULL;
  if(symbolList.count() > 0)
  {
    ui.listWidget->setCurrentRow(0);
    current = ui.listWidget->currentItem()->text();
    openSelected();
  }
  setWindowFlags(Qt::Widget);
  connect(ui.addButton, SIGNAL(clicked()), this, SLOT(add()));
  connect(ui.fromKeyButton, SIGNAL(clicked()), this, SLOT(fromKey()));
  connect(ui.deleteButton, SIGNAL(clicked()), this, SLOT(deleteSelected()));
  connect(ui.saveButton, SIGNAL(clicked()), this, SLOT(saveMeaning()));
  connect(ui.textEdit, SIGNAL(textChanged()), this, SLOT(textChanged()));
  // connect(ui.listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(openSelected()));
  connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(import()));
  connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(Export()));
  connect(ui.listWidget,
	  SIGNAL(currentItemChanged
		 (QListWidgetItem *, QListWidgetItem *)), this,
	  SLOT(openSelected()));
  connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(quit()));
}

symbolDialog::~symbolDialog()
{
  delete ad;
}

void symbolDialog::add()
{
  if(changed)
  {
    int q = QMessageBox::question(this, tr("Zamknij"),
				  tr
				  ("Treść znaczenia symbolu uległa zmianie, czy chcesz zapisać zmiany?"),
				  tr("Zapisz"), tr("Nie zapisuj"),
				  tr("Anuluj"));

    if(q == 0)
    {
      saveMeaning();
    }
    if(q == 2)
      return;
  }
  changed = false;
  bool ok;
  QString
    exp = QInputDialog::getText(this, tr("Dodaj Symbol"),
				tr("Podaj nazwę nowego symbolu"),
				QLineEdit::Normal, "", &ok, 0);

  if(ok)
  {
    for(int i = 0; i < symbolList.count(); i++)
      if(exp == symbolList.at(i).name)
      {
	QMessageBox::warning(this, tr("Ostrzeżenie"),
			     tr
			     ("Podany symbol już istnieje!"),
			     QMessageBox::Ok);
	return;
      }

    TSymbol tmp;
    tmp.name = exp;
    symbolList.push_back(tmp);
    current = exp;
    fillList();
    openSelected();
  }
}

void symbolDialog::saveMeaning()
{
  int i;
  for(i = 0; i < symbolList.count(); i++)
    if(current == symbolList.at(i).name)
    {
      symbolList[i].meaning = ui.textEdit->toPlainText();
      break;
    }

  this->setWindowTitle(current);
  changed = false;
  save();
}

void symbolDialog::fillList()
{
  int i;
  ui.listWidget->clear();
  for(i = 0; i < symbolList.count(); i++)
    ui.listWidget->addItem(symbolList.at(i).name);

  ui.listWidget->sortItems(Qt::AscendingOrder);

  for(i = 0; i < ui.listWidget->count(); i++)
    if(current == ui.listWidget->item(i)->text())
    {
      ui.listWidget->setCurrentRow(i);
      break;
    }

}

void symbolDialog::fromKey()
{
  if(ad == NULL)
    ad = new fromKeyDialog(this);

  const int x = (this->width() / 2) - (ad->width() / 2);
  const int y = (this->height() / 2) - (ad->height() / 2);
  const int w = ad->width();
  const int h = ad->height();
  ad->setGeometry(x, y, w, h);
  ad->exec();
  delete ad;
  ad = NULL;

}

void symbolDialog::deleteSelected()
{
  if(ui.listWidget->currentRow() != -1)
  {

    int q = QMessageBox::question(this, tr("Usuń"),
				  tr
				  ("Czy na pewno chcesz usunąć ten symbol?"),
				  tr("Usuń"), tr("Anuluj"));

    if(q == 0)
    {
      for(int i = 0; i < symbolList.count(); i++)
	if(symbolList.at(i).name == current)
	{
	  symbolList.removeAt(i);
	  break;
	}

      if(symbolList.count() == 0)
	QFile::remove(mw->userPath + mw->username + ".xsym");

      ui.textEdit->clear();
      changed = false;
      fillList();
      openSelected();
      saveMeaning();
    }
  }
}

void symbolDialog::openSelected()
{

  if(changed)
  {
    int q = QMessageBox::question(this, tr("Zamknij"),
				  tr
				  ("Treść znaczenia symbolu uległa zmianie, czy chcesz zapisać zmiany?"),
				  tr("Zapisz"), tr("Nie zapisuj"),
				  tr("Anuluj"));

    if(q == 0)
    {
      saveMeaning();
    }
    if(q == 2)
      return;
  }

  if(ui.listWidget->currentRow() != -1)
  {

    current = ui.listWidget->currentItem()->text();
    int i;
    for(i = 0; i < symbolList.count(); i++)
      if(current == symbolList.at(i).name)
      {
	ui.textEdit->setPlainText(symbolList.at(i).meaning);
	this->setWindowTitle(current);
	changed = false;
	break;
      }
  }
  else
    setWindowTitle(tr("Symbole"));
}

void symbolDialog::textChanged()
{
  changed = true;
  this->setWindowTitle(current + "*");
}

void symbolDialog::quit()
{
  if(changed)
  {
    int q = QMessageBox::question(this, tr("Zamknij"),
				  tr
				  ("Treść znaczenia symbolu uległa zmianie, czy chcesz zapisać zmiany przed wyjściem?"),
				  tr("Zapisz"), tr("Nie zapisuj"),
				  tr("Anuluj"));

    if(q == 0)
    {
      saveMeaning();
    }
    if(q == 2)
      return;
  }
  this->close();
}

void symbolDialog::Export()
{
  QString
    filename =
    QFileDialog::getSaveFileName(this, tr("Zapisz plik"), ".",
				 tr("Symbole My Dream Diary(*.xsym)"));
  if(filename.right(5) != ".xsym")
    filename += ".xsym";

  if(!filename.isEmpty())
    save(filename);
}

// --------------------------------------------------------------------
void symbolDialog::import()
{
  QString
    filename =
    QFileDialog::getOpenFileName(this, tr("Otwórz plik"), ".",
				 tr("Wszystkie pliki(*.*)"));
  if(!filename.isEmpty())
  {
    int q = QMessageBox::question(this, tr("Import"),
				  tr
				  ("Wszystkie dotychczasowe symbole zostaną bezpowrotnie utracone, czy na pewno chcesz importowac symbole z wybranego pliku?"),
				  tr("Importuj"), tr("Anuluj"));
    if(q == 0)
    {
      load(filename);
      ui.listWidget->setCurrentRow(0);
      openSelected();
      save();
    }
  }
}

// --------------------------------------------------------------------

void symbolDialog::save(QString filename)
{
  if(filename == "default")
    filename = mw->userPath + mw->username + ".xsym";

  QString allData;
  allData.clear();
  if(!symbolList.isEmpty())
  {

    QDomDocument xdoc("mdd_symbols");
    QDomElement root = xdoc.createElement("symbols");
    QDomElement document;
    QDomElement elem;
    QDomText txt;

    for(signed int i = 0; i < symbolList.size(); i++)
    {
      document = xdoc.createElement("symbol");

      elem = xdoc.createElement("name");
      txt = xdoc.createTextNode(symbolList.at(i).name);
      elem.appendChild(txt);
      document.appendChild(elem);

      elem = xdoc.createElement("meaning");
      txt = xdoc.createTextNode(symbolList.at(i).meaning);
      elem.appendChild(txt);
      document.appendChild(elem);

      root.appendChild(document);
    }
    xdoc.appendChild(root);

    QFile data(filename);
    if(data.open(QFile::WriteOnly | QFile::Text))
    {
      QTextStream ostream(&data);
      ostream.setAutoDetectUnicode(false);
      ostream.setCodec(QTextCodec::codecForName
		       (mw->config.codecName.toLatin1()));
      if(mw->config.sym_coding)
	ostream << vigcryp::encode(xdoc.toString(), mw->vig_key);
      else
	ostream << xdoc.toString();
    }
  }
}

void symbolDialog::load(QString filename)
{
  QString allData;
  QDomDocument xdoc("mdd_symbols");
  if(filename == "default")
    filename = mw->userPath + mw->username + ".xsym";

  QFile data(filename);

  if(data.exists())
    if(data.open(QFile::ReadOnly | QFile::Text))
    {
      QTextStream istream(&data);
      istream.setAutoDetectUnicode(false);
      istream.setCodec(QTextCodec::codecForName
		       (mw->config.codecName.toLatin1()));
      allData = istream.readAll();
      if((mw->config.sym_coding) && (filename != "default"))
	allData = vigcryp::decode(allData, mw->vig_key);

      if(allData.left(22) != "<!DOCTYPE mdd_symbols>")
	allData =
	  "<!DOCTYPE mdd_symbols>\n<symbols>\n" + allData + "\n</symbols>";

      if(!xdoc.setContent(allData))
      {
	data.close();
	return;
      }
      data.close();

      // wczytywanie symboli
      QDomNodeList symbols = xdoc.elementsByTagName("symbol");
      QDomNode document;
      symbolList.clear();
      for(int i = 0; i < symbols.count(); i++)
      {
	document = symbols.item(i);

	TSymbol sym;
	sym.name =
	  document.toElement().elementsByTagName("name").item
	  (0).firstChild().toText().data();
	sym.meaning =
	  document.toElement().elementsByTagName("meaning").item(0).
	  firstChild().toText().data();

	symbolList.push_back(sym);
      }

      fillList();
    }
}
