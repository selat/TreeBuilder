#include "mainwindow.h"
#include "glwidget.h"
#include "PropertiesWidget.h"

#include <QtWidgets>

#include <fstream>
#include <cstdio>
#include <ctime>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    currentFileName = "untitled.sav";
    currentFileSaved = false;

    srand(time(NULL));
    glWidget = new GLWidget(this);
    glWidget -> setMinimumSize(640, 480);
    setCentralWidget(glWidget);
    glWidget -> generate();

    propertiesDockWidget = new QDockWidget(tr("Tree Properties"), this);
    propertiesDockWidget -> setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    propertiesWidget = new PropertiesWidget(propertiesDockWidget);
    propertiesDockWidget -> setWidget(propertiesWidget);
    propertiesDockWidget -> setFloating(true);
    propertiesDockWidget -> setVisible(false);
    propertiesDockWidget -> setMinimumWidth(310);
    propertiesDockWidget -> setMinimumHeight(550);
    propertiesDockWidget -> setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    addDockWidget(Qt::RightDockWidgetArea, propertiesDockWidget);

    setProperties();
    createActions();
    createMenus();
    createStatusBar();
    createConnections();

    showFullScreen();

    propertiesDockWidget -> move(width() - 320, 50);
}

void MainWindow::showTrunkAngleEdit()
{
    propertiesWidget -> visualEditTrunkProcessBendingZ -> show();
}

void MainWindow::showBranchAngleEdit()
{
    propertiesWidget -> visualEditBranchProcessBendingZ -> show();
}

void MainWindow::setProperties()
{
    // Trunk properties
    propertiesWidget -> trunkRadius -> setValue(glWidget -> getTrunkRadius());
    propertiesWidget -> trunkRadiusChangeCof -> setValue(glWidget -> getTrunkRadiusChangeCof());
    propertiesWidget -> trunkProcessRadiusChangeCof -> setValue(glWidget -> getTrunkProcessRadiusChangeCof());
    propertiesWidget -> trunkMaxProcessesFromSegment -> setValue(glWidget -> getTrunkMaxProcessesFromSegment());
    propertiesWidget -> trunkBending -> setValue(glWidget -> getTrunkBending());
    propertiesWidget -> trunkProcessChance -> setValue(glWidget -> getTrunkProcessChance());
    propertiesWidget -> trunkProcessBendingY -> setValue(glWidget -> getTrunkProcessBendingY());
//    propertiesWidget -> trunkProcessBendingZ -> setValue(glWidget -> getTrunkProcessBendingZ());
    propertiesWidget -> minRForBranch -> setValue(glWidget -> getMinRForBranch());
    propertiesWidget -> maxRForBranch -> setValue(glWidget -> getMaxRForBranch());
    propertiesWidget -> trunkSegmentLength -> setValue(glWidget -> getTrunkSegmentLength());
    propertiesWidget -> trunkSegmentLengthChange -> setValue(glWidget -> getTrunkSegmentLengthChange());
    propertiesWidget -> trunkGravitation -> setValue(glWidget -> getTrunkGravitation());
    propertiesWidget -> trunkSegmentsNum -> setValue(glWidget -> getTrunkSegmentsNum());
    propertiesWidget -> trunkTexture -> setText(QString::fromStdString(glWidget -> getTrunkTexture()));

    // Branches properties
    propertiesWidget -> branchRChangeCof -> setValue(glWidget -> getBranchRChangeCof());
    propertiesWidget -> branchProcessRChangeCof -> setValue(glWidget -> getBranchProcessRChangeCof());
    propertiesWidget -> branchProcessChance -> setValue(glWidget -> getBranchProcessChance());
    propertiesWidget -> branchMaxProcessesFromSegment -> setValue(glWidget -> getBranchMaxProcessesFromSegment());
    propertiesWidget -> branchBending -> setValue(glWidget -> getBranchBending());
    propertiesWidget -> branchProcessBendingY -> setValue(glWidget -> getBranchProcessBendingY());
//    propertiesWidget -> branchProcessBendingZ -> setValue(glWidget -> getBranchProcessBendingZ());
    propertiesWidget -> branchSegmentLength -> setValue(glWidget -> getBranchSegmentLength());
    propertiesWidget -> branchSegmentLengthChange -> setValue(glWidget -> getBranchSegmentLengthChange());
    propertiesWidget -> branchGravitation -> setValue(glWidget -> getBranchGravitation());
    propertiesWidget -> branchSegmentsNum -> setValue(glWidget -> getBranchSegmentsNum());
    propertiesWidget -> bbSize -> setValue(glWidget -> getBBSize());
    propertiesWidget -> maxOverlapsCount -> setValue(glWidget -> getMaxOverlapsCount());
    propertiesWidget -> branchesTexture -> setText(QString::fromStdString(glWidget -> getBranchesTexture()));

    // Leaves properties
    propertiesWidget -> maxRadiusForLeaf -> setValue(glWidget -> getMaxRadiusForLeaf());
    propertiesWidget -> leavesSpreading -> setValue(glWidget -> getLeavesSpreading());
    propertiesWidget -> leafChance -> setValue(glWidget -> getLeafChance());
    propertiesWidget -> leafSize -> setValue(glWidget -> getLeafSize());
}

