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

#include "MyDreamBase.h"
// ===============================================================
// K L A S A D O K U M E N T O W

MyDreamBase::MyDreamBase(MainForm * parent)
{
  mw = parent;
  dreamsList = new QList < DreamDocument >;
  ignoredSuggestions = new QStringList();
  keys = new Keys();
  current = -1;

  currentColor = QColor(144, 144, 144);	// 
  foundColor = QColor(10, 149, 255);	// przeniesc do configa
  lucidColor = QColor(255, 251, 135);	// 
}

MyDreamBase::~MyDreamBase()
{
  dreamsList->clear();
  delete dreamsList;
  delete keys;
  ignoredSuggestions->clear();
  delete ignoredSuggestions;
}

// -----------------------------------------------------------------------------------------

bool MyDreamBase::loadBase(bool use_encoding)
{
  dreamsList->clear();

  QDir dataDir(mw->userPath + "/data/");
  QStringList files = dataDir.entryList(QDir::NoDotAndDotDot | QDir::Files);

  bool allOK = true;
  QString tmp, allData;
  QDomDocument xdoc;
  QDomNode document;
  for(int i = 0; i < files.count(); i++)
  {
    QFile file(mw->userPath + "data/" + files.at(i));
    if(file.open(QFile::ReadOnly | QFile::Text))
    {
      QTextStream fi(&file);
      fi.setAutoDetectUnicode(false);
      fi.setCodec(QTextCodec::codecForName(mw->config.codecName.toLatin1()));
      allData = fi.readAll();
      file.close();

      if(use_encoding)
	allData = vigcryp::decode(allData, mw->vig_key);

      if(!xdoc.setContent(allData))
	allOK = false;

      document = xdoc.elementsByTagName("document").item(0);

      DreamDocument doc;
      doc.title =
	document.toElement().elementsByTagName("title").item(0).firstChild().
	toText().data();
      doc.date =
	QDate::fromString(document.toElement().elementsByTagName("date").
			  item(0).firstChild().toText().data(), "ddMMyyyy");

      if(document.toElement().elementsByTagName("lucid").item(0).firstChild().
	 toText().data() == "0")
	doc.lucid = false;
      else
	doc.lucid = true;

      doc.sleep =
	QTime::fromString(document.toElement().elementsByTagName
			  ("sleeptime").item(0).firstChild().toText().data());
      doc.wakeup =
	QTime::fromString(document.toElement().elementsByTagName
			  ("wakeuptime").item(0).firstChild().toText().
			  data());

      doc.score =
	document.toElement().elementsByTagName("score").item(0).firstChild().
	toText().data().toInt();
      doc.content =
	document.toElement().elementsByTagName("content").item(0).
	firstChild().toText().data();
      doc.comment =
	document.toElement().elementsByTagName("comment").item(0).
	firstChild().toText().data();

      tmp = files.at(i);
      tmp.remove(tmp.size() - 3, 3);
      doc.ID = tmp.toInt();
      dreamsList->push_back(doc);
    }
    else
    {
      return false;
    }
  }
  return allOK;
}

// -----------------------------------------------------------------------------------------

bool MyDreamBase::openFile(QString filename)
{
  QString allData;
  QDomDocument xdoc;
  QDomNode document;
  QFile file(filename);
  if(file.open(QFile::ReadOnly | QFile::Text))
  {
    QTextStream fi(&file);
    fi.setAutoDetectUnicode(false);
    fi.setCodec(QTextCodec::codecForName(mw->config.codecName.toLatin1()));
    allData = fi.readAll();
    file.close();

    if(!xdoc.setContent(allData))
      return false;
    if(xdoc.doctype().name() != "mdd_document")
    {
      QMessageBox::critical(mw, tr("Error"),
                tr
                ("The selected file is not a  My Dream Diary file!"),
                QMessageBox::Ok);
      return false;
    }
    document = xdoc.elementsByTagName("document").item(0);

    DreamDocument doc;
    doc.title =
      document.toElement().elementsByTagName("title").item(0).firstChild().
      toText().data();
    doc.date =
      QDate::fromString(document.toElement().elementsByTagName("date").
			item(0).firstChild().toText().data(), "ddMMyyyy");

    if(document.toElement().elementsByTagName("lucid").item(0).firstChild().
       toText().data() == "0")
      doc.lucid = false;
    else
      doc.lucid = true;

    doc.sleep =
      QTime::fromString(document.toElement().elementsByTagName
			("sleeptime").item(0).firstChild().toText().data());
    doc.wakeup =
      QTime::fromString(document.toElement().elementsByTagName("wakeuptime").
			item(0).firstChild().toText().data());

    doc.score =
      document.toElement().elementsByTagName("score").item(0).firstChild().
      toText().data().toInt();
    doc.content =
      document.toElement().elementsByTagName("content").item(0).firstChild().
      toText().data();
    doc.comment =
      document.toElement().elementsByTagName("comment").item(0).firstChild().
      toText().data();

    doc.ID = generateID();
    dreamsList->push_back(doc);
    toFile(doc.ID, mw->config.base_coding);
    open(doc.ID);
  }
  else
  {
    return false;
  }
  return true;
}

