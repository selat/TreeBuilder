#include "PropertiesWidget.h"

PropertiesWidget::PropertiesWidget(QWidget *parent) : QWidget(parent)
{
    visualEditTrunkProcessBendingZ = new VisualEditProcessBending(this);
    visualEditTrunkProcessBendingZ -> setVisible(false);

    visualEditBranchProcessBendingZ = new VisualEditProcessBending(this);
    visualEditBranchProcessBendingZ -> setVisible(false);

    createWidgets();
    createActions();
}

void PropertiesWidget::saveOptions(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream out(&file);

    // Trunk
    out << trunkRadius -> value() << " ";
    out << trunkRadiusChangeCof -> value() << " ";
    out << trunkProcessRadiusChangeCof -> value() << " ";
    out << trunkMaxProcessesFromSegment -> value() << " ";
    out << trunkBending -> value() << " ";
    out << trunkProcessChance -> value() << " ";
    out << trunkProcessBendingY -> value() << " ";
//    out << trunkProcessBendingZ -> value() << " ";
    out << visualEditTrunkProcessBendingZ -> getMinAngle() << " ";
    out << visualEditTrunkProcessBendingZ -> getMaxAngle() << " ";
    out << minRForBranch -> value() << " ";
    out << maxRForBranch -> value() << " ";
    out << trunkSegmentLength -> value() << " ";
    out << trunkSegmentLengthChange -> value() << " ";
    out << trunkGravitation -> value() << " ";
    out << trunkSegmentsNum -> value() << " ";
    out << "\n" << trunkTexture -> text() << "\n";

    // Branches
    out << branchRChangeCof -> value() << " ";
    out << branchProcessRChangeCof -> value() << " ";
    out << branchProcessChance -> value() << " ";
    out << branchMaxProcessesFromSegment -> value() << " ";
    out << branchBending -> value() << " ";
    out << branchProcessBendingY -> value() << " ";
//    out << branchProcessBendingZ -> value() << " ";
    out << visualEditBranchProcessBendingZ -> getMinAngle() << " ";
    out << visualEditBranchProcessBendingZ -> getMaxAngle() << " ";
    out << branchSegmentLength -> value() << " ";
    out << branchSegmentLengthChange -> value() << " ";
    out << branchGravitation -> value() << " ";
    out << branchSegmentsNum -> value() << " ";
    out << bbSize -> value() << " ";
    out << maxOverlapsCount -> value() << " ";
    out << "\n" << branchesTexture -> text() << "\n";

    // Leaves
    out << maxRadiusForLeaf -> value() << " ";
    out << leavesSpreading -> value() << " ";
    out << leafChance -> value() << " ";
    out << leafSize -> value() << " ";

    out << "\n" << leavesTypes.size();

    for(int i = 0; i < leavesTypes.size(); ++i)
    {
        out << "\n" << leavesTypes[i].name;
        out << "\n" << leavesTypes[i].coefficient;
        out << "\n" << leavesTypes[i].texture;
    }
}

