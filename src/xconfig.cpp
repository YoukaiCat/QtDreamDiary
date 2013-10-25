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

#include "xconfig.h"

XConfig::XConfig(QString file)
{
  filepath = file;
  load();
}

// -------------------------------------------------------------

bool XConfig::createDefault(QString password)
{
  passwd = password;
  highlightColor = QColor(190, 200, 255);
  theme = "Oxygen";
  codecName = "UTF8";
  webAddress = "http://www.sennik.biz/?s=#exp#";
  update_url = "http://www.chodaksoft.republika.pl/mdd.ver";
  sugtime = month;
  sugwords = 0;
  sugignored = 6;
  sugsens = 3;
#ifdef Q_WS_X11
  if(QFile::exists("/usr/bin/firefox"))
    browserPath = "/usr/bin/firefox";
  else if(QFile::exists("/usr/bin/opera"))
    browserPath = "/usr/bin/opera";
  else if(QFile::exists("/usr/bin/konqueror"))
    browserPath = "/usr/bin/konqueror";
  else
    browserPath =
      QApplication::translate("XConfig", "Select your browser",
                  0, QApplication::UnicodeUTF8);

  font.fromString("DejaVu Serif,9,-1,5,50,0,0,0,0,0");
#endif
#ifdef Q_WS_WIN
  if(QFile::exists("C:\\Program Files\\Mozilla Firefox\\firefox.exe"))
    browserPath = "C:\\Program Files\\Mozilla Firefox\\firefox.exe";
  else if(QFile::exists("\"C:\\Program Files\\Opera\\opera.exe"))
    browserPath = "C:\\Program Files\\Opera\\opera.exe";
  else if(QFile::exists("C:\\Program Files\\Internet Explorer\\IEXPLORE.EXE"))
    browserPath = "C:\\Program Files\\Internet Explorer\\IEXPLORE.EXE";
  else
    browserPath =
      QApplication::translate("XConfig", "Select your browser",
                  0, QApplication::UnicodeUTF8);

  font.fromString("MS Shell Dlg 2,8.25,-1,5,50,0,0,0,0,0");
#endif
  mwX = QApplication::desktop()->width() / 2 - 450;
  mwY = QApplication::desktop()->height() / 2 - 300;
  mwWidth = 900;
  mwHeight = 600;
  dockX = 0;
  dockY = 70;
  dockWidth = 236;
  dockHeight = 506;
  dockArea = Qt::LeftDockWidgetArea;
  s1sizes << 273 << 206;
  s2sizes << 341 << 141;
  dockHidden = false;
  dockFloating = false;
  base_coding = false;
  keys_coding = false;
  sym_coding = false;
  ftp_autoupload = false;
  hkeys = true;
  freqSort = true;
  suggestions = true;
  trayicon = true;
  update = true;
  return save();

}

