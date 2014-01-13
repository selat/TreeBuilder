#include <cmath>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>

#include "Tree.h"
#include "TreeLibMatrix3.h"

Tree::Tree() : out("test.txt")
{
    // Trunk properties
    trunk_r = 1.0;
    trunk_r_change_cof = 0.9;
    trunk_process_r_change_cof = 0.8;
    trunk_process_chance = 0.6;
    trunk_bending = 1.0;
    trunk_process_bending_y = 4.0;
    trunk_process_bending_z_min_angle = M_PI - 0.5;
    trunk_process_bending_z_delta_angle = 0.2;
    trunk_process_bending_z = 2.0;
	trunk_max_processes_from_segment = 1;
    min_r_for_branch = 0.0;
    max_r_for_branch = 1000.0;
    trunk_segment_length = 1.0;
    trunk_segment_length_change = 1.0;
    trunk_gravitation = 0.0;
    trunk_segments_num = 6;
	min_r = 0.02;

    // Branches properties
    branch_r_change_cof = 0.9;
    branch_process_r_change_cof = 0.8;
    branch_process_chance = 0.6;
	branch_max_processes_from_segment = 1;
    branch_bending = 1.0;
    branch_process_bending_y = 2.0;
    branch_process_bending_z = 2.0;
    branch_process_bending_z_min_angle = M_PI - 0.5;
    branch_process_bending_z_delta_angle = 0.2;
    branch_segment_length = 1.0;
    branch_segment_length_change = 1.0;
    branch_gravitation = 0.0;
    branch_segments_num = 8;
    bbsize = 1.0;
    max_overlaps_count = 1;

    // Leaves properties
    max_radius_for_leaf = 0.5;
    leaves_spreading = 0.5;
    leaf_chance = 1.0;
    leaf_size = 0.5;
}

Tree::Tree(int &argc, char **argv)
{
    if(argc >= 31)
    {
        // Trunk properties
        trunk_r = atof(argv[1]);
        trunk_r_change_cof = atof(argv[2]);
        trunk_process_r_change_cof = atof(argv[3]);
        trunk_process_chance = atof(argv[4]);
        trunk_bending = atof(argv[5]);
        trunk_process_bending_y = atof(argv[6]);
        trunk_process_bending_z = atof(argv[7]);
        min_r_for_branch = atof(argv[8]);
        max_r_for_branch = atof(argv[9]);
        trunk_segment_length = atof(argv[10]);
        trunk_segment_length_change = atof(argv[11]);
        trunk_gravitation = atof(argv[12]);
        trunk_segments_num = atoi(argv[13]);
		min_r = 0.01;

        // Branches properties
        branch_r_change_cof = atof(argv[15]);
        branch_process_r_change_cof = atof(argv[16]);
        branch_process_chance = atof(argv[17]);
        branch_bending = atof(argv[18]);
        branch_process_bending_y = atof(argv[19]);
        branch_process_bending_z = atof(argv[20]);
        branch_segment_length = atof(argv[21]);
        branch_segment_length_change = atof(argv[22]);
        branch_gravitation = atof(argv[23]);
        branch_segments_num = atoi(argv[24]);
        bbsize = atof(argv[25]);
        max_overlaps_count = atoi(argv[26]);

        // Leaves properties
        max_radius_for_leaf = atof(argv[28]);
        leaves_spreading = atof(argv[29]);
        leaf_chance = atof(argv[30]);
        leaf_size = atof(argv[31]);

        int leaf_types_count = atoi(argv[32]);
        for(int i = 0; i < leaf_types_count; ++i)
        {
            TreeLibLeafProperties tmp_leaf_properties;
            tmp_leaf_properties.coff = atof(argv[33 + i * 2]);
            tmp_leaf_properties.texture = argv[34 + i * 2];
            leaves_properties.push_back(tmp_leaf_properties);
        }
        double total_coff_sum = 0.0;
        for(int i = 0; i < leaf_types_count; ++i)
        {
            total_coff_sum += leaves_properties[i].coff;
        }
        for(int i = 0; i < leaf_types_count; ++i)
        {
            leaves_properties[i].coff = leaves_properties[i].coff / total_coff_sum;
        }
        double current_sum = 0.0;
        for(int i = 0; i < leaf_types_count; ++i)
        {
            leaves_properties[i].coff = current_sum + leaves_properties[i].coff;
            current_sum += leaves_properties[i].coff;
        }
    } else {
        // Trunk properties
        trunk_r = 1.0;
        trunk_r_change_cof = 0.9;
        trunk_process_r_change_cof = 0.8;
        trunk_process_chance = 0.6;
        trunk_bending = 1.0;
        trunk_process_bending_y = 2.0;
        trunk_process_bending_z = 2.0;
        min_r_for_branch = 0.0;
        max_r_for_branch = 1000.0;
        trunk_segment_length = 1.0;
        trunk_segment_length_change = 1.0;
        trunk_gravitation = -0.005;
        trunk_segments_num = 8;
		min_r = 0.01;

        // Branches properties
        branch_r_change_cof = 0.9;
        branch_process_r_change_cof = 0.8;
        branch_process_chance = 0.6;
        branch_bending = 1.0;
        branch_process_bending_y = 2.0;
        branch_process_bending_z = 2.0;
        branch_segment_length = 1.0;
        branch_segment_length_change = 1.0;
        branch_gravitation = -0.005;
        branch_segments_num = 3;
        bbsize = 1.0;
        max_overlaps_count = 1;

        // Leaves properties
        max_radius_for_leaf = 0.1;
        leaves_spreading = 0.5;
        leaf_chance = 0.5;
        leaf_size = 0.5;
    }
}

Tree:: Tree(double _trunk_r, double _branch_r_change_cof, double _branch_process_r_change_cof, double _branch_process_chance,
    double _branch_bending, double _branch_process_bending_y, double _branch_process_bending_z, double _branch_segment_length,
    double _branch_segment_length_change, double _branch_gravitation, int _branch_segments_num)
{
    trunk_r = _trunk_r;
    branch_r_change_cof = _branch_r_change_cof;
    branch_process_r_change_cof = _branch_process_r_change_cof;
    branch_process_chance = _branch_process_chance;
    branch_bending = _branch_bending;
    branch_process_bending_y = _branch_process_bending_y;
    branch_process_bending_z = _branch_process_bending_z;
    branch_segment_length = _branch_segment_length;
    branch_segment_length_change = _branch_segment_length_change;
    branch_gravitation = _branch_gravitation;
    branch_segments_num = _branch_segments_num;
}

