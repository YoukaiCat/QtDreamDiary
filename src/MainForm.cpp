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
#include "MainForm.h"

// -----------------------------------------------------------------------------------------------

// ********************************************************************************
// ****************************P U B L IC*****************************************
// ********************************************************************************

// -----------------------------------------------------------------------------------------------

MainForm::MainForm()
{
#ifdef INSTALLATION_PREFIX
  exePath = INSTALLATION_PREFIX;
#else
  exePath = QCoreApplication::applicationDirPath();
#endif
  currentTranslation = QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/mdd/locale/current.qm";
  lastLoginPath = QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/mdd/last";
  ui.setupUi(this);
  loginOk = false;
  profile_deleting = false;
  full = false;
  config.ftp_autoupload = false;
  dreamBase = new MyDreamBase(this);
  counter = new CounterThread();
  ui.dateEdit->setDate(QDate::currentDate());

  currentSuggestion = new Suggestion;
  currentSuggestion->name.clear();
  currentSuggestion->occ = -1;
  ftp_dialog = NULL;
  login_dialog = NULL;
  find_dialog = NULL;
  replace_dialog = NULL;
  about_dialog = NULL;
  symbol_dialog = NULL;
  stat_dialog = NULL;
  settings_dialog = NULL;
  ignored_dialog = NULL;
  trayIcon = new QSystemTrayIcon();
  trayIcon->setIcon(QIcon(exePath + "/themes/winicon.png"));
  setWindowFlags(Qt::Dialog);

  ui.textEdit->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui.textEdit, SIGNAL(customContextMenuRequested(QPoint)), this,
	  SLOT(textEditContextMenu()));
  ui.docTree->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui.docTree, SIGNAL(customContextMenuRequested(QPoint)), this,
	  SLOT(docTreeContextMenu()));
  ui.keysTree->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui.keysTree, SIGNAL(customContextMenuRequested(QPoint)), this,
	  SLOT(keyListContextMenu()));
  // -----------------------------------------------------------------
  connect(ui.actionQuit, SIGNAL(triggered()), this, SLOT(close()));
  connect(ui.actionUndo, SIGNAL(triggered()), ui.textEdit->document(),
	  SLOT(undo()));
  connect(ui.actionRedo, SIGNAL(triggered()), ui.textEdit->document(),
	  SLOT(redo()));
  connect(ui.actionCut, SIGNAL(triggered()), ui.textEdit, SLOT(cut()));
  connect(ui.actionCopy, SIGNAL(triggered()), ui.textEdit, SLOT(copy()));
  connect(ui.actionPaste, SIGNAL(triggered()), ui.textEdit, SLOT(paste()));
  connect(ui.actionSelectAll, SIGNAL(triggered()), ui.textEdit,
	  SLOT(selectAll()));
  // ================================================================================
  connect(ui.actionNew, SIGNAL(triggered()), this, SLOT(aNew()));
  connect(ui.actionOpenFile, SIGNAL(triggered()), this, SLOT(aOpenFile()));
  connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(aSave()));
  connect(ui.actionSaveAs, SIGNAL(triggered()), this, SLOT(aSaveAs()));
  connect(ui.actionDelete, SIGNAL(triggered()), this, SLOT(aDelete()));
  connect(ui.actionDeleteAllDocs, SIGNAL(triggered()), this,
	  SLOT(aDeleteAll()));
  connect(ui.actionDeleteSelected, SIGNAL(triggered()), this,
	  SLOT(aDeleteSelected()));
  connect(ui.actionToHtml, SIGNAL(triggered()), this, SLOT(aToHtml()));
  connect(ui.actionToFile, SIGNAL(triggered()), this, SLOT(aToFile()));
  connect(ui.actionToFTP, SIGNAL(triggered()), this, SLOT(aToFTP()));
  connect(ui.actionFromFile, SIGNAL(triggered()), this, SLOT(aFromFile()));
  connect(ui.actionFromDrm, SIGNAL(triggered()), this, SLOT(aFromDrm()));
  connect(ui.actionFromFTP, SIGNAL(triggered()), this, SLOT(aToFTP()));
  connect(ui.actionLogout, SIGNAL(triggered()), this, SLOT(aLogout()));
  connect(ui.actionLoadTemplate, SIGNAL(triggered()), this,
	  SLOT(aLoadTemplate()));
  connect(ui.actionDeleteProfile, SIGNAL(triggered()), this,
	  SLOT(aDeleteProfile()));
  connect(ui.actionFontSettings, SIGNAL(triggered()), this,
	  SLOT(aFontSettings()));
  connect(ui.actionFind, SIGNAL(triggered()), this, SLOT(aFind()));
  connect(ui.actionSettings, SIGNAL(triggered()), this, SLOT(aSettings()));
  connect(ui.actionScale, SIGNAL(triggered()), this, SLOT(aScale()));
  connect(ui.actionWhatsThis, SIGNAL(triggered()), this, SLOT(aWhatsThis()));
  connect(ui.actionManual, SIGNAL(triggered()), this, SLOT(aManual()));
  connect(ui.actionReplace, SIGNAL(triggered()), this, SLOT(aReplace()));
  connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(aAbout()));
  connect(ui.actionStats, SIGNAL(triggered()), this, SLOT(aStats()));
  connect(ui.actionSymbols, SIGNAL(triggered()), this, SLOT(aSymbols()));
  connect(ui.actionCounterStart, SIGNAL(triggered()), this,
	  SLOT(aCounterStart()));
  connect(ui.actionIgnoredList, SIGNAL(triggered()), this,
	  SLOT(aIgnoredList()));
  connect(ui.actionInsertNewKey, SIGNAL(triggered()), this,
	  SLOT(insertNewKey()));
  connect(ui.actionWebFind, SIGNAL(triggered()), this, SLOT(webFind()));
  connect(ui.actionSearch, SIGNAL(triggered()), this, SLOT(aSearch()));
  connect(ui.actionSearchContent, SIGNAL(triggered()), this,
	  SLOT(aSearchContent()));
  connect(ui.actionRollDown, SIGNAL(triggered()), this, SLOT(aRollDown()));
  connect(ui.actionRollUp, SIGNAL(triggered()), this, SLOT(aRollUp()));

  connect(ui.actionPl, SIGNAL(triggered()), this, SLOT(aLangPl()));
  connect(ui.actionEn, SIGNAL(triggered()), this, SLOT(aLangEn()));
  connect(ui.actionRu, SIGNAL(triggered()), this, SLOT(aLangRu()));
  connect(ui.actionLang, SIGNAL(triggered()), this, SLOT(aLangOther()));

  connect(ui.actionDefaultView, SIGNAL(triggered()), this,
	  SLOT(aDefaultView()));
  connect(ui.actionDockLeft, SIGNAL(triggered()), this, SLOT(aDockLeft()));
  connect(ui.actionDockRight, SIGNAL(triggered()), this, SLOT(aDockRight()));
  connect(ui.actionDockLeft, SIGNAL(triggered()), this, SLOT(aDockLeft()));
  connect(ui.actionDockShowHide, SIGNAL(triggered()), this,
	  SLOT(aDockShowHide()));
  connect(ui.actionToolBarShowHide, SIGNAL(triggered()), this,
	  SLOT(aToolBarShowHide()));
  connect(ui.actionDockFloat, SIGNAL(triggered()), this, SLOT(aDockFloat()));
  connect(ui.actionKeyFreqSort, SIGNAL(triggered()), this,
	  SLOT(aKeyFreqSort()));
  connect(ui.actionKeyAscSort, SIGNAL(triggered()), this,
	  SLOT(aKeyAscSort()));

  // =============================
  connect(ui.newKeyButton, SIGNAL(clicked()), this, SLOT(addNewKey()));
  connect(ui.addNewSynonymButton, SIGNAL(clicked()), this,
	  SLOT(addNewSynonym()));
  connect(ui.deleteNewKeyButton, SIGNAL(clicked()), this, SLOT(deleteKey()));
  connect(ui.actionDeleteAllKeys, SIGNAL(triggered()), this,
	  SLOT(deleteAllKeys()));
  connect(ui.actionContextAddNewKey, SIGNAL(triggered()), this,
	  SLOT(addNewKey()));
  connect(ui.actionContextAddNewSynonym, SIGNAL(triggered()), this,
	  SLOT(addNewSynonym()));
  connect(ui.actionContextRenameKey, SIGNAL(triggered()), this,
	  SLOT(renameKey()));
  // =============================
  connect(ui.docTree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
	  SLOT(treeItemClicked()));
  connect(ui.titleEdit, SIGNAL(textChanged(QString)), this,
	  SLOT(contentChange()));
  connect(ui.dateEdit, SIGNAL(dateChanged(QDate)), this,
	  SLOT(contentChange()));
  connect(ui.commentEdit->document(),
	  SIGNAL(contentsChange(int, int, int)), this, SLOT(contentChange()));
  connect(ui.docTree, SIGNAL(itemCollapsed(QTreeWidgetItem *)), this,
	  SLOT(treeItemCollapsed(QTreeWidgetItem *)));
  connect(ui.docTree, SIGNAL(itemExpanded(QTreeWidgetItem *)), this,
	  SLOT(treeItemExpanded(QTreeWidgetItem *)));
  connect(ui.textEdit->document(), SIGNAL(modificationChanged(bool)),
	  this, SLOT(contentChange()));
  connect(ui.sleepTime, SIGNAL(timeChanged(QTime)), this,
	  SLOT(contentChange()));
  connect(ui.wakeupTime, SIGNAL(timeChanged(QTime)), this,
	  SLOT(contentChange()));
  connect(ui.scoreBox, SIGNAL(valueChanged(int)), this,
	  SLOT(contentChange()));
  connect(ui.lucidBox, SIGNAL(stateChanged(int)), this,
	  SLOT(lucidStateChanged()));
  connect(ui.dockWidget, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)),
	  this, SLOT(dockLocationChanged(Qt::DockWidgetArea)));
  connect(counter, SIGNAL(finished()), this, SLOT(counterFinished()));
  connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));

  connect(ui.textEdit->document(), SIGNAL(contentsChange(int, int, int)),
	  this, SLOT(textContentsChange(int, int, int)));

  connect(&http, SIGNAL(readyRead(const QHttpResponseHeader)), this,
	  SLOT(httpReadyRead()));

}