// -----------------------------------------------------------------------------------------
bool MyDreamBase::exists(int id)
{
  if(id < 0)
    return true;
  for(int i = 0; i < dreamsList->count(); i++)
    if(dreamsList->at(i).ID == id)
      return true;

  return false;
}

// -----------------------------------------------------------------------------------------
int MyDreamBase::generateID()
{
  int id = 0;
  while(exists(id))
  {
    id++;
  }
  return id;
}

// -----------------------------------------------------------------------------------------
void MyDreamBase::add(DreamDocument doc)
{
  doc.ID = generateID();
  dreamsList->push_back(doc);
  toFile(doc.ID, mw->config.keys_coding);
  open(doc.ID);
}

// -----------------------------------------------------------------------------------------
bool MyDreamBase::update(DreamDocument doc)
{
  int id = doc.ID;
  for(int i = 0; i < dreamsList->count(); i++)
  {
    if(dreamsList->at(i).ID == id)
    {

      dreamsList->replace(i, doc);
      toFile(id, mw->config.keys_coding);
      return true;
    }
  }

  return false;
}

// -----------------------------------------------------------------------------------------
bool MyDreamBase::toFile(int id, bool use_encoding, QString filename)
{
  if(id > -1)
  {
    QDomDocument xdoc("mdd_document");
    QDomElement document;
    QDomElement elem;
    QDomText txt;
    const DreamDocument *dd = getByID(id);
    // ----------------------------

    document = xdoc.createElement("document");

    elem = xdoc.createElement("title");
    txt = xdoc.createTextNode(dd->title);
    elem.appendChild(txt);
    document.appendChild(elem);

    elem = xdoc.createElement("date");
    txt = xdoc.createTextNode(dd->date.toString("ddMMyyyy"));
    elem.appendChild(txt);
    document.appendChild(elem);

    elem = xdoc.createElement("lucid");
    if(dd->lucid)
      txt = xdoc.createTextNode(dd->date.toString("1"));
    else
      txt = xdoc.createTextNode(dd->date.toString("0"));
    elem.appendChild(txt);
    document.appendChild(elem);

    elem = xdoc.createElement("sleeptime");
    txt = xdoc.createTextNode(dd->sleep.toString());
    elem.appendChild(txt);
    document.appendChild(elem);

    elem = xdoc.createElement("wakeuptime");
    txt = xdoc.createTextNode(dd->wakeup.toString());
    elem.appendChild(txt);
    document.appendChild(elem);

    QString n;
    n.setNum(dd->score);
    elem = xdoc.createElement("score");
    txt = xdoc.createTextNode(n);
    elem.appendChild(txt);
    document.appendChild(elem);

    elem = xdoc.createElement("content");
    txt = xdoc.createTextNode(dd->content);
    elem.appendChild(txt);
    document.appendChild(elem);

    elem = xdoc.createElement("comment");
    txt = xdoc.createTextNode(dd->comment);
    elem.appendChild(txt);
    document.appendChild(elem);

    xdoc.appendChild(document);

    if(filename == "default")
    {
      QDir dir;
      if(!dir.exists(mw->userPath + "/data"))
	dir.mkdir(mw->userPath + "/data");	// tworzenie
      // katalogu dla
      // dokumentow

      n.setNum(dd->ID);
      QFile file(mw->userPath + "/data/" + n + ".md");
      if(!file.open(QFile::WriteOnly | QFile::Text))
	return false;
      QTextStream fo(&file);
      fo.setAutoDetectUnicode(false);
      fo.setCodec(QTextCodec::codecForName(mw->config.codecName.toLatin1()));

      if(use_encoding)
	fo << vigcryp::encode(xdoc.toString(), mw->vig_key);
      else
	fo << xdoc.toString();

      file.close();
    }
    else
    {
      if(filename.right(3) != ".md")
	filename += ".md";

      QFile file(filename);
      if(!file.open(QFile::WriteOnly | QFile::Text))
	return false;
      QTextStream fo(&file);
      fo.setAutoDetectUnicode(false);
      fo.setCodec(QTextCodec::codecForName(mw->config.codecName.toLatin1()));
      fo << xdoc.toString();
      file.close();
    }
    return true;
  }
  return false;
}