void Tree::generate(double x, double y, double z)
{
    generate(x, y, z, 0.0, M_PI_2, trunk_segment_length, trunk_r);
}

void Tree::generate(double x, double y, double z, double y_angle, double z_angle,
    double length, double r1, bool is_trunk)
{
    double bending = branch_bending;
    double r_change_cof = branch_r_change_cof;
    double r_process_change_cof = branch_r_change_cof;
    double process_bending_y = branch_process_bending_y;
    double process_bending_z = branch_process_bending_z;
    double process_bending_z_min_angle = branch_process_bending_z_min_angle;
    double process_bending_z_delta_angle = branch_process_bending_z_delta_angle;
    double gravitation = branch_gravitation;
    double process_chance = branch_process_chance;
    int poly_num = branch_segments_num;
    if(is_trunk)
    {
        bending = trunk_bending;
        r_change_cof = trunk_r_change_cof;
        r_process_change_cof = trunk_process_r_change_cof;
        process_bending_y = trunk_process_bending_y;
        process_bending_z = trunk_process_bending_z;
        process_bending_z_min_angle = trunk_process_bending_z_min_angle;
        process_bending_z_delta_angle = trunk_process_bending_z_delta_angle;
        gravitation = trunk_gravitation;
        process_chance = trunk_process_chance;
        poly_num = trunk_segments_num;
    }
    TreeLibVector start_position, end_position, process_start_position;
    start_position.x = x;
    start_position.y = y;
    start_position.z = z;

    double start_y_angle = -y_angle;
    double start_z_angle = z_angle - M_PI_2;

    double end_y_angle = -y_angle;
    double end_z_angle = z_angle - M_PI_2;
    double y_angle_change = 0.0;
    double z_angle_change = 0.0;

    double process_y_angle, process_z_angle;
    double process_radius = 1.0;

    double r2 = r1 * ((rand() % 100 + 901) / 1000.0) * r_change_cof;

    if(r1 < 0.5)
    {
        poly_num = 3;
    }
    while(r1 >= min_r)
    {
        end_position.x = length * cos(z_angle) * cos(y_angle) + start_position.x;
        end_position.y = length * sin(z_angle) + start_position.y;
        end_position.z = length * cos(z_angle) * sin(y_angle) + start_position.z;

        // Creating OBB
        if(bbsize > MIN_BBSIZE)
        {
            TreeLibVector obb_position, obb_size;
            obb_position.x = (start_position.x + end_position.x) / 2.0;
            obb_position.y = (start_position.y + end_position.y) / 2.0;
            obb_position.z = (start_position.z + end_position.z) / 2.0;
            obb_size.x = r1 * bbsize;
            obb_size.y = length / 2.0;
            obb_size.z = r1 * bbsize;
            TreeLibMatrix3 obb_yrotate(
                cos(-y_angle), 0.0, sin(-y_angle),
                0.0, 1.0, 0.0,
                -sin(-y_angle), 0.0, cos(-y_angle));
            TreeLibMatrix3 obb_zrotate(
                cos(z_angle), -sin(z_angle), 0.0,
                sin(z_angle), cos(z_angle), 0.0,
                0.0, 0.0, 1.0);
            TreeLibMatrix3 obb_rotate = obb_yrotate * obb_zrotate;
            TreeLibOBB tmp_obb(obb_position, obb_size, obb_rotate);
            obb.push_back(tmp_obb);
        }

        genTruncatedPyramid((is_trunk) ? trunk : branches, start_position, end_position, r1, r2,
            start_y_angle, start_z_angle, end_y_angle, end_z_angle, poly_num);

        process_start_position.x = (start_position.x + end_position.x) / 2.0;
        process_start_position.y = (start_position.y + end_position.y) / 2.0;
        process_start_position.z = (start_position.z + end_position.z) / 2.0;

        if(process_bending_y > M_PI)
        {
            process_y_angle = (rand() % 1001) / 1000.0 * M_PI * 2.0;
        } else {
            y_angle_change = (rand() % 1001) / 1000.0 * (process_bending_y / M_PI) / M_PI;
            if(rand() % 2 == 0)
            {
                y_angle_change = -y_angle_change;
            }
            process_y_angle = y_angle + y_angle_change;
        }


//        z_angle_change = (50 + rand() % 50) / 400.0 * process_bending_z;
//        if(rand() % 2 == 0)
//        {
//            z_angle_change = -z_angle_change;
//        }

        z_angle_change = M_PI - (process_bending_z_min_angle + (rand() % 1001) / 1000.0 * process_bending_z_delta_angle);

        process_z_angle = z_angle + z_angle_change;

        process_radius = r1 * r_process_change_cof;

        bool process_generated = false;
        if(((1 + rand() % 100) / 100.0 <= process_chance) && (process_radius > min_r))
        {
			int max_processes_from_segment = is_trunk ? trunk_max_processes_from_segment : branch_max_processes_from_segment;
			for(int i = 0; i < max_processes_from_segment; ++i)
			{
				if(bbsize > MIN_BBSIZE)
				{
					TreeLibMatrix3 process_obb_yrotate(
						cos(-process_y_angle), 0.0, sin(-process_y_angle),
						0.0, 1.0, 0.0,
						-sin(-process_y_angle), 0.0, cos(-process_y_angle));
					TreeLibMatrix3 process_obb_zrotate(
						cos(process_z_angle), -sin(process_z_angle), 0.0,
						sin(process_z_angle), cos(process_z_angle), 0.0,
						0.0, 0.0, 1.0);
					TreeLibMatrix3 process_obb_rotate = process_obb_yrotate * process_obb_zrotate;
					TreeLibVector process_obb_size;
					process_obb_size.x = process_radius * bbsize;
					process_obb_size.y = branch_segment_length * branch_segment_length_change / 2.0;
					process_obb_size.z = process_radius * bbsize;
					TreeLibOBB tmp_obb(process_start_position, process_obb_size, process_obb_rotate);
					int overlaps_count = 0;
					for(unsigned int i = 0; i < obb.size(); ++i)
					{
						if(obb[i].overlapsOBB(tmp_obb))
						{
							++overlaps_count;
						}
					}
					if((overlaps_count <= max_overlaps_count) &&
					   (!is_trunk || (is_trunk && (r1 >= min_r_for_branch) && (r1 <= max_r_for_branch))))
                    {
						if(is_trunk)
						{
							process_generated = true;
							generate(process_start_position.x, process_start_position.y, process_start_position.z, process_y_angle,
                                process_z_angle, branch_segment_length * length / trunk_segment_length, process_radius, false);
                        } else {
                            process_generated = true;
                            generate(process_start_position.x, process_start_position.y, process_start_position.z, process_y_angle,
                                process_z_angle, length * branch_segment_length_change, process_radius, false);
                        }
					}
				} else if((!is_trunk || (is_trunk && (r1 >= min_r_for_branch) && (r1 <= max_r_for_branch))))
				{
					if(is_trunk)
					{
						process_generated = true;
						generate(process_start_position.x, process_start_position.y, process_start_position.z, process_y_angle,
                            process_z_angle, branch_segment_length * length / trunk_segment_length, process_radius, false);
                    } else {
                        process_generated = true;
                        generate(process_start_position.x, process_start_position.y, process_start_position.z, process_y_angle,
                                process_z_angle, length * branch_segment_length_change, process_radius, false);
                    }
				}
			}
        }
        if((leaves_properties.size() > 0) && (!process_generated) && (r1 <= max_radius_for_leaf)
           && ((1 + rand() % 100) / 100.0 <= leaf_chance))
        {
            TreeLibVector leaf_position;
            double leaf_angle = (1 + rand() % 100) / 50.0 * M_PI;
            double leaf_y_angle = y_angle;
            double leaf_z_angle = z_angle - M_PI_2;

            leaf_position.x = cos(leaf_angle) * leaves_spreading;
            leaf_position.y = 0.0;
            leaf_position.z = sin(leaf_angle) * leaves_spreading;

            double tmp_x = leaf_position.x;
            double tmp_y = leaf_position.y;
            double tmp_z = leaf_position.z;

            leaf_position.x = tmp_x * cos(leaf_z_angle) - tmp_y * sin(leaf_z_angle);
            leaf_position.y = tmp_x * sin(leaf_z_angle) + tmp_y * cos(leaf_z_angle);
            leaf_position.z = tmp_z;

            tmp_x = leaf_position.x;
            tmp_y = leaf_position.y;
            tmp_z = leaf_position.z;

            leaf_position.x = tmp_x * cos(leaf_y_angle) + tmp_z * sin(leaf_y_angle);
            leaf_position.y = tmp_y;
            leaf_position.z = -(tmp_x * sin(leaf_y_angle)) + tmp_z * cos(leaf_y_angle);

            leaf_position.x += (start_position.x + end_position.x) / 2.0;
            leaf_position.y += (start_position.y + end_position.y) / 2.0;
            leaf_position.z += (start_position.z + end_position.z) / 2.0;

            double set_leaf_type = (rand() % 1001) / 1000.0;
            int leaf_type = leaves_properties.size() - 1;
            for(unsigned int i = 0; i < leaves_properties.size(); ++i)
            {
                if(set_leaf_type <= leaves_properties[i].coff)
                {
                    leaf_type = i;
                    break;
                }
            }

            leaf_y_angle += rand() % 1000 / 5000.0f - rand() % 1000 / 5000.0f;
            leaf_z_angle += rand() % 1000 / 5000.0f - rand() % 1000 / 5000.0f;
            genLeaf(leaf_position, leaf_y_angle, leaf_z_angle, leaf_angle, leaf_type);
        }

        r1 = r2;
        r2 = r1 * ((rand() % 100 + 901) / 1000.0) * r_change_cof;
        start_position = end_position;
        start_y_angle = end_y_angle;
        start_z_angle = end_z_angle;

        y_angle_change = (50 + rand() % 50) / 200.0 * bending;
        if(rand() % 2 == 0)
        {
            y_angle_change = -y_angle_change;
        }

        z_angle_change = (50 + rand() % 50) / 400.0 * bending;
        if(rand() % 2 == 0)
        {
            z_angle_change = -z_angle_change;
        }

        y_angle += y_angle_change;
        z_angle += z_angle_change;
        if(branch_gravitation > 0.0)
        {
            z_angle += (-M_PI_2 - z_angle) * gravitation / r1;
        } else {
            z_angle += -(M_PI_2 - z_angle) * gravitation / r1;
        }

        end_y_angle = -y_angle;
        end_z_angle = z_angle - M_PI_2;
    }

    start_position = end_position;
    end_position.x = length * cos(z_angle) * cos(y_angle) + start_position.x;
    end_position.y = length * sin(z_angle) + start_position.y;
    end_position.z = length * cos(z_angle) * sin(y_angle) + start_position.z;

    if((leaves_properties.size() > 0) && (r1 <= max_radius_for_leaf)
       && ((1 + rand() % 100) / 100.0 <= leaf_chance))
    {
        TreeLibVector leaf_position;
        double leaf_angle = (1 + rand() % 100) / 50.0 * M_PI;
        double leaf_y_angle = y_angle;
        double leaf_z_angle = z_angle - M_PI_2;

        leaf_position.x = cos(leaf_angle) * leaves_spreading;
        leaf_position.y = 0.0;
        leaf_position.z = sin(leaf_angle) * leaves_spreading;

        double tmp_x = leaf_position.x;
        double tmp_y = leaf_position.y;
        double tmp_z = leaf_position.z;

        leaf_position.x = tmp_x * cos(leaf_z_angle) - tmp_y * sin(leaf_z_angle);
        leaf_position.y = tmp_x * sin(leaf_z_angle) + tmp_y * cos(leaf_z_angle);
        leaf_position.z = tmp_z;

        tmp_x = leaf_position.x;
        tmp_y = leaf_position.y;
        tmp_z = leaf_position.z;

        leaf_position.x = tmp_x * cos(leaf_y_angle) + tmp_z * sin(leaf_y_angle);
        leaf_position.y = tmp_y;
        leaf_position.z = -(tmp_x * sin(leaf_y_angle)) + tmp_z * cos(leaf_y_angle);

        leaf_position.x += (start_position.x + end_position.x) / 2.0;
        leaf_position.y += (start_position.y + end_position.y) / 2.0;
        leaf_position.z += (start_position.z + end_position.z) / 2.0;

        double set_leaf_type = (rand() % 1001) / 1000.0;
        int leaf_type = leaves_properties.size() - 1;
        for(unsigned int i = 0; i < leaves_properties.size(); ++i)
        {
            if(set_leaf_type <= leaves_properties[i].coff)
            {
                leaf_type = i;
                break;
            }
        }

        leaf_y_angle += rand() % 1000 / 5000.0f - rand() % 1000 / 5000.0f;
        leaf_z_angle += rand() % 1000 / 5000.0f - rand() % 1000 / 5000.0f;
        genLeaf(leaf_position, leaf_y_angle, leaf_z_angle, leaf_angle, leaf_type);
    }


    genPyramid((is_trunk) ? trunk : branches, start_position, end_position, r1, start_y_angle, start_z_angle, poly_num);
}

