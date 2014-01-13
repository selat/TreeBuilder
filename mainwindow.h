#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class GLWidget;
class PropertiesWidget;
class VisualEditTrunkProcessBending;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

private slots:
    void showTrunkAngleEdit();
    void showBranchAngleEdit();

    void newFile();
    void openFile();
    void saveFile();
    void saveAsFile();
    void exportFile();

    void showAbout();

    void changeScreenState(bool state);

    void generate();

protected:
    void keyPressEvent(QKeyEvent *keyEvent);

private:
    void setProperties();
    void createActions();
    void createMenus();
    void createStatusBar();
    void createConnections();

    QString currentFileName;
    bool currentFileSaved;

    GLWidget *glWidget;
    PropertiesWidget *propertiesWidget;

    QMenu *fileMenu;
    QAction *newFileAction;
    QAction *openFileAction;
    QAction *saveFileAction;
    QAction *saveAsFileAction;
    QAction *exportFileAction;
    QAction *exitAction;

    QMenu *viewMenu;
    QAction *fullScreenAction;
    QAction *wireframeAction;
    QAction *showTrunkAction;
    QAction *showBranchesAction;
    QAction *showLeavesAction;
    QAction *backgroundColorAction;

    QMenu *generatorMenu;
    QAction *generateAction;
    QAction *clearAction;

    QMenu *windowMenu;
    QAction *showTreePropertiesAction;

    QMenu *helpMenu;
    QAction *aboutAction;
    QAction *aboutQtAction;

    QDockWidget *propertiesDockWidget;

//    VisualEditTrunkProcessBending *visualEditTrunkProcessBendingZ;
};

#endif // MAINWINDOW_H
