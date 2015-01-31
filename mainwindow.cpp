#include "mainwindow.h"

K::tool MainWindow::_selectedTool = K::MOUSE;
extern bool Circuit::showGrid;

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
  loadSettings();
  initActions();
  initMenus();
  initObjectSettingsWidget();
  initToolBars();
  initMainWidget();
  initDialogs();

  //After everything open new file
  newFile();
}

void MainWindow::initMainWidget()
{
  mainWidget = new MainWidget();
  this->setCentralWidget(mainWidget);
}

void MainWindow::initActions()
{
  //newFileAction
  newFileAction = new QAction(QIcon::fromTheme("document-new"), tr("&New..."), nullptr);
  newFileAction->setShortcut(QKeySequence::New);
  connect(newFileAction, SIGNAL(triggered()), this, SLOT(newFile()));

  //openFileAction
  openFileAction = new QAction(QIcon::fromTheme("document-open"), tr("&Open"), nullptr);
  openFileAction->setShortcut(QKeySequence::Open);
  //moved to dialogs

  //saveFileAction
  saveFileAction = new QAction(QIcon::fromTheme("document-save"), tr("&Save"), nullptr);
  saveFileAction->setShortcut(QKeySequence::Save);

  //saveFileAsAction
  saveFileAsAction = new QAction(QIcon::fromTheme("document-save-as"), tr("Save as"), nullptr);
  saveFileAsAction->setShortcut(QKeySequence::SaveAs);
  /// TO MOVE TO DIALOGS connect(saveFileAsAction, SIGNAL(triggered()), this, SLOT(saveFileAs()));

  //quitAction
  quitAction = new QAction(QIcon::fromTheme("application-exit"), tr("Quit"), nullptr);
  quitAction->setShortcut(QKeySequence::Quit);
  connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));

  //mouseSelectAction
  mouseSelectAction = new QAction(QIcon::fromTheme("input-mouse"), tr("Mouse"), nullptr);
  mouseSelectAction->setCheckable(true);
  mouseSelectAction->setChecked(true);
  ///mouseSelectAction->setShortcut(QKeySequence::Quit);
  connect(mouseSelectAction, SIGNAL(triggered()), this, SLOT(mouseSelect()));

  //mouseSelectAction
  wireSelectAction = new QAction(QIcon::fromTheme("input-keyboard"), tr("Wire"), nullptr);
  wireSelectAction->setCheckable(true);
  ///wireSelectAction->setShortcut(QKeySequence::Quit);
  connect(wireSelectAction, SIGNAL(triggered()), this, SLOT(wireSelect()));

  //aboutAction
  aboutApplicationAction = new QAction(QIcon::fromTheme("help-about"), tr("About"), nullptr);
  connect(aboutApplicationAction, SIGNAL(triggered()), this, SLOT(aboutApplication()));

  //showGridAction
  showGridAction = new QAction(tr("Show grid"), nullptr);
  showGridAction->setCheckable(true);
  QSettings set;
  Circuit::showGrid = set.value("showgrid", true).toBool();
  showGridAction->setChecked(Circuit::showGrid);
  connect(showGridAction, SIGNAL(toggled(bool)), this, SLOT(showGrid(bool)));

  //zoomInAction
  zoomInAction = new QAction(QIcon::fromTheme("zoom-in"), tr("Zoom in"), nullptr);
  zoomInAction2 = new QAction(QIcon::fromTheme("zoom-in"), tr("Zoom in"), nullptr);
  zoomInAction3 = new QAction(QIcon::fromTheme("zoom-in"), tr("Zoom in"), nullptr);
  zoomInAction->setShortcut(QKeySequence::ZoomIn);
  zoomInAction2->setShortcut(QKeySequence(QString("ctrl+=")));
  zoomInAction3->setShortcut(QKeySequence(Qt::Key_1));
  connect(zoomInAction, SIGNAL(triggered()), this, SLOT(zoomIn()));
  connect(zoomInAction2, SIGNAL(triggered()), this, SLOT(zoomIn()));
  connect(zoomInAction3, SIGNAL(triggered()), this, SLOT(zoomIn()));
  this->addAction(zoomInAction2);
  this->addAction(zoomInAction3);

  //zoomOutAction
  zoomOutAction = new QAction(QIcon::fromTheme("zoom-out"), tr("Zoom out"), nullptr);
  zoomOutAction->setShortcut(QKeySequence::ZoomOut);
  connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(zoomOut()));
}

void MainWindow::initMenus()
{
  //fileMenu
  fileMenu = this->menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(newFileAction);
  fileMenu->addAction(openFileAction);
  fileMenu->addAction(saveFileAction);
  fileMenu->addAction(saveFileAsAction);
  fileMenu->addSeparator();
  fileMenu->addAction(quitAction);

  //editMenu

  //viewMenu
  viewMenu = this->menuBar()->addMenu(tr("&View"));
  viewMenu->addAction(zoomInAction);
  viewMenu->addAction(zoomOutAction);
  viewMenu->addSeparator();
  viewMenu->addAction(showGridAction);

  //helpMenu
  helpMenu = this->menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(aboutApplicationAction);
}