void Tree::write(const char *name)
{
    char file_name[1024];
    strcpy(file_name, name);
    strcat(file_name, ".obj");
    std::ofstream output_obj(file_name);

    strcpy(file_name, name);
    strcat(file_name, ".mtl");
    std::ofstream output_mtl(file_name);

    output_mtl << "newmtl Trunk\n";
    output_mtl << "map_Kd " << trunk_texture_name << "\n";

    output_mtl << "newmtl Branch\n";
    output_mtl << "map_Kd " << branches_texture_name << "\n";

    for(unsigned int i = 0; i < leaves_properties.size(); ++i)
    {
        output_mtl << "newmtl Leaves" << (i + 1) << std::endl;
        output_mtl << "map_Kd " << leaves_properties[i].texture << std::endl;
    }
    output_mtl.close();

    output_obj << "mtllib " << file_name << "\n";
    output_obj << "usemtl Trunk\n";
    output_obj << "o Trunk\n";

    for(unsigned int i = 0; i < trunk.size(); ++i)
    {
        output_obj << "v " << trunk[i].v[0].x;
        output_obj << " " << trunk[i].v[0].y;
        output_obj << " " << trunk[i].v[0].z << std::endl;
        output_obj << "vt " << trunk[i].v[0].u;
        output_obj << " " << trunk[i].v[0].v << std::endl;

        output_obj << "v " << trunk[i].v[1].x;
        output_obj << " " << trunk[i].v[1].y;
        output_obj << " " << trunk[i].v[1].z << std::endl;
        output_obj << "vt " << trunk[i].v[1].u;
        output_obj << " " << trunk[i].v[1].v << std::endl;

        output_obj << "v " << trunk[i].v[2].x;
        output_obj << " " << trunk[i].v[2].y;
        output_obj << " " << trunk[i].v[2].z << std::endl;
        output_obj << "vt " << trunk[i].v[2].u;
        output_obj << " " << trunk[i].v[2].v << std::endl;

        output_obj << "f -1/-1 -2/-2 -3/-3\n";
    }

    output_obj << "usemtl Branch\n";
    output_obj << "o Branches\n";

    for(unsigned int i = 0; i < branches.size(); ++i)
    {
        output_obj << "v " << branches[i].v[0].x;
        output_obj << " " << branches[i].v[0].y;
        output_obj << " " << branches[i].v[0].z << std::endl;
        output_obj << "vt " << branches[i].v[0].u;
        output_obj << " " << branches[i].v[0].v << std::endl;

        output_obj << "v " << branches[i].v[1].x;
        output_obj << " " << branches[i].v[1].y;
        output_obj << " " << branches[i].v[1].z << std::endl;
        output_obj << "vt " << branches[i].v[1].u;
        output_obj << " " << branches[i].v[1].v << std::endl;

        output_obj << "v " << branches[i].v[2].x;
        output_obj << " " << branches[i].v[2].y;
        output_obj << " " << branches[i].v[2].z << std::endl;
        output_obj << "vt " << branches[i].v[2].u;
        output_obj << " " << branches[i].v[2].v << std::endl;

        output_obj << "f -1/-1 -2/-2 -3/-3\n";
    }

    for(unsigned int i = 0; i < leaves_properties.size(); ++i)
    {
        output_obj << "usemtl Leaves" << (i + 1) << std::endl;
        output_obj << "o Leaves" << (i + 1) << std::endl;
        for(unsigned int j = 0; j < leaves.size(); ++j)
        {
            if(leaves[j].texture_id == i)
            {
                output_obj << "v " << leaves[j].v[0].x;
                output_obj << " " << leaves[j].v[0].y;
                output_obj << " " << leaves[j].v[0].z << std::endl;
                output_obj << "vt " << leaves[j].v[0].u;
                output_obj << " " << leaves[j].v[0].v << std::endl;

                output_obj << "v " << leaves[j].v[1].x;
                output_obj << " " << leaves[j].v[1].y;
                output_obj << " " << leaves[j].v[1].z << std::endl;
                output_obj << "vt " << leaves[j].v[1].u;
                output_obj << " " << leaves[j].v[1].v << std::endl;

                output_obj << "v " << leaves[j].v[2].x;
                output_obj << " " << leaves[j].v[2].y;
                output_obj << " " << leaves[j].v[2].z << std::endl;
                output_obj << "vt " << leaves[j].v[2].u;
                output_obj << " " << leaves[j].v[2].v << std::endl;

                output_obj << "v " << leaves[j].v[3].x;
                output_obj << " " << leaves[j].v[3].y;
                output_obj << " " << leaves[j].v[3].z << std::endl;
                output_obj << "vt " << leaves[j].v[3].u;
                output_obj << " " << leaves[j].v[3].v << std::endl;

                output_obj << "f -1/-1 -2/-2 -3/-3 -4/-4\n";
            }
        }
    }
    output_obj.close();
}

