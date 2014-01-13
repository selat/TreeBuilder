#ifndef TREE_H
#define TREE_H

#include <vector>
#include <fstream>

#include "TreeLibVertex.h"
#include "TreeLibPlain.h"
#include "TreeLibPlain4.h"
#include "TreeLibOBB.h"
#include "TreeLibLeafProperties.h"

class Tree
{
public:
    Tree();
    Tree(int &argc, char **argv);
    Tree(double _trunk_r, double _r_branch_change_cof, double _r_process_change_cof, double _process_chance,
        double _branch_bending, double _process_bending_y, double _process_bending_z, double _segment_length,
        double _segment_length_change, double _gravitation, int _segment_num);
    void generate(double x, double y, double z);
    void generate(double x, double y, double z, double y_angle, double z_angle,
        double length, double r1, bool is_trunk = true);
    void write(const char* name);
    void read(const char* name);
    void clear();

    // Getting algorithm parameters
    // Trunk
    double getTrunkRadius();
    double getTrunkRadiusChangeCof();
    double getTrunkProcessRadiusChangeCof();
	int getTrunkMaxProcessesFromSegment();
    double getTrunkBending();
    double getTrunkProcessChance();
    double getTrunkProcessBendingY();
    double getTrunkProcessBendingZ();
    double getMinRForBranch();
    double getMaxRForBranch();
    double getTrunkSegmentLength();
    double getTrunkSegmentLengthChange();
    double getTrunkGravitation();
    int getTrunkSegmentsNum();
    std::string getTrunkTexture();
	double getMinRadius();

    // Branches
    double getBranchRChangeCof();
    double getBranchProcessRChangeCof();
    double getBranchProcessChance();
	int getBranchMaxProcessesFromSegment();
    double getBranchBending();
    double getBranchProcessBendingY();
    double getBranchProcessBendingZ();
    double getBranchSegmentLength();
    double getBranchSegmentLengthChange();
    double getBranchGravitation();
    int getBranchSegmentsNum();
    double getBBSize();
    int getMaxOverlapsCount();
    std::string getBranchesTexture();

    // Leaves
    double getMaxRadiusForLeaf();
    double getLeavesSpreading();
    double getLeafChance();
    double getLeafSize();

    // Setting algorithm parameters
    // Trunk
    void setTrunkRadius(double radius);
    void setTrunkRadiusChangeCof(double cof);
    void setTrunkProcessRadiusChangeCof(double cof);
	void setTrunkMaxProcessesFromSegment(int value);
    void setTrunkBending(double value);
    void setTrunkProcessChance(double value);
    void setTrunkProcessBendingY(double value);
    void setTrunkProcessBendingZ(double value1, double value2);
    void setMinRForBranch(double value);
    void setMaxRForBranch(double value);
    void setTrunkSegmentLength(double value);
    void setTrunkSegmentLengthChange(double value);
    void setTrunkGravitation(double value);
    void setTrunkSegmentsNum(int value);
    void setTrunkTexture(std::string value);
	void setMinRadius(double value);

    // Branches
    void setBranchRChangeCof(double value);
    void setBranchProcessRChangeCof(double value);
    void setBranchProcessChance(double value);
	void setBranchMaxProcessesFromSegment(int value);
    void setBranchBending(double value);
    void setBranchProcessBendingY(double value);
    void setBranchProcessBendingZ(double value1, double value2);
    void setBranchSegmentLength(double value);
    void setBranchSegmentLengthChange(double value);
    void setBranchGravitation(double value);
    void setBranchSegmentsNum(int value);
    void setBBSize(double value);
    void setMaxOverlapsCount(int value);
    void setBranchesTexture(std::string value);

    // Leaves
    void setMaxRadiusForLeaf(double value);
    void setLeavesSpreading(double value);
    void setLeafChance(double value);
    void setLeafSize(double value);

    void clearLeavesTypes();
    void addLeafType(TreeLibLeafProperties tmp);

public:
    #define MIN_BBSIZE 0.01
    void genTruncatedPyramid(std::vector <TreeLibPlain> &plain, TreeLibVector& start_position,
        TreeLibVector& end_position, double r1, double r2, double start_y_angle, double start_z_angle,
        double end_y_angle, double end_z_angle, int num);
    void genPyramid(std::vector <TreeLibPlain> &plain, TreeLibVector& start_position,
        TreeLibVector& end_position, double r1,  double start_y_angle, double start_z_angle, int num);
    void genLeaf(TreeLibVector& position, double y_angle, double z_angle, double leaf_angle, int leaf_type);

    // Trunk properties
    double trunk_r;
    double trunk_r_change_cof;
    double trunk_process_r_change_cof;
	int trunk_max_processes_from_segment;
    double trunk_bending;
    double trunk_process_chance;
    double trunk_process_bending_y;
    double trunk_process_bending_z;
    double trunk_process_bending_z_min_angle;
    double trunk_process_bending_z_delta_angle;
    double min_r_for_branch;
    double max_r_for_branch;
    double trunk_segment_length;
    double trunk_segment_length_change;
    double trunk_gravitation;
    int trunk_segments_num;
	double min_r;

    // Branches properties
    double branch_r_change_cof;
    double branch_process_r_change_cof;
    double branch_process_chance;
	int branch_max_processes_from_segment;
    double branch_bending;
    double branch_process_bending_y;
    double branch_process_bending_z;
    double branch_process_bending_z_min_angle;
    double branch_process_bending_z_delta_angle;
    double branch_segment_length;
    double branch_segment_length_change;
    double branch_gravitation;
    int branch_segments_num;
    double bbsize;
    int max_overlaps_count;

    // Leaves properties
    double max_radius_for_leaf;
    double leaves_spreading;
    double leaf_chance;
    double leaf_size;

public:
    std::vector <TreeLibPlain> trunk;
    std::vector <TreeLibPlain> branches;
    std::vector <TreeLibOBB> obb;
    std::vector <TreeLibPlain4> leaves;
    std::vector <TreeLibLeafProperties> leaves_properties;
    std::string trunk_texture_name;
    std::string branches_texture_name;
    std::ofstream out;
};

#endif