bool XConfig::save()
{
  QString tmp, n;

  QDomDocument doc("xconfig");
  QDomElement root = doc.createElement("config");
  doc.appendChild(root);

  QDomElement tag = doc.createElement("highlightcolor");
  QDomText txt = doc.createTextNode(highlightColor.name());
  tag.appendChild(txt);
  root.appendChild(tag);

  tag = doc.createElement("icontheme");
  txt = doc.createTextNode(theme);
  tag.appendChild(txt);
  root.appendChild(tag);

  tag = doc.createElement("updateurl");
  txt = doc.createTextNode(update_url);
  tag.appendChild(txt);
  root.appendChild(tag);

  tag = doc.createElement("codecname");
  txt = doc.createTextNode(codecName);
  tag.appendChild(txt);
  root.appendChild(tag);

  tag = doc.createElement("browserpath");
  txt = doc.createTextNode(browserPath);
  tag.appendChild(txt);
  root.appendChild(tag);

  tag = doc.createElement("webaddress");
  txt = doc.createTextNode(webAddress);
  tag.appendChild(txt);
  root.appendChild(tag);

  tag = doc.createElement("font");
  txt = doc.createTextNode(font.toString());
  tag.appendChild(txt);
  root.appendChild(tag);

  tag = doc.createElement("passwd");
  txt = doc.createTextNode(passwd);
  tag.appendChild(txt);
  root.appendChild(tag);

  // dokowalne archiwum:
  if(dockArea == Qt::LeftDockWidgetArea)
    tmp = "left";
  else if(dockArea == Qt::RightDockWidgetArea)
    tmp = "right";
  else
    tmp = "no";
  tag = doc.createElement("dockarea");
  txt = doc.createTextNode(tmp);
  tag.appendChild(txt);
  root.appendChild(tag);

  tmp.setNum(sugwords);
  tag = doc.createElement("sugwords");
  txt = doc.createTextNode(tmp);
  tag.appendChild(txt);
  root.appendChild(tag);

  tmp.setNum(sugignored);
  tag = doc.createElement("sugignored");
  txt = doc.createTextNode(tmp);
  tag.appendChild(txt);
  root.appendChild(tag);

  tmp.setNum(sugsens);
  tag = doc.createElement("sugsens");
  txt = doc.createTextNode(tmp);
  tag.appendChild(txt);
  root.appendChild(tag);

  tmp.setNum(sugtime);
  tag = doc.createElement("sugtime");
  txt = doc.createTextNode(tmp);
  tag.appendChild(txt);
  root.appendChild(tag);

  tmp.setNum(dockX);
  tag = doc.createElement("dockx");
  txt = doc.createTextNode(tmp);
  tag.appendChild(txt);
  root.appendChild(tag);

  tmp.setNum(dockY);
  tag = doc.createElement("docky");
  txt = doc.createTextNode(tmp);
  tag.appendChild(txt);
  root.appendChild(tag);

  tmp.setNum(dockWidth);
  tag = doc.createElement("dockwidth");
  txt = doc.createTextNode(tmp);
  tag.appendChild(txt);
  root.appendChild(tag);

  tmp.setNum(dockHeight);
  tag = doc.createElement("dockheight");
  txt = doc.createTextNode(tmp);
  tag.appendChild(txt);
  root.appendChild(tag);

  // okno glowne
  tmp.setNum(mwX);
  tag = doc.createElement("wmx");
  txt = doc.createTextNode(tmp);
  tag.appendChild(txt);
  root.appendChild(tag);

  tmp.setNum(mwY);
  tag = doc.createElement("wmy");
  txt = doc.createTextNode(tmp);
  tag.appendChild(txt);
  root.appendChild(tag);

  tmp.setNum(mwWidth);
  tag = doc.createElement("wmwidth");
  txt = doc.createTextNode(tmp);
  tag.appendChild(txt);
  root.appendChild(tag);

  tmp.setNum(mwHeight);
  tag = doc.createElement("wmheight");
  txt = doc.createTextNode(tmp);
  tag.appendChild(txt);
  root.appendChild(tag);

  if(update)
    tmp = "1";
  else
    tmp = "0";
  tag = doc.createElement("update");
  txt = doc.createTextNode(tmp);
  tag.appendChild(txt);
  root.appendChild(tag);

  if(suggestions)
    tmp = "1";
  else
    tmp = "0";
  tag = doc.createElement("suggestions");
  txt = doc.createTextNode(tmp);
  tag.appendChild(txt);
  root.appendChild(tag);

  if(dockHidden)
    tmp = "1";
  else
    tmp = "0";
  tag = doc.createElement("dockhidden");
  txt = doc.createTextNode(tmp);
  tag.appendChild(txt);
  root.appendChild(tag);

  if(dockFloating)
    tmp = "1";
  else
    tmp = "0";
  tag = doc.createElement("dockfloating");
  txt = doc.createTextNode(tmp);
  tag.appendChild(txt);
  root.appendChild(tag);

  if(base_coding)
    tmp = "1";
  else
    tmp = "0";
  tag = doc.createElement("basecoding");
  txt = doc.createTextNode(tmp);
  tag.appendChild(txt);
  root.appendChild(tag);

  if(keys_coding)
    tmp = "1";
  else
    tmp = "0";
  tag = doc.createElement("keyscoding");
  txt = doc.createTextNode(tmp);
  tag.appendChild(txt);
  root.appendChild(tag);

  if(sym_coding)
    tmp = "1";
  else
    tmp = "0";
  tag = doc.createElement("symcoding");
  txt = doc.createTextNode(tmp);
  tag.appendChild(txt);
  root.appendChild(tag);

  if(ftp_autoupload)
    tmp = "1";
  else
    tmp = "0";
  tag = doc.createElement("ftpautoupload");
  txt = doc.createTextNode(tmp);
  tag.appendChild(txt);
  root.appendChild(tag);

  if(hkeys)
    tmp = "1";
  else
    tmp = "0";
  tag = doc.createElement("hkeys");
  txt = doc.createTextNode(tmp);
  tag.appendChild(txt);
  root.appendChild(tag);

  if(freqSort)
    tmp = "1";
  else
    tmp = "0";
  tag = doc.createElement("freqsort");
  txt = doc.createTextNode(tmp);
  tag.appendChild(txt);
  root.appendChild(tag);

  if(trayicon)
    tmp = "1";
  else
    tmp = "0";
  tag = doc.createElement("trayicon");
  txt = doc.createTextNode(tmp);
  tag.appendChild(txt);
  root.appendChild(tag);

  // ------------------

  QDomElement item = doc.createElement("expanded_items");
  for(int i = 0; i < dateItems.count(); i++)
  {
    tag = doc.createElement("ei");
    txt = doc.createTextNode(dateItems.at(i));
    tag.appendChild(txt);
    item.appendChild(tag);
  }
  root.appendChild(item);

  item = doc.createElement("s1sizes");
  for(int i = 0; i < s1sizes.count(); i++)
  {
    tmp.setNum(s1sizes.at(i));
    tag = doc.createElement("size");
    txt = doc.createTextNode(tmp);
    tag.appendChild(txt);
    item.appendChild(tag);
  }
  root.appendChild(item);

  item = doc.createElement("s2sizes");
  for(int i = 0; i < s2sizes.count(); i++)
  {
    tmp.setNum(s2sizes.at(i));
    tag = doc.createElement("size");
    txt = doc.createTextNode(tmp);
    tag.appendChild(txt);
    item.appendChild(tag);
  }
  root.appendChild(item);

  if(QFile::exists(filepath))
    if(!QFile::remove(filepath))
      return false;

  QFile config(filepath);
  if(!config.open(QFile::WriteOnly | QFile::Text))
    return false;
  QTextStream fio(&config);
  fio << doc.toString();
  config.close();
  return true;
}