void Tree::read(const char* name)
{
    trunk.clear();
    branches.clear();
    leaves.clear();
    obb.clear();

    char file_name[100];
    strcpy(file_name, name);
    strcat(file_name, ".obj");
    std::ifstream input_obj(file_name);
    std::string tmp;
    getline(input_obj, tmp);
    while(strcmp(tmp.c_str(), "o Trunk"))
    {
        getline(input_obj, tmp);
    }

    TreeLibPlain tmp_plain3;
    while(true)
    {
        input_obj >> tmp;
        if(tmp == "usemtl")
        {
            break;
        }
        input_obj >> tmp_plain3.v[0].x;
        input_obj >> tmp_plain3.v[0].y;
        input_obj >> tmp_plain3.v[0].z;
        input_obj >> tmp;
        input_obj >> tmp_plain3.v[0].u;
        input_obj >> tmp_plain3.v[0].v;

        input_obj >> tmp;
        input_obj >> tmp_plain3.v[1].x;
        input_obj >> tmp_plain3.v[1].y;
        input_obj >> tmp_plain3.v[1].z;
        input_obj >> tmp;
        input_obj >> tmp_plain3.v[1].u;
        input_obj >> tmp_plain3.v[1].v;

        input_obj >> tmp;
        input_obj >> tmp_plain3.v[2].x;
        input_obj >> tmp_plain3.v[2].y;
        input_obj >> tmp_plain3.v[2].z;
        input_obj >> tmp;
        input_obj >> tmp_plain3.v[2].u;
        input_obj >> tmp_plain3.v[2].v;

        getline(input_obj, tmp);
        getline(input_obj, tmp);
        trunk.push_back(tmp_plain3);
    }

    TreeLibPlain4 tmp_plain4;
    getline(input_obj, tmp);
    getline(input_obj, tmp);
    while(true)
    {
        if(!(input_obj >> tmp))
        {
            break;
        }
        input_obj >> tmp_plain4.v[0].x;
        input_obj >> tmp_plain4.v[0].y;
        input_obj >> tmp_plain4.v[0].z;
        input_obj >> tmp;
        input_obj >> tmp_plain4.v[0].u;
        input_obj >> tmp_plain4.v[0].v;

        input_obj >> tmp;
        input_obj >> tmp_plain4.v[1].x;
        input_obj >> tmp_plain4.v[1].y;
        input_obj >> tmp_plain4.v[1].z;
        input_obj >> tmp;
        input_obj >> tmp_plain4.v[1].u;
        input_obj >> tmp_plain4.v[1].v;

        input_obj >> tmp;
        input_obj >> tmp_plain4.v[2].x;
        input_obj >> tmp_plain4.v[2].y;
        input_obj >> tmp_plain4.v[2].z;
        input_obj >> tmp;
        input_obj >> tmp_plain4.v[2].u;
        input_obj >> tmp_plain4.v[2].v;

        input_obj >> tmp;
        input_obj >> tmp_plain4.v[3].x;
        input_obj >> tmp_plain4.v[3].y;
        input_obj >> tmp_plain4.v[3].z;
        input_obj >> tmp;
        input_obj >> tmp_plain4.v[3].u;
        input_obj >> tmp_plain4.v[3].v;

        getline(input_obj, tmp);
        getline(input_obj, tmp);
        leaves.push_back(tmp_plain4);
    }
    input_obj.close();
}

