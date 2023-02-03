function [foundPlateausIndex] = FindPlateausInData(data, lowerLimit, upperLimit, minimumPlateauPointCount)
%FINDPLATEAUSINDATA Summary of this function goes here
%   Detailed explanation goes here

    plateauPointCount = 0;
    plateauStartIdx = 0;
    plateauEndIdx = 0;
    
    plateauCounter = 0;
    
    out = zeros(100,2);
    
    for i = 1:length(data)
        if (data(i) >= lowerLimit && data(i) <= upperLimit) 
            plateauPointCount = plateauPointCount +1 ;
            
            if(plateauPointCount == 1)
                plateauStartIdx = i;
            end    
        else
            if(plateauPointCount > minimumPlateauPointCount)
                plateauEndIdx = i;
                plateauCounter = plateauCounter +1;
                out(plateauCounter,1) = plateauStartIdx;
                out(plateauCounter,2) = plateauEndIdx;
                plateauPointCount = 0;
                plateauStartIdx = 0;
                plateauEndIdx = 0;
            else
                plateauPointCount = 0;
                plateauStartIdx = 0;
                plateauEndIdx = 0;
            end     
        end    
    end
    foundPlateausIndex = sort(nonzeros(out));
end
  