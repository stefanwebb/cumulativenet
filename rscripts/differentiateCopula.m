u1 = 0.96065282914787531;
u2 = 1.0;
theta = 17.392411350662112;
gumbel2 = exp(-((-log(u1))^theta + (-log(u2))^theta) ^ (1 / theta))

%syms u1 u2 u3 u4 theta

%disp 'Gumbel 2'
%gumbel2 = exp(-((-log(u1))^theta + (-log(u2))^theta) ^ (1 / theta));
%ccode(gumbel2)

%disp 'Gumbel 2 du1'
%ccode(diff(gumbel2, u1))

%disp 'Gumbel 2 du2'
%ccode(diff(gumbel2, u2))

%disp 'Gumbel 2 du1 du2'
%ccode(diff(diff(gumbel2, u1), u2))

%disp 'Gumbel 2 dTheta'
%ccode(diff(gumbel2, theta))

%disp 'Gumbel 2 dTheta du1'
%ccode(diff(diff(gumbel2, u1), theta))

%disp 'Gumbel 2 dTheta du2'
%ccode(diff(diff(gumbel2, u2), theta))

%disp 'Gumbel 2 dTheta du1 du2'
%ccode(diff(diff(diff(gumbel2, u1), u2), theta))

%disp 'Gumbel 3'
%gumbel3 = exp(-((-log(u1))^theta + (-log(u2))^theta + (-log(u3))^theta) ^ (1 / theta));
%ccode(gumbel3)

%disp 'Gumbel 3 du1'
%ccode(diff(gumbel3, u1))

%disp 'Gumbel 3 du2'
%ccode(diff(gumbel3, u2))

%disp 'Gumbel 3 du1 du2'
%ccode(diff(diff(gumbel3, u1), u2))

%disp 'Gumbel 3 du3'
%ccode(diff(gumbel3, u3))

%disp 'Gumbel 3 du1 du3'
%ccode(diff(diff(gumbel3, u1), u3))

%disp 'Gumbel 3 du2 du3'
%ccode(diff(diff(gumbel3, u2), u3))

%disp 'Gumbel 3 du1 du2 du3'
%ccode(diff(diff(diff(gumbel3, u1), u2), u3))

%disp 'Gumbel 3 dTheta'
%ccode(diff(gumbel3, theta))

%disp 'Gumbel 3 dTheta du1'
%ccode(diff(diff(gumbel3, u1), theta))

%disp 'Gumbel 3 dTheta du2'
%ccode(diff(diff(gumbel3, u2), theta))

%disp 'Gumbel 3 dTheta du1 du2'
%ccode(diff(diff(diff(gumbel3, u1), u2), theta))

%disp 'Gumbel 3 dTheta du3'
%ccode(diff(diff(gumbel3, u3), theta))

%disp 'Gumbel 3 dTheta du1 du3'
%ccode(diff(diff(diff(gumbel3, u1), u3), theta))

%disp 'Gumbel 3 dTheta du2 du3'
%ccode(diff(diff(diff(gumbel3, u2), u3), theta))

%disp 'Gumbel 3 dTheta du1 du2 du3'
%ccode(diff(diff(diff(diff(gumbel3, u1), u2), u3), theta))