MainForm::~MainForm()
{
  delete ftp_dialog;
  delete dreamBase;
  delete trayIcon;
  if(counter->isRunning())
    counter->quit();
  delete counter;
  delete currentSuggestion;
}

// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
void MainForm::init()
{
  config.open(userPath + "/config.cfg");
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName
				  (config.codecName.toLatin1()));
  loadIcons();
  dreamBase->loadBase(config.base_coding);
  dreamBase->loadKeys(config.keys_coding);
  setGeometry(config.mwX, config.mwY, config.mwWidth, config.mwHeight);
  dreamBase->fillTree(ui.docTree);
  dreamBase->keys->fillTree(ui.keysTree, dreamBase->list(), config.freqSort);
  setFont(config.font);
  ui.splitter->setSizes(config.s1sizes);
  ui.splitter_2->setSizes(config.s2sizes);
  ui.docTree->setColumnHidden(1, true);
  ui.docTree->setColumnWidth(2, 50);
  ui.keysTree->setColumnHidden(2, true);
  ui.keysTree->setColumnHidden(3, true);
  ui.keysTree->setColumnWidth(0, 140);
  ui.keysTree->setColumnWidth(1, 70);

  if(config.trayicon)
    trayIcon->show();
  else
    trayIcon->hide();

  if(config.dockHidden)
  {
    ui.dockWidget->hide();
    ui.actionDockShowHide->setText(tr("Show archives"));
  }
  else
  {
    ui.dockWidget->show();
    ui.actionDockShowHide->setText(tr("Hide archives"));
    if(config.dockFloating)
      ui.dockWidget->setFloating(true);
    ui.dockWidget->setGeometry(config.dockX, config.dockY,
			       config.dockWidth, config.dockHeight);
  }
  addDockWidget(config.dockArea, ui.dockWidget);

  changed = false;
  closed = false;
  show();

  if(counter->isRunning())
    counter->quit();

  if(config.suggestions)
  {
    counter->setup(config.sugwords, config.sugignored,
		   config.sugsens, config.sugtime, trayIcon, dreamBase);
    currentSuggestion->occ = -1;
    counter->start();
  }

  if(config.update)
    checkVersion();
}

void MainForm::loadIcons()
{
  themePath = exePath + "/themes/" + config.theme + "/";

  ui.actionNew->setIcon(QIcon(themePath + "new.png"));
  ui.actionOpenFile->setIcon(QIcon(themePath + "open.png"));
  ui.actionSave->setIcon(QIcon(themePath + "save.png"));
  ui.actionSaveAs->setIcon(QIcon(themePath + "saveas.png"));
  ui.actionDelete->setIcon(QIcon(themePath + "delete.png"));
  ui.actionQuit->setIcon(QIcon(themePath + "exit.png"));
  ui.actionCut->setIcon(QIcon(themePath + "editcut.png"));
  ui.actionCopy->setIcon(QIcon(themePath + "editcopy.png"));
  ui.actionPaste->setIcon(QIcon(themePath + "editpaste.png"));
  ui.actionScale->setIcon(QIcon(themePath + "optimal_view.png"));
  ui.actionWhatsThis->setIcon(QIcon(themePath + "contexthelp.png"));
  ui.actionRollDown->setIcon(QIcon(themePath + "rolldown.png"));
  ui.actionRollUp->setIcon(QIcon(themePath + "rollup.png"));
  ui.actionSymbols->setIcon(QIcon(themePath + "dict.png"));
  ui.actionFind->setIcon(QIcon(themePath + "find.png"));
  ui.actionFontSettings->setIcon(QIcon(themePath + "fonts.png"));
  ui.actionLogout->setIcon(QIcon(themePath + "logout.png"));
  ui.actionSettings->setIcon(QIcon(themePath + "settings.png"));
  ui.actionStats->setIcon(QIcon(themePath + "stats.png"));
  ui.actionUndo->setIcon(QIcon(themePath + "undo.png"));
  ui.actionRedo->setIcon(QIcon(themePath + "redo.png"));
  ui.actionManual->setIcon(QIcon(themePath + "help.png"));

  ui.newKeyButton->setText("");
  ui.newKeyButton->setToolTip(tr("Add key..."));
  ui.newKeyButton->setIcon(QIcon(themePath + "addkey.png"));
  ui.addNewSynonymButton->setText("");
  ui.addNewSynonymButton->setToolTip(tr("Add synonym..."));
  ui.addNewSynonymButton->setIcon(QIcon(themePath + "addsynonym.png"));
  ui.deleteNewKeyButton->setText("");
  ui.deleteNewKeyButton->setToolTip(tr("Delete key..."));
  ui.deleteNewKeyButton->setIcon(QIcon(themePath + "deletekey.png"));

  ui.actionAbout->setIcon(QIcon(exePath + "/themes/winicon.png"));
}

// -----------------------------------------------------------------------------------------------
void MainForm::setFont(QFont font)
{
  QString cont = ui.textEdit->toPlainText();
  ui.textEdit->clear();
  ui.textEdit->setCurrentFont(font);
  ui.textEdit->document()->setDefaultFont(font);
  ui.textEdit->setPlainText(cont);
  config.font = font;
  config.save();
}

// -----------------------------------------------------------------------------------------------
void MainForm::highlightAll()
{
  highlightBlock(0, 0, ui.textEdit->toPlainText().size());

}

// -----------------------------------------------------------------------------------------------

// ********************************************************************************
// ****************************P R I V A T E***************************************
// ********************************************************************************

// -----------------------------------------------------------------------------------------------
void MainForm::highlightBlock(int pos, int rem, int add)
{
  QTextBlock block, lastBlock;
  QString item;

  block = ui.textEdit->document()->findBlock(pos);
  if(!block.isValid())
    return;

  if(add > rem)
    lastBlock = ui.textEdit->document()->findBlock(pos + add);
  else
    lastBlock = block;

  QTextCharFormat format;
  QBrush color(config.highlightColor);
  format.setBackground(color);

  while((block.isValid()) && !(lastBlock < block))
  {

    QList < QTextLayout::FormatRange > ranges;
    QTextLayout::FormatRange range;
    QString text = block.text().toLower();

    if(config.hkeys)
      for(int k = 0; k < dreamBase->keys->count(); k++)
	for(int s = 0; s < dreamBase->keys->at(k)->keys.count(); s++)
	{

	  item = dreamBase->keys->at(k)->keys.at(s).toLower();
	  int i = text.indexOf(item, 0);
	  while(i != -1)
	  {
	    range.format = format;

	    range.start = i;
	    range.length = item.size();
	    ranges << range;

	    i = text.indexOf(item, i + item.size());
	  }
	}

    block.layout()->setAdditionalFormats(ranges);
    ui.textEdit->document()->markContentsDirty(block.position(),
					       block.length());

    block = block.next();
  }

}

