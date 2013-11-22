%u1 = 0.96065282914787531;
%u2 = 1.0;
%theta = 17.392411350662112;
%frank2 = ;

syms u1 u2 u3 u4 theta

frank2 = -1/theta * log(1 + ((exp(-theta * u1) - 1) * (exp(-theta * u2) - 1)) / (exp(-theta) - 1)^(2 - 1));
frank3 = -1/theta * log(1 + ((exp(-theta * u1) - 1) * (exp(-theta * u2) - 1) * (exp(-theta * u3) - 1)) / (exp(-theta) - 1)^(3 - 1));
frank4 = -1/theta * log(1 + ((exp(-theta * u1) - 1) * (exp(-theta * u2) - 1) * (exp(-theta * u3) - 1) * (exp(-theta * u4) - 1)) / (exp(-theta) - 1)^(4 - 1));

fid = fopen('frank2.txt', 'w');

disp 'Frank 2'
deriv = ccode(frank2);
fprintf(fid, '%s\n', deriv);

disp 'Frank 2 du1'
deriv = ccode(diff(frank2, u1));
fprintf(fid, '%s\n', deriv);

disp 'Frank 2 du2'
deriv = ccode(diff(frank2, u2));
fprintf(fid, '%s\n', deriv);

disp 'Frank 2 du1 du2'
deriv = ccode(diff(diff(frank2, u1), u2));
fprintf(fid, '%s\n', deriv);

disp 'Frank 2 dTheta'
deriv = ccode(diff(frank2, theta));
fprintf(fid, '%s\n', deriv);

disp 'Frank 2 dTheta du1'
deriv = ccode(diff(diff(frank2, u1), theta));
fprintf(fid, '%s\n', deriv);

disp 'Frank 2 dTheta du2'
deriv = ccode(diff(diff(frank2, u2), theta));
fprintf(fid, '%s\n', deriv);

disp 'Frank 2 dTheta du1 du2'
deriv = ccode(diff(diff(diff(frank2, u1), u2), theta));
fprintf(fid, '%s\n', deriv);

fclose(fid);

fid = fopen('frank3.txt', 'w');

disp 'Frank 3'
deriv = ccode(frank3);
fprintf(fid, '%s\n', deriv);

disp 'Frank 3 du1'
deriv = ccode(diff(frank3, u1));
fprintf(fid, '%s\n', deriv);

disp 'Frank 3 du2'
deriv = ccode(diff(frank3, u2));
fprintf(fid, '%s\n', deriv);

disp 'Frank 3 du1 du2'
deriv = ccode(diff(diff(frank3, u1), u2));
fprintf(fid, '%s\n', deriv);

disp 'Frank 3 du3'
deriv = ccode(diff(frank3, u3));
fprintf(fid, '%s\n', deriv);

disp 'Frank 3 du1 du3'
deriv = ccode(diff(diff(frank3, u1), u3));
fprintf(fid, '%s\n', deriv);

disp 'Frank 3 du2 du3'
deriv = ccode(diff(diff(frank3, u2), u3));
fprintf(fid, '%s\n', deriv);

disp 'Frank 3 du1 du2 du3'
deriv = ccode(diff(diff(diff(frank3, u1), u2), u3));
fprintf(fid, '%s\n', deriv);

disp 'Frank 3 dTheta'
deriv = ccode(diff(frank3, theta));
fprintf(fid, '%s\n', deriv);

disp 'Frank 3 dTheta du1'
deriv = ccode(diff(diff(frank3, u1), theta));
fprintf(fid, '%s\n', deriv);

disp 'Frank 3 dTheta du2'
deriv = ccode(diff(diff(frank3, u2), theta));
fprintf(fid, '%s\n', deriv);

disp 'Frank 3 dTheta du1 du2'
deriv = ccode(diff(diff(diff(frank3, u1), u2), theta));
fprintf(fid, '%s\n', deriv);

