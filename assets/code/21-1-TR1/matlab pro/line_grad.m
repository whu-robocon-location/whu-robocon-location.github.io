%我们实验得到的脉冲数和真实脉冲数（假设真实脉冲数完全符合几个方程）有一定差距
%本程序通过迭代可算出真实脉冲数，从而确定其他系数

digits(20)

global S0;%给定距离S0
syms K11 K12 K13 K14 K21 K22 K23 K24;

%                                           赋值区,改变数据输入即可
S0 = 180;   
k11 = -31936.75;                     %走x+时码盘1的脉冲数（均值）
k12 = 32010.75;                      %走x-时码盘1的脉冲数
k13 = -31505;                        %走y+时码盘1的脉冲数
k14 = 31504.25;                      %走y-时码盘1的脉冲数
k21 = 31435.5;                       %走x+时码盘2的脉冲数
k22 = -31408.5;                      %走x-时码盘2的脉冲数
k23 = -32052.5;                      %走y+时码盘2的脉冲数
k24 = 32030;                         %走y-时码盘2的脉冲数
beita_period = -pi;                  %β由arctan解出来，因此会差周期，根据实际情况选择周期
 
h = 5000;%积分步长，根据实验，取较大积分步长时仍有较好的收敛性
max_iter_num = 200;%最大迭代次数
threshold = 0.0000000001;%能接受的最小误差


                                    %计算损失函数
%mp_angle_wrong
mp1 = atan((K11*K21+K13*K23)/(K23*K11 - K13*K21));
mp2 = -atan((K11*K21+K14*K24)/(K11*K24 - K14*K21));
mp3 = -atan((K12*K22+K13*K23)/(K12*K23 - K13*K22));
mp4 = atan((K12*K22+K14*K24)/(K24*K12 - K14*K22));
mp_ave = (mp1+mp2+mp3+mp4)/4;

%beita 根据不同的情况，beita需要选取不同的解以及选择积分区间
beita1 = -atan(K21/K23) + beita_period ;
beita2 = atan(K21/K24)  + beita_period;
beita3 = atan(K22/K23) + beita_period;
beita4 = -atan(K22/K24)  + beita_period;
beita_ave = (beita1+beita2+beita3+beita4)/4;

%m，中间变量
m1 = -(K11*cos(beita1)+K13*sin(beita1))/(K21*sin(beita1+mp1)-K23*cos(beita1+mp1));
m2 = (K11*cos(beita2)+K14*sin(beita2))/(K21*sin(beita2+mp2)-K24*cos(beita2+mp2));
m3 = (K12*cos(beita3)+K13*sin(beita3))/(K22*sin(beita3+mp3)-K23*cos(beita3+mp3));
m4 = -(K12*cos(beita4)+K14*sin(beita4))/(K22*sin(beita4+mp4)-K24*cos(beita4+mp4));

%CM_PER_CNT
POS_CM_PER_CNT1 = -S0/(K14*sin(beita4) + m4*K24*cos(beita4+mp4));
NEG_CM_PER_CNT1 = S0/(K13*sin(beita1) + m1*K23*cos(beita1+mp1));
POS_CM_PER_CNT2 = -m2*S0/(K14*sin(beita2) + m2*K24*cos(beita2+mp2));
NEG_CM_PER_CNT2 = m3*S0/(K13*sin(beita3)+ m3*K23*cos(beita3+mp3));