// --------------------------------------------------------------------------------------
void MainForm::clearUi()
{
  ui.docTree->clear();
  ui.titleEdit->clear();
  ui.textEdit->clear();
  ui.commentEdit->clear();
  ui.currentKeyList->clear();
  ui.dateEdit->setDate(QDate::currentDate());
  ui.sleepTime->setTime(QTime(0, 0, 0, 0));
  ui.wakeupTime->setTime(QTime(0, 0, 0, 0));
  ui.scoreBox->setValue(0);
  ui.lucidBox->setCheckState(Qt::Unchecked);
}

// ------------------------------------------------------------------------
void MainForm::closeEvent(QCloseEvent * e)
{
  if(!profile_deleting)
  {
    if(changed)
    {
      int q = QMessageBox::question(this, tr("Quit"),
                    tr
                    ("The current document has changed, save changes?"),
                    tr("Save"),
                    tr("Don't save"),
                    tr("Cancel"));

      if(q == 0)
	aSave();
      if(q == 2)
      {
	e->ignore();
	return;
      }
    }
    changed = false;
    closed = true;
    if(config.ftp_autoupload)
    {
      ftpDialog *cfd = new ftpDialog(this);
      cfd->upload();
      cfd->close();
      delete cfd;
    }
    if(counter->isRunning())
      counter->quit();

    config.saveExpandedItems(ui.docTree);
    config.dockHidden = ui.dockWidget->isHidden();
    config.dockFloating = ui.dockWidget->isFloating();
    config.dockX = ui.dockWidget->x();
    config.dockY = ui.dockWidget->y();
    config.dockWidth = ui.dockWidget->width();
    config.dockHeight = ui.dockWidget->height();
    config.mwX = this->geometry().x();
    config.mwY = this->geometry().y();
    config.mwWidth = this->width();
    config.mwHeight = this->height();
    config.s1sizes = ui.splitter->sizes();
    config.s2sizes = ui.splitter_2->sizes();
    config.save();
  }
  if(ftp_dialog != NULL)
    ftp_dialog->close();
  if(find_dialog != NULL)
    find_dialog->close();
  if(replace_dialog != NULL)
    replace_dialog->close();
  if(about_dialog != NULL)
    about_dialog->close();
  if(symbol_dialog != NULL)
    symbol_dialog->close();
  if(stat_dialog != NULL)
    stat_dialog->close();
  if(settings_dialog != NULL)
    settings_dialog->close();
}

// --------------------------------------------------------------------------------------
void MainForm::keyPressEvent(QKeyEvent * e)
{
  if((e->modifiers() == Qt::ControlModifier) && (e->key() == Qt::Key_F))
    aFind();
  if((e->modifiers() == Qt::ControlModifier) && (e->key() == Qt::Key_S))
    aSave();
  if((e->modifiers() == Qt::ControlModifier) && (e->key() == Qt::Key_N))
    aNew();
  if((e->modifiers() == Qt::ControlModifier) && (e->key() == Qt::Key_R))
    aReplace();
  if((e->modifiers() == Qt::ControlModifier) && (e->key() == Qt::Key_Q))
    close();

  if((e->key() == Qt::Key_Delete) && (ui.keysTree->hasFocus()))
    deleteKey();
  if((e->key() == Qt::Key_Delete) && (ui.docTree->hasFocus()))
    aDeleteSelected();
  // ------------------

  if((e->modifiers() == Qt::ControlModifier) && (e->key() == Qt::Key_H))
  {
    if(!ui.docTree->isColumnHidden(1))
    {
      ui.docTree->setColumnHidden(1, true);
      ui.keysTree->setColumnHidden(2, true);
      ui.keysTree->setColumnHidden(3, true);
    }
    else
    {
      ui.docTree->setColumnHidden(1, false);
      ui.keysTree->setColumnHidden(2, false);
      ui.keysTree->setColumnHidden(3, false);
    }
  }
}

void MainForm::suggestionMessage(Suggestion * suggestion, QString time)
{
  if(suggestion == NULL)
    return;
  if(suggestion->occ == -1)
  {
    if(counter->custom)
      trayIcon->showMessage(tr("My Dream Diary: Suggestion (") +
                time + ")",
                tr
                ("No suggestion were made from current archives"));
  }
  else
  {
    QString n;
    n.setNum(suggestion->occ);
    switch (config.sugtime)
    {
    case week:
      trayIcon->showMessage(tr("My Dream Diary: Suggestion (") +
                time + ")",
                tr("The \"") +
                suggestion->name +
                tr
                ("\" appeared in last week ")
                + n +
                tr
                (" times. I suggest to make a dream sign (key). \n (click here for more...)"),
                QSystemTrayIcon::Information, 20000);
      break;
    case month:
      trayIcon->showMessage(tr("My Dream Diary: Suggestion (") +
                time + ")",
                tr("The \"") +
                suggestion->name +
                tr
                ("\" appeared in last month ")
                + n +
                tr
                (" times. I suggest to make a dream sign (key). \n (click here for more...)"),
                QSystemTrayIcon::Information, 20000);
      break;
    case all:
      trayIcon->showMessage(tr("My Dream Diary: Suggestion (") +
                time + ")",
                tr("The \"") +
                suggestion->name +
                tr("\" appeared in Your dreams ")
                + n +
                tr
                (" times. I suggest to make a dream sign (key). \n (click here for more...)"),
                QSystemTrayIcon::Information, 20000);
      break;
    }
  }
  counter->custom = false;
}

// -----------------------------------------------------------------------------------------------
//
void MainForm::checkVersion()
{
  QUrl url(config.update_url);
  if(!url.isValid())
  {
    return;
  }
  http.setHost(url.host());
  http.get(url.path());
}

// -----------------------------------------------------------------------------------------------

// ********************************************************************************
// ****************************S L O T
// S*******************************************
// ********************************************************************************

// -----------------------------------------------------------------------------------------------
void MainForm::aNew()
{
  if(changed)
  {
    int q = QMessageBox::question(this, tr("New document"),
                  tr
                  ("The current document has changed, save changes?"),
                  tr("Save"), tr("Don't save"),
                  tr("Cancel"));

    if(q == 0)
      aSave();
    if(q == 2)
      return;
  }

  clearUi();
  dreamBase->fillTree(ui.docTree);
  dreamBase->closeCurrent();
  changed = false;
}

// -----------------------------------------------------------------------------------------------
void MainForm::aOpenFile()
{
  QString
    filename =
    QFileDialog::getOpenFileName(this, tr("Open file"), ".",
                 tr("My Dream Diary files (*.md)"));
  if(filename.isEmpty())
    return;

  if(changed)
  {
    int q = QMessageBox::question(this, tr("New document"),
                  tr
                  ("The current document has changed, save changes before opening a new document?"),
                  tr("Save"), tr("Don't save"),
                  tr("Cancel"));

    if(q == 0)
      aSave();
    if(q == 2)
      return;
  }

  if(dreamBase->openFile(filename))
  {
    QMessageBox::information(this, tr("Adding file complete"),
                 tr
                 ("The selected file was successfully added to your archive!"),
                 QMessageBox::Ok);
    dreamBase->fillTree(ui.docTree);
  }
}

// -----------------------------------------------------------------------------------------------
void MainForm::aSave()
{
  bool expand = false;

  if(ui.titleEdit->text().isEmpty())
    ui.titleEdit->setText(tr("(no title)"));

  DreamDocument newDoc;
  newDoc.ID = dreamBase->Current();
  newDoc.title = ui.titleEdit->text();
  newDoc.date = ui.dateEdit->date();
  newDoc.sleep = ui.sleepTime->time();
  newDoc.wakeup = ui.wakeupTime->time();
  newDoc.score = ui.scoreBox->value();
  newDoc.content = ui.textEdit->toPlainText();
  newDoc.comment = ui.commentEdit->toPlainText();
  if(ui.lucidBox->checkState() == Qt::Checked)
    newDoc.lucid = true;
  else
    newDoc.lucid = false;

  // jesli nowy:
  if(newDoc.ID == -1)
  {
    dreamBase->add(newDoc);
    expand = true;
  }
  else				// jesli istniejacy
    dreamBase->update(newDoc);

  changed = false;
  dreamBase->fillTree(ui.docTree);
  dreamBase->keys->fillTree(ui.keysTree, dreamBase->list(), config.freqSort);
  dreamBase->keys->fillCurrentList(ui.currentKeyList,
				   ui.textEdit->toPlainText());

  highlightAll();
  // rozwijanie edytowanej galezi
  if(expand)
    dreamBase->expandDoc(dreamBase->Current(), ui.docTree);

  if(config.suggestions)
    if(!counter->isRunning())
    {
      counter->setup(config.sugwords, config.sugignored,
		     config.sugsens, config.sugtime, trayIcon, dreamBase);
      currentSuggestion->occ = -1;
      counter->start();
    }
}

