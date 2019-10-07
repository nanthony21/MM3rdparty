#pragma once

/**
\brief 载物台事件
*/
#define XY_MOVING (0)
#define XY_MOVED (1)
/**
\brief Z 轴事件
*/
#define Z_MOVING (10)
#define Z_MOVED (11)
/**
\brief 物镜事件
*/
#define OBJECTIVE_CHANGING (20)
#define OBJECTIVE_PRECHANGED (21)
#define OBJECTIVE_CHANGED (22)
/**
\brief 照明事件
*/
#define ILLUMINATION_CHANGED (30)


/**
\brief 显微镜事件回调函数
*/
typedef void (*fnEventHandler)(int eventId, int data, void* userdata);

/**
\brief 初始化显微镜
*/
int mms_Initialize(const wchar_t* configpath);

/**
\brief 反初始化显微镜
*/
void mms_Uninitialize();

/**
\brief 设置显微镜回调函数
*/
void mms_SetEventHandler(fnEventHandler eh, void* userdata);

/**
\brief 载物台是否可用
*/
bool xy_Available();

/**
\brief 是否正在移动。		
*/
bool xy_IsMoving();

/**
\brief 获取x方向的范围。
\param min 最小位置(微米)。
\param max 最大位置(微米)。
*/
void xy_XRange(double* min,double* max);

/**
\brief 获取y方向的范围。
\param min 最小位置(微米)。
\param max 最大位置(微米)。
*/
void xy_YRange(double* min,double* max);

/**
\brief 获取速度范围。
\param min 最小。
\param max 最大。	
*/
void xy_SpeedRange(double* min,double* max);

/**
\brief 获取视场中心当前位置。
\param positionX [out]x位置(微米)。
\param positionY [out]y位置(微米)。
*/
void xy_CurrentPosition(double* positionX,double* positionY);

/**
\brief 相对移动。
\param deltaX x方向偏移量(微米)。
\param deltaY y方向偏移量(微米)。
\param speed 速度(微米/秒)。
\note 如果没有移动,已经处于目标位置产生XY_MOVED，
如果speed=0或者处于无法移动的状态,不产生任何消息,返回false。
*/
bool xy_MoveRelatively(double deltaX,double deltaY,double speed);

/**
\brief 移动到绝对位置。
\param positionX x位置(微米)。
\param positionY y位置(微米)。
\param speed 速度。
\note 如果没有移动,已经处于目标位置产生XY_MOVED,
speed=0或者处于无法移动的状态,不产生任何消息,返回false。
*/
bool xy_MoveAbsolutely(double positionX,double positionY,double speed);

/**
\brief 往一个方向连续移动。
\param angle 运动角度,逆时针为正,0度为x正方向(右)，(角度0~360)。
\param speed 速度。
\note 如果没有移动,已经处于目标位置产生XY_MOVED,
speed=0或者处于无法移动的状态,不产生任何消息,返回false。
*/
bool xy_MoveContinuously(double angle,double speed);

/**
\brief 停止移动。
*/
void xy_Stop();

/**
\brief 设置空回补偿。
\param compensationx X 轴空回补偿值(微米);
\param compensationy Y 轴空回补偿值(微米);
*/
void xy_SetCompensation(double compensationx, double compensationy);

/**
\brief 获取空回补偿设置。
\param compensationx X 轴空回补偿值(微米);
\param compensationy Y 轴空回补偿值(微米);
*/
void xy_GetCompensation(double* compensationx, double* compensationy);

/**
\brief 获取硬件运动比例。
\param scaleX [out]微米/硬件移动单位（电机步长等）。
\param scaleY [out]微米/硬件移动单位（电机步长等）。
*/
void xy_GetHardwareMotionScale(double* scaleX,double* scaleY);

/**
\brief Z 轴是否可用
*/
bool z_Available();

/**
\brief 是否正在移动。		
*/
bool z_IsMoving();

/**
\brief 获取移动范围。
\param min 最小位置(微米)。
\param max 最大位置(微米)。
*/
void z_Range(double* min,double* max);

/**
\brief 获取速度范围。
\param min 最小。
\param max 最大。
*/
void z_SpeedRange(double* min,double* max);

/**
\brief 获取当前位置。(微米)		
*/
double z_CurrentPosition();

/**
\brief 相对移动。
\param delta 相对位移(微米)。
\param speed 速度。
\note 如果没有移动,已经处于目标位置产生Z_MOVED，
speed=0或者处于无法移动的状态,不产生任何消息,返回false。
*/
bool z_MoveRelatively(double delta,double speed);

/**
\brief 移动到绝对位置。
\param position 位置(微米)。
\param speed 速度。
\note 如果没有移动,已经处于目标位置产生Z_MOVED，
speed=0或者处于无法移动的状态,不产生任何消息,返回false。
*/
bool z_MoveAbsolutely(double position,double speed);

/**
\brief 往一个方向进行连续移动。
\param dir 方向,true:正方向,false:负方向。
\param speed 速度。
\note 如果没有移动,已经处于目标位置产生Z_MOVED，
speed=0或者处于无法移动的状态,不产生任何消息,返回false。
*/
bool z_MoveContinuously(bool dir,double speed);

/**
\brief 停止。
*/
void z_Stop();

/**
\brief 设置空回补偿。
\param compensation 空回补偿值(微米);
*/
void z_SetCompensation(double compensation);

/**
\brief 获取空回补偿设置。
\return 空回补偿设置(微米)。
*/
double z_GetCompensation();

/**
\brief 获取硬件运动比例。(微米/硬件移动单位(电机步长等))。
*/
double z_GetHardwareMotionScale();


/**
\brief 物镜是否可用
*/
bool obj_Available();

/**
\brief 获取物镜数量。
*/
int obj_GetCount();

/**
\brief 获取物镜放大倍数。
\param n [in]基于0的物镜索引。
*/
double obj_GetObjectiveMagnification(int n);

/**
\brief 切换物镜。
\param n [in]目标物镜索引。
*/
bool obj_SwitchObjective(int n);

/**
\brief 获取当前物镜。
\return 物镜索引。
*/
int obj_GetCurrentObjective();


/**
\brief 照明是否可用
*/
bool illumination_Available();

/**
\brief 获取照明值范围。
\param min [out]最小值。
\param max [out]最大值。		
*/
void illumination_GetRange(int* min,int* max);

/**
\brief 获取照明值。
\return 当前照明值。
\see SetValue
*/
int illumination_GetValue();

/**
\brief 设置照明值。
\param val [in]照明值。
\see GetValue
*/
void illumination_SetValue(int val);