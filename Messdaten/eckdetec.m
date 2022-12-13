function [eck] = eckdetec(sample)

eck = [0,0];

for i = 3:size(sample,1)
    
    delta1 = sample(i,1)-sample(i-1,1);
    
    if delta1 ~= 0
        eck = [eck;[sample(i,1),sample(i,2)]];
    end

end

eck(1) = eck(2);
eck = [eck;[sample(end,1),sample(end,2)]]; 