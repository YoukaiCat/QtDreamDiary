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

#include <QDesktopServices>
#include "ftpDialog.h"

ftpDialog::ftpDialog(MainForm * parent)
{
  ui.setupUi(this);
  mw = parent;
  loadConfig();
  ui.lineEdit_4->setText(mw->username + ".mdd");

  infoDialog = new QDialog(this, Qt::Dialog);
  int x, y, w, h;
  w = 200;
  h = 100;
  x = QApplication::desktop()->width() / 2 - w / 2;
  y = QApplication::desktop()->height() / 2 - h / 2;
  infoDialog->setGeometry(x, y, w, h);
  QGridLayout *gl = new QGridLayout(infoDialog);
  label = new QLabel(infoDialog);
  label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  gl->addWidget(label);

  ftp = new QFtp(this);
  connect(ftp, SIGNAL(stateChanged(int)), this, SLOT(stateChanged(int)));
  connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(destroy()));
  connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(download()));
  connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(upload()));
  connect(ui.checkBox, SIGNAL(stateChanged(int)), this,
	  SLOT(autoupload(int)));
  connect(ftp, SIGNAL(dataTransferProgress(qint64, qint64)), this,
	  SLOT(dataTransferProgress(qint64, qint64)));
  connect(ftp, SIGNAL(commandFinished(int, bool)), this,
	  SLOT(commandFinished(int, bool)));

}

ftpDialog::~ftpDialog()
{
  delete infoDialog;
  delete ftp;
}

void ftpDialog::upload()
{
  QString host = ui.lineEdit->text();
  QString login = ui.lineEdit_2->text();
  QString password = ui.lineEdit_3->text();
  QString filename = ui.lineEdit_4->text();

  if(host == "" || login == "" || password == "" || filename == "")
  {
    QMessageBox::warning(this, tr("FTP"),
			 tr("Musisz wypełnić wszystkie pola!"),
			 QMessageBox::Ok);
    return;
  }
  // ============================================
  label->setText(tr("Łączenie z serwerem..."));
  infoDialog->show();
  if(ftp->state() == QFtp::Unconnected)
    ftp->connectToHost(host);
  if(ftp->state() != QFtp::LoggedIn)
    ftp->login(login, password);

  mw->dreamBase->exportToFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + filename);
  QFile file(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + filename);
  if(file.open(QFile::ReadOnly | QFile::Text))
    ftp->put(file.readAll(), filename, QFtp::Ascii);
  ftp->close();
  QFile::remove(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + filename);
  file.close();
}

void ftpDialog::download()
{

  QString host = ui.lineEdit->text();
  QString login = ui.lineEdit_2->text();
  QString password = ui.lineEdit_3->text();
  QString filename = ui.lineEdit_4->text();
  if(host == "" || login == "" || password == "" || filename == "")
  {
    QMessageBox::warning(this, tr("FTP"),
			 tr("Musisz wypełnić wszystkie pola!"),
			 QMessageBox::Ok);
    return;
  }
  int q = QMessageBox::question(this, tr("Potwierdź"),
				tr
				("Pobranie nowej bazy spowoduje utratę bieżącego archiwum. Czy na pewno chcesz kontynuować?"),
				tr("Pobierz"), tr("Anuluj"));
  if(q == 0)
  {

    // ============================================
    label->setText(tr("Łączenie z serwerem..."));
    infoDialog->show();
    if(ftp->state() == QFtp::Unconnected)
      ftp->connectToHost(host);
    if(ftp->state() != QFtp::LoggedIn)
      ftp->login(login, password);

    ftp->get(filename, 0, QFtp::Ascii);
    ftp->close();
  }

}

void ftpDialog::destroy()
{
  if(ui.checkBox->checkState() == Qt::Checked)
    saveConfig();
  else if(QFile::exists(mw->userPath + "ftp"))
    QFile::remove(mw->userPath + "ftp");

  close();
}

void ftpDialog::autoupload(int state)
{
  if(state == Qt::Checked)
    ui.checkBox_2->setEnabled(true);
  else
    ui.checkBox_2->setEnabled(false);
}

void ftpDialog::stateChanged(int state)
{
  switch (state)
  {
  case QFtp::Connected:
    label->setText(tr("Logowanie..."));
    break;
  case QFtp::LoggedIn:
    label->setText(tr("ZALOGOWANO"));
    break;
  case QFtp::Closing:
    infoDialog->close();
    break;
  }
}