void PropertiesWidget::loadOptions(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream in(&file);

    double dtmp;
    int itmp;

    // Trunk
    in >> dtmp; trunkRadius -> setValue(dtmp);
    in >> dtmp; trunkRadiusChangeCof -> setValue(dtmp);
    in >> dtmp; trunkProcessRadiusChangeCof -> setValue(dtmp);
    in >> itmp; trunkMaxProcessesFromSegment -> setValue(itmp);
    in >> dtmp; trunkBending -> setValue(dtmp);
    in >> dtmp; trunkProcessChance -> setValue(dtmp);
    in >> dtmp; trunkProcessBendingY -> setValue(dtmp);
//    in >> dtmp; trunkProcessBendingZ -> setValue(dtmp);
    in >> dtmp; visualEditTrunkProcessBendingZ -> setMinAngle(dtmp);
    in >> dtmp; visualEditTrunkProcessBendingZ -> setMaxAngle(dtmp);
    in >> dtmp; minRForBranch -> setValue(dtmp);
    in >> dtmp; maxRForBranch -> setValue(dtmp);
    in >> dtmp; trunkSegmentLength -> setValue(dtmp);
    in >> dtmp; trunkSegmentLengthChange -> setValue(dtmp);
    in >> dtmp; trunkGravitation -> setValue(dtmp);
    in >> itmp; trunkSegmentsNum -> setValue(itmp);
    in.readLine();
    trunkTexture -> setText(in.readLine());

    // Branches
    in >> dtmp; branchRChangeCof -> setValue(dtmp);
    in >> dtmp; branchProcessRChangeCof -> setValue(dtmp);
    in >> dtmp; branchProcessChance -> setValue(dtmp);
    in >> itmp; branchMaxProcessesFromSegment -> setValue(itmp);
    in >> dtmp; branchBending -> setValue(dtmp);
    in >> dtmp; branchProcessBendingY -> setValue(dtmp);
//    in >> dtmp; branchProcessBendingZ -> setValue(dtmp);
    in >> dtmp; visualEditBranchProcessBendingZ -> setMinAngle(dtmp);
    in >> dtmp; visualEditBranchProcessBendingZ -> setMaxAngle(dtmp);
    in >> dtmp; branchSegmentLength -> setValue(dtmp);
    in >> dtmp; branchSegmentLengthChange -> setValue(dtmp);
    in >> dtmp; branchGravitation -> setValue(dtmp);
    in >> itmp; branchSegmentsNum -> setValue(itmp);
    in >> dtmp; bbSize -> setValue(dtmp);
    in >> itmp; maxOverlapsCount -> setValue(itmp);
    in.readLine();
    branchesTexture -> setText(in.readLine());

    // Leaves
    in >> dtmp; maxRadiusForLeaf -> setValue(dtmp);
    in >> dtmp; leavesSpreading -> setValue(dtmp);
    in >> dtmp; leafChance -> setValue(dtmp);
    in >> dtmp; leafSize -> setValue(dtmp);

    in >> itmp; leavesTypes.resize(itmp);
    in.readLine();

    leavesTypesWidget -> clear();

    for(int i = 0; i < leavesTypes.size(); ++i)
    {
        leavesTypes[i].name = in.readLine();
        in >> leavesTypes[i].coefficient;
        in.readLine();
        leavesTypes[i].texture = in.readLine();

        leavesTypesWidget -> addItem(leavesTypes[i].name);
    }
}

void PropertiesWidget::clearLeavesTypes()
{
    leavesTypes.clear();
    leavesTypesWidget -> clear();
}

int PropertiesWidget::getLeavesTypesCount()
{
    return leavesTypes.size();
}

TreeLibLeafProperties PropertiesWidget::getConvertedLeafType(int current)
{
    TreeLibLeafProperties tmp;
    double totalSum = 0.0;
    for(int i = 0; i < leavesTypes.size(); ++i)
    {
        totalSum += leavesTypes[i].coefficient;
    }
    tmp.coff = leavesTypes[current].coefficient / totalSum;
    tmp.texture = leavesTypes[current].texture.toStdString();
    return tmp;
}

void PropertiesWidget::loadTrunkTexture()
{
    trunkTexture -> setText(QFileDialog::getOpenFileName(this, tr("Texture load"),
                            QCoreApplication::applicationDirPath(), tr("Images (*.png *.jpg *.bmp)")));
}

void PropertiesWidget::loadBranchesTexture()
{
    branchesTexture -> setText(QFileDialog::getOpenFileName(this, tr("Texture load"),
                            QCoreApplication::applicationDirPath(), tr("Images (*.png *.jpg *.bmp)")));
}

void PropertiesWidget::addNewLeafType()
{
    currentAction = ADD_LEAF_TYPE;
    addNewLeafTypeWidget -> show();
}

void PropertiesWidget::editLeafType()
{
    int current = leavesTypesWidget -> currentRow();
    if(current != -1)
    {
        currentAction = EDIT_LEAF_TYPE;
        addNewLeafTypeWidget -> setName(leavesTypes[current].name);
        addNewLeafTypeWidget -> setCoefficient(leavesTypes[current].coefficient);
        addNewLeafTypeWidget -> setTexture(leavesTypes[current].texture);
        addNewLeafTypeWidget -> show();
    }
}

void PropertiesWidget::removeLeafType()
{
    int current = leavesTypesWidget -> currentRow();
    if(current != -1)
    {
        leavesTypes.remove(current);
        delete (leavesTypesWidget -> takeItem(current));
    }
}

void PropertiesWidget::saveLeafType()
{
    if(addNewLeafTypeWidget -> accepted())
    {
        if(currentAction == ADD_LEAF_TYPE)
        {
            LeafType tmp;
            tmp.name = addNewLeafTypeWidget -> getName();
            tmp.coefficient = addNewLeafTypeWidget -> getCoefficient();
            tmp.texture = addNewLeafTypeWidget -> getTexture();
            leavesTypes.push_back(tmp);

            leavesTypesWidget -> addItem(tmp.name);
        } else if(currentAction == EDIT_LEAF_TYPE)
        {
            int current = leavesTypesWidget -> currentRow();
            leavesTypes[current].name = addNewLeafTypeWidget -> getName();
            leavesTypes[current].coefficient = addNewLeafTypeWidget -> getCoefficient();
            leavesTypes[current].texture = addNewLeafTypeWidget -> getTexture();

            // Updating leaf name in a list
            leavesTypesWidget -> item(current) -> setText(leavesTypes[current].name);
        }
    }
}

