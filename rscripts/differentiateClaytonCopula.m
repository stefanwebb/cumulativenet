syms u1 u2 u3 u4 theta

clayton2 = (u1^(-theta) + u2^(-theta) - 2 + 1)^(-1/theta);
clayton3 = (u1^(-theta) + u2^(-theta) + u3^(-theta) - 3 + 1)^(-1/theta);
clayton4 = (u1^(-theta) + u2^(-theta) + u3^(-theta) + u4^(-theta) - 4 + 1)^(-1/theta);

fid = fopen('clayton2.txt', 'w');

disp 'Clayton 2'
deriv = ccode(clayton2);
fprintf(fid, '%s\n', deriv);

disp 'Clayton 2 du1'
deriv = ccode(diff(clayton2, u1));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 2 du2'
deriv = ccode(diff(clayton2, u2));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 2 du1 du2'
deriv = ccode(diff(diff(clayton2, u1), u2));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 2 dTheta'
deriv = ccode(diff(clayton2, theta));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 2 dTheta du1'
deriv = ccode(diff(diff(clayton2, u1), theta));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 2 dTheta du2'
deriv = ccode(diff(diff(clayton2, u2), theta));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 2 dTheta du1 du2'
deriv = ccode(diff(diff(diff(clayton2, u1), u2), theta));
fprintf(fid, '%s\n', deriv);

fclose(fid);

fid = fopen('clayton3.txt', 'w');

disp 'Clayton 3'
deriv = ccode(clayton3);
fprintf(fid, '%s\n', deriv);

disp 'Clayton 3 du1'
deriv = ccode(diff(clayton3, u1));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 3 du2'
deriv = ccode(diff(clayton3, u2));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 3 du1 du2'
deriv = ccode(diff(diff(clayton3, u1), u2));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 3 du3'
deriv = ccode(diff(clayton3, u3));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 3 du1 du3'
deriv = ccode(diff(diff(clayton3, u1), u3));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 3 du2 du3'
deriv = ccode(diff(diff(clayton3, u2), u3));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 3 du1 du2 du3'
deriv = ccode(diff(diff(diff(clayton3, u1), u2), u3));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 3 dTheta'
deriv = ccode(diff(clayton3, theta));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 3 dTheta du1'
deriv = ccode(diff(diff(clayton3, u1), theta));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 3 dTheta du2'
deriv = ccode(diff(diff(clayton3, u2), theta));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 3 dTheta du1 du2'
deriv = ccode(diff(diff(diff(clayton3, u1), u2), theta));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 3 dTheta du3'
deriv = ccode(diff(diff(clayton3, u3), theta));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 3 dTheta du1 du3'
deriv = ccode(diff(diff(diff(clayton3, u1), u3), theta));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 3 dTheta du2 du3'
deriv = ccode(diff(diff(diff(clayton3, u2), u3), theta));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 3 dTheta du1 du2 du3'
deriv = ccode(diff(diff(diff(diff(clayton3, u1), u2), u3), theta));
fprintf(fid, '%s\n', deriv);

fclose(fid);

fid = fopen('clayton4.txt', 'w');

disp 'Clayton 4'
deriv = ccode(clayton4);
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 du1'
deriv = ccode(diff(clayton4, u1));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 du2'
deriv = ccode(diff(clayton4, u2));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 du1 du2'
deriv = ccode(diff(diff(clayton4, u1), u2));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 du3'
deriv = ccode(diff(clayton4, u3));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 du1 du3'
deriv = ccode(diff(diff(clayton4, u1), u3));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 du2 du3'
deriv = ccode(diff(diff(clayton4, u2), u3));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 du1 du2 du3'
deriv = ccode(diff(diff(diff(clayton4, u1), u2), u3));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 du4'
deriv = ccode(diff(clayton4, u4));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 du1 du4'
deriv = ccode(diff(diff(clayton4, u1), u4));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 du2 du4'
deriv = ccode(diff(diff(clayton4, u2), u4));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 du1 du2 du4'
deriv = ccode(diff(diff(diff(clayton4, u1), u2), u4));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 du3 du4'
deriv = ccode(diff(diff(clayton4, u3), u4));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 du1 du3 du4'
deriv = ccode(diff(diff(diff(clayton4, u1), u3), u4));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 du2 du3 du4'
deriv = ccode(diff(diff(diff(clayton4, u2), u3), u4));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 du1 du2 du3 du4'
deriv = ccode(diff(diff(diff(diff(clayton4, u1), u2), u3), u4));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 dTheta'
deriv = ccode(diff(clayton4, theta));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 dTheta du1'
deriv = ccode(diff(diff(clayton4, u1), theta));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 dTheta du2'
deriv = ccode(diff(diff(clayton4, u2), theta));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 dTheta du1 du2'
deriv = ccode(diff(diff(diff(clayton4, u1), u2), theta));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 dTheta du3'
deriv = ccode(diff(diff(clayton4, u3), theta));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 dTheta du1 du3'
deriv = ccode(diff(diff(diff(clayton4, u1), u3), theta));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 dTheta du2 du3'
deriv = ccode(diff(diff(diff(clayton4, u2), u3), theta));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 dTheta du1 du2 du3'
deriv = ccode(diff(diff(diff(diff(clayton4, u1), u2), u3), theta));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 dTheta du4'
deriv = ccode(diff(diff(clayton4, theta), u4));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 dTheta du1 du4'
deriv = ccode(diff(diff(diff(clayton4, u1), theta), u4));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 dTheta du2 du4'
deriv = ccode(diff(diff(diff(clayton4, u2), theta), u4));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 dTheta du1 du2 du4'
deriv = ccode(diff(diff(diff(diff(clayton4, u1), u2), u4), theta));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 dTheta du3 du4'
deriv = ccode(diff(diff(diff(clayton4, u3), u4), theta));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 dTheta du1 du3 du4'
deriv = ccode(diff(diff(diff(diff(clayton4, u1), u3), u4), theta));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 dTheta du2 du3 du4'
deriv = ccode(diff(diff(diff(diff(clayton4, u2), u3), u4), theta));
fprintf(fid, '%s\n', deriv);

disp 'Clayton 4 dTheta du1 du2 du3 du4'
deriv = ccode(diff(diff(diff(diff(diff(clayton4, u1), u2), u3), u4), theta));
fprintf(fid, '%s\n', deriv);

fclose(fid);