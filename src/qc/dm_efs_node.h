/**
* LICENSE PLACEHOLDER
*
* @file dm_efs_node.h
* @class OpenPST::DmEfsNode
* @package OpenPST
* @brief Represents a file or dir retrieved from the diagnostic monitor EFS subsystem
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#ifndef _QC_DM_EFS_NODE_H_
#define _QC_DM_EFS_NODE_H_

#include "include/definitions.h"
#include <iostream>
#include <vector>
#include "dm_efs.h"

namespace OpenPST {

	using namespace std;


	class DmEfsNode {

		protected:
			std::string name; 
			std::string path;
			int type;
			int32_t mode;
			size_t size;
			time_t atime;
			time_t mtime;
			time_t ctime;
			std::vector<DmEfsNode> children;
			
		public:
			/**
			* @brief - Constructor
			*/
			DmEfsNode(std::string path, diag_subsys_efs_read_dir_rx_t* data);
			
			/**
			* @brief - Constructor
			*/
			DmEfsNode(std::string path, std::string name);

			/**
			* @brief - Deconstructor
			*/
			~DmEfsNode();

			/**
			* @brief Copy Constructor
			*/
			DmEfsNode(const DmEfsNode& copy);
			
			/**
			* @brief isDir
			*/
			bool isDir();

			/**
			* @brief isFile
			*/
			bool isFile();

			/**
			* @brief isLink
			*/
			bool isLink();

			/**
			* @brief isImmovable
			*/
			bool isImmovable();

			std::string getPath();

			std::string getName();

			int32_t getMode();

			size_t getSize();

			time_t getATime();

			time_t getMTime();

			time_t getCTime();

			bool hasChildren();

			std::vector<DmEfsNode>& getChildren();

			void addChild(DmEfsNode child);

	};
}

#endif // _QC_DM_EFS_NODE_H_