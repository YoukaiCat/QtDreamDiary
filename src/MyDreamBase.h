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


#ifndef MYDREAMBASE_H
#define MYDREAMBASE_H
#include "MainForm.h"
#include "vigcryp.h"
#include <QApplication>
#include <QColor>
#include <QFont>
#include <QFile>
#include <QDate>
#include <QListWidget>
#include <QTreeWidget>
#include <QTextStream>
#include <QTextCodec>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

class MainForm;

struct DreamDocument
{
  int ID;
  QString content;
  QString title;
  QString comment;
  QDate date;
  QTime sleep;
  QTime wakeup;
  short int score;
  bool lucid;
};

struct Key
{
  int ID;
  int freq;
  QStringList keys;
};

struct Suggestion
{
  QString name;
  int occ;
};

//------------------------------------------------------------------
class Suggestions
{
public:
  Suggestions()
  {
  };
  ~Suggestions()
  {
  };
  void add(QString);		//dodaje, jesli istnieje dodaje +1 do occ
  Suggestion takeBest();	//zwraca sugestie z najwiekszym occ i usuwa ja z listy
  int exists(QString);		//zwraca -1 jesli nazwa nie istnieje, jesli tak zwraca jej index
  void removeSeldom(int);	//usuwa sugestie pojawiajace sie mniej razy niz podano w argumencie
  QList < Suggestion > sugs;
};

class Keys
{
public:
  Keys();
  ~Keys();
  const Key *at(int);
  int add(QString);
  bool addSynonym(int, QString);
  int generateID();
  bool remove(int);
  bool removeSynonym(int, QString);
  bool exists(int);
  bool contains(QString);
  const Key *getByID(int);
  int getByName(QString);
  int count();
  void rename(QString, QString);
  void countFreq(const QList < DreamDocument > *);
  void sortByName();
  void sortByFreq();
  void fillTree(QTreeWidget *, const QList < DreamDocument > *, bool);
  void fillCurrentList(QListWidget *, QString);
    QList < Key > *keysList;
};

class MyDreamBase:public QObject
{
Q_OBJECT public:
    MyDreamBase(MainForm * parent);
   ~MyDreamBase();
  Keys *keys;			//glowna lista kluczy
  QStringList *ignoredSuggestions;
  QColor currentColor;
  QColor foundColor;
  QColor lucidColor;
  bool loadBase(bool);		//wczytuje baze z pliku xml
  bool exists(int);		//sprawdza czy istnieje podane id
  void add(DreamDocument);	//dodaje nowy dokument
  bool update(DreamDocument);	//wstawia podany dokument w miejsce dokumentu z ID
  bool toFile(int, bool, QString file = "default");	//zapisuje dokument o padanym ID do pliku
  bool remove(int);		//usuwa dokument o podanym ID
  void exportToFile(QString);	//zapisuje archiwum do pliku
  void importFromFile(QString);	//wczytuje archiwum z pliku
  void importFromDrm(QString);	//wczytuje archiwum z pliku *.drm (ver<1.9)
  void saveToHtml(QString);	//zapisuje aktualny dokument wg szablonu
  bool openFile(QString);	//dodaje do bazy wczytany plik *.md
  void closeCurrent();		//zamyka obecny dokument
  void rewriteAll(bool);	//przepisuje pliki z szyfrowaniem lub bez
  int Current();		//zwraca ID aktualnie otwartego dokumentu
  int generateID();		//generuje nieistniejace ID
  int count();			//zwraca ilosc elementow w liscie
  bool saveKeys(bool);		//zapisuje baze kluczy do pliku
  bool loadKeys(bool);		//wczytuje baze kluczy z pliku pliku
  bool addIgnored(QString);	//dodaje wyrazenie do listy ignorowanych sugestii
  bool removeIgnored(QString);	//usuwa wyrazenie z listy ignorowanych sugestii
  const DreamDocument *at(int);	//zwraca adres dokumentu o podanym indeksie w liscie
  const DreamDocument *getByID(int);	//zwraca adres dokumentu o podanym ID
  const DreamDocument *open(int);	//zwraca asres dokumentu o podanym ID,
  //wypelnia GUI i ustawia current
    QList < int >getByTitle(QString);	//lista ID dokumentow o podanym tytule
    QList < int >getByContent(QString);	//ID dokumentow z podana trescia
  const QList < DreamDocument > *list();
  void fillTree(QTreeWidget *);	//wypelnia drzewo
  void expandDoc(int, QTreeWidget *);	//rozwija dokument o podanym ID w podanym drzewie
  void colorItems(QList < int >, QTreeWidget *, QColor);	//koloruje elementy o ID z listy
  void colorItem(int, QTreeWidget *, QColor);	//koloruje element o ID

private:
    QList < DreamDocument > *dreamsList;	//glowna lista dokumentow
  int current;
  MainForm *mw;
};


#endif