// -----------------------------------------------------------------------------------------
void MyDreamBase::exportToFile(QString filename)
{
  QDomDocument xdoc("MyDreamDiary_base");
  QDomElement root = xdoc.createElement("root");
  QDomElement document;
  QDomElement elem;
  QDomElement syn;
  QDomText txt;
  xdoc.appendChild(root);
  DreamDocument dd;

  // ----------------------------
  // baza dokumentow:
  for(int i = 0; i < dreamsList->count(); i++)
  {
    dd = dreamsList->at(i);
    document = xdoc.createElement("document");

    elem = xdoc.createElement("title");
    txt = xdoc.createTextNode(dd.title);
    elem.appendChild(txt);
    document.appendChild(elem);

    elem = xdoc.createElement("date");
    txt = xdoc.createTextNode(dd.date.toString("ddMMyyyy"));
    elem.appendChild(txt);
    document.appendChild(elem);

    elem = xdoc.createElement("lucid");
    if(dd.lucid)
      txt = xdoc.createTextNode(dd.date.toString("1"));
    else
      txt = xdoc.createTextNode(dd.date.toString("0"));
    elem.appendChild(txt);
    document.appendChild(elem);

    elem = xdoc.createElement("sleeptime");
    txt = xdoc.createTextNode(dd.sleep.toString());
    elem.appendChild(txt);
    document.appendChild(elem);

    elem = xdoc.createElement("wakeuptime");
    txt = xdoc.createTextNode(dd.wakeup.toString());
    elem.appendChild(txt);
    document.appendChild(elem);

    QString n;
    n.setNum(dd.score);
    elem = xdoc.createElement("score");
    txt = xdoc.createTextNode(n);
    elem.appendChild(txt);
    document.appendChild(elem);

    elem = xdoc.createElement("content");
    txt = xdoc.createTextNode(dd.content);
    elem.appendChild(txt);
    document.appendChild(elem);

    elem = xdoc.createElement("comment");
    txt = xdoc.createTextNode(dd.comment);
    elem.appendChild(txt);
    document.appendChild(elem);

    root.appendChild(document);
  }
  // baza kluczy:
  document = xdoc.createElement("keys");
  for(int i = 0; i < keys->count(); i++)
  {
    elem = xdoc.createElement("key");
    // ---------
    syn = xdoc.createElement("k");	// klucz bazowy
    txt = xdoc.createTextNode(keys->at(i)->keys.at(0));
    syn.appendChild(txt);
    elem.appendChild(syn);
    for(int j = 1; j < keys->at(i)->keys.count(); j++)
    {
      syn = xdoc.createElement("s");	// klucz pochodny/synonim
      txt = xdoc.createTextNode(keys->at(i)->keys.at(j));
      syn.appendChild(txt);
      elem.appendChild(syn);
    }

    document.appendChild(elem);
  }
  root.appendChild(document);

  QFile base(filename);
  if(!base.open(QFile::WriteOnly | QFile::Text))
    return;
  QTextStream fo(&base);
  fo.setAutoDetectUnicode(false);
  fo.setCodec(QTextCodec::codecForName(mw->config.codecName.toLatin1()));
  fo << xdoc.toString();

  base.close();
}

// -----------------------------------------------------------------------------------------
void MyDreamBase::importFromFile(QString filename)
{
  QString tmp;
  QFile base(filename);
  QDomDocument xdoc;
  if(!base.open(QFile::ReadOnly | QFile::Text))
    return;

  QString allData;
  QTextStream fi(&base);
  fi.setAutoDetectUnicode(false);
  fi.setCodec(QTextCodec::codecForName(mw->config.codecName.toLatin1()));
  allData = fi.readAll();
  base.close();

  if(!xdoc.setContent(allData))
  {
    base.close();
    return;
  }
  base.close();

  if(xdoc.doctype().name() != "MyDreamDiary_base")
  {
    QMessageBox::critical(mw, tr("Error"),
              tr
              ("The selected file is not a My Dream Diary archive file!"),
              QMessageBox::Ok);
    return;
  }

  dreamsList->clear();
  keys->keysList->clear();
  ignoredSuggestions->clear();
  // wczytywanie dokumentow
  QDomNodeList documents = xdoc.elementsByTagName("document");
  QDomNode document;
  for(int i = 0; i < documents.count(); i++)
  {
    document = documents.item(i);

    DreamDocument doc;
    doc.title =
      document.toElement().elementsByTagName("title").item(0).firstChild().
      toText().data();
    doc.date =
	QDate::fromString(document.toElement().elementsByTagName("date").
			  item(0).firstChild().toText().data(), "ddMMyyyy");

    if(document.toElement().elementsByTagName("lucid").item(0).firstChild().
       toText().data() == "0")
      doc.lucid = false;
    else
      doc.lucid = true;

    doc.sleep =
      QTime::fromString(document.toElement().elementsByTagName
			("sleeptime").item(0).firstChild().toText().data());
    doc.wakeup =
      QTime::fromString(document.toElement().elementsByTagName("wakeuptime").
			item(0).firstChild().toText().data());

    doc.score =
      document.toElement().elementsByTagName("score").item(0).firstChild().
      toText().data().toInt();
    doc.content =
      document.toElement().elementsByTagName("content").item(0).firstChild().
      toText().data();
    doc.comment =
      document.toElement().elementsByTagName("comment").item(0).firstChild().
      toText().data();

    doc.ID = i;
    dreamsList->push_back(doc);
  }

  // wczytywanie kluczy:
  documents =
    xdoc.elementsByTagName("keys").item(0).toElement().
    elementsByTagName("key");
  for(int i = 0; i < documents.count(); i++)
  {
    int
      id =
      keys->add(documents.item(i).firstChild().firstChild().toText().data());

    QDomNodeList
      synonyms = documents.item(i).toElement().elementsByTagName("s");
    for(int j = 0; j < synonyms.count(); j++)
      keys->addSynonym(id, synonyms.item(j).firstChild().toText().data());

  }
}