void MainWindow::createActions()
{
    // File menu
    newFileAction = new QAction(tr("New"), this);
    newFileAction -> setShortcuts(QKeySequence::New);
    newFileAction -> setStatusTip(tr("Create new configuration"));
    connect(newFileAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openFileAction = new QAction(tr("Open"), this);
    openFileAction -> setShortcuts(QKeySequence::Open);
    openFileAction -> setStatusTip(tr("Open saved configuration"));
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(openFile()));

    saveFileAction = new QAction(tr("Save"), this);
    saveFileAction -> setShortcut(QKeySequence::Save);
    saveFileAction -> setStatusTip(tr("Save current configuration"));
    connect(saveFileAction, SIGNAL(triggered()), this, SLOT(saveFile()));

    saveAsFileAction = new QAction(tr("Save As"), this);
    saveAsFileAction -> setShortcut(tr("Ctrl+Shift+S"));
    saveAsFileAction -> setStatusTip(tr("Save current configuration"));
    connect(saveAsFileAction, SIGNAL(triggered()), this, SLOT(saveAsFile()));

    exportFileAction = new QAction(tr("Export"), this);
    exportFileAction -> setShortcut(tr("Ctrl+E"));
    exportFileAction -> setStatusTip(tr("Export to other format"));
    connect(exportFileAction, SIGNAL(triggered()), this, SLOT(exportFile()));

    exitAction = new QAction(tr("Exit"), this);
    exitAction -> setShortcut(QKeySequence::Quit);
    exitAction -> setStatusTip(tr("Quit"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    // View menu
    fullScreenAction = new QAction(tr("Full Screen"), this);
    fullScreenAction -> setShortcut(QKeySequence::FullScreen);
    fullScreenAction -> setStatusTip(tr("Switch to the full screen"));
    fullScreenAction -> setCheckable(true);
    fullScreenAction -> setChecked(true);
    connect(fullScreenAction, SIGNAL(toggled(bool)), this, SLOT(changeScreenState(bool)));

    wireframeAction = new QAction(tr("Wireframe"), this);
    wireframeAction -> setShortcut(tr("Ctrl+W"));
    wireframeAction -> setStatusTip(tr("Show wireframe"));
    wireframeAction -> setCheckable(true);
    connect(wireframeAction, SIGNAL(toggled(bool)), glWidget, SLOT(setWireframeView(bool)));

    showTrunkAction = new QAction(tr("Show Trunk"), this);
    showTrunkAction -> setShortcut(tr("Ctrl+T"));
    showTrunkAction -> setStatusTip(tr("Show trunk geometry"));
    showTrunkAction -> setCheckable(true);
    showTrunkAction -> setChecked(true);
    connect(showTrunkAction, SIGNAL(toggled(bool)), glWidget, SLOT(setShowTrunk(bool)));

    showBranchesAction = new QAction(tr("Show Branches"), this);
    showBranchesAction -> setShortcut(tr("Ctrl+B"));
    showBranchesAction -> setStatusTip(tr("Show branches geometry"));
    showBranchesAction -> setCheckable(true);
    showBranchesAction -> setChecked(true);
    connect(showBranchesAction, SIGNAL(toggled(bool)), glWidget, SLOT(setShowBranches(bool)));

    showLeavesAction = new QAction(tr("Show Leaves"), this);
    showLeavesAction -> setShortcut(tr("Ctrl+L"));
    showLeavesAction -> setStatusTip(tr("Show leaves geometry"));
    showLeavesAction -> setCheckable(true);
    showLeavesAction -> setChecked(true);
    connect(showLeavesAction, SIGNAL(toggled(bool)), glWidget, SLOT(setShowLeaves(bool)));

    backgroundColorAction = new QAction(tr("Set background color"), this);
    backgroundColorAction -> setStatusTip(tr("Set background color"));
    connect(backgroundColorAction, SIGNAL(triggered()), glWidget, SLOT(setBackgroundColor()));

    // Generator menu
    generateAction = new QAction(tr("Generate"), this);
    generateAction -> setShortcut(QKeySequence::Refresh);
    generateAction -> setStatusTip(tr("Run tree generator"));
    connect(generateAction, SIGNAL(triggered()), this, SLOT(generate()));

    // Window menu
    showTreePropertiesAction = new QAction(tr("Tree Properties"), this);
    showTreePropertiesAction -> setShortcut(tr("Ctrl+P"));
    showTreePropertiesAction -> setStatusTip(tr("Show tree properties"));
    showTreePropertiesAction -> setCheckable(true);
    connect(showTreePropertiesAction, SIGNAL(toggled(bool)), propertiesDockWidget, SLOT(setVisible(bool)));

    // Help menu
    aboutAction = new QAction(tr("About"), this);
    aboutAction -> setStatusTip(tr("About this program"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));

    aboutQtAction = new QAction(tr("About Qt"), this);
    aboutQtAction -> setStatusTip(tr("About Qt"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar() -> addMenu(tr("&File"));
    fileMenu -> addAction(newFileAction);
    fileMenu -> addAction(openFileAction);
    fileMenu -> addAction(saveFileAction);
    fileMenu -> addAction(saveAsFileAction);
    fileMenu -> addSeparator();
    fileMenu -> addAction(exportFileAction);
    fileMenu -> addSeparator();
    fileMenu -> addAction(exitAction);

    viewMenu = menuBar() -> addMenu(tr("&View"));
    viewMenu -> addAction(fullScreenAction);
    viewMenu -> addAction(wireframeAction);
    viewMenu -> addSeparator();
    viewMenu -> addAction(showTrunkAction);
    viewMenu -> addAction(showBranchesAction);
    viewMenu -> addAction(showLeavesAction);
    viewMenu -> addSeparator();
    viewMenu -> addAction(backgroundColorAction);

    generatorMenu = menuBar() -> addMenu(tr("&Generator"));
    generatorMenu -> addAction(generateAction);

    windowMenu = menuBar() -> addMenu(tr("&Window"));
    windowMenu -> addAction(showTreePropertiesAction);

    helpMenu = menuBar() -> addMenu(tr("&Help"));
    helpMenu -> addAction(aboutAction);
    helpMenu -> addAction(aboutQtAction);
}

void MainWindow::createStatusBar()
{
    statusBar() -> showMessage(tr("Ready"));
}

void MainWindow::createConnections()
{
    connect(propertiesWidget -> trunkTexture, &QLineEdit::textChanged, glWidget, &GLWidget::loadTrunkTexture);
    connect(propertiesWidget -> branchesTexture, &QLineEdit::textChanged, glWidget, &GLWidget::loadBranchesTexture);
    connect(propertiesWidget -> trunkProcessBendingEditButton, &QPushButton::clicked, this, &MainWindow::showTrunkAngleEdit);
    connect(propertiesWidget -> branchProcessBendingEditButton, &QPushButton::clicked, this, &MainWindow::showBranchAngleEdit);
}

void MainWindow::newFile()
{
    // Trunk properties
    propertiesWidget -> trunkRadius -> setValue(1.0);
    propertiesWidget -> trunkRadiusChangeCof -> setValue(0.9);
    propertiesWidget -> trunkProcessRadiusChangeCof -> setValue(0.8);
    propertiesWidget -> trunkMaxProcessesFromSegment -> setValue(1);
    propertiesWidget -> trunkBending -> setValue(1.0);
    propertiesWidget -> trunkProcessChance -> setValue(0.6);
    propertiesWidget -> trunkProcessBendingY -> setValue(2.0);
//    propertiesWidget -> trunkProcessBendingZ -> setValue(2.0);
    propertiesWidget -> visualEditTrunkProcessBendingZ -> setMinAngle(M_PI_2 - 0.5);
    propertiesWidget -> visualEditTrunkProcessBendingZ -> setMaxAngle(M_PI_2 - 0.3);
    propertiesWidget -> minRForBranch -> setValue(0.0);
    propertiesWidget -> maxRForBranch -> setValue(1000.0);
    propertiesWidget -> trunkSegmentLength -> setValue(1.0);
    propertiesWidget -> trunkSegmentLength -> setValue(1.0);
    propertiesWidget -> trunkGravitation -> setValue(0.0);
    propertiesWidget -> trunkSegmentsNum -> setValue(8);
    propertiesWidget -> trunkTexture -> setText("trunk.png");

    // Branches properties
    propertiesWidget -> branchRChangeCof -> setValue(0.9);
    propertiesWidget -> branchProcessRChangeCof -> setValue(0.8);
    propertiesWidget -> branchProcessChance -> setValue(0.6);
    propertiesWidget -> branchMaxProcessesFromSegment -> setValue(1);
    propertiesWidget -> branchBending -> setValue(1.0);
    propertiesWidget -> branchProcessBendingY -> setValue(2.0);
//    propertiesWidget -> branchProcessBendingZ -> setValue(2.0);
    propertiesWidget -> visualEditBranchProcessBendingZ -> setMinAngle(M_PI_2 - 0.5);
    propertiesWidget -> visualEditBranchProcessBendingZ -> setMaxAngle(M_PI_2 - 0.3);
    propertiesWidget -> branchSegmentLength -> setValue(1.0);
    propertiesWidget -> branchSegmentLengthChange -> setValue(1.0);
    propertiesWidget -> branchGravitation -> setValue(0.0);
    propertiesWidget -> branchSegmentsNum -> setValue(8);
    propertiesWidget -> bbSize -> setValue(1.0);
    propertiesWidget -> maxOverlapsCount -> setValue(1);
    propertiesWidget -> branchesTexture -> setText("trunk.png");

    // Leaves properties
    propertiesWidget -> maxRadiusForLeaf -> setValue(0.1);
    propertiesWidget -> leavesSpreading -> setValue(0.5);
    propertiesWidget -> leafChance -> setValue(0.5);
    propertiesWidget -> leafSize -> setValue(0.5);
    propertiesWidget -> clearLeavesTypes();

    currentFileSaved = false;
    generate();
}

void MainWindow::openFile()
{
    currentFileName = QFileDialog::getOpenFileName(this, tr("Open file"),
                                                    QApplication::applicationDirPath(), tr("Save (*.sav)"));
    propertiesWidget -> loadOptions(currentFileName);
    currentFileSaved = true;
	generate();
}

void MainWindow::saveFile()
{
    if(!currentFileSaved)
    {
        currentFileName = QFileDialog::getSaveFileName(this, tr("Save file"),
                                                        QApplication::applicationDirPath(), tr("Save (*.sav)"));
        currentFileSaved = true;
    }
    propertiesWidget -> saveOptions(currentFileName);
}

void MainWindow::saveAsFile()
{
    currentFileName = QFileDialog::getSaveFileName(this, tr("Save file"),
                                QApplication::applicationDirPath(), tr("Save (*.sav)"));
    propertiesWidget -> saveOptions(currentFileName);
    currentFileSaved = true;
}

void MainWindow::exportFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export file"),
                                                    QApplication::applicationDirPath(), tr("Wavefront (*.obj)"));
    fileName.remove(fileName.size() - 4, 4);
    glWidget -> write(fileName.toUtf8());
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, tr("About Tree Builder"),
                       tr("This program allows you to create realistic models of trees."));
}

void MainWindow::changeScreenState(bool state)
{
    if(state)
    {
        showFullScreen();
    } else {
        showNormal();
    }
}

void MainWindow::generate()
{
    // Loading trunk properties
    glWidget -> setTrunkRadius(propertiesWidget -> trunkRadius -> value());
    glWidget -> setTrunkRadiusChangeCof(propertiesWidget -> trunkRadiusChangeCof -> value());
    glWidget -> setTrunkProcessRadiusChangeCof(propertiesWidget -> trunkProcessRadiusChangeCof -> value());
    glWidget -> setTrunkMaxProcessesFromSegment(propertiesWidget -> trunkMaxProcessesFromSegment -> value());
    glWidget -> setTrunkBending(propertiesWidget -> trunkBending -> value());
    glWidget -> setTrunkProcessChance(propertiesWidget -> trunkProcessChance -> value());
    glWidget -> setTrunkProcessBendingY(propertiesWidget -> trunkProcessBendingY -> value());
//    glWidget -> setTrunkProcessBendingZ(propertiesWidget -> trunkProcessBendingZ -> value());
    double minAngle = propertiesWidget -> visualEditTrunkProcessBendingZ -> getMinAngle() + M_PI_2;
    minAngle = (minAngle >= M_PI * 2.0) ? (minAngle - M_PI * 2.0) : minAngle;
    double maxAngle = propertiesWidget -> visualEditTrunkProcessBendingZ -> getMaxAngle() + M_PI_2;

    glWidget -> setTrunkProcessBendingZ(minAngle, maxAngle);
    glWidget -> setMinRForBranch(propertiesWidget -> minRForBranch -> value());
    glWidget -> setMaxRForBranch(propertiesWidget -> maxRForBranch -> value());
    glWidget -> setTrunkSegmentLength(propertiesWidget -> trunkSegmentLength -> value());
    glWidget -> setTrunkSegmentLengthChange(propertiesWidget -> trunkSegmentLengthChange -> value());
    glWidget -> setTrunkGravitation(propertiesWidget -> trunkGravitation -> value());
    glWidget -> setTrunkSegmentsNum(propertiesWidget -> trunkSegmentsNum -> value());

    // Loading branches properties
    glWidget -> setBranchRChangeCof(propertiesWidget -> branchRChangeCof -> value());
    glWidget -> setBranchProcessRChangeCof(propertiesWidget -> branchProcessRChangeCof -> value());
    glWidget -> setBranchProcessChance(propertiesWidget -> branchProcessChance -> value());
    glWidget -> setBranchMaxProcessesFromSegment(propertiesWidget -> branchMaxProcessesFromSegment -> value());
    glWidget -> setBranchBending(propertiesWidget -> branchBending -> value());
    glWidget -> setBranchProcessBendingY(propertiesWidget -> branchProcessBendingY -> value());

    minAngle = propertiesWidget -> visualEditBranchProcessBendingZ -> getMinAngle() + M_PI_2;
    minAngle = (minAngle >= M_PI * 2.0) ? (minAngle - M_PI * 2.0) : minAngle;
    maxAngle = propertiesWidget -> visualEditBranchProcessBendingZ -> getMaxAngle() + M_PI_2;

    glWidget -> setBranchProcessBendingZ(minAngle, maxAngle);
//    glWidget -> setBranchProcessBendingZ(propertiesWidget -> branchProcessBendingZ -> value());1
    glWidget -> setBranchSegmentLength(propertiesWidget -> branchSegmentLength -> value());
    glWidget -> setBranchSegmentLengthChange(propertiesWidget -> branchSegmentLengthChange -> value());
    glWidget -> setBranchGravitation(propertiesWidget -> branchGravitation -> value());
    glWidget -> setBranchSegmentsNum(propertiesWidget -> branchSegmentsNum -> value());
    glWidget -> setBBSize(propertiesWidget -> bbSize -> value());
    glWidget -> setMaxOverlapsCount(propertiesWidget -> maxOverlapsCount -> value());

    // Loading leaves properties
    glWidget -> setMaxRadiusForLeaf(propertiesWidget -> maxRadiusForLeaf -> value());
    glWidget -> setLeavesSpreading(propertiesWidget -> leavesSpreading -> value());
    glWidget -> setLeafChance(propertiesWidget -> leafChance -> value());
    glWidget -> setLeafSize(propertiesWidget -> leafSize -> value());

    glWidget -> clearLeavesTypes();

    for(int i = 0; i < propertiesWidget -> getLeavesTypesCount(); ++i)
    {
        glWidget -> addLeafType(propertiesWidget -> getConvertedLeafType(i));
    }

    glWidget -> loadTextures();
    glWidget -> generate();
}

void MainWindow::keyPressEvent(QKeyEvent *keyEvent)
{
}