% fid = fopen('gumbel4.txt', 'w');
% 
% disp 'Gumbel 4'
% gumbel4 = exp(-((-log(u1))^theta + (-log(u2))^theta + (-log(u3))^theta + (-log(u4))^theta) ^ (1 / theta));
% deriv = ccode(gumbel4);
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 du1'
% deriv = ccode(diff(gumbel4, u1));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 du2'
% deriv = ccode(diff(gumbel4, u2));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 du1 du2'
% deriv = ccode(diff(diff(gumbel4, u1), u2));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 du3'
% deriv = ccode(diff(gumbel4, u3));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 du1 du3'
% deriv = ccode(diff(diff(gumbel4, u1), u3));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 du2 du3'
% deriv = ccode(diff(diff(gumbel4, u2), u3));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 du1 du2 du3'
% deriv = ccode(diff(diff(diff(gumbel4, u1), u2), u3));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 du4'
% deriv = ccode(diff(gumbel4, u4));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 du1 du4'
% deriv = ccode(diff(diff(gumbel4, u1), u4));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 du2 du4'
% deriv = ccode(diff(diff(gumbel4, u2), u4));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 du1 du2 du4'
% deriv = ccode(diff(diff(diff(gumbel4, u1), u2), u4));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 du3 du4'
% deriv = ccode(diff(diff(gumbel4, u3), u4));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 du1 du3 du4'
% deriv = ccode(diff(diff(diff(gumbel4, u1), u3), u4));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 du2 du3 du4'
% deriv = ccode(diff(diff(diff(gumbel4, u2), u3), u4));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 du1 du2 du3 du4'
% deriv = ccode(diff(diff(diff(diff(gumbel4, u1), u2), u3), u4));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 dTheta'
% deriv = ccode(diff(gumbel4, theta));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 dTheta du1'
% deriv = ccode(diff(diff(gumbel4, u1), theta));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 dTheta du2'
% deriv = ccode(diff(diff(gumbel4, u2), theta));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 dTheta du1 du2'
% deriv = ccode(diff(diff(diff(gumbel4, u1), u2), theta));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 dTheta du3'
% deriv = ccode(diff(diff(gumbel4, u3), theta));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 dTheta du1 du3'
% deriv = ccode(diff(diff(diff(gumbel4, u1), u3), theta));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 dTheta du2 du3'
% deriv = ccode(diff(diff(diff(gumbel4, u2), u3), theta));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 dTheta du1 du2 du3'
% deriv = ccode(diff(diff(diff(diff(gumbel4, u1), u2), u3), theta));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 dTheta du4'
% deriv = ccode(diff(diff(gumbel4, theta), u4));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 dTheta du1 du4'
% deriv = ccode(diff(diff(diff(gumbel4, u1), theta), u4));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 dTheta du2 du4'
% deriv = ccode(diff(diff(diff(gumbel4, u2), theta), u4));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 dTheta du1 du2 du4'
% deriv = ccode(diff(diff(diff(diff(gumbel4, u1), u2), u4), theta));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 dTheta du3 du4'
% deriv = ccode(diff(diff(diff(gumbel4, u3), u4), theta));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 dTheta du1 du3 du4'
% deriv = ccode(diff(diff(diff(diff(gumbel4, u1), u3), u4), theta));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 dTheta du2 du3 du4'
% deriv = ccode(diff(diff(diff(diff(gumbel4, u2), u3), u4), theta));
% fprintf(fid, '%s\n', deriv);
% 
% disp 'Gumbel 4 dTheta du1 du2 du3 du4'
% deriv = ccode(diff(diff(diff(diff(diff(gumbel4, u1), u2), u3), u4), theta));
% fprintf(fid, '%s\n', deriv);
% 
% fclose(fid);

%diff(gumbel2, u1)
%diff(gumbel3, u1)
%diff(gumbel4, u1)

%disp 'second derivatives'
%diff(diff(gumbel2, u1), u2)
%simple(diff(diff(gumbel3, u1), u2))
%diff(diff(gumbel4, u1), u2)

%disp 'third derivatives'
%diff(diff(diff(gumbel3, u1), u2), u3)
%diff(diff(diff(gumbel4, u1), u2), u3)

%disp 'fourth derivatives'
%diff(diff(diff(diff(gumbel4, u1), u2), u3), u4)

%simple((1/theta-1)^2 + (2/theta-2)*(2/theta-3) + 2 * (1/theta - 1) * (1/theta - 2))

%disp 'parameter derivative'
%diff(gumbel2, theta)
%diff(gumbel3, theta)
%simple(diff(gumbel3, theta))
%diff(gumbel4, theta)

%disp 'first par derivative'
%diff(diff(gumbel3, theta), u1)
%diff(diff(gumbel3, u1), theta)

% This is a factor involved in the parameter derivatives
%B = (log((-log(u1))^theta + (-log(u2))^theta + (-log(u3))^theta)*((-log(u1))^theta + (-log(u2))^theta + (-log(u3))^theta)^(1/theta))/theta^2 - (((-log(u1))^theta + (-log(u2))^theta + (-log(u3))^theta)^(1/theta - 1)*(log(-log(u1))*(-log(u1))^theta + log(-log(u2))*(-log(u2))^theta + log(-log(u3))*(-log(u3))^theta))/theta;
%simple(diff(B, u1))

%dBdu1 = - ((((-log(u1))^theta/(u1*log(u1)) - (theta*log(-log(u1))*(-log(u1))^(theta - 1))/u1)*((-log(u1))^theta + (-log(u2))^theta + (-log(u3))^theta)^(1/theta - 1))/theta + ((-log(u1))^(theta - 1)*((-log(u1))^theta + (-log(u2))^theta + (-log(u3))^theta)^(1/theta))/(theta*u1*((-log(u1))^theta + (-log(u2))^theta + (-log(u3))^theta)) + (log((-log(u1))^theta + (-log(u2))^theta + (-log(u3))^theta)*(-log(u1))^(theta - 1)*((-log(u1))^theta + (-log(u2))^theta + (-log(u3))^theta)^(1/theta - 1))/(theta^2*u1) - ((-log(u1))^(theta - 1)*(1/theta - 1)*((-log(u1))^theta + (-log(u2))^theta + (-log(u3))^theta)^(1/theta - 2)*(log(-log(u1))*(-log(u1))^theta + log(-log(u2))*(-log(u2))^theta + log(-log(u3))*(-log(u3))^theta))/u1);

%diff(dBdu1, u2)