void Tree::genTruncatedPyramid(std::vector <TreeLibPlain> &plain, TreeLibVector& start_position,
    TreeLibVector& end_position, double r1, double r2, double start_y_angle, double start_z_angle,
    double end_y_angle, double end_z_angle, int num)
{
    TreeLibPlain tmp_plain;
    double delta_angle = 1.0 / num * M_PI;
    double tmp_x, tmp_y, tmp_z;

    for(int i = 0; i < num; ++i)
    {
        tmp_plain.v[0].x = cos(i / (double)num * M_PI * 2.0 + delta_angle) * r1 * cos(start_z_angle);
        tmp_plain.v[0].y = tmp_plain.v[0].x * tan(start_z_angle);
        tmp_plain.v[0].z = sin(i / (double)num * M_PI * 2.0 + delta_angle) * r1;

        tmp_x = tmp_plain.v[0].x;
        tmp_y = tmp_plain.v[0].y;
        tmp_z = tmp_plain.v[0].z;

        tmp_plain.v[0].x = tmp_x * cos(start_y_angle) + tmp_z * sin(start_y_angle);
        tmp_plain.v[0].y = tmp_y;
        tmp_plain.v[0].z = -(tmp_x * sin(start_y_angle)) + tmp_z * cos(start_y_angle);

        tmp_plain.v[0].x += start_position.x;
        tmp_plain.v[0].y += start_position.y;
        tmp_plain.v[0].z += start_position.z;
        tmp_plain.v[0].u = (double)i / (double)num;
        tmp_plain.v[0].v = 0.0;


        tmp_plain.v[1].x = cos(i / (double)num * M_PI * 2.0 - delta_angle) * r2 * cos(end_z_angle);
        tmp_plain.v[1].y = tmp_plain.v[1].x * tan(end_z_angle);
        tmp_plain.v[1].z = sin(i / (double)num * M_PI * 2.0 - delta_angle) * r2;

        tmp_x = tmp_plain.v[1].x;
        tmp_y = tmp_plain.v[1].y;
        tmp_z = tmp_plain.v[1].z;

        tmp_plain.v[1].x = tmp_x * cos(end_y_angle) + tmp_z * sin(end_y_angle);
        tmp_plain.v[1].y = tmp_y;
        tmp_plain.v[1].z = -(tmp_x * sin(end_y_angle)) + tmp_z * cos(end_y_angle);

        tmp_plain.v[1].x += end_position.x;
        tmp_plain.v[1].y += end_position.y;
        tmp_plain.v[1].z += end_position.z;
        tmp_plain.v[1].u = (double)(i + 1) / (double)num;
        tmp_plain.v[1].v = 1.0;


        tmp_plain.v[2].x = cos(i / (double)num * M_PI * 2.0 + delta_angle) * r2 * cos(end_z_angle);
        tmp_plain.v[2].y = tmp_plain.v[2].x * tan(end_z_angle);
        tmp_plain.v[2].z = sin(i / (double)num * M_PI * 2.0 + delta_angle) * r2;

        tmp_x = tmp_plain.v[2].x;
        tmp_y = tmp_plain.v[2].y;
        tmp_z = tmp_plain.v[2].z;

        tmp_plain.v[2].x = tmp_x * cos(end_y_angle) + tmp_z * sin(end_y_angle);
        tmp_plain.v[2].y = tmp_y;
        tmp_plain.v[2].z = -(tmp_x * sin(end_y_angle)) + tmp_z * cos(end_y_angle);

        tmp_plain.v[2].x += end_position.x;
        tmp_plain.v[2].y += end_position.y;
        tmp_plain.v[2].z += end_position.z;
        tmp_plain.v[2].u = (double)i / (double)num;
        tmp_plain.v[2].v = 1.0;

        plain.push_back(tmp_plain);


        tmp_plain.v[0].x = cos(i / (double)num * M_PI * 2.0 + delta_angle) * r2 * cos(end_z_angle);
        tmp_plain.v[0].y = tmp_plain.v[0].x * tan(end_z_angle);
        tmp_plain.v[0].z = sin(i / (double)num * M_PI * 2.0 + delta_angle) * r2;

        tmp_x = tmp_plain.v[0].x;
        tmp_y = tmp_plain.v[0].y;
        tmp_z = tmp_plain.v[0].z;

        tmp_plain.v[0].x = tmp_x * cos(end_y_angle) + tmp_z * sin(end_y_angle);
        tmp_plain.v[0].y = tmp_y;
        tmp_plain.v[0].z = -(tmp_x * sin(end_y_angle)) + tmp_z * cos(end_y_angle);

        tmp_plain.v[0].x += end_position.x;
        tmp_plain.v[0].y += end_position.y;
        tmp_plain.v[0].z += end_position.z;
        tmp_plain.v[0].u = (double)(i + 1) / (double)num;
        tmp_plain.v[0].v = 1.0;


        tmp_plain.v[1].x = cos((i + 1) / (double)num * M_PI * 2.0 + delta_angle) * r1 * cos(start_z_angle);
        tmp_plain.v[1].y = tmp_plain.v[1].x * tan(start_z_angle);
        tmp_plain.v[1].z = sin((i + 1) / (double)num * M_PI * 2.0 + delta_angle) * r1;

        tmp_x = tmp_plain.v[1].x;
        tmp_y = tmp_plain.v[1].y;
        tmp_z = tmp_plain.v[1].z;

        tmp_plain.v[1].x = tmp_x * cos(start_y_angle) + tmp_z * sin(start_y_angle);
        tmp_plain.v[1].y = tmp_y;
        tmp_plain.v[1].z = -(tmp_x * sin(start_y_angle)) + tmp_z * cos(start_y_angle);

        tmp_plain.v[1].x += start_position.x;
        tmp_plain.v[1].y += start_position.y;
        tmp_plain.v[1].z += start_position.z;
        tmp_plain.v[1].u = (double)(i) / (double)num;
        tmp_plain.v[1].v = 0.0;


        tmp_plain.v[2].x = cos(i / (double)num * M_PI * 2.0 + delta_angle) * r1 * cos(start_z_angle);
        tmp_plain.v[2].y = tmp_plain.v[2].x * tan(start_z_angle);
        tmp_plain.v[2].z = sin(i / (double)num * M_PI * 2.0 + delta_angle) * r1;

        tmp_x = tmp_plain.v[2].x;
        tmp_y = tmp_plain.v[2].y;
        tmp_z = tmp_plain.v[2].z;

        tmp_plain.v[2].x = tmp_x * cos(start_y_angle) + tmp_z * sin(start_y_angle);
        tmp_plain.v[2].y = tmp_y;
        tmp_plain.v[2].z = -(tmp_x * sin(start_y_angle)) + tmp_z * cos(start_y_angle);

        tmp_plain.v[2].x += start_position.x;
        tmp_plain.v[2].y += start_position.y;
        tmp_plain.v[2].z += start_position.z;
        tmp_plain.v[2].u = (double)(i + 1) / (double)num;
        tmp_plain.v[2].v = 0.0;

        plain.push_back(tmp_plain);
    }
}