// -----------------------------------------------------------------------------------------
void MyDreamBase::importFromDrm(QString filename)
{
  QString tmp;
  QFile base(filename);
  QDomDocument xdoc;
  DreamDocument doc;
  if(!base.open(QFile::ReadOnly | QFile::Text))
    return;

  QString allData;
  QTextStream fi(&base);
  fi.setAutoDetectUnicode(false);
  fi.setCodec(QTextCodec::codecForName(mw->config.codecName.toLatin1()));
  allData = fi.readAll();
  base.close();

  allData = "<!DOCTYPE drm_base>\n<root>\n" + allData + "\n</root>\n";

  if(!xdoc.setContent(allData))
  {
    base.close();
    return;
  }
  base.close();

  dreamsList->clear();
  keys->keysList->clear();
  ignoredSuggestions->clear();
  // wczytywanie dokumentow
  QDomNodeList documents = xdoc.elementsByTagName("document"), mddkeys;

  QDomNodeList drmkeys;
  QDomNode document;
  for(int i = 0; i < documents.count(); i++)
  {
    document = documents.item(i);

    doc.title =
      document.toElement().elementsByTagName("title").item(0).firstChild().
      toText().data();
    doc.date =
      QDate::fromString(document.toElement().elementsByTagName("date").
			item(0).firstChild().toText().data(), "ddMMyyyy");

    doc.lucid = false;

    doc.sleep =
      QTime::fromString(document.toElement().elementsByTagName("begin").
			item(0).firstChild().toText().data());
    doc.wakeup =
      QTime::fromString(document.toElement().elementsByTagName("end").
			item(0).firstChild().toText().data());

    doc.score =
      document.toElement().elementsByTagName("score").item(0).firstChild().
      toText().data().toInt();
    doc.content =
      document.toElement().elementsByTagName("content").item(0).firstChild().
      toText().data();
    doc.comment =
      document.toElement().elementsByTagName("comment").item(0).firstChild().
      toText().data();

    doc.ID = generateID();
    dreamsList->push_back(doc);

    // wczytywanie kluczy:
    mddkeys = document.toElement().elementsByTagName("k");
    for(int i = 0; i < mddkeys.count(); i++)
      keys->add(mddkeys.item(i).firstChild().toText().data());
  }

}

// -----------------------------------------------------------------------------------------
void MyDreamBase::rewriteAll(bool use_encoding)
{
  // kasowanie plikow data/*.md
  QDir dataDir(mw->userPath + "/data/");
  QStringList files = dataDir.entryList(QDir::NoDotAndDotDot | QDir::Files);

  for(int i = 0; i < files.count(); i++)
  {
    if(files.at(i).right(3) == ".md")
      QFile::remove(mw->userPath + "/data/" + files.at(i));
  }

  // kasowanie kluczy
  QFile::remove(mw->userPath + "keys.mdk");

  // zapisywanie do plikow wszystkich dokumentow
  for(int i = 0; i < dreamsList->count(); i++)
    toFile(dreamsList->at(i).ID, use_encoding);

  // zapisywanie kluczy
  saveKeys(use_encoding);
}

// -----------------------------------------------------------------------------------------
void MyDreamBase::saveToHtml(QString filename)
{
  QString content;
  if(!QFile::exists(mw->userPath + "/template.html"))
  {
    if(!mw->config.createDefaultHtmlTemplate(mw->userPath + "/template.html"))
    {
      QMessageBox::critical(mw, tr("Error"),
                tr("Creating template for html file failed! Please check for file") + mw->userPath + "/template.html",
                QMessageBox::Ok);
      return;
    }
  }

  QFile temp(mw->userPath + "/template.html");
  temp.open(QFile::ReadOnly | QFile::Text);
  QTextStream str(&temp);
  str.setAutoDetectUnicode(false);
  str.setCodec(QTextCodec::codecForName(mw->config.codecName.toLatin1()));
  content = temp.readAll();
  temp.close();

  int e;
  QString n;
  const DreamDocument *cd = getByID(current);

  e = content.indexOf("#title#");
  while(e != -1)
  {
    content.replace(e, 7, cd->title);
    e = content.indexOf("#title#");
  }

  e = content.indexOf("#date#");
  while(e != -1)
  {
    content.replace(e, 6, cd->date.toString());
    e = content.indexOf("#date#");
  }

  e = content.indexOf("#asleep#");
  while(e != -1)
  {
    content.replace(e, 8, cd->sleep.toString("hh:mm"));
    e = content.indexOf("#asleep#");
  }

  e = content.indexOf("#wakeup#");
  while(e != -1)
  {
    content.replace(e, 8, cd->wakeup.toString("hh:mm"));
    e = content.indexOf("#wakeup#");
  }

  n.setNum(cd->score);
  n += "/10";
  e = content.indexOf("#score#");
  while(e != -1)
  {
    content.replace(e, 7, n);
    e = content.indexOf("#score#");
  }

  e = content.indexOf("#comment#");
  while(e != -1)
  {
    content.replace(e, 9, cd->comment);
    e = content.indexOf("#comment#");
  }

  e = content.indexOf("#content#");
  while(e != -1)
  {
    content.replace(e, 9, cd->content);
    e = content.indexOf("#content#");
  }

  if((filename.right(5) != ".html") && (filename.right(4) != ".htm"))
    filename += ".html";

  QFile file(filename);
  file.open(QFile::WriteOnly | QFile::Text);
  str.reset();
  str.setDevice(&file);
  str.setAutoDetectUnicode(false);
  str.setCodec(QTextCodec::codecForName(mw->config.codecName.toLatin1()));
  str << content;
  file.close();
}

