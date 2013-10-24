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

#include "settingsDialog.h"
#include "symbolDialog.h"

settingsDialog::settingsDialog(MainForm * parent)
{
  ui.setupUi(this);
  mw = parent;
  loadConfigData();
  ui.pushButton_3->setIcon(QIcon
			   (mw->exePath + "/themes/" + config->theme +
			    "/contexthelp.png"));
  connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(pushButtonClicked()));
  connect(ui.pushButton_2, SIGNAL(clicked()), this,
	  SLOT(pushButton2Clicked()));
  connect(ui.pushButton_3, SIGNAL(clicked()), this,
	  SLOT(pushButton3Clicked()));
  connect(ui.toolButton, SIGNAL(clicked()), this, SLOT(toolButtonClicked()));
  connect(ui.okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
  connect(ui.checkBox_4, SIGNAL(stateChanged(int)), this,
	  SLOT(checkBox4Changed(int)));
  connect(ui.checkBox_7, SIGNAL(stateChanged(int)), this,
	  SLOT(checkBox7Changed(int)));
  connect(ui.checkBox_8, SIGNAL(stateChanged(int)), this,
	  SLOT(checkBox8Changed(int)));
  connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(close()));

}

void settingsDialog::loadConfigData()
{
  config = &mw->config;
  // opcje logowania:
  Last last;
  if(last.load(mw->exePath + "/last"))
  {
    ui.checkBox_5->setEnabled(true);
    ui.checkBox_4->setCheckState(Qt::Checked);
    if(last.autologin)
      ui.checkBox_5->setCheckState(Qt::Checked);
    else
      ui.checkBox_5->setCheckState(Qt::Unchecked);
  }
  else
  {
    ui.checkBox_5->setCheckState(Qt::Unchecked);
    ui.checkBox_4->setCheckState(Qt::Unchecked);
  }

  // opcje szyfrowania
  obs = config->base_coding;
  oks = config->keys_coding;
  oss = config->sym_coding;
  if(config->base_coding)
    ui.checkBox_2->setCheckState(Qt::Checked);
  else
    ui.checkBox_2->setCheckState(Qt::Unchecked);

  if(config->keys_coding)
    ui.checkBox_6->setCheckState(Qt::Checked);
  else
    ui.checkBox_6->setCheckState(Qt::Unchecked);

  if(config->sym_coding)
    ui.checkBox_3->setCheckState(Qt::Checked);
  else
    ui.checkBox_3->setCheckState(Qt::Unchecked);

  // opcje podswietlania kluczy
  hcolor = config->highlightColor;
  if(config->hkeys)
    ui.checkBox->setCheckState(Qt::Checked);
  else
    ui.checkBox->setCheckState(Qt::Unchecked);

  // wczytywanie dostepnych zestawow
  QDir themeDir = mw->exePath + "/themes/";
  ui.comboBox->addItems(themeDir.entryList
			(QDir::NoDotAndDotDot | QDir::AllDirs, QDir::Name));
  int ti = ui.comboBox->findText(config->theme);
  if(ti == -1)
    QMessageBox::warning(this, tr("Błąd"),
			 tr
			 ("Nie odnaleziono motywu ikon, wybierz inny działający motyw lub dodaj własny"),
			 QMessageBox::Ok);
  else
    ui.comboBox->setCurrentIndex(ti);
  if(config->trayicon)
  {
    ui.checkBox_7->setCheckState(Qt::Checked);
    ui.groupBox_3->setEnabled(true);
  }
  else
  {
    ui.checkBox_7->setCheckState(Qt::Unchecked);
    ui.groupBox_3->setEnabled(false);
  }
  // aktualizacje:
  if(config->update)
    ui.checkBox_9->setCheckState(Qt::Checked);
  else
    ui.checkBox_9->setCheckState(Qt::Unchecked);

  // opcje sugestii:
  if(config->suggestions)
  {
    ui.checkBox_8->setCheckState(Qt::Checked);
    ui.checkBox_7->setEnabled(false);
  }
  else
    ui.checkBox_8->setCheckState(Qt::Unchecked);
  switch (config->sugtime)
  {
  case week:
    ui.comboBox_2->setCurrentIndex(0);
    break;
  case month:
    ui.comboBox_2->setCurrentIndex(1);
    break;
  case all:
    ui.comboBox_2->setCurrentIndex(2);
  }
  ui.comboBox_3->setCurrentIndex(config->sugwords);
  ui.spinBox->setValue(config->sugignored);
  ui.spinBox_2->setValue(config->sugsens);

  // opcje przegladarki:
  ui.browserEdit->setText(config->browserPath);
  ui.addressEdit->setText(config->webAddress);
}

void settingsDialog::pushButtonClicked()
{

  QColor old = hcolor;
  hcolor = QColorDialog::getColor(hcolor);
  if(!hcolor.isValid())
    hcolor = old;
}

