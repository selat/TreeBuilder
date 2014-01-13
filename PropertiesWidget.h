#ifndef PROPERTIESWIDGET_H
#define PROPERTIESWIDGET_H

#include <QtWidgets>

#include "newleaftypewidget.h"
#include "leaftype.h"
#include "TreeLibLeafProperties.h"
#include "visualeditprocessbending.h"

class QPushButton;

class PropertiesWidget : public QWidget
{
    Q_OBJECT

public:
    PropertiesWidget(QWidget *parent = 0);

    void saveOptions(QString fileName);
    void loadOptions(QString fileName);

    void clearLeavesTypes();

    int getLeavesTypesCount();
    TreeLibLeafProperties getConvertedLeafType(int current);

    // Trunk
    QDoubleSpinBox *trunkRadius;
    QDoubleSpinBox *trunkRadiusChangeCof;
    QDoubleSpinBox *trunkProcessRadiusChangeCof;
    QSpinBox *trunkMaxProcessesFromSegment;
    QDoubleSpinBox *trunkBending;
    QPushButton *trunkProcessBendingEditButton;
    QDoubleSpinBox *trunkProcessChance;
    QDoubleSpinBox *trunkProcessBendingY;
    QDoubleSpinBox *trunkProcessBendingZ;
    QDoubleSpinBox *minRForBranch;
    QDoubleSpinBox *maxRForBranch;
    QDoubleSpinBox *trunkSegmentLength;
    QDoubleSpinBox *trunkSegmentLengthChange;
    QDoubleSpinBox *trunkGravitation;
    QSpinBox *trunkSegmentsNum;
    QLineEdit *trunkTexture;
    QPushButton *loadTrunkTextureButton;

    // Branches
    QDoubleSpinBox *branchRChangeCof;
    QDoubleSpinBox *branchProcessRChangeCof;
    QDoubleSpinBox *branchProcessChance;
    QSpinBox* branchMaxProcessesFromSegment;
    QDoubleSpinBox *branchBending;
    QDoubleSpinBox *branchProcessBendingY;
    QDoubleSpinBox *branchProcessBendingZ;
    QPushButton *branchProcessBendingEditButton;
    QDoubleSpinBox *branchSegmentLength;
    QDoubleSpinBox *branchSegmentLengthChange;
    QDoubleSpinBox *branchGravitation;
    QSpinBox *branchSegmentsNum;
    QDoubleSpinBox *bbSize;
    QSpinBox *maxOverlapsCount;
    QLineEdit *branchesTexture;
    QPushButton *loadBranchesTextureButton;

    // Leaves
    QDoubleSpinBox *maxRadiusForLeaf;
    QDoubleSpinBox *leavesSpreading;
    QDoubleSpinBox *leafChance;
    QDoubleSpinBox *leafSize;
    QListWidget *leavesTypesWidget;
    QPushButton *addNewLeafTypeButton;
    QPushButton *editLeafTypeButton;
    QPushButton *removeLeafTypeButton;

    VisualEditProcessBending *visualEditTrunkProcessBendingZ;
    VisualEditProcessBending *visualEditBranchProcessBendingZ;

public slots:
    void loadTrunkTexture();
    void loadBranchesTexture();
    void addNewLeafType();
    void editLeafType();
    void removeLeafType();

private:
    void saveLeafType();

    void createWidgets();

    void createTrunkTab();
    void createBranchesTab();
    void createLeavesTab();

    void createActions();

    enum {ADD_LEAF_TYPE, EDIT_LEAF_TYPE} currentAction;

    QScrollArea *scrollArea;
    QTabWidget *tabs;

    QWidget *trunkTab;
    QWidget *branchesTab;
    QWidget *leavesTab;

    NewLeafTypeWidget *addNewLeafTypeWidget;

    QVector <LeafType> leavesTypes;
};

#endif // PROPERTIESWIDGET_H