// -----------------------------------------------------------------------------------------------
void MainForm::aSaveAs()
{
  QString
    filename =
    QFileDialog::getSaveFileName(this, tr("Save file"), ".",
                 tr("My Dream Diary file (*.md)"));
  if(!filename.isEmpty())
    dreamBase->toFile(dreamBase->Current(), false, filename);
}

// -----------------------------------------------------------------------------------------------
void MainForm::aDelete()
{
  int q = QMessageBox::question(this, tr("Delete document"),
                tr
                ("Do you want to remove the current document?"),
                tr("Delete"), tr("Cancel"));
  if(q == 0)
  {
    int c = dreamBase->Current();
    aNew();
    dreamBase->remove(c);
    dreamBase->fillTree(ui.docTree);
  }
}

// -----------------------------------------------------------------------------------------------
void MainForm::aDeleteAll()
{
  int q = QMessageBox::question(this, tr("Delete archives"),
                tr
                ("Do you want to remove all documents in your archives?"),
                tr("Delete"), tr("Cancel"));
  if(q == 0)
  {
    bool ok;
    QString
      str = QInputDialog::getText(this, tr("Verification"),
                  tr
                  ("To confirm your identity please retype your password:"),
                  QLineEdit::PasswordEchoOnEdit, "", &ok, 0);
    QString
      hash = QCryptographicHash::hash(str.toUtf8(),
				      QCryptographicHash::Md5).toHex();
    if(hash != config.passwd)
      QMessageBox::warning(this, tr("Error"),
               tr
               ("Verification failed, wrong password!"),
               QMessageBox::Ok);
    else
    {
      // kasowanie plikow data/*.md
      QDir dataDir(userPath + "/data/");
      QStringList
	files = dataDir.entryList(QDir::NoDotAndDotDot | QDir::Files);

      for(int i = 0; i < files.count(); i++)
      {
	if(files.at(i).right(3) == ".md")
	  QFile::remove(userPath + "/data/" + files.at(i));
      }
      changed = false;
      aNew();
      dreamBase->loadBase(config.base_coding);
      dreamBase->fillTree(ui.docTree);
    }
  }
}

// -----------------------------------------------------------------------------------------------
void MainForm::aDeleteSelected()
{
  int id = ui.docTree->currentItem()->text(1).toInt();
  if(id == -1)
    return;
  QString title = ui.docTree->currentItem()->text(0);
  int q = QMessageBox::question(this, tr("Delete document"),
                tr
                ("Do you want to remove the document \"")
                + title + "\"?", tr("Delete"),
                tr("Cancel"));
  if(q == 0)
  {

    aNew();
    dreamBase->remove(id);
    dreamBase->fillTree(ui.docTree);
  }
}

// -----------------------------------------------------------------------------------------------
void MainForm::aToHtml()
{

  QString
    filename =
    QFileDialog::getSaveFileName(this, tr("Save file"), ".",
                 tr("html files (*.html)"));
  if(!filename.isEmpty())
    dreamBase->saveToHtml(filename);

}

// -----------------------------------------------------------------------------------------------
void MainForm::aToFile()
{
  QString
    filename =
    QFileDialog::getSaveFileName(this, tr("Save file"), ".",
                 tr("My Dream Diary archives (*.mdd)"));
  if(filename.right(4) != ".mdd")
    filename += ".mdd";

  if(!filename.isEmpty())
    dreamBase->exportToFile(filename);
}

// -----------------------------------------------------------------------------------------------
void MainForm::aFromFile()
{
  QString
    filename =
    QFileDialog::getOpenFileName(this, tr("Open file"), ".",
                 tr
                 ("My Dream Diary archives (*.mdd);;All files (*.*)"));
  if(!filename.isEmpty())
  {
    int q = QMessageBox::question(this, tr("Updating base"),
                  tr
                  ("All documents in archives will be removed, do you want to continue?"),
                  tr("Import"), tr("Cancel"));
    if(q == 0)
    {
      dreamBase->importFromFile(filename);
      dreamBase->rewriteAll(config.base_coding);
      dreamBase->fillTree(ui.docTree);
      dreamBase->keys->fillTree(ui.keysTree,
				dreamBase->list(), config.freqSort);
    }
  }
}

// -----------------------------------------------------------------------------------------------
void MainForm::aFromDrm()
{
  QString
    filename =
    QFileDialog::getOpenFileName(this, tr("Open file"), ".",
                 tr
                 ("My Dream Diary archives (*.drm);;All files (*.*)"));
  if(!filename.isEmpty())
  {
    int q = QMessageBox::question(this, tr("Updating base"),
                  tr
                  ("All documents in archives will be removed, do you want to continue?"),
                  tr("Import"), tr("Cancel"));
    if(q == 0)
    {
      dreamBase->importFromDrm(filename);
      dreamBase->rewriteAll(config.base_coding);
      dreamBase->fillTree(ui.docTree);
      dreamBase->keys->fillTree(ui.keysTree,
				dreamBase->list(), config.freqSort);
    }
  }
}

// -----------------------------------------------------------------------------------------------
void MainForm::aToFTP()
{
  if(ftp_dialog == NULL)
    ftp_dialog = new ftpDialog(this);

  ftp_dialog->setGeometry(QApplication::desktop()->width() / 2 - 295,
			  QApplication::desktop()->height() / 2 - 140,
			  590, 280);
  ftp_dialog->exec();
  delete ftp_dialog;
  ftp_dialog = NULL;
}

// -----------------------------------------------------------------------------------------------
//
void MainForm::aLogout()
{
  this->close();
  if(closed)
  {
    username.clear();
    userPath.clear();
    clearUi();
    dreamBase->closeCurrent();
    loginOk = false;
    full = false;

    if(login_dialog == NULL)
      login_dialog = new loginDialog(this);

    login_dialog->setGeometry(QApplication::desktop()->width() / 2 -
			      login_dialog->width() / 2,
			      QApplication::desktop()->height() /
			      2 - login_dialog->height() / 2,
			      login_dialog->width(), login_dialog->height());
    login_dialog->exec();
    delete login_dialog;
    login_dialog = NULL;

    if(loginOk)
      init();
  }
}

// -----------------------------------------------------------------------------------------------
//
void MainForm::aLoadTemplate()
{
  QString
    filename =
    QFileDialog::getOpenFileName(this, tr("Load template file"), ".",
                 tr
                 ("All files(*.*);;HTML files(*.html)"));
  if(!filename.isEmpty())
  {
    QString tempfile = userPath + "template.html";
    if(QFile::exists(tempfile + ".old"))
    {
      if(!QFile::remove(tempfile + ".old"))
    QMessageBox::critical(this, tr("Error"),
                  tr
                  ("Deleting backup files failed"),
                  QMessageBox::Ok);
      if(!QFile::copy(tempfile, tempfile + ".old"))
    QMessageBox::critical(this, tr("Error"),
                  tr
                  ("Making backup files faild"),
                  QMessageBox::Ok);
    }
    if(QFile::exists(tempfile))
      if(!QFile::remove(tempfile))
    QMessageBox::critical(this, tr("Error"),
                  tr
                  ("Removing current archive faild"),
                  QMessageBox::Ok);
    if(!QFile::copy(filename, tempfile))
      QMessageBox::critical(this, tr("Error"),
                tr
                ("Archive import failed!"),
                QMessageBox::Ok);
    else
      QMessageBox::information(this, tr("Success"),
                   tr
                   ("New document template has been added!"),
                   QMessageBox::Ok);
  }
}

