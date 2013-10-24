/***************************************************************************
 *   Copyvigcryp::right (C) 2008 by Łukasz Chodyła   *
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

#include "loginDialog.h"

loginDialog::loginDialog(MainForm * parent)
{
  ui.setupUi(this);
  mw = parent;
  savePass = false;
  AutoLogin = false;
  nd = NULL;
  // jesli ostatni profil byl zapamietany
  Last last;
  if(last.load(mw->exePath + "/last"))
  {
    ui.loginEdit->setText(last.login);
    ui.passwordEdit->setText(last.passwd);
    ui.checkBox->setCheckState(Qt::Checked);
    AutoLogin = last.autologin;

    savePass = true;
  }
  setWindowFlags(Qt::Widget);
  connect(ui.loginButton, SIGNAL(clicked()), this,
	  SLOT(loginButtonClicked()));
  connect(ui.newButton, SIGNAL(clicked()), this, SLOT(newButtonClicked()));
  connect(ui.checkBox, SIGNAL(stateChanged(int)), this,
	  SLOT(checkBoxChanged(int)));
  // ------------------------------------------------------------------------------------
  QString userPath = mw->exePath + "/profiles/" + ui.loginEdit->text() + "/";

  if(ui.checkBox->checkState() == Qt::Checked)
    ui.checkBox_2->setEnabled(true);	// nie potrzebne w
  // normalnych warunkach

  if(AutoLogin)
  {
    login = last.login;;
    password = last.passwd;

    // md5 z hasla:
    QString hash = QCryptographicHash::hash(password.toUtf8(),
					    QCryptographicHash::Md5).toHex();

    // sprawdzamy czy pola sa wypelnione
    if((login.isEmpty()) || (password.isEmpty()))
      AutoLogin = false;

    // sprawdzamy czy istnieje podany user
    if(!QFile::exists(userPath + "config.cfg"))
      AutoLogin = false;

    // wysylamy klucz szyfrowania i haslo
    mw->vig_key = hash;
    mw->password = password;

    // sprawdzamy czy zaszyfrowane haslo sie zgadza
    XConfig config(userPath + "config.cfg");

    if(hash != config.passwd)
      AutoLogin = false;
  }

  if(AutoLogin)
  {
    mw->userPath = userPath;
    mw->username = login;
    mw->setWindowTitle("My Dream Diary " + VER + " - " + mw->username);
    mw->loginOk = true;
  }
}

// ------------------------------------------------------------------------------------
void loginDialog::loginButtonClicked()
{
  login = ui.loginEdit->text();
  password = ui.passwordEdit->text();

  // md5 z hasla:
  QString
    hash = QCryptographicHash::hash(password.toUtf8(),
				    QCryptographicHash::Md5).toHex();

  // sprawdzamy czy pola sa wypelnione
  if((login.isEmpty()) || (password.isEmpty()))
  {
    QMessageBox::warning(this, tr("Błąd"),
			 tr
			 ("Nie wszystkie wymagane pola zostały wypełnione!"),
			 QMessageBox::Ok);
    return;
  }
  // sprawdzamy czy istnieje podany user
  QString userPath = mw->exePath + "/profiles/" + login + "/";
  if(!QFile::exists(userPath + "config.cfg"))
  {
    QMessageBox::warning(this, tr("Błąd"),
			 tr("Podany użytkownik nie istnieje!"),
			 QMessageBox::Ok);
    return;
  }
  // wysylamy do glownego okna klucz szyfrowania i haslo
  mw->vig_key = hash;
  mw->password = password;

  // sprawdzamy czy zaszyfrowane haslo sie zgadza
  XConfig config(userPath + "config.cfg");

  if(hash != config.passwd)
  {
    QMessageBox::warning(this, tr("Błąd"),
			 tr("Niepoprawne hasło!"), QMessageBox::Ok);
    return;
  }
  else
  {
    if(savePass)
    {
      Last last;
      last.passwd = ui.passwordEdit->text();
      last.login = ui.loginEdit->text();
      if(ui.checkBox_2->checkState() == Qt::Checked)
	last.autologin = true;
      else
	last.autologin = false;

      last.save(mw->exePath + "/last");
    }
    else
    {
      Last last;
      if(last.load(mw->exePath + "/last"))
	if(last.login == mw->username)
	  if(QFile::exists(mw->exePath + "/last"))
	    QFile::remove(mw->exePath + "/last");
    }

    mw->userPath = userPath;
    mw->username = login;
    mw->setWindowTitle("My Dream Diary " + VER + " - " + mw->username);
    mw->loginOk = true;
    this->close();
  }

}

void loginDialog::newButtonClicked()
{
  if(nd == NULL)
    nd = new newDialogClass(mw);
  nd->exec();
}

void loginDialog::checkBoxChanged(int s)
{
  if(s == Qt::Checked)
  {
    savePass = true;
    ui.checkBox_2->setEnabled(true);
    QMessageBox::information(this, tr("informacja"),
			     tr
			     ("Zezwalasz na zapis niezaszyfrowanych danych, każdy użytkownik tej stacji może mieć do nich dostęp."),
			     QMessageBox::Ok);
  }
  else
  {
    savePass = false;
    ui.checkBox_2->setEnabled(false);
  }
}