disp 'Frank 3 dTheta du3'
deriv = ccode(diff(diff(frank3, u3), theta));
fprintf(fid, '%s\n', deriv);

disp 'Frank 3 dTheta du1 du3'
deriv = ccode(diff(diff(diff(frank3, u1), u3), theta));
fprintf(fid, '%s\n', deriv);

disp 'Frank 3 dTheta du2 du3'
deriv = ccode(diff(diff(diff(frank3, u2), u3), theta));
fprintf(fid, '%s\n', deriv);

disp 'Frank 3 dTheta du1 du2 du3'
deriv = ccode(diff(diff(diff(diff(frank3, u1), u2), u3), theta));
fprintf(fid, '%s\n', deriv);

fclose(fid);

fid = fopen('frank4.txt', 'w');

disp 'Frank 4'
deriv = ccode(frank4);
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 du1'
deriv = ccode(diff(frank4, u1));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 du2'
deriv = ccode(diff(frank4, u2));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 du1 du2'
deriv = ccode(diff(diff(frank4, u1), u2));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 du3'
deriv = ccode(diff(frank4, u3));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 du1 du3'
deriv = ccode(diff(diff(frank4, u1), u3));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 du2 du3'
deriv = ccode(diff(diff(frank4, u2), u3));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 du1 du2 du3'
deriv = ccode(diff(diff(diff(frank4, u1), u2), u3));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 du4'
deriv = ccode(diff(frank4, u4));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 du1 du4'
deriv = ccode(diff(diff(frank4, u1), u4));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 du2 du4'
deriv = ccode(diff(diff(frank4, u2), u4));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 du1 du2 du4'
deriv = ccode(diff(diff(diff(frank4, u1), u2), u4));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 du3 du4'
deriv = ccode(diff(diff(frank4, u3), u4));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 du1 du3 du4'
deriv = ccode(diff(diff(diff(frank4, u1), u3), u4));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 du2 du3 du4'
deriv = ccode(diff(diff(diff(frank4, u2), u3), u4));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 du1 du2 du3 du4'
deriv = ccode(diff(diff(diff(diff(frank4, u1), u2), u3), u4));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 dTheta'
deriv = ccode(diff(frank4, theta));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 dTheta du1'
deriv = ccode(diff(diff(frank4, u1), theta));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 dTheta du2'
deriv = ccode(diff(diff(frank4, u2), theta));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 dTheta du1 du2'
deriv = ccode(diff(diff(diff(frank4, u1), u2), theta));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 dTheta du3'
deriv = ccode(diff(diff(frank4, u3), theta));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 dTheta du1 du3'
deriv = ccode(diff(diff(diff(frank4, u1), u3), theta));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 dTheta du2 du3'
deriv = ccode(diff(diff(diff(frank4, u2), u3), theta));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 dTheta du1 du2 du3'
deriv = ccode(diff(diff(diff(diff(frank4, u1), u2), u3), theta));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 dTheta du4'
deriv = ccode(diff(diff(frank4, theta), u4));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 dTheta du1 du4'
deriv = ccode(diff(diff(diff(frank4, u1), theta), u4));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 dTheta du2 du4'
deriv = ccode(diff(diff(diff(frank4, u2), theta), u4));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 dTheta du1 du2 du4'
deriv = ccode(diff(diff(diff(diff(frank4, u1), u2), u4), theta));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 dTheta du3 du4'
deriv = ccode(diff(diff(diff(frank4, u3), u4), theta));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 dTheta du1 du3 du4'
deriv = ccode(diff(diff(diff(diff(frank4, u1), u3), u4), theta));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 dTheta du2 du3 du4'
deriv = ccode(diff(diff(diff(diff(frank4, u2), u3), u4), theta));
fprintf(fid, '%s\n', deriv);

disp 'Frank 4 dTheta du1 du2 du3 du4'
deriv = ccode(diff(diff(diff(diff(diff(frank4, u1), u2), u3), u4), theta));
fprintf(fid, '%s\n', deriv);

fclose(fid);