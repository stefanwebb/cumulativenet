syms theta t

% Clayton
rho = (1 + theta * t)^(-1/theta);
invrho = 1/theta * (t^-theta - 1);

fid = fopen('clayton.txt', 'w');

fprintf(fid, 'generator and derivatives\n');

deriv = ccode(rho);
fprintf(fid, '%s\n', deriv);

deriv = ccode(diff(rho, t));
fprintf(fid, '%s\n', deriv);

deriv = ccode(diff(diff(rho, t), t));
fprintf(fid, '%s\n', deriv);

deriv = ccode(diff(diff(diff(rho, t), t), t));
fprintf(fid, '%s\n', deriv);

deriv = ccode(diff(diff(diff(diff(rho, t), t), t), t));
fprintf(fid, '%s\n', deriv);

fprintf(fid, 'inverse generator \n');

deriv = ccode(invrho);
fprintf(fid, '%s\n', deriv);

% Gumbel
rho = exp(-t^(1/theta));
invrho = (-log(t))^theta;
diff(rho, t)

fid = fopen('gumbel.txt', 'w');

fprintf(fid, 'generator and derivatives\n');

deriv = ccode(rho);
fprintf(fid, '%s\n', deriv);

deriv = ccode(diff(rho, t));
fprintf(fid, '%s\n', deriv);

deriv = ccode(diff(diff(rho, t), t));
fprintf(fid, '%s\n', deriv);

deriv = ccode(diff(diff(diff(rho, t), t), t));
fprintf(fid, '%s\n', deriv);

deriv = ccode(diff(diff(diff(diff(rho, t), t), t), t));
fprintf(fid, '%s\n', deriv);

fprintf(fid, 'inverse generator \n');

deriv = ccode(invrho);
fprintf(fid, '%s\n', deriv);

fclose(fid);

% Frank
rho = -1/theta * log(1 - (1 - exp(-theta)) * exp(-t));
invrho = -log((exp(-theta * t) - 1) / (exp(-theta) - 1));

fid = fopen('frank.txt', 'w');

fprintf(fid, 'generator and derivatives\n');

deriv = ccode(rho);
fprintf(fid, '%s\n', deriv);

deriv = ccode(diff(rho, t));
fprintf(fid, '%s\n', deriv);

deriv = ccode(diff(diff(rho, t), t));
fprintf(fid, '%s\n', deriv);

deriv = ccode(diff(diff(diff(rho, t), t), t));
fprintf(fid, '%s\n', deriv);

deriv = ccode(diff(diff(diff(diff(rho, t), t), t), t));
fprintf(fid, '%s\n', deriv);

fprintf(fid, 'inverse generator \n');

deriv = ccode(invrho);
fprintf(fid, '%s\n', deriv);

fclose(fid);