void PropertiesWidget::createWidgets()
{
    tabs = new QTabWidget(this);

    createTrunkTab();
    createBranchesTab();
    createLeavesTab();
}

void PropertiesWidget::createTrunkTab()
{
    trunkTab = new QWidget(tabs);
    QFormLayout *trunkTabLayout = new QFormLayout(trunkTab);

    trunkRadius = new QDoubleSpinBox(tabs);
    trunkRadius -> setSingleStep(0.1);
    trunkRadius -> setMinimum(0.1);
    trunkRadius -> setMaximum(3.0);
    trunkTabLayout -> addRow("Trunk radius", trunkRadius);

    trunkRadiusChangeCof = new QDoubleSpinBox(tabs);
    trunkRadiusChangeCof -> setSingleStep(0.1);
    trunkRadiusChangeCof -> setMinimum(0.1);
    trunkRadiusChangeCof -> setMaximum(0.99);
    trunkTabLayout -> addRow("Trunk radius reduction factor", trunkRadiusChangeCof);

    trunkProcessRadiusChangeCof = new QDoubleSpinBox(tabs);
    trunkProcessRadiusChangeCof -> setSingleStep(0.1);
    trunkProcessRadiusChangeCof -> setMinimum(0.1);
    trunkProcessRadiusChangeCof -> setMaximum(1.0);
    trunkTabLayout -> addRow("Process radius reduction factor", trunkProcessRadiusChangeCof);

    trunkMaxProcessesFromSegment = new QSpinBox(tabs);
    trunkMaxProcessesFromSegment -> setMinimum(0);
    trunkMaxProcessesFromSegment -> setMaximum(100);
    trunkTabLayout -> addRow("Max processes from segment", trunkMaxProcessesFromSegment);

    trunkBending = new QDoubleSpinBox(tabs);
    trunkBending -> setSingleStep(0.1);
    trunkBending -> setMinimum(0.0);
    trunkBending -> setMaximum(2.0);
    trunkTabLayout -> addRow("Trunk bending", trunkBending);

    trunkProcessBendingEditButton = new QPushButton("Edit trunk process bending", this);
    trunkTabLayout -> addRow(trunkProcessBendingEditButton);

    trunkProcessChance = new QDoubleSpinBox(tabs);
    trunkProcessChance -> setSingleStep(0.1);
    trunkProcessChance -> setMinimum(0.3);
    trunkProcessChance -> setMaximum(0.9);
    trunkTabLayout -> addRow("Process appearing possibility", trunkProcessChance);

    trunkProcessBendingY = new QDoubleSpinBox(tabs);
    trunkProcessBendingY -> setSingleStep(0.1);
    trunkProcessBendingY -> setMinimum(0.0);
    trunkProcessBendingY -> setMaximum(5.0);
    trunkTabLayout -> addRow("Process discrepancy index (Oy)", trunkProcessBendingY);

//    trunkProcessBendingZ = new QDoubleSpinBox(tabs);
//    trunkProcessBendingZ -> setSingleStep(0.1);
//    trunkProcessBendingZ -> setMinimum(0.0);
//    trunkProcessBendingZ -> setMaximum(2.0);
//    trunkTabLayout -> addRow("Process discrepancy index (Oz)", trunkProcessBendingZ);

    minRForBranch = new QDoubleSpinBox(tabs);
    minRForBranch -> setSingleStep(0.1);
    minRForBranch -> setMinimum(0.0);
    minRForBranch -> setMaximum(1.0);
    trunkTabLayout -> addRow("Mimimum radius for branch appearing", minRForBranch);

    maxRForBranch = new QDoubleSpinBox(tabs);
    maxRForBranch -> setSingleStep(0.1);
    maxRForBranch -> setMinimum(0.1);
    maxRForBranch -> setMaximum(100.0);
    trunkTabLayout -> addRow("Maximum radius for branch appearing", maxRForBranch);

    trunkSegmentLength = new QDoubleSpinBox(tabs);
    trunkSegmentLength -> setSingleStep(0.1);
    trunkSegmentLength -> setMinimum(0.1);
    trunkSegmentLength -> setMaximum(2.0);
    trunkTabLayout -> addRow("Segment length", trunkSegmentLength);

    trunkSegmentLengthChange = new QDoubleSpinBox(tabs);
    trunkSegmentLengthChange -> setSingleStep(0.1);
    trunkSegmentLengthChange -> setMinimum(0.1);
    trunkSegmentLengthChange -> setMaximum(2.0);
    trunkTabLayout -> addRow("Segment length change coefficient", trunkSegmentLengthChange);

    trunkGravitation = new QDoubleSpinBox(tabs);
    trunkGravitation -> setSingleStep(0.1);
    trunkGravitation -> setMinimum(-10.0);
    trunkGravitation -> setMaximum(10.0);
    trunkTabLayout -> addRow("Gravitation", trunkGravitation);

    trunkSegmentsNum = new QSpinBox(tabs);
    trunkSegmentsNum -> setMinimum(3);
    trunkSegmentsNum -> setMaximum(50);
    trunkTabLayout -> addRow("Segments quantity", trunkSegmentsNum);

    trunkTexture = new QLineEdit("trunk.png", this);
    trunkTabLayout -> addRow("Texture: ", trunkTexture);

    loadTrunkTextureButton = new QPushButton("Load texture", this);
    trunkTabLayout -> addRow(loadTrunkTextureButton);

    trunkTab -> setLayout(trunkTabLayout);
    tabs -> addTab(trunkTab, QString("Trunk"));
}