bool XConfig::load()
{
  QString tmp;
  QFile config(filepath);
  QDomDocument doc("xconfig");
  if(!config.open(QFile::ReadOnly | QFile::Text))
    return false;

  if(!doc.setContent(&config))
  {
    config.close();
    return false;
  }
  config.close();

  highlightColor =
    doc.elementsByTagName("highlightcolor").item(0).firstChild().toText().
    data();
  theme =
    doc.elementsByTagName("icontheme").item(0).firstChild().toText().data();
  update_url =
    doc.elementsByTagName("updateurl").item(0).firstChild().toText().data();
  codecName =
    doc.elementsByTagName("codecname").item(0).firstChild().toText().data();
  browserPath =
    doc.elementsByTagName("browserpath").item(0).firstChild().toText().data();
  webAddress =
    doc.elementsByTagName("webaddress").item(0).firstChild().toText().data();
  passwd =
    doc.elementsByTagName("passwd").item(0).firstChild().toText().data();
  font.fromString(doc.elementsByTagName("font").item(0).firstChild().toText().
		  data());
  sugwords =
    doc.elementsByTagName("sugwords").item(0).firstChild().toText().data().
    toInt();
  sugignored =
    doc.elementsByTagName("sugignored").item(0).firstChild().toText().data().
    toInt();
  sugsens =
    doc.elementsByTagName("sugsens").item(0).firstChild().toText().data().
    toInt();
  dockX =
    doc.elementsByTagName("dockx").item(0).firstChild().toText().data().
    toInt();
  dockY =
    doc.elementsByTagName("docky").item(0).firstChild().toText().data().
    toInt();
  dockWidth =
    doc.elementsByTagName("dockwidth").item(0).firstChild().toText().data().
    toInt();
  dockHeight =
    doc.elementsByTagName("dockheight").item(0).firstChild().toText().data().
    toInt();
  mwX =
    doc.elementsByTagName("wmx").item(0).firstChild().toText().data().toInt();
  mwY =
    doc.elementsByTagName("wmy").item(0).firstChild().toText().data().toInt();
  mwWidth =
    doc.elementsByTagName("wmwidth").item(0).firstChild().toText().data().
    toInt();
  mwHeight =
    doc.elementsByTagName("wmheight").item(0).firstChild().toText().data().
    toInt();

  int
    sugtmp =
    doc.elementsByTagName("sugtime").item(0).firstChild().toText().data().
    toInt();
  switch (sugtmp)
  {
  case 7:
    sugtime = week;
    break;
  case 30:
    sugtime = month;
    break;
  case 0:
    sugtime = all;
  }

  tmp =
    doc.elementsByTagName("dockarea").item(0).firstChild().toText().data();
  if(tmp == "left")
    dockArea = Qt::LeftDockWidgetArea;
  else if(tmp == "right")
    dockArea = Qt::RightDockWidgetArea;
  else
    dockArea = Qt::NoDockWidgetArea;

  tmp = doc.elementsByTagName("update").item(0).firstChild().toText().data();
  if(tmp == "0")
    update = false;
  else
    update = true;

  tmp =
    doc.elementsByTagName("suggestions").item(0).firstChild().toText().data();
  if(tmp == "0")
    suggestions = false;
  else
    suggestions = true;

  tmp =
    doc.elementsByTagName("dockhidden").item(0).firstChild().toText().data();
  if(tmp == "0")
    dockHidden = false;
  else
    dockHidden = true;

  tmp =
    doc.elementsByTagName("dockfloating").item(0).firstChild().toText().
    data();
  if(tmp == "0")
    dockFloating = false;
  else
    dockFloating = true;

  tmp =
    doc.elementsByTagName("basecoding").item(0).firstChild().toText().data();
  if(tmp == "0")
    base_coding = false;
  else
    base_coding = true;

  tmp =
    doc.elementsByTagName("keyscoding").item(0).firstChild().toText().data();
  if(tmp == "0")
    keys_coding = false;
  else
    keys_coding = true;

  tmp =
    doc.elementsByTagName("symcoding").item(0).firstChild().toText().data();
  if(tmp == "0")
    sym_coding = false;
  else
    sym_coding = true;

  tmp =
    doc.elementsByTagName("ftpautoupload").item(0).firstChild().toText().
    data();
  if(tmp == "0")
    ftp_autoupload = false;
  else
    ftp_autoupload = true;

  tmp = doc.elementsByTagName("hkeys").item(0).firstChild().toText().data();
  if(tmp == "0")
    hkeys = false;
  else
    hkeys = true;

  tmp =
    doc.elementsByTagName("freqsort").item(0).firstChild().toText().data();
  if(tmp == "0")
    freqSort = false;
  else
    freqSort = true;

  tmp =
    doc.elementsByTagName("trayicon").item(0).firstChild().toText().data();
  if(tmp == "0")
    trayicon = false;
  else
    trayicon = true;

  QDomNodeList
    items =
    doc.elementsByTagName("expanded_items").item(0).toElement().
    elementsByTagName("ei");
  for(int i = 0; i < items.count(); i++)
    dateItems.push_back(items.item(i).firstChild().toText().data());

  items =
    doc.elementsByTagName("s1sizes").item(0).toElement().
    elementsByTagName("size");
  for(int i = 0; i < items.count(); i++)
    s1sizes.push_back(items.item(i).firstChild().toText().data().toInt());

  items =
    doc.elementsByTagName("s2sizes").item(0).toElement().
    elementsByTagName("size");
  for(int i = 0; i < items.count(); i++)
    s2sizes.push_back(items.item(i).firstChild().toText().data().toInt());

  return true;
}

