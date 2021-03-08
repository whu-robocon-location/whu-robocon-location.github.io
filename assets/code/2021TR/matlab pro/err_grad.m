%INPUT: 走x轴的误差
%更改的参数：正反轮径系数

%想法一：八个未知数八个方程
% syms X_POS BEITA MP CM1_POS CM1_NEG CM2_POS CM2_NEG ;
% syms K1_X_POS K2_X_POS K1_X_NEG K2_X_NEG K1_Y_POS K2_Y_POS K1_Y_NEG K2_Y_NEG;
% syms Angle_X Angle_Y;
% Position_Conds = [
%     cos(BEITA)*CM1_POS*K1_X_POS - sin(BEITA + MP)* CM2_NEG*K2_POS_X == 500;%X+
%     sin(BEITA)*CM1_POS*K1_X_POS  + cos(BEITA + MP)*CM2_NEG*K2_POS_X == 0;
%     cos(BEITA)*CM1_NEG*K1_X_NEG - sin(BEITA + MP)* CM2_POS*K2_NEG_X == -500;%X-
%     sin(BEITA)*CM1_NEG*K1_X_NEG  + cos(BEITA + MP)*CM2_POS*K2_NEG_X == 0;   
%     sin(BEITA)*CM1_POS*K1_Y_POS + cos(BEITA + MP)*CM2_POS*K2_Y_POS ==0;%Y+
%     cos(BEITA)*CM1_POS*K1_Y_POS - sin(BEITA + MP)*CM2_POS*K2_Y_POS == 500;
%     sin(BEITA)*CM1_NEG*K1_Y_NEG + cos(BEITA + MP)*CM2_NEG*K2_Y_NEG ==0;%Y-
%     cos(BEITA)*CM1_NEG*K1_Y_NEG - sin(BEITA + MP)*CM2_NEG*K2_Y_NEG == -500;
% ];


%1.只用距离建立损失函数，通过初值反复迭代
%2.八个未知数八个方程，解出一组数值解。实际上误差远比想象的多，都是平衡过的，标准就行
%3.当作实验不准，直接新建立方程组，相当于符合实际解
% syms  CM1_POS CM1_NEG CM2_POS CM2_NEG ;

%码盘脉冲数，只需要初值；轮径系数
syms K1_X_POS K2_X_POS K1_X_NEG K2_X_NEG K1_Y_POS K2_Y_POS K1_Y_NEG K2_Y_NEG;
syms CM1_POS  CM1_NEG  CM2_POS  CM2_NEG ；

%走每个轴的距离误差
X_POS_ERR = 1;
X_NEG_ERR =1 ;
Y_POS_ERR = 1;
Y_NEG_ERR =1;

%码盘误差角以及β
MP = -0.00127046873;
BEITA = 0.7853981;

 %轮径系数初值，用于解算脉冲数   
cm1_pos =0.0039978349 ;
cm1_neg = 0.0040002464;
cm2_pos = 0.0039848803;
cm2_neg =0.0039854631 ;

%解算一组脉冲数
Position_Conds =[
    cos(BEITA)*CM1_POS*K1_X_POS - sin(BEITA + MP)* CM2_NEG*K2_X_POS == 500,
    sin(BEITA)*CM1_POS*K1_X_POS  + cos(BEITA + MP)*CM2_NEG*K2_X_POS == 0,
    cos(BEITA)*CM1_NEG*K1_X_NEG - sin(BEITA + MP)* CM2_POS*K2_X_NEG == -500,
    sin(BEITA)*CM1_NEG*K1_X_NEG  + cos(BEITA + MP)*CM2_POS*K2_X_NEG == 0,
    sin(BEITA)*CM1_POS*K1_Y_POS + cos(BEITA + MP)*CM2_POS*K2_Y_POS ==500,
    cos(BEITA)*CM1_POS*K1_Y_POS - sin(BEITA + MP)*CM2_POS*K2_Y_POS == 0,
    sin(BEITA)*CM1_NEG*K1_Y_NEG + cos(BEITA + MP)*CM2_NEG*K2_Y_NEG ==-500,
    cos(BEITA)*CM1_NEG*K1_Y_NEG - sin(BEITA + MP)*CM2_NEG*K2_Y_NEG == 0;
 ];
vars = [K1_X_POS K2_X_POS K1_X_NEG K2_X_NEG K1_Y_POS K2_Y_POS K1_Y_NEG K2_Y_NEG];
[K1_X_POS K2_X_POS K1_X_NEG K2_X_NEG K1_Y_POS K2_Y_POS K1_Y_NEG K2_Y_NEG]=solve(subs(Position_Conds,{CM1_POS,CM1_NEG,CM2_POS,CM2_NEG},{cm1_pos,cm1_neg,cm2_pos,cm2_neg}),vars);

%对结果进行数据转换
K1_X_POS = eval(vpa(K1_X_POS,5));
K2_X_POS = eval(vpa(K2_X_POS,5));
K1_X_NEG =eval( vpa(K1_X_NEG,5));
K2_X_NEG = eval(vpa(K2_X_NEG,5));
K1_Y_POS = eval(vpa(K1_Y_POS,5));
K2_Y_POS = eval(vpa(K2_Y_POS,5));
K1_Y_NEG = eval(vpa(K1_Y_NEG,5));
K2_Y_NEG = eval(vpa(K2_Y_NEG,5));