void Tree::genPyramid(std::vector <TreeLibPlain> &plain, TreeLibVector& start_position,
    TreeLibVector& end_position, double r1, double start_y_angle, double start_z_angle, int num)
{
    TreeLibPlain tmp_plain;
    double delta_angle = 1.0 / num * M_PI;
    double tmp_x, tmp_y, tmp_z;

    for(int i = 0; i < num; ++i)
    {
        tmp_plain.v[0].x = cos(i / (double)num * M_PI * 2.0 + delta_angle) * r1 * cos(start_z_angle);
        tmp_plain.v[0].y = tmp_plain.v[0].x * tan(start_z_angle);
        tmp_plain.v[0].z = sin(i / (double)num * M_PI * 2.0 + delta_angle) * r1;

        tmp_x = tmp_plain.v[0].x;
        tmp_y = tmp_plain.v[0].y;
        tmp_z = tmp_plain.v[0].z;

        tmp_plain.v[0].x = tmp_x * cos(start_y_angle) + tmp_z * sin(start_y_angle);
        tmp_plain.v[0].y = tmp_y;
        tmp_plain.v[0].z = -(tmp_x * sin(start_y_angle)) + tmp_z * cos(start_y_angle);

        tmp_plain.v[0].x += start_position.x;
        tmp_plain.v[0].y += start_position.y;
        tmp_plain.v[0].z += start_position.z;
        tmp_plain.v[0].u = 0.0;
        tmp_plain.v[0].v = 0.0;

		
        tmp_plain.v[1].x = end_position.x;
        tmp_plain.v[1].y = end_position.y;
        tmp_plain.v[1].z = end_position.z;

        tmp_plain.v[1].u = 0.0;
        tmp_plain.v[1].v = 1.0;
		

        tmp_plain.v[2].x = cos((i + 1) / (double)num * M_PI * 2.0 + delta_angle) * r1 * cos(start_z_angle);
        tmp_plain.v[2].y = tmp_plain.v[2].x * tan(start_z_angle);
        tmp_plain.v[2].z = sin((i + 1) / (double)num * M_PI * 2.0 + delta_angle) * r1;

        tmp_x = tmp_plain.v[2].x;
        tmp_y = tmp_plain.v[2].y;
        tmp_z = tmp_plain.v[2].z;

        tmp_plain.v[2].x = tmp_x * cos(start_y_angle) + tmp_z * sin(start_y_angle);
        tmp_plain.v[2].y = tmp_y;
        tmp_plain.v[2].z = -(tmp_x * sin(start_y_angle)) + tmp_z * cos(start_y_angle);

        tmp_plain.v[2].x += start_position.x;
        tmp_plain.v[2].y += start_position.y;
        tmp_plain.v[2].z += start_position.z;
        tmp_plain.v[2].u = 1.0;
        tmp_plain.v[2].v = 0.0;

        plain.push_back(tmp_plain);
    }
}