// -----------------------------------------------------------------------------------------
bool MyDreamBase::remove(int id)
{
  QString strid;
  strid.setNum(id);
  for(int i = 0; i < dreamsList->count(); i++)
  {
    if(dreamsList->at(i).ID == id)
    {
      dreamsList->removeAt(i);
      QFile::remove(mw->userPath + "data/" + strid + ".md");
      return true;
    }
  }
  return false;

}

// -----------------------------------------------------------------------------------------
void MyDreamBase::closeCurrent()
{
  current = -1;
}

// -----------------------------------------------------------------------------------------
int MyDreamBase::Current()
{
  return current;
}

// -----------------------------------------------------------------------------------------
int MyDreamBase::count()
{
  return dreamsList->count();
}

// -----------------------------------------------------------------------------------------
bool MyDreamBase::saveKeys(bool use_encoding)
{
  QDomDocument xdoc("mdd_keys");
  QDomElement document;
  QDomElement elem, syn;
  QDomText txt;
  // ----------------------------

  document = xdoc.createElement("keys");
  for(int i = 0; i < keys->count(); i++)
  {
    elem = xdoc.createElement("key");
    // -------
    syn = xdoc.createElement("k");	// klucz bazowy
    txt = xdoc.createTextNode(keys->at(i)->keys.at(0));
    syn.appendChild(txt);
    elem.appendChild(syn);
    for(int j = 1; j < keys->at(i)->keys.count(); j++)	// synonimy
    {
      syn = xdoc.createElement("s");
      txt = xdoc.createTextNode(keys->at(i)->keys.at(j));
      syn.appendChild(txt);
      elem.appendChild(syn);
    }

    document.appendChild(elem);
  }

  // lista ignorowanych sugestii:
  elem = xdoc.createElement("ignored");
  for(int i = 0; i < ignoredSuggestions->count(); i++)
  {
    syn = xdoc.createElement("ig");
    txt = xdoc.createTextNode(ignoredSuggestions->at(i));
    syn.appendChild(txt);
    elem.appendChild(syn);

  }
  document.appendChild(elem);
  xdoc.appendChild(document);

  QFile base(mw->userPath + "keys.mdk");
  if(!base.open(QFile::WriteOnly | QFile::Text))
    return false;
  QTextStream fo(&base);
  fo.setAutoDetectUnicode(false);
  fo.setCodec(QTextCodec::codecForName(mw->config.codecName.toLatin1()));
  if(use_encoding)
    fo << vigcryp::encode(xdoc.toString(), mw->vig_key);
  else
    fo << xdoc.toString();

  base.close();
  return true;
}

// -----------------------------------------------------------------------------------------
bool MyDreamBase::loadKeys(bool use_encoding)
{
  keys->keysList->clear();
  ignoredSuggestions->clear();
  QFile file(mw->userPath + "keys.mdk");
  if(file.open(QFile::ReadOnly | QFile::Text))
  {
    QTextStream fi(&file);
    fi.setAutoDetectUnicode(false);
    fi.setCodec(QTextCodec::codecForName(mw->config.codecName.toLatin1()));
    QString allData = fi.readAll();
    file.close();

    if(use_encoding)
      allData = vigcryp::decode(allData, mw->vig_key);

    QDomDocument xdoc("mdd_keys");
    if(!xdoc.setContent(allData))
      return false;

    QDomNodeList
      documents =
      xdoc.elementsByTagName("keys").item(0).toElement().
      elementsByTagName("key");
    for(int i = 0; i < documents.count(); i++)
    {
      // klucz bazowy
      int
	id =
	keys->add(documents.item(i).firstChild().firstChild().toText().
		  data());

      // synonimy
      QDomNodeList
	synonyms = documents.item(i).toElement().elementsByTagName("s");
      for(int j = 0; j < synonyms.count(); j++)
	keys->addSynonym(id, synonyms.item(j).firstChild().toText().data());

    }

    // ignorowane:
    QDomNodeList
      igs =
      xdoc.elementsByTagName("ignored").item(0).toElement().
      elementsByTagName("ig");
    for(int i = 0; i < igs.count(); i++)
      addIgnored(igs.item(i).firstChild().toText().data());

    return true;
  }
  return false;
}