// -----------------------------------------------------------------------------------------------
void MainForm::aDeleteProfile()
{
  int q = QMessageBox::question(this, tr("Delete profile"),
                tr
                ("All data and configuration files will be removed! Do you want to delete this profile? "),
                tr("Delete"), tr("Cancel"));

  if(q == 0)
  {

    bool ok;
    QString
      str = QInputDialog::getText(this, tr("Verification"),
                  tr
                  ("To confirm your identity please retype your password:"),
                  QLineEdit::PasswordEchoOnEdit, "", &ok, 0);
    QString
      hash = QCryptographicHash::hash(str.toUtf8(),
				      QCryptographicHash::Md5).toHex();
    if(hash != config.passwd)
    {
      QMessageBox::warning(this, tr("Error"),
               tr
               ("Verification failed, wrong password!"),
               QMessageBox::Ok);
      return;
    }

    QFile pfile;
    pfile.remove(userPath + "config.cfg");
    pfile.remove(userPath + username + ".mdd");
    pfile.remove(userPath + username + ".mdd.old");
    pfile.remove(userPath + "keys.mdk");
    pfile.remove(userPath + username + ".xsym");
    pfile.remove(userPath + username + ".xsym.old");
    pfile.remove(userPath + "template.html");
    pfile.remove(userPath + "template.html.old");
    pfile.remove(userPath + "ftp");

    QDir dataDir(userPath + "data/");
    QStringList files = dataDir.entryList(QDir::NoDotAndDotDot | QDir::Files);
    for(int i = 0; i < files.count(); i++)
      if(files.at(i).right(3) == ".md")
	QFile::remove(userPath + "data/" + files.at(i));

    dataDir.rmdir(userPath + "data/");
    dataDir.rmdir(userPath);

    Last last;
    if(last.load(lastLoginPath))
    {
      if(last.login == username)
	QFile::remove(lastLoginPath);
    }
    profile_deleting = true;
    aLogout();
  }

}

// -----------------------------------------------------------------------------------------------
void MainForm::aFontSettings()
{
  bool ok;
  QFont font = QFontDialog::getFont(&ok, ui.textEdit->currentFont());
  if(ok)
    setFont(font);
}

// -----------------------------------------------------------------------------------------------
void MainForm::aFind()
{
  if(find_dialog == NULL)
    find_dialog = new findDialog(this);

  const int
    x = (QApplication::desktop()->width() / 2) - (find_dialog->width() / 2);
  const int
    y = (QApplication::desktop()->height() / 2) - (find_dialog->height() / 2);
  const int w = find_dialog->width();
  const int h = find_dialog->height();
  find_dialog->setGeometry(x, y, w, h);
  find_dialog->exec();
  delete find_dialog;
  find_dialog = NULL;
}

// -----------------------------------------------------------------------------------------------
void MainForm::aReplace()
{
  if(replace_dialog == NULL)
    replace_dialog = new replaceDialog(this);

  const int
    x =
    (QApplication::desktop()->width() / 2) - (replace_dialog->width() / 2);
  const int
    y =
    (QApplication::desktop()->height() / 2) - (replace_dialog->height() / 2);
  const int w = replace_dialog->width();
  const int h = replace_dialog->height();
  replace_dialog->setGeometry(x, y, w, h);
  replace_dialog->exec();
  delete replace_dialog;
  replace_dialog = NULL;
}

// -----------------------------------------------------------------------------------------------
void MainForm::aAbout()
{
  if(about_dialog == NULL)
    about_dialog = new aboutDialog(this);
  const int
    x = (QApplication::desktop()->width() / 2) - (about_dialog->width() / 2);
  const int
    y =
    (QApplication::desktop()->height() / 2) - (about_dialog->height() / 2);
  const int w = about_dialog->width();
  const int h = about_dialog->height();
  about_dialog->setGeometry(x, y, w, h);
  about_dialog->exec();
  delete about_dialog;
  about_dialog = NULL;
}

// -----------------------------------------------------------------------------------------------
void MainForm::aSymbols()
{
  if(symbol_dialog == NULL)
    symbol_dialog = new symbolDialog(this);

  const int
    x = (QApplication::desktop()->width() / 2) - (symbol_dialog->width() / 2);
  const int
    y =
    (QApplication::desktop()->height() / 2) - (symbol_dialog->height() / 2);
  const int w = symbol_dialog->width();
  const int h = symbol_dialog->height();
  symbol_dialog->setGeometry(x, y, w, h);
  symbol_dialog->exec();
  delete symbol_dialog;
  symbol_dialog = NULL;
}

// -----------------------------------------------------------------------------------------------
void MainForm::aCounterStart()
{
  if(counter->isRunning())
    trayIcon->showMessage(tr("Operation forbidden"),
              tr
              ("Suggestion scanner is already running!"),
              QSystemTrayIcon::Warning);
  else
  {
    trayIcon->showMessage(tr("Suggestion scanner"),
              tr("Archives scanning started..."));
    counter->setup(config.sugwords, config.sugignored, config.sugsens,
		   config.sugtime, trayIcon, dreamBase);
    currentSuggestion->occ = -1;
    counter->custom = true;
    counter->start();
  }
}

// -----------------------------------------------------------------------------------------------
void MainForm::aIgnoredList()
{
  if(ignored_dialog == NULL)
    ignored_dialog = new ignoredDialog(dreamBase, config.keys_coding);

  const int w = 250;
  const int h = 350;
  const int x = (QApplication::desktop()->width() / 2) - (w / 2);
  const int y = (QApplication::desktop()->height() / 2) - (h / 2);
  ignored_dialog->setGeometry(x, y, w, h);
  ignored_dialog->exec();
  delete ignored_dialog;
  ignored_dialog = NULL;
}

// -----------------------------------------------------------------------------------------------
void MainForm::aStats()
{
  // sprawdzamy czy sa jakies dane do wyswietlenia statystyk
  bool statEnable = false;
  for(int i = 0; i < dreamBase->count(); i++)
    if((dreamBase->at(i)->score > 0) &&
       (dreamBase->at(i)->sleep != dreamBase->at(i)->wakeup))
    {
      statEnable = true;
      break;
    }

  if(statEnable)
  {
    if(stat_dialog == NULL)
      stat_dialog = new statDialog(dreamBase->list());

    const int
      x = (QApplication::desktop()->width() / 2) - (stat_dialog->width() / 2);
    const int
      y =
      (QApplication::desktop()->height() / 2) - (stat_dialog->height() / 2);
    const int w = stat_dialog->width();
    const int h = stat_dialog->height();
    stat_dialog->setGeometry(x, y, w, h);
    stat_dialog->exec();
    delete stat_dialog;
    stat_dialog = NULL;
  }
  else
    QMessageBox::information(this, tr("No notes"),
                 tr("There is not enough data to make statistics"),
                 QMessageBox::Ok);
}

// -----------------------------------------------------------------------------------------------
void MainForm::aSettings()
{
  if(settings_dialog == NULL)
    settings_dialog = new settingsDialog(this);

  const int
    x =
    (QApplication::desktop()->width() / 2) - (settings_dialog->width() / 2);
  const int
    y =
    (QApplication::desktop()->height() / 2) - (settings_dialog->height() / 2);
  const int w = settings_dialog->width();
  const int h = settings_dialog->height();
  settings_dialog->setGeometry(x, y, w, h);
  settings_dialog->exec();
  delete settings_dialog;
  settings_dialog = NULL;
}

// -----------------------------------------------------------------------------------------------
void MainForm::aDefaultView()
{
  ui.dockWidget->setFloating(false);
  addDockWidget(Qt::LeftDockWidgetArea, ui.dockWidget);
  ui.dockWidget->show();
  ui.actionDockShowHide->setText(tr("Hide archives"));
  ui.toolBar->show();
  ui.actionToolBarShowHide->setText(tr("Hide tool bar"));
  ui.actionScale->setIcon(QIcon(themePath + "optimal_view.png"));
  if(full)
  {
    full = false;
    ui.actionScale->setText(tr("Optimal view"));
    ui.actionScale->setIconText(tr("Optimal view"));
    ui.actionScale->setToolTip(tr("Optimal view"));
  }
  QList < int > s1, s2;
  s1 << 273 << 206;
  s2 << 341 << 141;
  this->setGeometry(x(), y(), 900, 600);
  ui.splitter->setSizes(s1);
  ui.splitter_2->setSizes(s2);
}

// -----------------------------------------------------------------------------------------------
void MainForm::aLangEn()
{
    changeLanguageTo("en");
}

// -----------------------------------------------------------------------------------------------
void MainForm::aLangPl()
{
    changeLanguageTo("pl");
}

// -----------------------------------------------------------------------------------------------
void MainForm::aLangRu()
{
    changeLanguageTo("ru");
}

// -----------------------------------------------------------------------------------------------
void MainForm::changeLanguageTo(QString LanguageISOCode)
{
    QFile::remove(currentTranslation);
    if(LanguageISOCode != "en")
        if((!QFile::copy(exePath + "/locale/" + LanguageISOCode + ".qm",
                         currentTranslation)) ||
           (!QFile::exists(exePath + "/locale/" + LanguageISOCode + ".qm")))
          QMessageBox::critical(this, tr("Error"),
                    tr("Copying translation file failed! Please check the 'locale' directory."),
                    QMessageBox::Ok);
        else
          QMessageBox::information(this, tr("information"),
                       tr("All changes will be available after application restart."),
                       QMessageBox::Ok);
}

