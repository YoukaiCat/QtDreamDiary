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

#ifndef XCONFIG_H
#define XCONFIG_H

#include <iostream>
#include <QApplication>
#include <QDesktopWidget>
#include <QColor>
#include <QFont>
#include <QFile>
#include <QTextStream>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QTreeWidget>

enum Range
{ week = 7, month = 30, all = 0 };

class XConfig
{
private:
  QString filepath;
public:
  XConfig(QString);
  XConfig()
  {
  };
  ~XConfig()
  {
  };


  QColor highlightColor;
  QString codecName;
  QString browserPath;
  QString webAddress;
  QString passwd;
  QString theme;
  QString update_url;
  QFont font;
  bool update;
  bool suggestions;
  bool freqSort;
  bool hkeys;
  bool base_coding;
  bool keys_coding;
  bool sym_coding;
  bool ftp_autoupload;
  bool dockFloating;
  bool dockHidden;
  bool createDefaultHtmlTemplate(QString);
  bool trayicon;
  Qt::DockWidgetArea dockArea;
  Range sugtime;
  int sugwords;
  int sugignored;
  int sugsens;
  int dockX;
  int dockY;
  int dockWidth;
  int dockHeight;
  int mwX;
  int mwY;
  int mwWidth;
  int mwHeight;
  QList < int >s1sizes;
  QList < int >s2sizes;
  QList < QString > dateItems;	//lista rozwinietych itemow
  //--------------
  bool createDefault(QString);
  bool load();
  bool save();
  bool saveAs(QString);
  bool open(QString);
  void saveExpandedItems(QTreeWidget *);

};

class Last
{
public:
  bool load(QString);
  bool save(QString);
  bool autologin;
  QString passwd;
  QString login;
};


#endif
