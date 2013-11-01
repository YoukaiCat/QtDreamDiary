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

#include <QApplication>
#include <QTranslator>
#include <QDesktopServices>
#include "MainForm.h"
#include "loginDialog.h"
using namespace std;
int main(int args, char *argv[])
{
  QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF8"));

  QApplication app(args, argv);
#ifdef INSTALLATION_PREFIX
  QApplication::setWindowIcon(QIcon(QString(INSTALLATION_PREFIX) + "/themes/winicon.png"));
#else
  QApplication::setWindowIcon(QIcon(QCoreApplication::applicationDirPath() + "/themes/winicon.png"));
#endif
  QTranslator translator;
  translator.load(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/mdd/locale/current");
  app.installTranslator(&translator);

  int w = QApplication::desktop()->width();
  int h = QApplication::desktop()->height();
  MainForm *mainWindow = new MainForm();
  loginDialog *login = new loginDialog(mainWindow);
  if(!login->AutoLogin)
  {
    login->setGeometry(w / 2 - login->width() / 2,
		       h / 2 - login->height() / 2, login->width(),
		       login->height());
    login->exec();
    delete login;
  }

  if(mainWindow->loginOk)
  {
    mainWindow->init();
    app.exec();
  }

  delete mainWindow;
  return 0;

}