void MainWindow::loadSettings()
{
  QSettings settings;
  settings.beginGroup("MainWindow");
  this->resize(settings.value("size", QSize(800, 600)).toSize());
  this->move(settings.value("pos", QPoint(200, 200)).toPoint());
  settings.endGroup();
}

void MainWindow::saveSettings()
{
  QSettings settings;
  settings.beginGroup("MainWindow");
  settings.setValue("size", this->size());
  settings.setValue("pos", this->pos());
  settings.endGroup();
}

void MainWindow::initToolBars()
{
  applicationBar = new QToolBar(tr("application"));
  applicationBar->addAction(newFileAction);
  applicationBar->addAction(openFileAction);
  applicationBar->addAction(saveFileAction);
  applicationBar->addAction(saveFileAsAction);
  applicationBar->addSeparator();
  applicationBar->addAction(zoomInAction);
  applicationBar->addAction(zoomOutAction);
  this->addToolBar(Qt::TopToolBarArea, applicationBar);

  toolBar = new QToolBar(tr("items")); ///TODO nazwa :<
  toolBar->addAction(mouseSelectAction);
  toolBar->addAction(wireSelectAction);
  this->addToolBar(Qt::LeftToolBarArea, toolBar);

  objectSettingsBar = new QToolBar(tr("Object settings"));
  objectSettingsBar->addWidget(objectSettingsWidget);
  this->addToolBar(Qt::RightToolBarArea, objectSettingsBar);
}

void MainWindow::initObjectSettingsWidget()
{
  objectSettingsWidget = new ObjectSettingsWidget();
}

void MainWindow::unselectLastUsed()
{
  switch(_selectedTool)
    {
      case K::MOUSE: mouseSelectAction->setChecked(false);
      break;

      case K::WIRE: wireSelectAction->setChecked(false);
      break;

    }
}

void MainWindow::initDialogs()
{
  // file dialogs
  openFileDialog = new QFileDialog(this);
  saveFileDialog = new QFileDialog(this);
  saveFileAsDialog = new QFileDialog(this);

  QStringList fileList;
  fileList << "QtCircuit (*.qtc)" << tr("All filest (*.*)");

  openFileDialog->setNameFilters(fileList);
  openFileDialog->setDefaultSuffix(QString(".qtc"));
  openFileDialog->setAcceptMode(QFileDialog::AcceptOpen);
  connect(openFileAction, SIGNAL(triggered()), openFileDialog, SLOT(exec()));
  connect(openFileDialog, SIGNAL(fileSelected(QString)), this, SLOT(openFile(QString)));

  saveFileDialog->setNameFilters(fileList);
  saveFileDialog->setDefaultSuffix(QString(".qtc"));
  saveFileDialog->setAcceptMode(QFileDialog::AcceptSave);
  connect(saveFileAction, SIGNAL(triggered()), this, SLOT(saveFileTest()));
  connect(saveFileDialog, SIGNAL(fileSelected(QString)), this, SLOT(saveFile(QString)));

  saveFileAsDialog->setNameFilters(fileList);
  saveFileAsDialog->setDefaultSuffix(QString(".qtc"));
  saveFileAsDialog->setAcceptMode(QFileDialog::AcceptSave);
  connect(saveFileAsAction, SIGNAL(triggered()), saveFileAsDialog, SLOT(exec()));
  connect(saveFileAsDialog, SIGNAL(fileSelected(QString)), this, SLOT(saveFileAs(QString)));

  // about dialog
  aboutDialog = new AboutDialog(nullptr);
}

void MainWindow::newFile()
{
  this->mainWidget->newTab(new Circuit(tr("new")));
}

void MainWindow::saveFile(QString file)
{

}

void MainWindow::saveFileTest()
{
  ///TODO jeżeli już zapisany to zapisać i nie pytać~
  saveFileDialog->exec();
}

void MainWindow::saveFileAs(QString file)
{

}

void MainWindow::openFile(QString file)
{
  this->mainWidget->newTab(new Circuit(file));
}



void MainWindow::quit()
{
  saveSettings();
  QApplication::quit();
}


void MainWindow::wireSelect()
{
  unselectLastUsed();
  wireSelectAction->setChecked(true);
  _selectedTool = K::WIRE;
  mainWidget->setMouseTrackingOnTabs(true);
}

void MainWindow::mouseSelect()
{
  unselectLastUsed();
  mouseSelectAction->setChecked(true);
  _selectedTool = K::MOUSE;
  mainWidget->setMouseTrackingOnTabs(false);
}

void MainWindow::aboutApplication()
{
  aboutDialog->show();
}

void MainWindow::showGrid(bool b)
{
  QSettings set;
  set.setValue("showgrid", b);
  Circuit::showGrid = b;
  mainWidget->updateCurrent();
}

void MainWindow::zoomIn()
{
  this->mainWidget->getCurrent()->circuitWidget->scaleUp();
}

void MainWindow::zoomOut()
{
  this->mainWidget->getCurrent()->circuitWidget->scaleDown();
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    event->ignore();
    this->quit();
}


