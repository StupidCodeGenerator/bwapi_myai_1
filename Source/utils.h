#pragma once
#include <stdlib.h>


#define FUCK_SUCCESS 1

// ALL ERR < 0 so you can use < 0 to detect error
#define FUCK_ERR -1
#define FUCK_ERR__CANNOT_FIND_UNIT -2           // ��ʾ�Ҳ������Unit
#define FUCK_ERR__UNKNOWN_UNIT_TYPE -3          // ��ʾδ֪��Unit����, 
#define FUCK_ERR__NULL_UNIT -4                  // ��ʾUnitΪ��
#define FUCK_ERR__NULL_AI_MODULE -5             // ��ʾAI_MODULE ������
#define FUCK_ERR__UNIT_ALREADY_IN -6            // ��ʾ����UnitBrain����MineralExtend��ʱ��, �Ѿ���������
#define FUCK_ERR__NULL_RESOURCE_MANAGER -7      // ��ʾ�Ҳ���Resource������

#define FUCK_ERR__SET_SCV_FAIL -6               // ���ڸ�ĳƬ������ũ��, �����ʾ����ʧ��, ��ռ��. SCV��Ҫ����ѡ���Ŀ�

// ��ʾ��UnitID
#define NULL_UNIT_ID -1

// ��ʾSCV�ɿ���Ч�뾶
#define SCV_MINERAL_MAX_DISTRANCE 400

// �����ع�Ͻ�뾶: һ���߼�
#define COMMAND_CENTER_RANGE 400

// ��������С�Ľ������, (��������������̫������)(һ���߼�)
#define COMMAND_CENTER_MIN_BUILD_DISTANCE 60

#define SAFE_FREE(X) {if((X)) free((X)); (X) = NULL;}
#define SAFE_DELETE(X) {if((X)) delete (X); (X) = NULL;}


// Collisions

// LINE/LINE
extern bool lineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);


// LINE/RECTANGLE
extern bool lineRect(float x1, float y1, float x2, float y2, float rx, float ry, float rw, float rh);