// -----------------------------------------------------------------------------------------------
void MainForm::aLangOther()
{
  QString
    filename =
    QFileDialog::getOpenFileName(this, tr("Open file"), ".",
                 tr("QT translation file (*.qm)"));
  if(!filename.isEmpty())
  {
    QFile::remove(currentTranslation);
    if(QFile::link(filename, currentTranslation))
      QMessageBox::information(this, tr("information"),
			       tr
			       ("All changes will be available after application restart."),
			       QMessageBox::Ok);
    else
      QMessageBox::critical(this, tr("Error"),
                tr
                ("Creating symbolic link failed! Please check the 'locale' directory."),
                QMessageBox::Ok);
  }
}

// -----------------------------------------------------------------------------------------------
void MainForm::aDockLeft()
{
  ui.dockWidget->show();
  ui.actionDockShowHide->setText(tr("Hide archives"));
  if(ui.dockWidget->isFloating())
    ui.dockWidget->setFloating(false);

  addDockWidget(Qt::LeftDockWidgetArea, ui.dockWidget);
}

// -----------------------------------------------------------------------------------------------
void MainForm::aDockRight()
{
  ui.dockWidget->show();
  ui.actionDockShowHide->setText(tr("Hide archives"));
  if(ui.dockWidget->isFloating())
    ui.dockWidget->setFloating(false);

  addDockWidget(Qt::RightDockWidgetArea, ui.dockWidget);
}

// -----------------------------------------------------------------------------------------------
void MainForm::aDockFloat()
{
  ui.dockWidget->show();
  ui.actionDockShowHide->setText(tr("Hide archives"));
  if(!ui.dockWidget->isFloating())
    ui.dockWidget->setFloating(true);
}

// -----------------------------------------------------------------------------------------------
void MainForm::aKeyFreqSort()
{
  config.freqSort = true;
  dreamBase->keys->fillTree(ui.keysTree, dreamBase->list(), config.freqSort);
}

// -----------------------------------------------------------------------------------------------
void MainForm::aKeyAscSort()
{
  config.freqSort = false;
  dreamBase->keys->fillTree(ui.keysTree, dreamBase->list(), config.freqSort);
}

// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
void MainForm::addNewKey()
{
  bool ok;
  QString
    keyname = QInputDialog::getText(this, tr("New key"),
                    tr("Please enter new key:"),
                    QLineEdit::Normal, "", &ok);

  if(ok)
  {
    if(dreamBase->keys->add(keyname) != -1)
    {
      dreamBase->keys->fillTree(ui.keysTree,
				dreamBase->list(), config.freqSort);
      dreamBase->keys->fillCurrentList(ui.currentKeyList,
				       ui.textEdit->toPlainText());
      dreamBase->saveKeys(config.keys_coding);
      highlightAll();
    }
  }

}

// -----------------------------------------------------------------------------------------------
void MainForm::addNewSynonym()
{
  if(ui.keysTree->currentItem() == NULL)
    return;
  bool ok;
  int id;
  QString parent;
  if(ui.keysTree->currentItem()->text(2) == "M")
  {
    id = ui.keysTree->currentItem()->text(3).toInt();
    parent = ui.keysTree->currentItem()->text(0);
  }
  else if(ui.keysTree->currentItem()->text(2) == "S")
  {
    id = ui.keysTree->currentItem()->parent()->text(3).toInt();
    parent = ui.keysTree->currentItem()->parent()->text(0);
  }
  else
    return;

  QString
    keyname = QInputDialog::getText(this, tr("New key"),
                    tr
                    ("Please enter a new synonym for key \"")
                    + parent + "\"",
                    QLineEdit::Normal, "", &ok);

  if(ok)
  {
    if(dreamBase->keys->addSynonym(id, keyname))
    {
      dreamBase->keys->fillTree(ui.keysTree,
				dreamBase->list(), config.freqSort);
      dreamBase->keys->fillCurrentList(ui.currentKeyList,
				       ui.textEdit->toPlainText());
      dreamBase->saveKeys(config.keys_coding);
      highlightAll();
    }
  }

}

// -----------------------------------------------------------------------------------------------
void MainForm::insertNewKey()
{
  QString keyname = ui.textEdit->textCursor().selectedText();
  if(dreamBase->keys->add(keyname) != -1)
  {
    dreamBase->keys->fillTree(ui.keysTree, dreamBase->list(),
			      config.freqSort);
    dreamBase->keys->fillCurrentList(ui.currentKeyList,
				     ui.textEdit->toPlainText());
    dreamBase->saveKeys(config.keys_coding);
    highlightAll();
  }
}

// -----------------------------------------------------------------------------------------------
void MainForm::webFind()
{
  int q = QMessageBox::question(this, tr("Question"),
                tr
                ("The application will run the defined internet browser. Do you want to continue?"),
                tr("Activate"), tr("Cancel"));

  if(q == 1)
    return;

  if(!QFile::exists(config.browserPath))
  {
    QMessageBox::critical(this,
              tr("Activating browser failed"),
              tr
              ("The selected file doesn't exists! To use this function please define your internet browser"),
              QMessageBox::Ok);
    aSettings();
  }

  QString exp = ui.textEdit->textCursor().selectedText();
  QString command = "\""+config.browserPath+"\" " + config.webAddress;
  int i = command.indexOf("#exp#");
  if(i == -1)
    QMessageBox::warning(this,
             tr("Activating browser failed"),
             tr
             ("The #exp# phrase is missing! Please check the propriety of tke configuration file"),
             QMessageBox::Ok);
  else
  {
    command.replace(i, 5, exp);
    QProcess *browser = new QProcess;
    browser->start(command);
  }

}

// -----------------------------------------------------------------------------------------------
void MainForm::deleteKey()
{
  if(ui.keysTree->currentItem() == NULL)
    return;

  QString keyname = ui.keysTree->currentItem()->text(0);
  int q = -1;
  if(ui.keysTree->currentItem()->text(2) == "M")
    q = QMessageBox::question(this, tr("Confirm"),
                  tr
                  ("Do you want to remove the key \"")
                  + keyname +
                  tr
                  ("\" and all his synonyms?"),
                  tr("Delete"), tr("Cancel"));
  else
    q = QMessageBox::question(this, tr("Confirm"),
                  tr
                  ("Do you want to remove the key (synonym) \"")
                  + keyname + "\"?", tr("Delete"), tr("Cancel"));
  if(q == 0)
  {
    if(ui.keysTree->currentItem()->text(2) == "M")
    {
      int id = ui.keysTree->currentItem()->text(3).toInt();
      dreamBase->keys->remove(id);
    }
    if(ui.keysTree->currentItem()->text(2) == "S")
    {
      int id = ui.keysTree->currentItem()->parent()->text(3).toInt();
      dreamBase->keys->removeSynonym(id, keyname);
    }
    dreamBase->keys->fillCurrentList(ui.currentKeyList,
				     ui.textEdit->toPlainText());
    dreamBase->keys->fillTree(ui.keysTree, dreamBase->list(),
			      config.freqSort);
    dreamBase->saveKeys(config.keys_coding);
    highlightAll();
  }
}

// -----------------------------------------------------------------------------------------------
void MainForm::deleteAllKeys()
{
  int q = QMessageBox::question(this, tr("Confirm"),
                tr
                ("Do you want to remove all keys?"),
                tr("Delete"), tr("Cancel"));
  if(q == 0)
  {

    dreamBase->keys->keysList->clear();
    dreamBase->keys->fillTree(ui.keysTree, dreamBase->list(),
			      config.freqSort);
    dreamBase->saveKeys(config.keys_coding);
    highlightAll();
  }
}

// -----------------------------------------------------------------------------------------------
void MainForm::renameKey()
{
  if(ui.keysTree->currentItem() == NULL)
    return;

  QString keyname = ui.keysTree->currentItem()->text(0);
  bool ok;
  QString
    newname = QInputDialog::getText(this, tr("Rename key"),
                    tr
                    ("Please enter a new name for the key \"")
                    + keyname + "\"",
                    QLineEdit::Normal, "", &ok);
  if(!ok)
    return;
  dreamBase->keys->rename(keyname, newname);
  dreamBase->keys->fillCurrentList(ui.currentKeyList,
				   ui.textEdit->toPlainText());
  dreamBase->keys->fillTree(ui.keysTree, dreamBase->list(), config.freqSort);
  dreamBase->saveKeys(config.keys_coding);
  highlightAll();
}