// -----------------------------------------------------------------------------------------
bool MyDreamBase::addIgnored(QString exp)
{
  if(ignoredSuggestions->contains(exp, Qt::CaseInsensitive))
    return false;
  ignoredSuggestions->push_back(exp);
  return true;
}

// -----------------------------------------------------------------------------------------
bool MyDreamBase::removeIgnored(QString exp)
{
  for(int i = 0; i < ignoredSuggestions->count(); i++)
    if(ignoredSuggestions->at(i) == exp)
    {
      ignoredSuggestions->removeAt(i);
      saveKeys(mw->config.keys_coding);
      return true;
    }
  return false;
}

// -----------------------------------------------------------------------------------------
const DreamDocument *MyDreamBase::at(int index)
{
  return &dreamsList->at(index);
}

// -----------------------------------------------------------------------------------------
const DreamDocument *MyDreamBase::getByID(int id)
{
  for(int i = 0; i < dreamsList->count(); i++)
  {
    if(dreamsList->at(i).ID == id)
      return &dreamsList->at(i);
  }
  return NULL;
}

// -----------------------------------------------------------------------------------------
const DreamDocument *MyDreamBase::open(int id)
{
  for(int i = 0; i < dreamsList->count(); i++)
  {
    if(dreamsList->at(i).ID == id)
    {
      DreamDocument doc = dreamsList->at(i);
      current = id;
      mw->ui.titleEdit->setText(doc.title);
      mw->ui.dateEdit->setDate(doc.date);
      mw->ui.sleepTime->setTime(doc.sleep);
      mw->ui.wakeupTime->setTime(doc.wakeup);
      mw->ui.scoreBox->setValue(doc.score);
      mw->ui.textEdit->setPlainText(doc.content);
      mw->ui.commentEdit->setPlainText(doc.comment);
      if(doc.lucid)
	mw->ui.lucidBox->setCheckState(Qt::Checked);
      else
	mw->ui.lucidBox->setCheckState(Qt::Unchecked);
      mw->changed = false;
      return &dreamsList->at(i);
    }
  }
  return NULL;
}

// -----------------------------------------------------------------------------------------
const QList < DreamDocument > *MyDreamBase::list()
{
  return dreamsList;
}

// -----------------------------------------------------------------------------------------
QList < int > MyDreamBase::getByTitle(QString exp)
{
  QList < int >list;
  for(int i = 0; i < dreamsList->count(); i++)
  {
    if(dreamsList->at(i).title.contains(exp, Qt::CaseInsensitive))
      list.push_back(dreamsList->at(i).ID);
  }
  return list;
}

// -----------------------------------------------------------------------------------------
QList < int > MyDreamBase::getByContent(QString exp)
{
  QList < int >list;
  for(int i = 0; i < dreamsList->count(); i++)
  {
    if(dreamsList->at(i).content.contains(exp, Qt::CaseInsensitive))
      list.push_back(dreamsList->at(i).ID);
  }
  return list;
}

// -----------------------------------------------------------------------------------------
void MyDreamBase::fillTree(QTreeWidget * list)
{
  list->clear();
  for(int i = 0; i < dreamsList->count(); i++)
  {
    QString date = dreamsList->at(i).date.toString("yyyy-MM-dd");
    int p = -1;
    for(int n = 0; n < list->topLevelItemCount(); n++)
      if(date == list->topLevelItem(n)->text(0))
      {
	p = n;
	break;
      }

    if(p != -1)			// jesli jest juz kontener w postaci daty
    {
      QTreeWidgetItem *item = new QTreeWidgetItem();
      item->setText(0, dreamsList->at(i).title);
      QString id;
      id.setNum(dreamsList->at(i).ID);
      item->setText(1, id);
      id.setNum(dreamsList->at(i).score);
      if(id == "0")
	item->setText(2, "n/a");	// brak oceny
      else
	item->setText(2, id + "/10");	// ocena
      list->topLevelItem(p)->addChild(item);
      if(dreamsList->at(i).lucid)
      {
	item->setBackgroundColor(0, lucidColor);
	item->setBackgroundColor(2, lucidColor);
      }
    }
    else			// jesli nie istnieje jeszczee data dla
      // dokumentu
    {
      QTreeWidgetItem *topItem = new QTreeWidgetItem(list);
      QTreeWidgetItem *item = new QTreeWidgetItem();
      topItem->setText(0, date);
      topItem->setText(1, "-1");
      item->setText(0, dreamsList->at(i).title);
      QString id;
      id.setNum(dreamsList->at(i).ID);
      item->setText(1, id);
      id.setNum(dreamsList->at(i).score);
      if(id == "0")
	item->setText(2, "n/a");	// brak oceny
      else
	item->setText(2, id + "/10");	// ocena
      topItem->addChild(item);
      list->addTopLevelItem(topItem);
      if(dreamsList->at(i).lucid)
      {
	item->setBackgroundColor(0, lucidColor);
	item->setBackgroundColor(2, lucidColor);
      }
    }

  }
  list->sortItems(0, Qt::DescendingOrder);
  for(int i = 0; i < list->topLevelItemCount(); i++)
    if(mw->config.dateItems.contains(list->topLevelItem(i)->text(0)))
      list->setItemExpanded(list->topLevelItem(i), true);
    else
      list->setItemExpanded(list->topLevelItem(i), false);

  list->setColumnWidth(0, 160);
}