// =====================================

bool XConfig::saveAs(QString file)
{
  QString tmp = filepath;
  filepath = file;
  bool result = save();
  filepath = tmp;
  return result;
}

bool XConfig::open(QString file)
{
  filepath = file;
  return load();
}

// **********************************************************************
// **********************************************************************
// **********************************************************************
// **********************************************************************

bool Last::load(QString filepath)
{
  if(!QFile::exists(filepath))
    return false;

  QFile config(filepath);
  QDomDocument doc("last_login");
  if(!config.open(QFile::ReadOnly | QFile::Text))
    return false;

  if(!doc.setContent(&config))
  {
    config.close();
    return false;
  }
  else
    config.close();

  passwd =
    doc.elementsByTagName("passwd").item(0).firstChild().toText().data();
  login = doc.elementsByTagName("login").item(0).firstChild().toText().data();
  QString
    al =
    doc.elementsByTagName("autologin").item(0).firstChild().toText().data();

  if(al == "0")
    autologin = false;
  else
    autologin = true;

  return true;
}

bool Last::save(QString filepath)
{
  QString al;
  if(autologin)
    al = "1";
  else
    al = "0";

  QDomDocument doc("last_login");

  QDomElement root = doc.createElement("last_login");
  doc.appendChild(root);

  QDomElement tag = doc.createElement("login");
  QDomText txt = doc.createTextNode(login);
  tag.appendChild(txt);
  root.appendChild(tag);

  tag = doc.createElement("passwd");
  txt = doc.createTextNode(passwd);
  tag.appendChild(txt);
  root.appendChild(tag);

  tag = doc.createElement("autologin");
  txt = doc.createTextNode(al);
  tag.appendChild(txt);
  root.appendChild(tag);

  if(QFile::exists(filepath))
    if(!QFile::remove(filepath))
      return false;

  QFile config(filepath);
  if(!config.open(QFile::WriteOnly | QFile::Text))
    return false;
  QTextStream fio(&config);
  fio << doc.toString();
  config.close();
  return true;
}

