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
#include "vigcryp.h"

QString vigcryp::encode(QString qtxt, QString qkey)
{
  int *pass = new int[qkey.size()];
  int ascii, pp = 0, index;
  QList < short int > chars = makeList();

  for(int i = 0; i < qkey.size(); i++)
    pass[i] = qkey.at(i).toAscii();

  for(int i = 0; i < qtxt.size(); i++)
  {
    ascii = qtxt.at(i).toAscii();
    index = chars.indexOf(ascii);
    if(index != -1)
    {
      index += pass[pp];
      index = index % chars.size();
      qtxt.replace(i, 1, QChar(chars.at(index)));
    }

    pp++;
    if(pp >= qkey.size())
      pp = 0;
  }

  delete[]pass;
  return qtxt;
}

// ------------------------------------------------------
QString vigcryp::decode(QString qtxt, QString qkey)
{
  int *pass = new int[qkey.size()];
  int ascii, pp = 0, index;
  QList < short int > chars = makeList();

  for(int i = 0; i < qkey.size(); i++)
    pass[i] = qkey.at(i).toAscii();

  for(int i = 0; i < qtxt.size(); i++)
  {
    ascii = qtxt.at(i).toAscii();

    index = chars.indexOf(ascii);
    if(index != -1)
    {
      index -= pass[pp];
      while(index < 0)
	index += chars.size();
      qtxt.replace(i, 1, QChar(chars.at(index)));
    }

    pp++;
    if(pp >= qkey.size())
      pp = 0;
  }

  delete[]pass;
  return qtxt;
}

QString vigcryp::right(QString qtxt)
{
  int ascii, index;
  QList < short int > chars = makeList();

  for(int i = 0; i < qtxt.size(); i++)
  {
    ascii = qtxt.at(i).toAscii();
    index = chars.indexOf(ascii);
    if(index != -1)
    {
      index += 1;
      index = index % chars.size();
      qtxt.replace(i, 1, QChar(chars.at(index)));
    }
  }

  return qtxt;
}

// --------------------------------------------------
QList < short int > vigcryp::makeList()
{
  QList < short int >chars;
  for(short int i = 47; i <= 57; i++)	// '/' - '9'
    chars.push_back(i);
  for(short int i = 60; i <= 62; i++)	// '<' - '>'
    chars.push_back(i);
  for(short int i = 65; i <= 90; i++)	// 'A' - 'Z'
    chars.push_back(i);
  for(short int i = 97; i <= 122; i++)	// 'a' - 'z'
    chars.push_back(i);
  chars.push_back(32);		// spacja
  chars.push_back(95);		// _
  return chars;
}