// -----------------------------------------------------------------------------------------------
void MainForm::aScale()
{
  if(!full)
  {
    if(!ui.dockWidget->isFloating())
    {
      ui.dockWidget->hide();
      ui.actionDockShowHide->setText(tr("Show archives"));
    }
    QList < int > s;
    s << 600 << 0;
    ui.splitter_2->setSizes(s);
    ui.actionScale->setIcon(QIcon(themePath + "standard_view.png"));
    full = true;
    ui.actionScale->setText(tr("Standard view"));
    ui.actionScale->setIconText(tr("Standard view"));
    ui.actionScale->setToolTip(tr("Standard view"));
  }
  else
  {
    ui.dockWidget->show();
    ui.actionDockShowHide->setText(tr("Hide archives"));
    QList < int > s;
    s << 341 << 141;
    ui.splitter_2->setSizes(s);
    ui.actionScale->setIcon(QIcon(themePath + "optimal_view.png"));
    full = false;
    ui.actionScale->setText(tr("Optimal view"));
    ui.actionScale->setIconText(tr("Optimal view"));
    ui.actionScale->setToolTip(tr("Optimal view"));
  }
}

void MainForm::aWhatsThis()
{
  QWhatsThis::enterWhatsThisMode();
}

// -----------------------------------------------------------------------------------------------
void MainForm::aManual()
{

  if(!QFile::exists(config.browserPath))
  {
    QMessageBox::critical(this,
              tr("Activating browser failed"),
              tr
              ("The selected file doesn't exists! To use this function please define your internet browser"),
              QMessageBox::Ok);
    aSettings();
  }

  QProcess *man = new QProcess;
  man->start("\""+config.browserPath + "\" \"file://" + exePath + "/doc/manual.html\"");
}

// -----------------------------------------------------------------------------------------------
//
void MainForm::treeItemClicked()
{
  int id = ui.docTree->currentItem()->text(1).toInt();
  if(id == -1)
    return;

  if(changed)
  {
    int q = QMessageBox::question(this, tr("Open document"),
                  tr
                  ("The current document has changed, save changes before opening a new document?"),
                  tr("Save"), tr("Don't save"),
                  tr("Cancel"));

    if(q == 0)
      aSave();
    if(q == 2)
      return;
  }
  dreamBase->open(id);
  dreamBase->keys->fillCurrentList(ui.currentKeyList,
				   ui.textEdit->toPlainText());
  highlightAll();
}

// -----------------------------------------------------------------------------------------------
void MainForm::aDockShowHide()
{
  if(ui.dockWidget->isHidden())
  {
    ui.dockWidget->show();
    ui.actionDockShowHide->setText(tr("Hide archives"));
  }
  else
  {
    ui.dockWidget->hide();
    ui.actionDockShowHide->setText(tr("Show archives"));
  }
}

// -----------------------------------------------------------------------------------------------
void MainForm::aToolBarShowHide()
{
  if(ui.toolBar->isHidden())
    ui.toolBar->show();
  else
    ui.toolBar->hide();
}

// -----------------------------------------------------------------------------------------------
void MainForm::treeItemExpanded(QTreeWidgetItem * item)
{
  if(!config.dateItems.contains(item->text(0)))
    config.dateItems.push_back(item->text(0));
}

// -----------------------------------------------------------------------------------------------
void MainForm::treeItemCollapsed(QTreeWidgetItem * item)
{
  if(config.dateItems.contains(item->text(0)))
    for(int i = 0; i < config.dateItems.count(); i++)
      if(config.dateItems.at(i) == item->text(0))
      {
	config.dateItems.removeAt(i);
	break;
      }
}

// -----------------------------------------------------------------------------------------------
void MainForm::textContentsChange(int pos, int rem, int add)
{
  highlightBlock(pos, rem, add);
}

// -----------------------------------------------------------------------------------------------
void MainForm::contentChange()
{
  changed = true;
}

// -----------------------------------------------------------------------------------------------
void MainForm::lucidStateChanged()
{
  changed = true;
}

// -----------------------------------------------------------------------------------------------
void MainForm::dockLocationChanged(Qt::DockWidgetArea area)
{
  config.dockArea = area;
}

// -----------------------------------------------------------------------------------------------
void MainForm::textEditContextMenu()
{
  QMenu *menu = new QMenu();

  menu->addAction(ui.actionUndo);
  menu->addAction(ui.actionRedo);
  menu->addSeparator();
  menu->addAction(ui.actionCut);
  menu->addAction(ui.actionCopy);
  menu->addAction(ui.actionPaste);
  menu->addSeparator();
  menu->addAction(ui.actionSelectAll);
  menu->addSeparator();
  menu->addAction(ui.actionInsertNewKey);
  menu->addSeparator();
  menu->addAction(ui.actionWebFind);

  if(!ui.textEdit->document()->isUndoAvailable())
    ui.actionUndo->setEnabled(false);
  else
    ui.actionUndo->setEnabled(true);

  if(!ui.textEdit->document()->isRedoAvailable())
    ui.actionRedo->setEnabled(false);
  else
    ui.actionRedo->setEnabled(true);

  if(!ui.textEdit->textCursor().hasSelection())
    ui.actionCopy->setEnabled(false);
  else
    ui.actionCopy->setEnabled(true);

  if(!ui.textEdit->textCursor().hasSelection())
    ui.actionCut->setEnabled(false);
  else
    ui.actionCut->setEnabled(true);

  if(!ui.textEdit->textCursor().hasSelection())
    ui.actionInsertNewKey->setEnabled(false);
  else
    ui.actionInsertNewKey->setEnabled(true);

  if(!ui.textEdit->textCursor().hasSelection())
    ui.actionWebFind->setEnabled(false);
  else
    ui.actionWebFind->setEnabled(true);

  menu->exec(QCursor::pos());

  ui.actionCopy->setEnabled(true);
  ui.actionCut->setEnabled(true);
  delete menu;
}

// -----------------------------------------------------------------------------------------------
void MainForm::docTreeContextMenu()
{
  QMenu *menu = new QMenu();

  menu->addAction(ui.actionRollDown);
  menu->addAction(ui.actionRollUp);
  menu->addSeparator();
  menu->addAction(ui.actionSearch);
  menu->addAction(ui.actionSearchContent);
  menu->addSeparator();
  menu->addAction(ui.actionDeleteSelected);
  menu->addSeparator();
  menu->addAction(ui.actionDeleteAllDocs);

  menu->exec(QCursor::pos());
  delete menu;
}

// ----------------------------------------------------------------------------------------------
void MainForm::keyListContextMenu()
{
  QMenu *menu = new QMenu();

  menu->addAction(ui.actionContextAddNewKey);
  menu->addAction(ui.actionContextAddNewSynonym);
  menu->addAction(ui.actionContextRenameKey);
  menu->addSeparator();
  menu->addAction(ui.actionKeyAscSort);
  menu->addAction(ui.actionKeyFreqSort);
  menu->addSeparator();
  menu->addAction(ui.actionDeleteAllKeys);

  menu->exec(QCursor::pos());
  delete menu;
}

// ----------------------------------------------------------------------------------------------
void MainForm::aSearch()
{
  bool ok;
  QString
    exp = QInputDialog::getText(this, tr("Search"),
                tr("Please enter the title of the document"),
                QLineEdit::Normal, "", &ok, 0);

  if(ok)
  {

    QList < int > list = dreamBase->getByTitle(exp);
    dreamBase->colorItems(list, ui.docTree, dreamBase->foundColor);
    for(int i = 0; i < list.count(); i++)
      dreamBase->expandDoc(list.at(i), ui.docTree);

    exp = exp.setNum(list.size());
    QMessageBox::information(this, tr("Finish"),
                 tr
                 ("File searching finished. The number of matching documents is: ")
                 + exp, QMessageBox::Ok);
  }

}

// ----------------------------------------------------------------------------------------------
void MainForm::aSearchContent()
{
  bool ok;
  QString
    exp = QInputDialog::getText(this, tr("Search"),
                tr
                ("Please enter the contents of the document"),
                QLineEdit::Normal, "", &ok, 0);

  if(ok)
  {

    QList < int > list = dreamBase->getByContent(exp);
    dreamBase->colorItems(list, ui.docTree, dreamBase->foundColor);
    for(int i = 0; i < list.count(); i++)
      dreamBase->expandDoc(list.at(i), ui.docTree);

    exp.setNum(list.size());
    QMessageBox::information(this, tr("Finish"),
                 tr
                 ("File searching finished. The number of matching documents is: ")
                 + exp, QMessageBox::Ok);
  }
}