// szablon:
bool XConfig::createDefaultHtmlTemplate(QString file)
{
  QFile tmp(file);
  if(!tmp.open(QFile::WriteOnly | QFile::Text))
    return false;

  QString html = "<html xml:lang=\"pl\" lang=\"pl\">\n\
\t<head>\n\
\t\t<title>#title#</title>\n\
\t\t<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\n\
\t\t<style>\n\
\t\t\tbody\n\
\t\t\t{\n\
\t\t\t\tbackground-color:\trgb(230,230,230);\n\
\t\t\t}\n\
\t\t\tdiv#content\n\
\t\t\t{\n\
\t\t\t\tpadding:\t10px;\n\
\t\t\t\tbackground-color:\twhite;\n\
\t\t\t\tborder:\trgb(50,50,50) 1px solid;\n\
\t\t\t\tfont-size:\t16px;\n\
\t\t\t}\n\
\t\t\tdiv#title\n\
\t\t\t{\n\
\t\t\t\tfont-size:\t24px;\n\
\t\t\t\tfont-weight:\tbold;\n\
\t\t\t}\n\
\t\t\tdiv#info\n\
\t\t\t{\n\
\t\t\t\tmargin:\t10px 0px 10px 0px;\n\
\t\t\t}\n\
\t\t\tdiv.infoElem\n\
\t\t\t{\n\
\t\t\t\tbackground-color:\twhite;\n\
\t\t\t\tborder:\trgb(200,200,200) 1px solid;\n\
\t\t\t\tpadding:\t5px;\n\
\t\t\t\tmargin:\t2px 0px 2px 0px;\n\
\t\t\t}\n\
\t\t\tspan.infoElemTitle\n\
\t\t\t{\n\
\t\t\t\t/*color:\trgb(50,50,50);*/\n\
\t\t\t}\n\
\t\t\tspan.infoElemContent\n\
\t\t\t{\n\
\t\t\t\tcolor:\tgreen;\n\
\t\t\t\tfont-weight:\tbold;\n\
\t\t\t}\n\
\t\t</style>\n\
\t</head>\n\
\t<body>\n\
\t\t<div id=\"title\">#title#</div>\n\
\t\t<div id=\"info\">\n\
\t\t\t<div class=\"infoElem\">\n\
\t\t\t\t<span class=\"infoElemTitle\">Date:</span>\n\
\t\t\t\t<span class=\"infoElemContent\">#date#</span>\n\
\t\t\t</div>\n\
\t\t\t<div class=\"infoElem\">\n\
\t\t\t\t<span class=\"infoElemTitle\">Asleep time:</span>\n\
\t\t\t\t<span class=\"infoElemContent\">#asleep#</span>\n\
\t\t\t</div>\n\
\t\t\t<div class=\"infoElem\">\n\
\t\t\t\t<span class=\"infoElemTitle\">Wake up time:</span>\n\
\t\t\t\t<span class=\"infoElemContent\">#wakeup#</span>\n\
\t\t\t</div>\n\
\t\t\t<div class=\"infoElem\">\n\
\t\t\t\t<span class=\"infoElemTitle\">Score:</span>\n\
\t\t\t\t<span class=\"infoElemContent\">#score#</span>\n\
\t\t\t</div>\n\
\t\t\t<div class=\"infoElem\">\n\
\t\t\t\t<span class=\"infoElemTitle\">Comment:</span>\n\
\t\t\t\t<span class=\"infoElemContent\">#comment#</span>\n\
\t\t\t</div>\n\
\t\t</div>\n\
\t\t<div id=\"content\">\n\
\t\t#content#\n\
\t\t</div>\n\
\t<body>\n\
</html>";

  QTextStream tmpstream(&tmp);
  tmpstream << html;
  tmp.close();

  return true;
}

void XConfig::saveExpandedItems(QTreeWidget * tree)
{
  dateItems.clear();
  for(int i = 0; i < tree->topLevelItemCount(); i++)
    if(tree->topLevelItem(i)->isExpanded())
      dateItems.push_back(tree->topLevelItem(i)->text(0));
}
