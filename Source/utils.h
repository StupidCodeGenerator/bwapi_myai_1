#pragma once
#include <stdlib.h>


#define FUCK_SUCCESS 1

// ALL ERR < 0 so you can use < 0 to detect error
#define FUCK_ERR -1
#define FUCK_ERR__CANNOT_FIND_UNIT -2           // 表示找不到这个Unit
#define FUCK_ERR__UNKNOWN_UNIT_TYPE -3          // 表示未知的Unit类型, 
#define FUCK_ERR__NULL_UNIT -4                  // 表示Unit为空
#define FUCK_ERR__NULL_AI_MODULE -5             // 表示AI_MODULE 不存在
#define FUCK_ERR__UNIT_ALREADY_IN -6            // 表示创建UnitBrain或者MineralExtend的时候, 已经创建过了
#define FUCK_ERR__NULL_RESOURCE_MANAGER -7      // 表示找不到Resource管理器

#define FUCK_ERR__SET_SCV_FAIL -6               // 用于给某片矿设置农民, 这个表示设置失败, 被占了. SCV需要优先选择别的矿

// 表示无UnitID
#define NULL_UNIT_ID -1

// 表示SCV采矿有效半径
#define SCV_MINERAL_MAX_DISTRANCE 400

// 主基地管辖半径: 一阶逻辑
#define COMMAND_CENTER_RANGE 400

// 主基地最小的建造距离, (不允许贴主基地太近建造)(一阶逻辑)
#define COMMAND_CENTER_MIN_BUILD_DISTANCE 60

#define SAFE_FREE(X) {if((X)) free((X)); (X) = NULL;}
#define SAFE_DELETE(X) {if((X)) delete (X); (X) = NULL;}


// Collisions

// LINE/LINE
extern bool lineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);


// LINE/RECTANGLE
extern bool lineRect(float x1, float y1, float x2, float y2, float rx, float ry, float rw, float rh);


