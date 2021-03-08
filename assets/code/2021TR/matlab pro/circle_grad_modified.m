%圆弧更新在实验中一直有误差。我们认为是机械组模型量的不准导致的误差，所以需要更新x，y，得到正确的系数
%由于k1，k2的差恒为定值，圆弧更新收敛得很少。不过实验成功过。

digits(20)

syms X Y;
%                                       赋值区域

x= 0.02335;                     %走180°以后小车x坐标的改变量，初值是机械组量的模型参数
y = -104.810688;                %走180°以后小车x坐标的改变量，初值是机械组量的模型参数

K1_POS = -13.94717351 + x;    %正转180°时解算出来的K1的值。注意，这里中的K对应的实际上是ARC_K1_POS
K2_POS = -14.4707069 + y;       %正转180°时解算出来的K2的值
K1_NEG = -14.22696467 + x;    %反转180°时解算出来的K1的值
K2_NEG = -14.4069862 + y;       %反转180°时解算出来的K2的值

a_pos_period = 0;              %α由tan函数解算出来，所以需要加周期。假设α实际为90°，正转解算出来是-90°，那就令α_pos_period = pi
a_neg_period = -0;               %假设α实际为90°，反转解算出来是-90°，那就令α_neg_period = pi，一般来说，neg和pos，一个是±pi，另一个就是0

% 梯度下降(更新x，y)参数修改

h = 1;                           %积分步长，根据实验，取较大积分步长时仍有较好的收敛
max_iter_num = 1;             %最大迭代次数

x_max = 1;
x_min = -1;
y_max = -102;
y_min = -106;


%                                       函数部分

%为保证损失函数中两者权值一样，L,a解算略有变化，可以用距离差值除以距离的均值。8那一块也需要改一改
a_POS = atan((K2_POS-Y)/(K1_POS - X)) + a_pos_period;
a_NEG = atan((K2_NEG-Y)/(K1_NEG - X)) + a_neg_period;
L_POS = (sqrt((K1_POS - X)*(K1_POS - X) +( K2_POS - Y)*(K2_POS- Y)) / 2) / 6;
L_NEG = (sqrt((K1_NEG - X)*(K1_NEG - X) +( K2_NEG - Y)*(K2_NEG- Y)) / 2) / 6;

%F = (abs(a_POS) - abs( a_NEG))*(abs(a_POS) - abs( a_NEG)) +  (L_POS - L_NEG)* (L_POS - L_NEG);
F = (a_POS-  a_NEG)*(a_POS -  a_NEG) +(L_POS - L_NEG)* (L_POS - L_NEG);
%F =  (L_POS - L_NEG)* (L_POS - L_NEG);

Fx = diff(F,'X',1);
Fy = diff(F,'Y',1);

iter_num = 0;                   %当前迭代次数
threshold = 0.0000000001;       %能接受的最小误差
f_next = 0.1;                   %下一次迭代的值,只要大于threshold即可，由于threshold不改，这个也无需修改
f_min = 0;
f_begin = eval(vpa(subs(F,{X,Y},{x,y})));

while( (iter_num < max_iter_num) && (f_next > threshold))

    %计算函数值
    f_now = eval(vpa(subs(F,{X,Y},{x,y})));

    fx_now =  eval(vpa(subs(Fx,{X,Y},{x,y})));
    fy_now =  eval(vpa(subs(Fy,{X,Y},{x,y})));

    %更新x和y值，梯度下降法
    if ( x>x_min && x<x_max && y>y_min && y<y_max)
         x_last = x;
         y_last = y;
         
         x = x - h*fx_now;
         y = y - h*fy_now;
         
        f_next = eval(vpa(subs(F,{X,Y},{x,y})));    
        iter_num  = iter_num + 1;    
        fprintf("当前迭代次数为%d\n",iter_num);

         if(f_next<=f_now)
              f_min = f_next; 
              K = [vpa(f_min), vpa(x),vpa(y)];
              fprintf("迭代收敛中，当前损失函数的值为%g\n",K(1));
              circle(1) = subs(a_POS,{X,Y},{x,y});
              circle(2) = subs(a_NEG,{X,Y},{x,y});
              circle(3) = subs(L_POS*4,{X,Y},{x,y});
              circle(4) = subs(L_NEG*4,{X,Y},{x,y});
         end
    else
        h = h/10;
        x = x_last - h*fx_now;
        y = y_last - h*fy_now;
         
    end
end
    
 fprintf("损失函数F的初值：%f\t",vpa(f_begin));
 fprintf("损失函数F现在的值：%f\n",vpa(f_min));
 fprintf("转180°后小车坐标X的改变量：%f\t",vpa(x));
 fprintf("转180°后小车坐标Y的改变量：%f\n",vpa(y));
 
 fprintf("正转得到的α值a_POS：%f\t",vpa(circle(1)));
 fprintf("反转得到的α值a_NEG：%f\n",vpa(circle(2)));
 fprintf("正转得到的L值L_POS：%f\t",vpa(circle(3)));
 fprintf("反转得到的L值L_NEG：%f\n",vpa(circle(4)));