void PropertiesWidget::createBranchesTab()
{
    branchesTab = new QWidget(this);
    QFormLayout *branchesTabLayout = new QFormLayout(branchesTab);

    branchRChangeCof = new QDoubleSpinBox(tabs);
    branchRChangeCof -> setSingleStep(0.1);
    branchRChangeCof -> setMinimum(0.1);
    branchRChangeCof -> setMaximum(0.99);
    branchesTabLayout -> addRow("Branch radius reduction factor", branchRChangeCof);

    branchProcessRChangeCof = new QDoubleSpinBox(tabs);
    branchProcessRChangeCof -> setSingleStep(0.1);
    branchProcessRChangeCof -> setMinimum(0.1);
    branchProcessRChangeCof -> setMaximum(1.0);
    branchesTabLayout -> addRow("Process radius reduction factor", branchProcessRChangeCof);

    branchProcessChance = new QDoubleSpinBox(tabs);
    branchProcessChance -> setSingleStep(0.1);
    branchProcessChance -> setMinimum(0.01);
    branchProcessChance -> setMaximum(1.0);
    branchesTabLayout -> addRow("Process appearing possibility", branchProcessChance);

    branchMaxProcessesFromSegment = new QSpinBox(tabs);
    branchMaxProcessesFromSegment -> setMinimum(0);
    branchMaxProcessesFromSegment -> setMaximum(100);
    branchesTabLayout -> addRow("Max processes from segment", branchMaxProcessesFromSegment);

    branchBending = new QDoubleSpinBox(tabs);
    branchBending -> setSingleStep(0.1);
    branchBending -> setMinimum(0.0);
    branchBending -> setMaximum(2.0);
    branchesTabLayout -> addRow("Branch bending", branchBending);

    branchProcessBendingY = new QDoubleSpinBox(tabs);
    branchProcessBendingY -> setSingleStep(0.1);
    branchProcessBendingY -> setMinimum(0.0);
    branchProcessBendingY -> setMaximum(10.0);
    branchesTabLayout -> addRow("Process discrepancy index (Oy)", branchProcessBendingY);

    branchProcessBendingEditButton = new QPushButton("Edit branch process bending", this);
    branchesTabLayout -> addRow(branchProcessBendingEditButton);

//    branchProcessBendingZ = new QDoubleSpinBox(tabs);
//    branchProcessBendingZ -> setSingleStep(0.1);
//    branchProcessBendingZ -> setMinimum(0.0);
//    branchProcessBendingZ -> setMaximum(2.0);
//    branchesTabLayout -> addRow("Process discrepancy index (Oz)", branchProcessBendingZ);

    branchSegmentLength = new QDoubleSpinBox(tabs);
    branchSegmentLength -> setSingleStep(0.1);
    branchSegmentLength -> setMinimum(0.1);
    branchSegmentLength -> setMaximum(2.0);
    branchesTabLayout -> addRow("Segment length", branchSegmentLength);

    branchSegmentLengthChange = new QDoubleSpinBox(tabs);
    branchSegmentLengthChange -> setSingleStep(0.1);
    branchSegmentLengthChange -> setMinimum(0.1);
    branchSegmentLengthChange -> setMaximum(2.0);
    branchesTabLayout -> addRow("Segment length change coefficient", branchSegmentLengthChange);

    branchGravitation = new QDoubleSpinBox(tabs);
    branchGravitation -> setSingleStep(0.1);
    branchGravitation -> setMinimum(-10.0);
    branchGravitation -> setMaximum(10.0);
    branchesTabLayout -> addRow("Branch gravitation", branchGravitation);

    branchSegmentsNum = new QSpinBox(tabs);
    branchSegmentsNum -> setMinimum(3);
    branchSegmentsNum -> setMaximum(50);
    branchesTabLayout -> addRow("Segments quantity", branchSegmentsNum);

    bbSize = new QDoubleSpinBox(tabs);
    bbSize -> setSingleStep(0.1);
    bbSize -> setMinimum(0.0);
    bbSize -> setMaximum(10.0);
    branchesTabLayout -> addRow("Bounding box size", bbSize);

    maxOverlapsCount = new QSpinBox(tabs);
    maxOverlapsCount -> setMinimum(0);
    maxOverlapsCount -> setMaximum(100);
    branchesTabLayout -> addRow("Maximum number of overlaps", maxOverlapsCount);

    branchesTexture = new QLineEdit("trunk.png", this);
    branchesTabLayout -> addRow("Texture: ", branchesTexture);

    loadBranchesTextureButton = new QPushButton("Load texture", this);
    branchesTabLayout -> addRow(loadBranchesTextureButton);

    branchesTab -> setLayout(branchesTabLayout);
    tabs -> addTab(branchesTab, "Branches");
}

