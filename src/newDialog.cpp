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

#include "newDialog.h"
#include <QCryptographicHash>

newDialogClass::newDialogClass(MainForm * parent)
{
  ui.setupUi(this);
  mw = parent;
  setWindowFlags(Qt::Widget);
  connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(pushButtonClicked()));
  connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(close()));
}

void newDialogClass::pushButtonClicked()
{
  QString username = ui.lineEdit->text();
  QString
    userPath =
    QCoreApplication::applicationDirPath() + "/profiles/" + username;
  // lista dozwolonych znakow:
  QList < short int > chars;
  for(short int i = 48; i <= 57; i++)	// '0' - '9'
    chars.push_back(i);
  for(short int i = 65; i <= 90; i++)	// 'A' - 'Z'
    chars.push_back(i);
  for(short int i = 97; i <= 122; i++)	// 'a' - 'z'
    chars.push_back(i);
  chars.push_back(95);		// _

  // sprawdzanie czy wszystkie znaki sa dozwolone:
  // w nazwie usera
  for(int i = 0; i < ui.lineEdit->text().size(); i++)
    if(!chars.contains(ui.lineEdit->text().at(i).toAscii()))
    {
      QMessageBox::warning(this, tr("Error"),
               tr
               ("Forbidden characters apeared in the Username. \"")
               + ui.lineEdit->text().at(i) + "\"",
               QMessageBox::Ok);
      return;
    }
  // w hasle
  for(int i = 0; i < ui.lineEdit_2->text().size(); i++)
    if(!chars.contains(ui.lineEdit_2->text().at(i).toAscii()))
    {
      QMessageBox::warning(this, tr("Error"),
               tr
               ("Forbidden characters appear in the password: \"")
               + ui.lineEdit_2->text().at(i) +
               "\"", QMessageBox::Ok);
      return;
    }
  // sprawdzanie czy wypelniono wszystkie pola
  if((ui.lineEdit->text().size() == 0) ||
     (ui.lineEdit_2->text().size() == 0) ||
     (ui.lineEdit_3->text().size() == 0))
  {
    QMessageBox::warning(this, tr("Error"),
             tr("You need to fill all fields!"),
             QMessageBox::Ok);
    return;
  }

  /*
   * fragment usuniety na prosbe userow
   *
   * //czy haslo ma co najmniej 3 znaki if(
   * (ui.lineEdit->text().size()<3) || (ui.lineEdit_2->text().size() <3)
   * ) { QMessageBox::warning(this,tr("Error"),tr("Wszystkie dane
   * muszą zawierać co najmniej trzy znaki!"),QMessageBox::Ok);
   * return; }
   *
   * if(ui.lineEdit->text() == ui.lineEdit_2->text()) {
   * QMessageBox::warning(this,tr("Error"),tr("Login i hasło nie mogą
   * być takie same!"),QMessageBox::Ok); return; }
   */

  if(ui.lineEdit_3->text() != ui.lineEdit_2->text())
  {
    QMessageBox::warning(this, tr("Error"),
             tr("Wrong password! Retype it!"),
             QMessageBox::Ok);
    ui.lineEdit_3->clear();
    ui.lineEdit_2->clear();
    return;
  }

  if(QFile::exists(userPath + "/config.cfg"))
  {
    QMessageBox::warning(this, tr("Error"),
             tr("User already exists!"),
             QMessageBox::Ok);
    return;
  }
  // tworzenie usera:

  QString
    hash = QCryptographicHash::hash(ui.lineEdit_2->text().toUtf8(),
				    QCryptographicHash::Md5).toHex();

  QDir dir;
  if(!dir.exists(QCoreApplication::applicationDirPath() + "/profiles"))	// tworzenie 
    // katalogu 
    // profiles 
    // jesli 
    // go 
    // nie 
    // ma
    dir.mkdir(QCoreApplication::applicationDirPath() + "/profiles");
  dir.mkdir(userPath);		// tworzenie katalogu usera
  dir.mkdir(userPath + "/data");	// tworzenie katalogu dla
  // dokumentow

  XConfig config(userPath + "/config.cfg");

  if((config.createDefault(hash)) && (config.createDefaultHtmlTemplate(userPath + "/template.html")))	// tworzenie 
    // domyslnego 
    // pliku 
    // konfiguracyjnego
  {
    QMessageBox::information(this, tr("Success"),
                 tr
                 ("The new profile has been created! You can login now"),
                 QMessageBox::Ok);
    this->close();
  }
  else
  {
    QMessageBox::information(this, tr("Error"),
                 tr
                 ("Unknown error appeard while creating profile. Your account can work inappropriate."),
                 QMessageBox::Ok);
    this->close();
  }
}

void newDialogClass::pushButton_2Clicked()
{
  this->close();
}