%解算各个轴的偏移量
x1 = cos(beita_ave)*NEG_CM_PER_CNT1 *K11 - sin(beita_ave+mp_ave) *POS_CM_PER_CNT2*K21;%x+
y1 = sin(beita_ave)*NEG_CM_PER_CNT1*K11 + cos(beita_ave+mp_ave)*POS_CM_PER_CNT2*K21;
x2 = cos(beita_ave)*POS_CM_PER_CNT1*K12 - sin(beita_ave+mp_ave)*NEG_CM_PER_CNT2*K22;%x-
y2 = sin(beita_ave)*POS_CM_PER_CNT1*K12 + cos(beita_ave+mp_ave)*NEG_CM_PER_CNT2*K22;
x3 = cos(beita_ave)*NEG_CM_PER_CNT1*K13 - sin(beita_ave+mp_ave)*NEG_CM_PER_CNT2*K23;%y+
y3 = sin(beita_ave)*NEG_CM_PER_CNT1*K13 + cos(beita_ave+mp_ave)*NEG_CM_PER_CNT2*K23;
x4 = cos(beita_ave)*POS_CM_PER_CNT1*K14 - sin(beita_ave+mp_ave)*POS_CM_PER_CNT2*K24;%y-
y4 = sin(beita_ave)*POS_CM_PER_CNT1*K14 + cos(beita_ave+mp_ave)*POS_CM_PER_CNT2*K24;

%损失函数
f(K11,K12,K13,K14,K21,K22,K23,K24) = y1*y1 + y2*y2 + x3*x3 + x4*x4 + (x1-S0)*(x1-S0) + (x2+S0)*(x2+S0) + (y3-S0)*(y3-S0)+ (y4+S0)*(y4+S0);

%寻找下降方向
f11 = diff(f(K11,K12,K13,K14,K21,K22,K23,K24),'K11',1);
f12 = diff(f(K11,K12,K13,K14,K21,K22,K23,K24),'K12',1);
f13 = diff(f(K11,K12,K13,K14,K21,K22,K23,K24),'K13',1);
f14 = diff(f(K11,K12,K13,K14,K21,K22,K23,K24),'K14',1);
f21 = diff(f(K11,K12,K13,K14,K21,K22,K23,K24),'K21',1);
f22 = diff(f(K11,K12,K13,K14,K21,K22,K23,K24),'K22',1);
f23 = diff(f(K11,K12,K13,K14,K21,K22,K23,K24),'K23',1);
f24 = diff(f(K11,K12,K13,K14,K21,K22,K23,K24),'K24',1);

iter_num = 0;%当前迭代次数
f_next = 0.002;%下一次迭代的值
    
while( (iter_num < max_iter_num) && (f_next > threshold))

    %计算函数值
    f_now = eval(vpa(subs(f(k11,k12,k13,k14,k21,k22,k23,k24),{K11,K12,K13,K14,K21,K22,K23,K24},{k11,k12,k13,k14,k21,k22,k23,k24})));

    f11_now = eval(vpa(subs(f11,{K11,K12,K13,K14,K21,K22,K23,K24},{k11,k12,k13,k14,k21,k22,k23,k24})));
    f12_now = eval(vpa(subs(f12,{K11,K12,K13,K14,K21,K22,K23,K24},{k11,k12,k13,k14,k21,k22,k23,k24})));
    f13_now = eval(vpa(subs(f13,{K11,K12,K13,K14,K21,K22,K23,K24},{k11,k12,k13,k14,k21,k22,k23,k24})));
    f14_now = eval(vpa(subs(f14,{K11,K12,K13,K14,K21,K22,K23,K24},{k11,k12,k13,k14,k21,k22,k23,k24})));
    f21_now = eval(vpa(subs(f21,{K11,K12,K13,K14,K21,K22,K23,K24},{k11,k12,k13,k14,k21,k22,k23,k24})));
    f22_now = eval(vpa(subs(f22,{K11,K12,K13,K14,K21,K22,K23,K24},{k11,k12,k13,k14,k21,k22,k23,k24})));
    f23_now = eval(vpa(subs(f23,{K11,K12,K13,K14,K21,K22,K23,K24},{k11,k12,k13,k14,k21,k22,k23,k24})));
    f24_now = eval(vpa(subs(f24,{K11,K12,K13,K14,K21,K22,K23,K24},{k11,k12,k13,k14,k21,k22,k23,k24})));

    %更新脉冲数，梯度下降法
    k11 = k11 - h*f11_now;
    k12 = k12 - h*f12_now;
    k13 = k13 - h*f13_now;
    k14 = k14 - h*f14_now;
    k21 = k21 - h*f21_now;
    k22 = k22 - h*f22_now;
    k23 = k23 - h*f23_now;
    k24 = k24 - h*f24_now;
 
    f_next = eval(vpa(subs(f(k11,k12,k13,k14,k21,k22,k23,k24),{K11,K12,K13,K14,K21,K22,K23,K24},{k11,k12,k13,k14,k21,k22,k23,k24})));
    
    iter_num  = iter_num + 1;
    
     fprintf("当前迭代次数为%d\n",iter_num);
     %disp(vpa(f_next))
     
     if(f_next<=f_now)
         f_min = f_next; 
         K = [vpa(f_min), vpa(k11),vpa(k12),vpa(k13),vpa(k14),vpa(k21),vpa(k22),vpa(k23),vpa(k24)];
          fprintf("迭代收敛中，当前损失函数的值为%g\n",K(1));
     end
    
