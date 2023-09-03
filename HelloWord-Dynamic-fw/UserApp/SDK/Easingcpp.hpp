//
// Created by Huiyicc on 2023/9/1.
// 针对瀚文项目自https://github.com/Huiyicc/Easingcpp.git移植
//
#ifndef EASINGCLASS_EASINGCPP_HPP
#define EASINGCLASS_EASINGCPP_HPP

#include <cmath>

#define PI 3.1415926535

#define EasingType_Linear 1							//线性运动
#define EasingType_CircularLinearInterpolation 2	//圆线性插值
#define EasingType_Elasticity 3						//弹性缓动
#define EasingType_Impact 4 						//冲击缓动
#define EasingType_Quadratic_In 5					//二次方缓动
#define EasingType_Quadratic_Out 6					//二次方缓动_In
#define EasingType_Quadratic_InOut 7				//二次方缓动_InOut
#define EasingType_Cube_In 8						//三次方缓动_In
#define EasingType_Cube_Out 9						//三次方缓动_Out
#define EasingType_Cube_InOut 10					//三次方缓动_InOut
#define EasingType_Biquadrate_In 11					//四次方缓动_In
#define EasingType_Biquadrate_Out 12				//四次方缓动_Out
#define EasingType_Biquadrate_InOut 13				//四次方缓动_InOut
#define EasingType_Fifth_In 14						//五次方缓动_In
#define EasingType_Fifth_Out 15						//五次方缓动_Out
#define EasingType_Fifth_InOut 16					//五次方缓动_InOut
#define EasingType_Sinusoid_In 17					//正弦曲线缓动
#define EasingType_Sinusoid_Out 18					//正弦曲线缓动_InOut
#define EasingType_Sinusoid_InOut 19				//正弦曲线缓动_InOut
#define EasingType_IndexCurve_In 20					//指数曲线缓动_In
#define EasingType_IndexCurve_Out 21				//指数曲线缓动_Out
#define EasingType_IndexCurve_InOut 22				//指数曲线缓动_InOut
#define EasingType_CircularCurve_In 23				//圆曲线缓动_In
#define EasingType_CircularCurve_Out 24				//圆曲线缓动_Out
#define EasingType_CircularCurve_InOut 25			//圆曲线缓动_InOut
#define EasingType_Rebound_Damping_In 26			//反弹缓动_指数衰减_In
#define EasingType_Rebound_Damping_Out 27			//反弹缓动_指数衰减_Out
#define EasingType_Rebound_Damping_InOut 28			//反弹缓动_指数衰减_InOut
#define EasingType_Cube_Spill_In 29					//三次方缓动_溢出_In
#define EasingType_Cube_Spill_Out 30				//三次方缓动_溢出_Out
#define EasingType_Cube_Spill_InOut 31				//三次方缓动_溢出_InOut
#define EasingType_Sinusoid_Damping_In 32			//正弦曲线缓动_指数衰减_In
#define EasingType_Sinusoid_Damping_Out 33			//正弦曲线缓动_指数衰减_Out
#define EasingType_Sinusoid_Damping_InOut 34		//正弦曲线缓动_指数衰减_InOut

void Easing_Strat (
        int Type,			//运动类型
        int AllTime,		//运动总时间
        int intervalTime,	//运动间隔时间
        double start,		//缓动开始值
        double end,			//缓动结束值
        void (*lpCbk)(double, double, int, int, bool&a, int, void*),			//回调指针
        bool bThread,		//是否线程模式
        int param1,			//附加参数1
        void* param2			//附加参数2
);

#endif //EASINGCLASS_EASINGCPP_HPP