void settingsDialog::toolButtonClicked()
{
  QString
    filename =
    QFileDialog::getOpenFileName(this, tr("Otwórz plik"), ".",
				 tr("Wszystkie pliki(*)"));
  if(!filename.isEmpty())
    ui.browserEdit->setText(filename);
}

// ------------------------------------------------------------------------------------------------------------ 
// 
void settingsDialog::okButtonClicked()
{
  bool OK = true;
  // zmiana ustawien logowania:
  Last last;
  if(ui.checkBox_4->checkState() == Qt::Checked)
  {
    last.passwd = mw->password;
    last.login = mw->username;
    if(ui.checkBox_5->checkState() == Qt::Checked)
      last.autologin = true;
    else
      last.autologin = false;
    last.save(mw->exePath + "/last");
  }
  else if(last.load(mw->exePath + "/last"))
    if(last.login == mw->username)
      if(QFile::exists(mw->exePath + "/last"))
	QFile::remove(mw->exePath + "/last");

  // ustawienia szyfrowania:
  if(ui.checkBox_2->checkState() == Qt::Checked)
    config->base_coding = true;
  else
    config->base_coding = false;

  if(config->base_coding != obs)
    mw->dreamBase->rewriteAll(config->base_coding);

  // *****************************************************888eighteight
  if(ui.checkBox_6->checkState() == Qt::Checked)
    config->keys_coding = true;
  else
    config->keys_coding = false;
  if(config->keys_coding != oks)
    mw->dreamBase->saveKeys(config->keys_coding);
  // *****************************************************
  if(ui.checkBox_3->checkState() == Qt::Checked)
    config->sym_coding = true;
  else
    config->sym_coding = false;

  if(config->sym_coding != oss)
  {
    symbolDialog *ad = new symbolDialog(mw);
    ad->load();
    ad->save();
    ad->close();
    delete ad;
  }
  // ustawienia wygladu
  config->highlightColor = hcolor;
  if(ui.checkBox_7->checkState() == Qt::Checked)
  {
    config->trayicon = true;
    mw->trayIcon->show();
  }
  else
  {
    config->trayicon = false;
    mw->trayIcon->hide();
  }
  if(ui.checkBox->checkState() == Qt::Checked)
    config->hkeys = true;
  else
    config->hkeys = false;
  config->theme = ui.comboBox->currentText();
  mw->loadIcons();

  if(ui.checkBox_9->checkState() == Qt::Checked)
    config->update = true;
  else
    config->update = false;

  // ustawienia sugestii:
  if(ui.checkBox_8->checkState() == Qt::Checked)
    config->suggestions = true;
  else
    config->suggestions = false;
  switch (ui.comboBox_2->currentIndex())
  {
  case 0:
    config->sugtime = week;
    break;
  case 1:
    config->sugtime = month;
    break;
  case 2:
    config->sugtime = all;
    break;
  default:
    config->sugtime = month;
    break;

  }
  config->sugwords = ui.comboBox_3->currentIndex();
  config->sugignored = ui.spinBox->value();
  config->sugsens = ui.spinBox_2->value();

  // ustawienia przegladarki
  config->browserPath = ui.browserEdit->text();
  if(!ui.addressEdit->text().contains("#exp#"))
  {
    QMessageBox::warning(this, tr("Błąd"),
			 tr
			 ("Błędnie wprowadzony adres strony, brak wymaganego wyrażenia #exp#. Szczegóły w pliku pomocy."),
			 QMessageBox::Ok);
    OK = false;
  }
  else
    config->webAddress = ui.addressEdit->text();

  // zatwierdzanie
  if(OK)
  {
    config->save();
    mw->highlightAll();
    this->close();
  }
}

void settingsDialog::pushButton3Clicked()
{
  QWhatsThis::enterWhatsThisMode();
}

void settingsDialog::pushButton2Clicked()
{
  QString uurl = config->update_url;
  QString p = config->passwd;
  config->createDefault(p);
  config->update_url = uurl;
  loadConfigData();
  QMessageBox::information(this, tr("Ustawienia"),
			   tr
			   ("Przywrócono domyślne ustawienia programu."),
			   QMessageBox::Ok);
}

void settingsDialog::checkBox4Changed(int s)
{
  if(s == Qt::Checked)
    ui.checkBox_5->setEnabled(true);
  else
    ui.checkBox_5->setEnabled(false);
}

void settingsDialog::checkBox7Changed(int s)
{
  if(s == Qt::Checked)
    ui.groupBox_3->setEnabled(true);
  else
    ui.groupBox_3->setEnabled(false);
}

void settingsDialog::checkBox8Changed(int s)
{
  if(s == Qt::Checked)
  {
    ui.checkBox_7->setCheckState(Qt::Checked);
    ui.checkBox_7->setEnabled(false);
  }
  else
  {
    ui.checkBox_7->setEnabled(true);
    if(config->trayicon)
      ui.checkBox_7->setCheckState(Qt::Checked);
    else
      ui.checkBox_7->setCheckState(Qt::Unchecked);
  }
}