end
%参数矩阵
M = [vpa(eval((subs(beita_ave,{K11,K12,K13,K14,K21,K22,K23,K24},{k11,k12,k13,k14,k21,k22,k23,k24}))));
    vpa(subs(mp_ave,{K11,K12,K13,K14,K21,K22,K23,K24},{k11,k12,k13,k14,k21,k22,k23,k24}));
    vpa(subs(POS_CM_PER_CNT1,{K11,K12,K13,K14,K21,K22,K23,K24},{k11,k12,k13,k14,k21,k22,k23,k24}));
    vpa(subs(POS_CM_PER_CNT2,{K11,K12,K13,K14,K21,K22,K23,K24},{k11,k12,k13,k14,k21,k22,k23,k24}));
    vpa(subs(NEG_CM_PER_CNT1,{K11,K12,K13,K14,K21,K22,K23,K24},{k11,k12,k13,k14,k21,k22,k23,k24}));
    vpa(subs(NEG_CM_PER_CNT2,{K11,K12,K13,K14,K21,K22,K23,K24},{k11,k12,k13,k14,k21,k22,k23,k24}))];


% disp(vpa(eval(subs(m4,{K11,K12,K13,K14,K21,K22,K23,K24},{k11,k12,k13,k14,k21,k22,k23,k24}))))
 fprintf("迭代完成，K11(走x+轴码盘1的脉冲数）为%f\n",K(2));
 fprintf("迭代完成，K12(走x-轴码盘1的脉冲数）为%f\n",K(3));
 fprintf("迭代完成，K13(走y+轴码盘1的脉冲数）为%f\n",K(4));
 fprintf("迭代完成，K14(走y-轴码盘1的脉冲数）为%f\n",K(5));
 fprintf("迭代完成，K21(走x+轴码盘2的脉冲数）为%f\n",K(6));
 fprintf("迭代完成，K22(走x-轴码盘1的脉冲数）为%f\n",K(7));
 fprintf("迭代完成，K23(走y+轴码盘1的脉冲数）为%f\n",K(8));
 fprintf("迭代完成，K24(走y-轴码盘2的脉冲数）为%f\n",K(9));
 
 fprintf("POS_CM_PER_CNT1为%20.10f\n",M(3));
 fprintf("POS_CM_PER_CNT2为%20.10f\n",M(4));
 fprintf("NEG_CM_PER_CNT1为%20.10f\n",M(5));
 fprintf("NEG_CM_PER_CNT2为%20.10f\n",M(6));
 fprintf("beita_ave为%20.10f\n",M(1));
 fprintf("mp_ave为%20.11f\n",M(2));
% text('Interpreter','latex','position',[0,5],'string',['当前的损失函数的值为',latex(K(1))])