// -----------------------------------------------------------------------------------------
void MyDreamBase::expandDoc(int id, QTreeWidget * list)
{
  QString ID;
  ID.setNum(id);
  for(int ti = 0; ti < list->topLevelItemCount(); ti++)	// data
    for(int ci = 0; ci < list->topLevelItem(ti)->childCount(); ci++)	// tytul
      if(ID == list->topLevelItem(ti)->child(ci)->text(1))
      {
	list->setItemExpanded(list->topLevelItem(ti), true);
	return;
      }
}

// -----------------------------------------------------------------------------------------
void
  MyDreamBase::colorItems(QList < int >IDs, QTreeWidget * list, QColor color)
{
  QString id;
  for(int ti = 0; ti < list->topLevelItemCount(); ti++)	// data
    for(int ci = 0; ci < list->topLevelItem(ti)->childCount(); ci++)	// tytul
      for(int i = 0; i < IDs.count(); i++)
      {				// id
	id.setNum(IDs.at(i));
	if(id == list->topLevelItem(ti)->child(ci)->text(1))
	{
	  list->topLevelItem(ti)->child(ci)->setBackgroundColor(0, color);
	  list->topLevelItem(ti)->child(ci)->setBackgroundColor(2, color);
	}
      }
}

// -----------------------------------------------------------------------------------------
void MyDreamBase::colorItem(int ID, QTreeWidget * list, QColor color)
{
  QString id;
  for(int ti = 0; ti < list->topLevelItemCount(); ti++)	// data
    for(int ci = 0; ci < list->topLevelItem(ti)->childCount(); ci++)	// tytul
    {
      id.setNum(ID);
      if(id == list->topLevelItem(ti)->child(ci)->text(1))
      {
	list->topLevelItem(ti)->child(ci)->setBackgroundColor(0, color);
	list->topLevelItem(ti)->child(ci)->setBackgroundColor(2, color);
      }
    }
}

// ===============================================================
// K L U C Z E

Keys::Keys()
{
  keysList = new QList < Key >;
}

Keys::~Keys()
{
  keysList->clear();
  delete keysList;
}

const Key *Keys::at(int index)
{
  return &keysList->at(index);
}

int Keys::add(QString name)
{
  // unikniecie powtorek:
  for(int i = 0; i < keysList->count(); i++)
    for(int j = 0; j < keysList->at(i).keys.count(); j++)
      if(name == keysList->at(i).keys.at(j))
	return -1;

  Key k;
  k.keys.push_front(name);
  k.ID = generateID();
  keysList->push_back(k);

  return k.ID;
}

bool Keys::addSynonym(int id, QString name)
{
  if(!exists(id))
    return false;

  // unikniecie powtorek:
  for(int i = 0; i < keysList->count(); i++)
    for(int j = 0; j < keysList->at(i).keys.count(); j++)
      if(name == keysList->at(i).keys.at(j))
	return false;

  for(int i = 0; i < keysList->count(); i++)
    if(keysList->at(i).ID == id)
    {
      Key k = keysList->takeAt(i);
      k.keys.push_back(name);
      keysList->push_back(k);
      break;
    }

  return true;
}

int Keys::generateID()
{
  int id = 0;
  while(exists(id))
    id++;
  return id;
}

bool Keys::exists(int id)
{
  if(id < 0)
    return true;
  for(int i = 0; i < keysList->count(); i++)
    if(keysList->at(i).ID == id)
      return true;
  return false;
}

bool Keys::contains(QString exp)
{
  for(int i = 0; i < keysList->count(); i++)
    for(int j = 0; j < keysList->at(i).keys.count(); j++)
      if(keysList->at(i).keys.at(j).contains(exp, Qt::CaseInsensitive))
	return true;
  return false;
}

bool Keys::remove(int id)
{
  for(int i = 0; i < keysList->count(); i++)
  {
    if(keysList->at(i).ID == id)
    {
      keysList->removeAt(i);
      return true;
    }
  }
  return false;
}

bool Keys::removeSynonym(int id, QString name)
{
  if(id < 0)
    return false;
  if(!exists(id))
    return false;

  for(int i = 0; i < keysList->count(); i++)
    if(keysList->at(i).ID == id)
    {
      Key k = keysList->takeAt(i);
      for(int j = 0; j < k.keys.count(); j++)
	if(k.keys.at(j) == name)
	{
	  k.keys.removeAt(j);
	  keysList->push_back(k);
	  return true;
	}

      keysList->push_back(k);
    }

  return false;
}

