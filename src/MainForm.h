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


#ifndef MAINFORM_H
#define MAINFORM_H
#include "ui_MainForm.h"
#include "vigcryp.h"
#include "xconfig.h"
#include <iostream>
#include <QtNetwork/QFtp>
#include <QtNetwork/QHttp>
#include <QUrl>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QThread>
#include <QTextCodec>
#include <QTextStream>
#include <QTextBlock>
#include <QTextLayout>
#include <QColorDialog>
#include <QFontDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QSystemTrayIcon>
#include <QWhatsThis>
#include <QPainter>
#include <QProcess>
#include <QTime>
#include <QIcon>
#include <QList>
#include <QColor>
#include <QFile>
#include <QDir>
#include "loginDialog.h"
#include "findDialog.h"
#include "replaceDialog.h"
#include "aboutDialog.h"
#include "settingsDialog.h"
#include "statDialog.h"
#include "symbolDialog.h"
#include "ftpDialog.h"
#include "MyDreamBase.h"
#include "ignoredDialog.h"

class MyDreamBase;
class aboutDialog;
class loginDialog;
class findDialog;
class ftpDialog;
class symbolDialog;
class statDialog;
class settingsDialog;
class replaceDialog;
class ignoredDialog;

class Suggestions;
class CounterThread;
struct Suggestion;
struct DreamDocument;

const QString VER = "0.2.5";

class MainForm:public QMainWindow
{
Q_OBJECT public:
    MainForm();
   ~MainForm();
  XConfig config;
    Ui::MainWindow ui;
  MyDreamBase *dreamBase;
  QSystemTrayIcon *trayIcon;
  QHttp http;
  QString username;
  QString userPath;
  QString exePath;
  QString themePath;
  QString vig_key;
  QString password;
  bool loginOk;
  bool changed;
  void init();
  void loadIcons();
  void setFont(QFont);
  void highlightAll();
private:
    ftpDialog * ftp_dialog;
  loginDialog *login_dialog;	//do testow
  findDialog *find_dialog;
  replaceDialog *replace_dialog;
  aboutDialog *about_dialog;
  symbolDialog *symbol_dialog;
  statDialog *stat_dialog;
  settingsDialog *settings_dialog;
  ignoredDialog *ignored_dialog;
  CounterThread *counter;
  Suggestion *currentSuggestion;
  bool profile_deleting;
  bool full;
  bool closed;
  //void fillList();
  //void fillBySelected();
  void highlightBlock(int, int, int);
  void clearUi();
  void closeEvent(QCloseEvent *);
  void keyPressEvent(QKeyEvent *);
  void suggestionMessage(Suggestion *, QString);
  void checkVersion();
  private slots:
    //akcje
  void aNew();
  void aOpenFile();
  void aSave();
  void aSaveAs();
  void aDelete();
  void aDeleteAll();
  void aDeleteSelected();
  void aToHtml();
  void aToFile();
  void aFromFile();
  void aFromDrm();
  void aToFTP();
  void aLogout();
  void aLoadTemplate();
  void aDeleteProfile();
  void aFontSettings();
  void aFind();
  void aSettings();
  void aScale();
  void aWhatsThis();
  void aManual();
  void aReplace();
  void aAbout();
  void aSearch();
  void aSearchContent();
  void aRollUp();
  void aRollDown();
  void aStats();
  void aSymbols();
  void aCounterStart();
  void aIgnoredList();
  //akcje widoku:
  void aDefaultView();
  void aDockShowHide();
  void aToolBarShowHide();
  void aDockLeft();
  void aDockRight();
  void aDockFloat();
  void aKeyFreqSort();
  void aKeyAscSort();
  void aLangEn();
  void aLangPl();
  void aLangOther();

  //---------------------------
  void addNewKey();
  void addNewSynonym();
  void insertNewKey();
  void webFind();
  void deleteKey();
  void deleteAllKeys();
  void renameKey();
  //=======================
  void textContentsChange(int, int, int);
  void contentChange();
  void treeItemClicked();
  void textEditContextMenu();
  void docTreeContextMenu();
  void keyListContextMenu();
  void treeItemExpanded(QTreeWidgetItem *);
  void treeItemCollapsed(QTreeWidgetItem *);
  void lucidStateChanged();
  void dockLocationChanged(Qt::DockWidgetArea);
  void counterFinished();
  void messageClicked();

  void httpReadyRead();
};

//--------------------------------------------------

class CounterThread:public QThread
{
public:
  CounterThread();
  ~CounterThread();
  bool custom;
  int timeElapsed;
  void run();
  void setup(int wordcount, int ignored, int sensitiveness,
	     Range range, QSystemTrayIcon * ti, MyDreamBase * mb);

  void prepare1(QStringList *);	//usuwa powtarzajace sie wyrazy
  void prepare2(QStringList *);	//usuwa powtarzajace sie wyrazenia dwuwyrazowe
  QStringList wordList();	//zwraca liste dokumentow z podanego okresu

  Suggestions *suggestions;
private:
    bool ready;
  QStringList *ignored_list;
  MyDreamBase *base;
  QSystemTrayIcon *tray;
  int words;
  int ignored_size;
  int sensit;
  Range days_limit;		//deklaracja typu Range w xconfig.h
  QDate weekAgo();
  QDate monthAgo();
};

#endif