void Tree::genLeaf(TreeLibVector& start_position, double y_angle,
    double z_angle, double leaf_angle, int leaf_type)
{
    TreeLibPlain4 tmp_plain;
    leaf_angle = -leaf_angle;
    double tmp_x, tmp_y, tmp_z;

    tmp_plain.v[0].x = -leaf_size * cos(leaf_angle) - leaf_size * sin(leaf_angle);
    tmp_plain.v[0].y = 0.0f;
    tmp_plain.v[0].z = leaf_size * sin(leaf_angle) - leaf_size * cos(leaf_angle);

    tmp_x = tmp_plain.v[0].x;
    tmp_y = tmp_plain.v[0].y;
    tmp_z = tmp_plain.v[0].z;

    tmp_plain.v[0].x = cos(z_angle) * tmp_x - sin(z_angle) * tmp_y;
    tmp_plain.v[0].y = sin(z_angle) * tmp_x + cos(z_angle) * tmp_y;
    tmp_plain.v[0].z = tmp_z;

    tmp_x = tmp_plain.v[0].x;
    tmp_y = tmp_plain.v[0].y;
    tmp_z = tmp_plain.v[0].z;

    tmp_plain.v[0].x = tmp_x * cos(y_angle) + tmp_z * sin(y_angle);
    tmp_plain.v[0].y = tmp_y;
    tmp_plain.v[0].z = -(tmp_x * sin(y_angle)) + tmp_z * cos(y_angle);

    tmp_plain.v[0].x += start_position.x;
    tmp_plain.v[0].y += start_position.y;
    tmp_plain.v[0].z += start_position.z;

    tmp_plain.v[0].u = 1.0;
    tmp_plain.v[0].v = 0.0;


    tmp_plain.v[1].x = -leaf_size * cos(leaf_angle) + leaf_size * sin(leaf_angle);
    tmp_plain.v[1].y = 0.0f;
    tmp_plain.v[1].z = leaf_size * sin(leaf_angle) + leaf_size * cos(leaf_angle);

    tmp_x = tmp_plain.v[1].x;
    tmp_y = tmp_plain.v[1].y;
    tmp_z = tmp_plain.v[1].z;

    tmp_plain.v[1].x = cos(z_angle) * tmp_x - sin(z_angle) * tmp_y;
    tmp_plain.v[1].y = sin(z_angle) * tmp_x + cos(z_angle) * tmp_y;
    tmp_plain.v[1].z = tmp_z;

    tmp_x = tmp_plain.v[1].x;
    tmp_y = tmp_plain.v[1].y;
    tmp_z = tmp_plain.v[1].z;

    tmp_plain.v[1].x = tmp_x * cos(y_angle) + tmp_z * sin(y_angle);
    tmp_plain.v[1].y = tmp_y;
    tmp_plain.v[1].z = -(tmp_x * sin(y_angle)) + tmp_z * cos(y_angle);

    tmp_plain.v[1].x += start_position.x;
    tmp_plain.v[1].y += start_position.y;
    tmp_plain.v[1].z += start_position.z;

    tmp_plain.v[1].u = 0.0;
    tmp_plain.v[1].v = 0.0;


    tmp_plain.v[2].x = leaf_size * cos(leaf_angle) + leaf_size * sin(leaf_angle);
    tmp_plain.v[2].y = 0.0f;
    tmp_plain.v[2].z = -leaf_size * sin(leaf_angle) + leaf_size * cos(leaf_angle);

    tmp_x = tmp_plain.v[2].x;
    tmp_y = tmp_plain.v[2].y;
    tmp_z = tmp_plain.v[2].z;

    tmp_plain.v[2].x = cos(z_angle) * tmp_x - sin(z_angle) * tmp_y;
    tmp_plain.v[2].y = sin(z_angle) * tmp_x + cos(z_angle) * tmp_y;
    tmp_plain.v[2].z = tmp_z;

    tmp_x = tmp_plain.v[2].x;
    tmp_y = tmp_plain.v[2].y;
    tmp_z = tmp_plain.v[2].z;

    tmp_plain.v[2].x = tmp_x * cos(y_angle) + tmp_z * sin(y_angle);
    tmp_plain.v[2].y = tmp_y;
    tmp_plain.v[2].z = -(tmp_x * sin(y_angle)) + tmp_z * cos(y_angle);

    tmp_plain.v[2].x += start_position.x;
    tmp_plain.v[2].y += start_position.y;
    tmp_plain.v[2].z += start_position.z;

    tmp_plain.v[2].u = 0.0;
    tmp_plain.v[2].v = 1.0;


    tmp_plain.v[3].x = leaf_size * cos(leaf_angle) - leaf_size * sin(leaf_angle);
    tmp_plain.v[3].y = 0.0f;
    tmp_plain.v[3].z = -leaf_size * sin(leaf_angle) - leaf_size * cos(leaf_angle);

    tmp_x = tmp_plain.v[3].x;
    tmp_y = tmp_plain.v[3].y;
    tmp_z = tmp_plain.v[3].z;

    tmp_plain.v[3].x = cos(z_angle) * tmp_x - sin(z_angle) * tmp_y;
    tmp_plain.v[3].y = sin(z_angle) * tmp_x + cos(z_angle) * tmp_y;
    tmp_plain.v[3].z = tmp_z;

    tmp_x = tmp_plain.v[3].x;
    tmp_y = tmp_plain.v[3].y;
    tmp_z = tmp_plain.v[3].z;

    tmp_plain.v[3].x = tmp_x * cos(y_angle) + tmp_z * sin(y_angle);
    tmp_plain.v[3].y = tmp_y;
    tmp_plain.v[3].z = -(tmp_x * sin(y_angle)) + tmp_z * cos(y_angle);

    tmp_plain.v[3].x += start_position.x;
    tmp_plain.v[3].y += start_position.y;
    tmp_plain.v[3].z += start_position.z;

    tmp_plain.v[3].u = 1.0;
    tmp_plain.v[3].v = 1.0;
    tmp_plain.texture_id = leaf_type;

    leaves.push_back(tmp_plain);
}

void Tree::clear()
{
    trunk.clear();
    branches.clear();
    obb.clear();
    leaves.clear();
}

double Tree::getTrunkRadius()
{
    return trunk_r;
}

double Tree::getTrunkRadiusChangeCof()
{
    return trunk_r_change_cof;
}