void PropertiesWidget::createLeavesTab()
{
    leavesTab = new QWidget(this);
    QVBoxLayout *leavesTabLayout = new QVBoxLayout(leavesTab);
    QFormLayout *leavesTabOptionsLayout = new QFormLayout();

    maxRadiusForLeaf = new QDoubleSpinBox(tabs);
    maxRadiusForLeaf -> setSingleStep(0.1);
    maxRadiusForLeaf -> setMinimum(0.0);
    maxRadiusForLeaf -> setMaximum(10.0);
    leavesTabOptionsLayout -> addRow("Maximum radius for leaf", maxRadiusForLeaf);

    leavesSpreading = new QDoubleSpinBox(tabs);
    leavesSpreading -> setSingleStep(0.1);
    leavesSpreading -> setMinimum(0.0);
    leavesSpreading -> setMaximum(10.0);
    leavesTabOptionsLayout -> addRow("The distance between laves and branches", leavesSpreading);

    leafChance = new QDoubleSpinBox(tabs);
    leafChance -> setSingleStep(0.1);
    leafChance -> setMinimum(0.0);
    leafChance -> setMaximum(1.0);
    leavesTabOptionsLayout -> addRow("The leaf appearing possibility", leafChance);

    leafSize = new QDoubleSpinBox(tabs);
    leafSize -> setSingleStep(0.1);
    leafSize -> setMinimum(0.1);
    leafSize -> setMaximum(10.0);
    leavesTabOptionsLayout -> addRow("Leaf size", leafSize);

    leavesTabLayout -> addLayout(leavesTabOptionsLayout);

    leavesTypesWidget = new QListWidget(this);
    leavesTabLayout -> addWidget(leavesTypesWidget);

    addNewLeafTypeButton = new QPushButton("Add", this);
    leavesTabLayout -> addWidget(addNewLeafTypeButton);

    QHBoxLayout *editRemoveLayout = new QHBoxLayout();

    editLeafTypeButton = new QPushButton("Edit", this);
    editRemoveLayout -> addWidget(editLeafTypeButton);

    removeLeafTypeButton = new QPushButton("Remove", this);
    editRemoveLayout -> addWidget(removeLeafTypeButton);

    leavesTabLayout -> addLayout(editRemoveLayout);

    leavesTab -> setLayout(leavesTabLayout);
    tabs -> addTab(leavesTab, "Leaves");

    addNewLeafTypeWidget = new NewLeafTypeWidget;
}

void PropertiesWidget::createActions()
{
    connect(loadTrunkTextureButton, &QPushButton::clicked, this, &PropertiesWidget::loadTrunkTexture);
    connect(loadBranchesTextureButton, &QPushButton::clicked, this, &PropertiesWidget::loadBranchesTexture);
    connect(addNewLeafTypeButton, &QPushButton::clicked, this, &PropertiesWidget::addNewLeafType);
    connect(addNewLeafTypeWidget, &NewLeafTypeWidget::closed, this, &PropertiesWidget::saveLeafType);
    connect(editLeafTypeButton, &QPushButton::clicked, this, &PropertiesWidget::editLeafType);
    connect(removeLeafTypeButton, &QPushButton::clicked, this, &PropertiesWidget::removeLeafType);
}