void ftpDialog::dataTransferProgress(qint64 done, qint64 total)
{
  if(done < total)
  {
    QString d, t;
    d.setNum(done);
    t.setNum(total);
    label->setText(tr("transfer danych: ") + d + "/" + t);
    infoDialog->show();
  }
  else
  {
    label->setText(tr("Zakończono"));
    QMessageBox::information(this, tr("FTP"),
			     tr
			     ("Transfer pliku zakończony sukcesem!"),
			     QMessageBox::Ok);

    infoDialog->close();

    mw->dreamBase->rewriteAll(mw->config.base_coding);
    mw->dreamBase->fillTree(mw->ui.docTree);
    mw->dreamBase->keys->fillTree(mw->ui.keysTree,
				  mw->dreamBase->list(), mw->config.freqSort);

  }
}

void ftpDialog::commandFinished(int, bool error)
{
  if(ftp->currentCommand() == QFtp::Get)
  {
    if(error)
    {
      QMessageBox::critical(this, tr("Błąd"),
			    tr
			    ("Wystąpił błąd podczas pobierania archiwum"),
			    QMessageBox::Ok);
      return;
    }

    QString data = ftp->readAll();
    if(data.isEmpty())
      return;

    QFile file(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/ftp.mdd");
    if(file.open(QFile::WriteOnly | QFile::Text))
    {

      QTextStream ostream(&file);
      ostream.setAutoDetectUnicode(false);
      ostream.setCodec(QTextCodec::codecForName
		       (mw->config.codecName.toLatin1()));
      ostream << data;
      file.close();
      mw->dreamBase->importFromFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/ftp.mdd");
      mw->dreamBase->rewriteAll(mw->config.base_coding);
      QFile::remove(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/ftp.mdd");
    }
    else
      QMessageBox::critical(this, tr("Błąd"),
			    tr
			    ("Zapis pobranego archiwum zakończony niepowodzeniem"),
			    QMessageBox::Ok);
  }
}

// -------------------------------------------------------------------------
void ftpDialog::saveConfig()
{
  QString au;
  if(ui.checkBox_2->checkState() == Qt::Checked)
  {
    au = "1";
    mw->config.ftp_autoupload = true;
  }
  else
  {
    au = "0";
    mw->config.ftp_autoupload = false;
  }
  QString
    data = "<host>" + ui.lineEdit->text() + "</host>\n" +
    "<login>" + ui.lineEdit_2->text() + "</login>\n" +
    "<pass>" + ui.lineEdit_3->text() + "</pass>\n" +
    "<autoupload>" + au + "</autoupload>";

  QFile config(mw->userPath + "ftp");
  config.open(QFile::WriteOnly | QFile::Text);
  QTextStream cs(&config);
  cs << data;
  config.close();
}

void ftpDialog::loadConfig()
{
  if(QFile::exists(mw->userPath + "ftp"))
  {
    QFile config(mw->userPath + "ftp");
    config.open(QFile::ReadOnly | QFile::Text);
    QString data = config.readAll();

    int a1, a2;
    a1 = data.indexOf("<host>") + 6;
    a2 = data.indexOf("</host>");
    host = data.mid(a1, a2 - a1);
    a1 = data.indexOf("<login>") + 7;
    a2 = data.indexOf("</login>");
    login = data.mid(a1, a2 - a1);
    a1 = data.indexOf("<pass>") + 6;
    a2 = data.indexOf("</pass>");
    password = data.mid(a1, a2 - a1);
    a1 = data.indexOf("<autoupload>") + 12;
    a2 = data.indexOf("</autoupload>");
    if(data.mid(a1, a2 - a1) == "1")
    {
      ui.checkBox_2->setCheckState(Qt::Checked);
      mw->config.ftp_autoupload = true;
    }
    else
    {
      ui.checkBox_2->setCheckState(Qt::Unchecked);
      mw->config.ftp_autoupload = false;
    }

    ui.lineEdit->setText(host);
    ui.lineEdit_2->setText(login);
    ui.lineEdit_3->setText(password);
    ui.checkBox->setCheckState(Qt::Checked);
    ui.checkBox_2->setEnabled(true);
  }
}