double Tree::getTrunkProcessRadiusChangeCof()
{
    return trunk_process_r_change_cof;
}

int Tree::getTrunkMaxProcessesFromSegment()
{
	return trunk_max_processes_from_segment;
}

double Tree::getTrunkBending()
{
    return trunk_bending;
}

double Tree::getTrunkProcessChance()
{
    return trunk_process_chance;
}

double Tree::getTrunkProcessBendingY()
{
    return trunk_process_bending_y;
}

double Tree::getTrunkProcessBendingZ()
{
    return trunk_process_bending_z;
}

double Tree::getMinRForBranch()
{
    return min_r_for_branch;
}

double Tree::getMaxRForBranch()
{
    return max_r_for_branch;
}

double Tree::getTrunkSegmentLength()
{
    return trunk_segment_length;
}

double Tree::getTrunkSegmentLengthChange()
{
    return trunk_segment_length_change;
}

double Tree::getTrunkGravitation()
{
    return trunk_gravitation;
}

int Tree::getTrunkSegmentsNum()
{
    return trunk_segments_num;
}

std::string Tree::getTrunkTexture()
{
    return trunk_texture_name;
}

double Tree::getMinRadius()
{
	return min_r;
}

double Tree::getBranchRChangeCof()
{
    return branch_r_change_cof;
}

double Tree::getBranchProcessRChangeCof()
{
    return branch_process_r_change_cof;
}

double Tree::getBranchProcessChance()
{
    return branch_process_chance;
}

int Tree::getBranchMaxProcessesFromSegment()
{
	return branch_max_processes_from_segment;
}

double Tree::getBranchBending()
{
    return branch_bending;
}

double Tree::getBranchProcessBendingY()
{
    return branch_process_bending_y;
}

double Tree::getBranchProcessBendingZ()
{
    return branch_process_bending_z;
}

double Tree::getBranchSegmentLength()
{
    return branch_segment_length;
}

double Tree::getBranchSegmentLengthChange()
{
    return branch_segment_length_change;
}

double Tree::getBranchGravitation()
{
    return branch_gravitation;
}

int Tree::getBranchSegmentsNum()
{
    return branch_segments_num;
}

double Tree::getBBSize()
{
    return bbsize;
}

int Tree::getMaxOverlapsCount()
{
    return max_overlaps_count;
}

std::string Tree::getBranchesTexture()
{
    return branches_texture_name;
}

double Tree::getMaxRadiusForLeaf()
{
    return max_radius_for_leaf;
}

double Tree::getLeavesSpreading()
{
    return leaves_spreading;
}

double Tree::getLeafChance()
{
    return leaf_chance;
}

double Tree::getLeafSize()
{
    return leaf_size;
}

void Tree::setTrunkRadius(double radius)
{
    trunk_r = radius;
}

void Tree::setTrunkRadiusChangeCof(double cof)
{
    trunk_r_change_cof = cof;
}

void Tree::setTrunkProcessRadiusChangeCof(double cof)
{
    trunk_process_r_change_cof = cof;
}

void Tree::setTrunkMaxProcessesFromSegment(int value)
{
	trunk_max_processes_from_segment = value;
}

void Tree::setTrunkBending(double value)
{
    trunk_bending = value;
}

void Tree::setTrunkProcessChance(double value)
{
    trunk_process_chance = value;
}

void Tree::setTrunkProcessBendingY(double value)
{
    trunk_process_bending_y = value;
}

void Tree::setTrunkProcessBendingZ(double value1, double value2)
{
    trunk_process_bending_z_min_angle = value1;
    trunk_process_bending_z_delta_angle = value2 - value1;
}

void Tree::setMinRForBranch(double value)
{
    min_r_for_branch = value;
}

void Tree::setMaxRForBranch(double value)
{
    max_r_for_branch = value;
}

void Tree::setTrunkSegmentLength(double value)
{
    trunk_segment_length = value;
}

void Tree::setTrunkSegmentLengthChange(double value)
{
    trunk_segment_length_change = value;
}

void Tree::setTrunkGravitation(double value)
{
    trunk_gravitation = value / 1000.0;
}

void Tree::setTrunkSegmentsNum(int value)
{
    trunk_segments_num = value;
}

void Tree::setTrunkTexture(std::string value)
{
    trunk_texture_name = value;
}

void Tree::setMinRadius(double value)
{
	min_r = value;
}

void Tree::setBranchRChangeCof(double value)
{
    branch_r_change_cof = value;
}

void Tree::setBranchProcessRChangeCof(double value)
{
    branch_process_r_change_cof = value;
}

void Tree::setBranchProcessChance(double value)
{
    branch_process_chance = value;
}

void Tree::setBranchMaxProcessesFromSegment(int value)
{
	branch_max_processes_from_segment = value;
}

void Tree::setBranchBending(double value)
{
    branch_bending = value;
}

void Tree::setBranchProcessBendingY(double value)
{
    branch_process_bending_y = value;
}

void Tree::setBranchProcessBendingZ(double value1, double value2)
{
    branch_process_bending_z_min_angle = value1;
    branch_process_bending_z_delta_angle = value2 - value1;
}

void Tree::setBranchSegmentLength(double value)
{
    branch_segment_length = value;
}

void Tree::setBranchSegmentLengthChange(double value)
{
    branch_segment_length_change = value;
}

void Tree::setBranchGravitation(double value)
{
    branch_gravitation = value / 1000.0;
}

void Tree::setBranchSegmentsNum(int value)
{
    branch_segments_num = value;
}

void Tree::setBBSize(double value)
{
    bbsize = value;
}

void Tree::setMaxOverlapsCount(int value)
{
    max_overlaps_count = value;
}

void Tree::setBranchesTexture(std::string value)
{
    branches_texture_name = value;
}

void Tree::setMaxRadiusForLeaf(double value)
{
    max_radius_for_leaf = value;
}

void Tree::setLeavesSpreading(double value)
{
    leaves_spreading = value;
}

void Tree::setLeafChance(double value)
{
    leaf_chance = value;
}

void Tree::setLeafSize(double value)
{
    leaf_size = value;
}

void Tree::clearLeavesTypes()
{
    leaves_properties.clear();
}

void Tree::addLeafType(TreeLibLeafProperties tmp)
{
    leaves_properties.push_back(tmp);
}