%误差计算方程
F_X_POS = cos(BEITA) * CM1_POS * K1_X_POS - sin(BEITA + MP) * CM2_NEG * K2_X_POS;%X+
F_X_NEG = cos(BEITA) * CM1_NEG * K1_X_NEG - sin(BEITA + MP) * CM2_POS * K2_X_NEG;%X- 
F_Y_POS = sin(BEITA) * CM1_POS * K1_Y_POS + cos(BEITA + MP) * CM2_POS * K2_Y_POS;%Y+
F_Y_NEG = sin(BEITA) * CM1_NEG * K1_Y_NEG + cos(BEITA + MP) * CM2_NEG * K2_Y_NEG;%Y-

%各个方向的误差函数
X_POS_LOSE = (F_X_POS - (500*500/(500 + X_POS_ERR)))*(F_X_POS - (500*500/(500 + X_POS_ERR)));
X_NEG_LOSE = (F_X_NEG - (500*500/(-500 + X_NEG_ERR)))*(F_X_NEG - (500*500/(-500 + X_NEG_ERR)));
Y_POS_LOSE = (F_Y_POS - (500*500/(500 + Y_POS_ERR)))*(F_Y_POS - (500*500/(500 + Y_POS_ERR)));
Y_NEG_LOSE = (F_Y_NEG - (500*500/(-500 + Y_NEG_ERR)))*(F_Y_NEG - (500*500/(-500 + Y_NEG_ERR)));

%损失函数
F_LOSE  = X_POS_LOSE + X_NEG_LOSE + Y_POS_LOSE + Y_NEG_LOSE;

%寻找下降方向
GRAD_CM1_POS = diff(F_LOSE,'CM1_POS',1);
GRAD_CM1_NEG = diff(F_LOSE,'CM1_NEG',1);
GRAD_CM2_POS = diff(F_LOSE,'CM2_POS',1);
GRAD_CM2_NEG = diff(F_LOSE,'CM2_NEG',1);

%梯度下降法，设置积分步长以及迭代次数
   h = 0.000000000001;%积分步长，根据实验，建议往小了搓
   iter_num = 0;%当前迭代次数
   f_next = 0.2;%下一次迭代的值
    
    %设置迭代结束条件
    max_iter_num = 1000;%最大迭代次数
    threshold = 0.00001;%能接受的最小误差

while( (iter_num < max_iter_num) && (f_next > threshold))

    %计算函数值
f_now= eval(vpa(subs(F_LOSE, {CM1_POS,CM1_NEG,CM2_POS,CM2_NEG},{cm1_pos,cm1_neg,cm2_pos,cm2_neg})));

grad_cm1_pos = eval(vpa(subs(GRAD_CM1_POS,{CM1_POS,CM1_NEG,CM2_POS,CM2_NEG},{cm1_pos,cm1_neg,cm2_pos,cm2_neg})));
grad_cm1_neg = eval(vpa(subs(GRAD_CM1_NEG,{CM1_POS,CM1_NEG,CM2_POS,CM2_NEG},{cm1_pos,cm1_neg,cm2_pos,cm2_neg})));
grad_cm2_pos = eval(vpa(subs(GRAD_CM2_POS,{CM1_POS,CM1_NEG,CM2_POS,CM2_NEG},{cm1_pos,cm1_neg,cm2_pos,cm2_neg})));
grad_cm2_neg = eval(vpa(subs(GRAD_CM2_NEG,{CM1_POS,CM1_NEG,CM2_POS,CM2_NEG},{cm1_pos,cm1_neg,cm2_pos,cm2_neg})));

    %更新脉冲数，梯度下降法
  cm1_pos = cm1_pos - h*grad_cm1_pos;
  cm1_neg = cm1_neg - h*grad_cm1_neg;
  cm2_pos = cm2_pos - h*grad_cm2_pos;
  cm2_neg = cm2_neg - h*grad_cm2_neg;
 
   f_next = eval(vpa(subs(F_LOSE, {CM1_POS,CM1_NEG,CM2_POS,CM2_NEG},{cm1_pos,cm1_neg,cm2_pos,cm2_neg})));
    
    iter_num  = iter_num + 1;
    
     fprintf("当前迭代次数为%d\n",iter_num);
     %disp(vpa(f_next))
      
     if(f_next<=f_now)
         f_min = f_next; 
         K = [vpa(f_min), vpa(cm1_pos),vpa(cm1_neg),vpa(cm2_pos),vpa(cm2_neg)];
          fprintf("迭代收敛中，当前损失函数的值为%g\n",K(1));
     end
    
end

fprintf("迭代完成，当前损失函数的值为%g\n",K(1));
fprintf("迭代完成，当前cm1_pos为%g\n",K(2));
fprintf("迭代完成，当前cm1_neg的值为%g\n",K(3));
fprintf("迭代完成，当前cm2_pos的值为%g\n",K(4));
fprintf("迭代完成，当前cm2_neg的值为%g\n",K(5));