const Key *Keys::getByID(int id)
{
  for(int i = 0; i < keysList->count(); i++)
  {
    if(keysList->at(i).ID == id)
      return &keysList->at(i);
  }
  return NULL;
}

int Keys::getByName(QString exp)
{
  for(int i = 0; i < keysList->count(); i++)
    for(int j = 0; j < keysList->at(i).keys.count(); j++)
      if(keysList->at(i).keys.at(j) == exp)
	return keysList->at(i).ID;
  return -1;
}

int Keys::count()
{
  return keysList->count();
}

void Keys::rename(QString oldname, QString newname)
{
  for(int i = 0; i < keysList->count(); i++)
    for(int j = 0; j < keysList->at(i).keys.count(); j++)
      if(keysList->at(i).keys.at(j) == oldname)
      {
	Key k = keysList->at(i);
	k.keys[j] = newname;
	keysList->replace(i, k);
      }
}

void Keys::countFreq(const QList < DreamDocument > *dreamsList)
{

  Key k;

  for(int i = 0; i < keysList->count(); i++)
  {
    k = keysList->at(i);
    k.freq = 0;
    for(int d = 0; d < dreamsList->count(); d++)
      for(int j = 0; j < keysList->at(i).keys.count(); j++)
      {

	if(dreamsList->at(d).content.contains(keysList->at(i).keys.at(j),
					      Qt::CaseInsensitive))
	{
	  k.freq++;
	  break;
	}
      }
    keysList->replace(i, k);
  }

}

void Keys::sortByName()
{
  for(int i = 0; i < keysList->count(); i++)
    for(int n = i + 1; n < keysList->count(); n++)
      if(keysList->at(i).keys.at(0) > keysList->at(n).keys.at(0))
	keysList->swap(i, n);
}

void Keys::sortByFreq()
{
  for(int i = 0; i < keysList->count(); i++)
    for(int n = i + 1; n < keysList->count(); n++)
      if(keysList->at(i).freq < keysList->at(n).freq)
	keysList->swap(i, n);
}

void
  Keys::fillTree(QTreeWidget * tree,
		 const QList < DreamDocument > *dreamsList, bool freqSort)
{
  countFreq(dreamsList);
  tree->clear();
  if(freqSort)
    sortByFreq();
  else
    sortByName();
  QString f;

  // kolumny:
  // 0 - nazwa
  // 1 - ilosc wystapien
  // 2 - typ
  // 3 - id

  for(int i = 0; i < keysList->count(); i++)
  {
    QTreeWidgetItem *topItem = new QTreeWidgetItem(tree);
    topItem->setText(0, keysList->at(i).keys.at(0));
    f.setNum(keysList->at(i).freq);
    topItem->setText(1, f);
    topItem->setText(2, "M");
    f.setNum(keysList->at(i).ID);
    topItem->setText(3, f);
    for(int j = 1; j < keysList->at(i).keys.count(); j++)
    {
      QTreeWidgetItem *item = new QTreeWidgetItem();
      item->setText(0, keysList->at(i).keys.at(j));
      item->setText(1, "");
      item->setText(2, "S");
      item->setText(3, "");

      topItem->addChild(item);
      tree->addTopLevelItem(topItem);
    }
    topItem->setExpanded(true);
  }
}

void Keys::fillCurrentList(QListWidget * list, QString txt)
{
  list->clear();
  for(int i = 0; i < keysList->count(); i++)
    for(int j = 0; j < keysList->at(i).keys.count(); j++)
      if(txt.contains(keysList->at(i).keys.at(j), Qt::CaseInsensitive))
	list->addItem(keysList->at(i).keys.at(j));
}

// ///////////////////////////////////////////////////////////////////////////
// //////////////////////////S U G E S T I
// E//////////////////////////////////

int Suggestions::exists(QString name)
{
  for(int i = 0; i < sugs.count(); i++)
    if(name == sugs.at(i).name)
      return i;

  return -1;
}

void Suggestions::removeSeldom(int min_occ)
{
  for(int i = 0; i < sugs.count(); i++)
    if(min_occ > sugs.at(i).occ)
    {
      sugs.removeAt(i);
      i--;
    }
}

void Suggestions::add(QString name)
{
  int i = exists(name);
  if(i == -1)
  {
    Suggestion s;
    s.name = name;
    s.occ = 1;
    sugs.push_back(s);
  }
  else
    sugs[i].occ++;
}

Suggestion Suggestions::takeBest()
{
  if(sugs.count() < 1)
  {
    Suggestion s;
    s.name = "";
    s.occ = -1;
    return s;
  }

  int max = 1, p = 0;
  for(int i = 0; i < sugs.count(); i++)
    if(sugs.at(i).occ > max)
    {
      max = sugs.at(i).occ;
      p = i;
    }
  return sugs.takeAt(p);
}
