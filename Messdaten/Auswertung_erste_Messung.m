% Projekt Sensorsysteme
% Auswertung erste Messungen 

file =  fileread("ZYX_hinUndher.TXT");
% Split the data at $
data = regexp(file,',','split')
z = data(:,3);
plot(z)