// ---------------------------------------------------------------------------------------------
void MainForm::aRollUp()
{
  for(int i = 0; i < ui.docTree->topLevelItemCount(); i++)
    ui.docTree->setItemExpanded(ui.docTree->topLevelItem(i), false);
}

// ---------------------------------------------------------------------------------------------
void MainForm::aRollDown()
{
  for(int i = 0; i < ui.docTree->topLevelItemCount(); i++)
    ui.docTree->setItemExpanded(ui.docTree->topLevelItem(i), true);
}

// ---------------------------------------------------------------------------------------------
void MainForm::counterFinished()
{
  if(!loginOk)
    return;

  QString e, t;
  e.clear();
  t.setNum(counter->timeElapsed / 1000);
  e = t;
  t.setNum(counter->timeElapsed % 1000);
  e += "." + t + "sec";
  *currentSuggestion = counter->suggestions->takeBest();

  suggestionMessage(currentSuggestion, e);
}

// ---------------------------------------------------------------------------------------------
void MainForm::messageClicked()
{
  if(currentSuggestion->occ == -1)
    return;
  // ======
  int q;
  q = QMessageBox::question(this, tr("Suggestion"),
                tr
                ("It is suggested to create a key from the expression: \"")
                + currentSuggestion->name + "\"",
                tr("Create key"), tr("Add to ignored"));

  if(q == 0)
  {
    if(dreamBase->keys->add(currentSuggestion->name) != -1)
    {
      dreamBase->keys->fillTree(ui.keysTree,
				dreamBase->list(), config.freqSort);
      dreamBase->keys->fillCurrentList(ui.currentKeyList,
				       ui.textEdit->toPlainText());
      highlightAll();
    }
  }
  else
    dreamBase->addIgnored(currentSuggestion->name);

  dreamBase->saveKeys(config.keys_coding);

  QString e, t;
  e.clear();
  t.setNum(counter->timeElapsed / 1000);
  e = t;
  t.setNum(counter->timeElapsed % 1000);
  e += "." + t + "sec";
  *currentSuggestion = counter->suggestions->takeBest();
  if(currentSuggestion->occ != -1)
    suggestionMessage(currentSuggestion, e);
}

// ---------------------------------------------------------------------------------------------
void MainForm::httpReadyRead()
{
  // mdd_version=x.x.x;
  // new_update_path=http://xxxxx.xxxx.xx;
  QString ver_tag = "mdd_version=", path_tag = "new_update_path=";
  QString httpdata = http.readAll().simplified();
  for(int i = 0; i < httpdata.size(); i++)
    if(httpdata.at(i).isSpace())
      httpdata.remove(i, 1);

  int e, b = httpdata.indexOf(ver_tag);
  if(b == -1)
    return;
  b += ver_tag.size();
  e = httpdata.indexOf(";", b);
  if(e == -1)
    return;

  QString http_ver = httpdata.mid(b, e - b);
  if(http_ver != VER)
  {
    QMessageBox::information(this, tr("Update"),
                 tr
                 ("The latest version of My Dream Diary is ")
                 + http_ver, QMessageBox::Ok);
  }

  b = httpdata.indexOf(path_tag);
  if(b == -1)
    return;
  b += path_tag.size();
  e = httpdata.indexOf(";", b);
  if(e == -1)
    return;

  QString new_path = httpdata.mid(b, e - b);
  QUrl url(new_path);
  if((config.update_url != new_path) && (url.isValid()) &&
     (url.scheme() == "http"))
  {
    config.update_url = new_path;
    config.save();
  }
}

// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// COUNTER THREAD CLASS:

CounterThread::CounterThread()
{
  suggestions = new Suggestions();
  ignored_size = 6;
  days_limit = month;
  tray = NULL;
  base = NULL;
  ignored_list = NULL;
  suggestions->sugs.clear();
  ready = false;
  custom = false;
}

CounterThread::~CounterThread()
{
  delete suggestions;
}

void
  CounterThread::setup(int word_count, int ignored, int sensitiveness,
		       Range range, QSystemTrayIcon * ti, MyDreamBase * mb)
{
  words = word_count;
  ignored_size = ignored;
  ignored_list = mb->ignoredSuggestions;
  sensit = sensitiveness;
  days_limit = range;
  tray = ti;
  base = mb;
  ready = true;
}

QStringList CounterThread::wordList()
{
  QStringList sl;
  sl.clear();
  const QList < DreamDocument > *list = base->list();
  for(int i = 0; i < list->count(); i++)
    sl +=
      list->at(i).content.split(" ", QString::SkipEmptyParts,
				Qt::CaseInsensitive);
  return sl;
}

void CounterThread::prepare2(QStringList * list)
{
  for(int i = 0; i < list->count() - 1; i++)
    for(int j = i + 1; j < list->count() - 1; j++)
      if((list->at(i).toLower() == list->at(j).toLower())
	 && (list->at(i + 1).toLower() == list->at(j + 1).toLower()))
      {
	list->replace(j + 1, "#");
	list->replace(j, "#");
      }
}

void CounterThread::prepare1(QStringList * list)
{
  for(int i = 0; i < list->count() - 1; i++)
    for(int j = i + 1; j < list->count() - 1; j++)
      if(list->at(i).toLower() == list->at(j).toLower())
      {
	list->removeAt(j);
	j--;
      }
}

QDate CounterThread::weekAgo()
{
  int jd = QDate::currentDate().toJulianDay();
  jd -= 7;
  return QDate::fromJulianDay(jd);
}

QDate CounterThread::monthAgo()
{
  int jd = QDate::currentDate().toJulianDay();
  jd -= 30;
  return QDate::fromJulianDay(jd);
}

void CounterThread::run()
{
  setPriority(QThread::LowestPriority);
  QTime time;
  timeElapsed = 0;
  time.start();
  // -----------
  if(!ready)
    return;
  if((!tray->isVisible()) || (base->count() < 2))
    return;
  suggestions->sugs.clear();
  const QList < DreamDocument > *list = base->list();
  QDate date;
  if(days_limit == week)
    date = weekAgo();
  if(days_limit == month)
    date = monthAgo();

  QStringList ml;
  QString m0, m1;

  // NOTA:
  // zrezygnowalem z optymalnej metody i rozdzielilem petle w celu
  // nadania najwyzszego priorytetu wyrazeniom dwuczlonowym oraz dla
  // mozliwosci
  // konfiguracji ktory rodzaj skanowania przeprowadzic

  // skanowanie wyrazen zlozonych:
  if((words == 0) || (words == 2))
  {
    ml = wordList();
    prepare2(&ml);
    for(int i = 0; i < list->count(); i++)
      if((days_limit != all) && (list->at(i).date < date))
	continue;
      else
	for(int m = 0; m < ml.count() - 1; m++)
	{
	  if(ml.at(m) == "#" || ml.at(m + 1) == "#")
	    continue;
	  m0 = ml.at(m).toLower();
	  m1 = ml.at(m + 1).toLower();
	  // wielokrotne warunki dla czytelnosci
	  if((list->at(i).content.contains(m0 + " " + m1,
					   Qt::CaseInsensitive)))
	    if((m0.size() + m1.size() >= ignored_size))
	      if((!ignored_list->contains(m0, Qt::CaseInsensitive))
		 && (!ignored_list->contains(m1, Qt::CaseInsensitive))
		 && (!ignored_list->contains(m0 + " " + m1,
					     Qt::CaseInsensitive)))
		if(!base->keys->contains(m0 + " " + m1))
		  suggestions->add(m0 + " " + m1);
	}
  }
  suggestions->removeSeldom(sensit);
  timeElapsed = time.elapsed();
  if(suggestions->sugs.count() > 0)
    return;
  // skanowanie wyrazen pojedynczych:
  if((words == 0) || (words == 1))
  {
    ml = wordList();
    prepare1(&ml);
    for(int i = 0; i < list->count(); i++)
      if((days_limit != all) && (list->at(i).date < date))
	continue;
      else
	for(int m = 0; m < ml.count() - 1; m++)
	{
	  m0 = ml.at(m).toLower();
	  // wielokrotne warunki dla czytelnosci
	  if((list->at(i).content.contains(m0, Qt::CaseInsensitive)))
	    if((m0.size() >= ignored_size))
	      if(!ignored_list->contains(m0, Qt::CaseInsensitive))
		if(!base->keys->contains(m0))
		  suggestions->add(m0);
	}
  }
  suggestions->removeSeldom(sensit);
  timeElapsed = time.elapsed();
}
